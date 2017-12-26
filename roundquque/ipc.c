#include <sys/msg.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <string.h>

#define RCV 1000
#define SND 500
#define SHARED_KEY 5000
#define MAX 101
#define DATA 1024

typedef struct _msg{
	long mtype;
	char mtext[DATA];
}mymsg;

typedef struct _sharedM{
	mymsg msgs[MAX];
	int msg_start;
	int msg_end;
}sharedM;

int rcv_qid[4];
int snd_qid[4];
sharedM * msgs;

void * client1(void * arg);
void * client2(void * arg);
void * client3(void * arg);

void add_message(int n, mymsg msg);


int main(){
	
	int i;
	pthread_t tid[3];
	key_t rcvkey, sndkey;
	void * shm_addr = (void*)0;
	int * num;
	
	for(i = 1; i < 4; i++){
		rcvkey = RCV + i;
		sndkey = SND + i;
		rcv_qid[i] = msgget(rcvkey,IPC_CREAT|0666);
		snd_qid[i] = msgget(sndkey,IPC_CREAT|0666);
	}
	
	int shm_id; 
	if(-1 ==(shm_id = shmget((key_t)SHARED_KEY, sizeof(sharedM),IPC_CREAT|0666))){
		printf("shmget fail\n");
		return -1;
	}
	if((void*)-1 == (shm_addr = shmat(shm_id,(void*)0,0))){
		printf("shmat fail\n");
		return -1;

	}
	msgs = (sharedM *)shm_addr;

	memset(msgs,0,sizeof(sharedM));
	msgs->msg_start = 0;
	msgs->msg_end = 1;

	pthread_create(&tid[0], NULL, client1, NULL);
	pthread_create(&tid[1], NULL, client2, NULL);
	pthread_create(&tid[2], NULL, client3, NULL);

	for(i = 0; i < 3; i++){
		pthread_join(tid[i],NULL);
	}
}

void * client1(void * arg){
	size_t msgsize = sizeof(mymsg) - sizeof(long);
	int nbyte;
	int type;
	mymsg msg; 
	while(1){
		nbyte = msgrcv(rcv_qid[1], &msg, msgsize, 0, 0);
		//printf("rcv from client1\n");
		switch(msg.mtype){
			case 4:
				add_message(1,msg);
				break;
			case 1:
				msg.mtype = 1;
				msgsnd(snd_qid[1], &msg, msgsize, 0);
				break;
			case 2:
				msg.mtype = 1;
				msgsnd(snd_qid[2], &msg, msgsize, 0);
				break;
			case 3:
				msg.mtype = 1;
				msgsnd(snd_qid[3], &msg, msgsize, 0);
				break;
			default:
				break;
		}
	}
}

void * client2(void * arg){
	size_t msgsize = sizeof(mymsg) - sizeof(long);
	int nbyte;
	int type;
	mymsg msg; 
	while(1){
		nbyte = msgrcv(rcv_qid[2], &msg, msgsize, 0, 0);
		//printf("rcv from client2\n");
		switch(msg.mtype){
			case 4:
				add_message(2,msg);
				break;
			case 1:
				msg.mtype = 2;
				msgsnd(snd_qid[1], &msg, msgsize, 0);
				break;
			case 2:
				msg.mtype = 2;
				msgsnd(snd_qid[2], &msg, msgsize, 0);
				break;
			case 3:
				msg.mtype = 2;
				msgsnd(snd_qid[3], &msg, msgsize, 0);
				break;
			default:
				break;
		}
	}
}


void * client3(void * arg){
	size_t msgsize = sizeof(mymsg) - sizeof(long);
	int nbyte;
	int type;
	mymsg msg; 
	while(1){
		nbyte = msgrcv(rcv_qid[3], &msg, msgsize, 0, 0);
		//printf("rcv from client3\n");
		switch(msg.mtype){
			case 4:
				add_message(3,msg);
				break;
			case 1:
				msg.mtype = 3;
				msgsnd(snd_qid[1], &msg, msgsize, 0);
				break;
			case 2:
				msg.mtype = 3;
				msgsnd(snd_qid[2], &msg, msgsize, 0);
				break;
			case 3:
				msg.mtype = 3;
				msgsnd(snd_qid[3], &msg, msgsize, 0);
				break;
			default:
				break;
		}
	}
}

void add_message(int n, mymsg msg){
	msg.mtype = n;
	msgs->msgs[msgs->msg_end] = msg;
	if(msgs->msg_end == MAX - 1){
		msgs->msg_end = 0;
	}else{
		msgs->msg_end++;
	}
	if(msgs->msg_end == msgs->msg_start){
		if(msgs->msg_start == MAX - 1){
			msgs->msg_start = 0;
		}else{
			msgs->msg_start++;
		}
	}
	printf("[%d] ", n);
	printf("%s\n",msg.mtext);
	//printf("%d\n",msgs->msg_num);
}