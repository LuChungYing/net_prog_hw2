#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#define ackstate 0
#define sackstate 1
#define buffsize 32 //32KB
static int state;

typedef struct sackpack{
	int length;
	int left[4];
	int right[4];
}sackpacket;
typedef struct buffer{
	int exist;
	char data[1004];
}buffer;
typedef struct TCP_header{
	uint32_t seq;
	uint32_t ACK;
	int request;
	int finish;
	uint16_t source_port;
	uint16_t dest_port;
}TCP_header;                    //5*4 = 20 bytes
typedef struct segment{
	TCP_header head;
	sackpacket sack;
	char data[1004];
}segment;
typedef struct req{
	unsigned request:5;
}Req;

int main(int argc,char* argv[]){
  struct sockaddr_in server;
	static int socketfd;
	int n;
	int length = sizeof(struct sockaddr_in);
	int nbytes;
	Req req;req.request=0;
	int cli_seq,cli_ack;
	for(int i=4; argc-i>0; i++){
		int a = atoi(argv[i]);
		int two=1;
		for(int x=0;x<a;x++) two*=2;
		req.request = req.request|(two);
	}req.request >>= 1;

	memset((char*)&server,0,sizeof(server));
		server.sin_addr.s_addr =inet_addr(argv[1]);
		server.sin_family = AF_INET;
    server.sin_port = atoi(argv[2]);
    if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

		srand(time(NULL));
		struct segment *pack = (struct segment*) malloc (sizeof(struct segment));
		pack->head.seq = (rand()%10000);
		pack->head.ACK = 0;
		pack->head.dest_port =  server.sin_port;
	printf("=====Start the three-way hand shake=====\n");
		if (sendto(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&server, length) < 0) {
		        perror("Could not send datagram!!\n");
		        printf("1:%d\n",errno);
    }
		printf("Send a packet(SYN) to %s : %d\n",argv[1], server.sin_port);
		memset(pack,0,sizeof(segment));
		if ((n=nbytes = recvfrom(socketfd,pack , sizeof(segment), 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) {
				 			                        perror ("could not read datagram!!");
		}
								int len=20;
							  char buff[len];
							  inet_ntop(AF_INET, &(server.sin_addr), buff, len);
							  printf("Recive a packet(SYN/ACK) from %s : %d\n",buff, server.sin_port);
							  printf("\t Recive a packet (seq_num = %u, ack_num = %u)\n",pack->head.seq,pack->head.ACK);
								if(pack->head.seq != 0 ) printf("Recive non SYN/ACK\n");


							  cli_seq = pack->head.ACK;
								cli_ack = pack->head.seq+1;
								memset(pack,0,sizeof(segment));
								pack->head.request = req.request;
								pack->head.ACK = cli_ack;
								pack->head.seq = cli_seq;
		if (sendto(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&server, length) < 0) {
		        perror("Could not send datagram!!\n");
		        printf("1:%d\n",errno);
    }
		printf("Send a packet(ACK) to %s : %d\n",buff,server.sin_port);
		printf("=====Complet the three-way hand shake=====\n");

		struct timeval nNewTimeout = {0};

		for(int i=1,x=0;i<16;i*=2){
			x++;
		if(req.request & i) {
				char file[10];
				sprintf(file,"%d",x);
				strcat(file,"_copy.mp4");
				FILE* fptr1 = fopen(file,"wb");
				cli_ack = 1;
				state=ackstate;
				buffer buf[300];
				int currentbuffersize=0;
				memset(buf,0,sizeof(buf));
				sackpacket sackpack;
				int f=0;
				buf[0].exist=1;
				while(1){
					f=0;
					memset(pack,0,sizeof(segment));
					nNewTimeout.tv_sec = 3;
					setsockopt(socketfd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
					//recive data
					if ((n=nbytes = recvfrom(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) {perror ("Timeout!\n");}
					printf("Received data form %s : %d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));
					printf("\tReceive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
					memset(&sackpack,0,sizeof(sackpack));
					int seq = pack->head.seq;int ri=0;
					if(seq == cli_ack && state == ackstate){
										buf[seq].exist=1;
										ri=1;
										f = pack->head.finish;
										cli_seq = pack->head.ACK;
										cli_ack = pack->head.seq+1;
					}
					else if(seq == cli_ack && state == sackstate){
										buf[seq].exist=1;
										f = pack->head.finish;
										cli_seq = pack->head.ACK;
										if(!f) {
														int num=0;
														while(num<1004){
															fwrite(pack->data+num,sizeof(char),1,fptr1);
															num++;
														}
														for(int i=seq+1;i<300;++i){
															if(buf[i].exist){
																num=0;
																currentbuffersize--;
																while(num<1004){
																	fwrite(buf[i].data+num,sizeof(char),1,fptr1);
																	num++;
																}
															}
															else break;
														}
										}
					}
					if(seq > cli_ack && currentbuffersize < 32 && (seq - cli_ack < 32) ){
										//state = sackstate;
											buf[seq].exist=1;
											int t=0;
											int len=0;
											for(int i=0;i<299;i++){
												if(buf[i].exist == 0 && t==0){t=1;}
												if(buf[i].exist == 1 && buf[i+1].exist == 0){t=1;++len;}
												if(buf[i].exist == 1 && t==1){t=0;}
												if(buf[i].exist == 1 && t==0){}
											}
											--len;
										if(len <= 4){
											state = sackstate;
											buf[seq].exist=1;
											memcpy(buf[seq].data,pack->data,1004);
											currentbuffersize++;
										}
										else{
											buf[seq].exist=0;
										}
					}
					//deliver ack or sack
					if(state == ackstate){
										memset(pack,0,sizeof(pack));
										pack->head.seq=cli_seq;
										pack->head.ACK=cli_ack;
										if (sendto(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&server, length) < 0) {
																	perror("Could not send datagram!!\n");
																	printf("1:%d\n",errno);
										}
										printf("send ack ack_num=(%d)\n",cli_ack);
										if(f) break;
										if(ri){
										int num=0;
										while(num<1004){
											fwrite(pack->data+num,sizeof(char),1,fptr1);
											num++;
										}
									}
					 }
					else if(state == sackstate){
										memset(pack,0,sizeof(segment));
										int t=0;
										int x=0;
										len=0;
										for(int i=0;i<299;i++){
											if(buf[i].exist == 0 && t==0){cli_ack = i;t=1;x=1;}
											if(buf[i].exist == 1 && t==1){sackpack.left[len]=i;t=0;}
											if(buf[i].exist == 1 && t==0){sackpack.right[len]=i;}
											if(buf[i].exist == 1 && buf[i+1].exist == 0 && x==1){t=1;++len;}
										}
										pack->head.seq=cli_seq;
										pack->head.ACK=cli_ack;
										if(len!=0)
											pack->sack=sackpack;
										else
											state=ackstate;
										sackpack.length = len;
										pack->sack=sackpack;
										for(int i=0;i<len;i++){
											printf("left%d = %d ",i+1,sackpack.left[i]);
											printf("right%d = %d\n",i+1,sackpack.right[i]);
										}
										if (sendto(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&server, length) < 0) {
																	perror("Could not send datagram!!\n");
																	printf("1:%d\n",errno);
										}
										printf("send sack ack_num=(%d)\n",cli_ack);
					}
									for(int i=0;i<280;i++){
										printf("%d",buf[i].exist);
										if(!(i%5)) printf(" ");
									}
									printf("\n\n\n");

					}fclose(fptr1);
			}
		}
}
