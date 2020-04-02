#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#define MSS 1024 //bytes
#define RTT 200  //ms
#define rwnd 10240 //bytes
#define slow_start 1
#define congestion_avoidance 2
#define fast_recovery 3
static int ssthresh = 16 ;//KB
static int port;  //Listening server port
typedef struct sackpack{
	int length;
	int left[4];
	int right[4];
}sackpacket;
typedef struct TCP_header{
	uint32_t seq;
	uint32_t ACK;
	int request;
	int finish;
	uint16_t source_port;
	uint16_t dest_port;
}TCP_header;                    //5*4 = 20 bytes
typedef struct segment{
	struct TCP_header head;
	sackpacket sack;
	char data[1004];
}segment;
typedef struct req{
	unsigned request:5;
}Req;

char video1[123*1004],video2[45*1004],video3[257*1004],video4[204*1004];
int portlist[200];

typedef struct argu{
	struct sockaddr_in cli;
	int seq;
}argu;
struct argu queue[250];
void enqueue(struct sockaddr_in a,int seq);
struct argu dequeue();
int head,tail;
void transmit(int fd,int ser_seq,int ser_ack,int video,struct sockaddr_in client);

int congestion_control(int duplicate, int newack ,int timeout);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* child(){
	struct sockaddr_in server,client;int length=sizeof(struct sockaddr);
	int se;
	while(1){
				while(1){
					sleep(3);
					//printf("queue: head=%d, tail=%d.\n",head,tail);
					//for(int i=head; i<tail ;i++) printf("%d ",queue[i].seq);
					pthread_mutex_lock( &mutex1 );
					if(head!=tail) {
						struct argu tem =	dequeue();
						memcpy(&client,&tem.cli,sizeof(struct sockaddr_in));
						se=tem.seq;
						pthread_mutex_unlock( &mutex1 );
						break;
					}
					else
						pthread_mutex_unlock( &mutex1 );
				}       Req req;req.request=0;
								int sockfd, n;
								int nbytes;
								memset(&server, 0, sizeof(server));
								if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
									printf("socket");
								int addport;
								for(int i=1;i<200;i++){
									if(portlist[i]==0) {portlist[i]=1;addport=i;break;}
								}
								server.sin_family = AF_INET;
								server.sin_port = port+addport;
								server.sin_addr.s_addr = INADDR_ANY;
								if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1)
									printf("bind\n");
								printf("=====Start the three-way handshake=====\n");
												uint32_t ser_seq = se;
												uint32_t ser_ack = 0;
												int len=20;
												char buffer[len];
												inet_ntop(AF_INET, &(client.sin_addr), buffer, len);
												printf("Recive a packet(SYN) from %s : %d\n",buffer, client.sin_port);
												printf("\t Recive a packet (seq_num = %u, ack_num = %u)\n",ser_seq,ser_ack);
								/* return to client */
												struct segment *pack = (struct segment *) malloc (sizeof(struct segment));
												memset(pack,0,sizeof(struct segment));
												pack->head.seq = 0;
												pack->head.ACK = se+1;
												pack->head.finish = 0;
								if ((n=sendto(sockfd, pack, sizeof(segment), 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");

												printf("Send a packet(SYN/ACK) to %s : %d\n",buffer,client.sin_port);
												memset(pack,0,sizeof(struct segment));
								if (recvfrom(sockfd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0) perror ("could not read datagram!!\n");
												ser_seq = pack->head.ACK;
												ser_ack = pack->head.seq+1;
												req.request = pack->head.request;
								printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
								printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
								printf("=====Complet the three-way handshake=====\n");
								/*start to transmit video*/
								pid_t pid;
								pid = fork();
								printf("request=%d\n",req.request);
								if(pid==0){
									int x=1;
									for(int i=1;i<16;i*=2){
										if(req.request & i){
											printf("%d ",x);
											transmit(sockfd,ser_seq,ser_ack,x,client);
										}
										x++;
									}
							  }
								else if(pid>0)
									waitpid(pid,NULL,0);
								portlist[addport]=0;
								printf("thread finish \n");
				}
}
int main(int argc,char* argv[]){
	head=0;tail=0;
	struct sockaddr_in Server,client;
	int socketfd;
	int length=sizeof(struct sockaddr);
	memset(portlist,0,sizeof(portlist));
	port=atoi(argv[1]);
	memset(&Server, 0, sizeof(Server));
	memset(&client, 0, sizeof(client));
	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");
	Server.sin_family = AF_INET;
	Server.sin_port = port;
	Server.sin_addr.s_addr = INADDR_ANY;
	if(bind(socketfd, (struct sockaddr *) &Server, sizeof(Server)) == -1)
		printf("bind\n");
  //prepare data
	FILE* fptr1 = fopen("1.mp4","rb");
	FILE* fptr2 = fopen("2.mp4","rb");
	FILE* fptr3 = fopen("3.mp4","rb");
	FILE* fptr4 = fopen("4.mp4","rb");
	memset(video1,0,sizeof(video1));
	memset(video2,0,sizeof(video2));
	memset(video3,0,sizeof(video3));
	memset(video4,0,sizeof(video4));
	int num=0;
	while(!feof(fptr1)){
  	fread(video1+num,sizeof(char),1,fptr1);num++;
}
	num=0;
	while(!feof(fptr2)){
  	fread(video2+num,sizeof(char),1,fptr2);num++;
}
	num=0;
	while(!feof(fptr3)){
  	fread(video3+num,sizeof(char),1,fptr3);num++;
}
	num=0;
	while(!feof(fptr4)){
  	fread(video4+num,sizeof(char),1,fptr4);num++;
}

pthread_t t[300];
for(int i=0;i<300;i++){
	pthread_create(t+i,NULL,child,NULL);
}

  printf("============================\n");
	printf("Listening for client...\n");
		/*recive SYN from client */
	 segment *pack = (segment*) malloc (sizeof(segment));
	 memset(queue,0,sizeof(queue));
while(1){
  		if (recvfrom(socketfd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0)
                        perror ("could not read datagram!!");
									pthread_mutex_lock( &mutex1 );
									if(pack->head.ACK == 0)
											enqueue(client,pack->head.seq);
									pthread_mutex_unlock( &mutex1 );
									memset(pack,0,sizeof(segment));
		}
				return 0;
}
void enqueue(struct sockaddr_in a, int seq){
			printf("Enqueue one element\n");
			memcpy(&queue[tail].cli,&a,sizeof(struct sockaddr_in));
			queue[tail].seq=seq;
		  tail++;
}
struct argu dequeue(){
			printf("There is one pthread do dequeue\n");
			struct argu tem;
			memcpy(&tem,queue+head,sizeof(struct argu));
			head++;
			return tem;
}
int congestion_control(int duplicate, int newack ,int timeout){
	static int cwnd = 1, congestion_control_state = slow_start , dup_ack = 0 ;
			if(congestion_control_state == slow_start){
			if(duplicate)
				dup_ack ++ ;
			if(newack){
				cwnd += newack;
				dup_ack = 0;
			}
			if(timeout){
				ssthresh = cwnd / 2;
				if(ssthresh < 1) ssthresh = 1;
				cwnd = 1;
				dup_ack = 0;
			}
			if(cwnd >= ssthresh){
				printf("========Go to congestion_avoidance=======\n");
				congestion_control_state = congestion_avoidance;
			}
			if(dup_ack == 3){
				ssthresh = cwnd/2;
				if(ssthresh < 1) ssthresh = 1;
				cwnd = ssthresh + 3;
				printf("=========Go to fast_recovery========\n");
				congestion_control_state = fast_recovery;
			}
		}
		else if(congestion_control_state == congestion_avoidance){
			if(newack == cwnd){
				cwnd ++ ;
				dup_ack = 0;
			}
			if(duplicate)
				dup_ack ++ ;
			if(timeout){
				ssthresh = cwnd/2;
				if(ssthresh < 1) ssthresh = 1;
				cwnd = 1;
				dup_ack=0;
				printf("=========Go to slow_start========\n");

				congestion_control_state = slow_start;
			}
			if(dup_ack == 3){
				ssthresh = cwnd/2;
				if(ssthresh < 1) ssthresh = 1;
				cwnd = ssthresh + 3;
				printf("=========Go to fast_recovery========.\n");

				congestion_control_state = fast_recovery;
			}
		}
		else if(congestion_control_state == fast_recovery){
			if(duplicate)
				cwnd ++ ;
			if(timeout){
				ssthresh = cwnd / 2;
				if(ssthresh < 1) ssthresh = 1;
				cwnd = 1;
				dup_ack = 0;
				printf("=========Go to slow_start==========..\n");
				congestion_control_state = slow_start;
			}
			if(newack){
				cwnd = ssthresh;
				dup_ack = 0;
				printf("==========Go to congestion_avoidance========..\n");
				congestion_control_state = congestion_avoidance;
			}
		}
		return cwnd;
}
void transmit(int fd,int ser_seq,int ser_ack,int video,struct sockaddr_in client){

	printf("start to transmit  %d.mp4\n",video);
		struct segment *pack = (struct segment *) malloc (sizeof(struct segment));
		memset(pack,0,sizeof(segment));
		int length=sizeof(struct sockaddr);
		int cwnd = 1; //send one packet in one time
		int len=20,n;
		char buffer[len];
		inet_ntop(AF_INET, &(client.sin_addr), buffer, len);
		int Max_ack=1;int Max_seq=ser_ack -1;
		struct timeval nNewTimeout = {0};
		nNewTimeout.tv_usec = 10000;
		setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
		int num=0;int times=0;
		int duplicate,newack,timeout;
		srand(time(NULL));
		num=0;times=0;
		char isack[300];
		memset(isack,0,sizeof(isack));
		isack[0]=1;

	switch (video){
		case 1:
			while(1){
				for(int i=0;i<125;i++){
					printf("%d",isack[i]);
					if(!(i%5)) printf(" ");
				}
				printf("\n");
				duplicate = 0;
				newack = 0;
				timeout = 0;
				printf("Max_ack=%d\n",Max_ack);
				int tcwnd=cwnd;
				ser_seq = Max_ack;
				ser_ack = Max_seq+1;
				num = (Max_ack-1) * 1004;
				times = Max_ack;
				for(int z=0;z<tcwnd;z++){
					memset(pack,0,sizeof(segment));
					int i=0;
					if(times<124){
					while(i<1004){
						pack->data[i]=video1[num];num++;i++;
						}
					}
					pack->head.seq=ser_seq++;
					pack->head.ACK=ser_ack++;
					if(pack->head.seq > 124) pack->head.seq=124;
					if(times>=124){
						pack->head.finish=1;
					}
					else
						pack->head.finish=0;
					times++;
					printf("cwnd = %d\n",cwnd);
					int loss = (rand()%20);
					if(isack[ser_seq-1]) {z--;continue;}
					if(loss == 5) printf("lose %d \n",ser_seq-1);
					if(loss != 5){
						if ((n=sendto(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
						else
							{memset(pack,0,sizeof(segment));printf("Send a packet to %s : %d (seq_num=%d, ack_num=%d)\n",buffer,client.sin_port,ser_seq-1,ser_ack-1);}
					}
					if(times>125) break;
			 }
			 		ser_seq--;
					ser_ack--;

					for(int i=0;i<tcwnd;i++){
						duplicate=0;
						newack=0;
						timeout=0;
						if (recvfrom(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0) {
							//timeout
							timeout=1;
							printf("No recive ACK %d\n",ser_seq+1);
							continue;
						}
						isack[pack->head.ACK-1]=1;
						int sacklen = pack->sack.length;
						for(int i=0;i<sacklen;i++){
							for(int x=0;x<300;x++){
									if(pack->sack.left[i] <= x && x <= pack->sack.right[i] || x < Max_ack)
										isack[x]=1;
							}
						}
						  if(Max_ack == pack->head.ACK) duplicate = 1;
							if(Max_ack < pack->head.ACK){
								newack = pack->head.ACK - Max_ack;
								Max_ack = pack->head.ACK;
								Max_seq = pack->head.seq;
							}
						printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
						printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
						cwnd = congestion_control(duplicate, newack, timeout);
						if(pack->head.ACK == ser_seq + 1) break;
					}
							if(Max_ack >= 125 || pack->head.ACK >= 125) break;
		}memset(pack,0,sizeof(pack));
		printf("=============transmit Complete=============\n");
		 break;
		case 2:
		while(1){
				for(int i=0;i<47;i++){
					printf("%d",isack[i]);
					if(!(i%5)) printf(" ");
				}
				printf("\n");
				duplicate = 0;
				newack = 0;
				timeout = 0;
				printf("Max_ack=%d\n",Max_ack);
				int tcwnd=cwnd;
				ser_seq = Max_ack;
				ser_ack = Max_seq+1;
				num = (Max_ack-1) * 1004;
				times = Max_ack;
				for(int z=0;z<tcwnd;z++){
					memset(pack,0,sizeof(segment));
					int i=0;
					if(times<46){
					while(i<1004){
						pack->data[i]=video2[num];num++;i++;
						}
					}
					pack->head.seq=ser_seq++;
					pack->head.ACK=ser_ack++;
					if(pack->head.seq > 46) pack->head.seq=46;
					if(times>=46){
						pack->head.finish=1;
					}
					else
						pack->head.finish=0;
					times++;
					printf("cwnd = %d\n",cwnd);
					int loss = (rand()%100);
					if(isack[ser_seq-1]) {z--;continue;}
					if(loss == 5) printf("lose %d \n",ser_seq-1);
					if(loss != 5){
						if ((n=sendto(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
						else
							{memset(pack,0,sizeof(segment));printf("Send a packet to %s : %d (seq_num=%d, ack_num=%d)\n",buffer,client.sin_port,ser_seq-1,ser_ack-1);}
					}
					if(times>47) break;
			 }
			 		ser_seq--;
					ser_ack--;

					for(int i=0;i<tcwnd;i++){
						duplicate=0;
						newack=0;
						timeout=0;
						if (recvfrom(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0) {
							//timeout
							timeout=1;
							printf("No recive ACK %d\n",ser_seq+1);
							continue;
						}
						isack[pack->head.ACK-1]=1;
						int sacklen = pack->sack.length;
						for(int i=0;i<sacklen;i++){
							for(int x=0;x<300;x++){
									if(pack->sack.left[i] <= x && x <= pack->sack.right[i] || x < Max_ack)
										isack[x]=1;
							}
						}
						  if(Max_ack == pack->head.ACK) duplicate = 1;
							if(Max_ack < pack->head.ACK){
								newack = pack->head.ACK - Max_ack;
								Max_ack = pack->head.ACK;
								Max_seq = pack->head.seq;
							}
						printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
						printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
						cwnd = congestion_control(duplicate, newack, timeout);
						if(pack->head.ACK == ser_seq + 1) break;
					}
							if(Max_ack >= 47 || pack->head.ACK >= 47) break;
		}memset(pack,0,sizeof(pack));
		printf("=============transmit Complete=============\n");
		break;
		case 3:
		while(1){
				for(int i=0;i<259;i++){
					printf("%d",isack[i]);
					if(!(i%5)) printf(" ");
				}
				printf("\n");
				duplicate = 0;
				newack = 0;
				timeout = 0;
				printf("Max_ack=%d\n",Max_ack);
				int tcwnd=cwnd;
				ser_seq = Max_ack;
				ser_ack = Max_seq+1;
				num = (Max_ack-1) * 1004;
				times = Max_ack;
				for(int z=0;z<tcwnd;z++){
					memset(pack,0,sizeof(segment));
					int i=0;
					if(times<258){
					while(i<1004){
						pack->data[i]=video3[num];num++;i++;
						}
					}
					pack->head.seq=ser_seq++;
					pack->head.ACK=ser_ack++;
					if(pack->head.seq > 258) pack->head.seq=258;
					if(times>=258){
						pack->head.finish=1;
					}
					else
						pack->head.finish=0;
					times++;
					printf("cwnd = %d\n",cwnd);
					int loss = (rand()%20);
					if(isack[ser_seq-1]) {z--;continue;}
					if(loss == 5) printf("lose %d \n",ser_seq-1);
					if(loss != 5){
						if ((n=sendto(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
						else
							{memset(pack,0,sizeof(segment));printf("Send a packet to %s : %d (seq_num=%d, ack_num=%d)\n",buffer,client.sin_port,ser_seq-1,ser_ack-1);}
					}
					if(times>259) break;
			 }
			 		ser_seq--;
					ser_ack--;

					for(int i=0;i<tcwnd;i++){
						duplicate=0;
						newack=0;
						timeout=0;
						if (recvfrom(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0) {
							//timeout
							timeout=1;
							printf("No recive ACK %d\n",ser_seq+1);
							continue;
						}
						isack[pack->head.ACK-1]=1;
						int sacklen = pack->sack.length;
						for(int i=0;i<sacklen;i++){
							for(int x=0;x<300;x++){
									if(pack->sack.left[i] <= x && x <= pack->sack.right[i] || x < Max_ack)
										isack[x]=1;
							}
						}
						  if(Max_ack == pack->head.ACK) duplicate = 1;
							if(Max_ack < pack->head.ACK){
								newack = pack->head.ACK - Max_ack;
								Max_ack = pack->head.ACK;
								Max_seq = pack->head.seq;
							}
						printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
						printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
						cwnd = congestion_control(duplicate, newack, timeout);
						if(pack->head.ACK == ser_seq + 1) break;
					}
							if(Max_ack >= 259 || pack->head.ACK >= 259) break;
		}memset(pack,0,sizeof(pack));
		printf("=============transmit Complete=============\n");
		 break;
		case 4:
		while(1){
				for(int i=0;i<206;i++){
					printf("%d",isack[i]);
					if(!(i%5)) printf(" ");
				}
				printf("\n");
				duplicate = 0;
				newack = 0;
				timeout = 0;
				printf("Max_ack=%d\n",Max_ack);
				int tcwnd=cwnd;
				ser_seq = Max_ack;
				ser_ack = Max_seq+1;
				num = (Max_ack-1) * 1004;
				times = Max_ack;
				for(int z=0;z<tcwnd;z++){
					memset(pack,0,sizeof(segment));
					int i=0;
					if(times<205){
					while(i<1004){
						pack->data[i]=video4[num];num++;i++;
						}
					}
					pack->head.seq=ser_seq++;
					pack->head.ACK=ser_ack++;
					if(pack->head.seq > 205) pack->head.seq=205;
					if(times>=205){
						pack->head.finish=1;
					}
					else
						pack->head.finish=0;
					times++;
					printf("cwnd = %d\n",cwnd);
					int loss = (rand()%20);
					if(isack[ser_seq-1]) {z--;continue;}
					if(loss == 5) printf("lose %d \n",ser_seq-1);
					if(loss != 5){
						if ((n=sendto(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
						else
							{memset(pack,0,sizeof(segment));printf("Send a packet to %s : %d (seq_num=%d, ack_num=%d)\n",buffer,client.sin_port,ser_seq-1,ser_ack-1);}
					}
					if(times>205) break;
			 }
			 		ser_seq--;
					ser_ack--;

					for(int i=0;i<tcwnd;i++){
						duplicate=0;
						newack=0;
						timeout=0;
						if (recvfrom(fd, pack, sizeof(segment), 0, (struct sockaddr*)&client , &length) <0) {
							//timeout
							timeout=1;
							printf("No recive ACK %d\n",ser_seq+1);
							continue;
						}
						isack[pack->head.ACK-1]=1;
						int sacklen = pack->sack.length;
						for(int i=0;i<sacklen;i++){
							for(int x=0;x<300;x++){
									if(pack->sack.left[i] <= x && x <= pack->sack.right[i] || x < Max_ack)
										isack[x]=1;
							}
						}
						  if(Max_ack == pack->head.ACK) duplicate = 1;
							if(Max_ack < pack->head.ACK){
								newack = pack->head.ACK - Max_ack;
								Max_ack = pack->head.ACK;
								Max_seq = pack->head.seq;
							}
						printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
						printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
						cwnd = congestion_control(duplicate, newack, timeout);
						if(pack->head.ACK == ser_seq + 1) break;
					}
							if(Max_ack >= 206 || pack->head.ACK >= 206) break;
		}memset(pack,0,sizeof(pack));
		printf("=============transmit Complete=============\n");
		 break;
}
}
