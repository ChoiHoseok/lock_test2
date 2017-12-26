#include <sys/msg.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>

#define RCV 1000
#define SND 500
#define SHARED_KEY 5000
#define CLIENT 3
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

int main(){

	char command;
	int lognum;
	mymsg msg;
	key_t rcvkey = SND + CLIENT;
	key_t sndkey = RCV + CLIENT;
	int rcv_qid = msgget(rcvkey,0666);
	int snd_qid = msgget(sndkey,0666);
	size_t msgsize = sizeof(mymsg) - sizeof(long);
	int checkpoint = 1;
	sharedM * msgs;

	void * shm_addr = (void*)0;

	int shm_id = shmget((key_t)SHARED_KEY, sizeof(sharedM), 0666);
	shm_addr = shmat(shm_id,(void*)0,0);
	msgs = (sharedM *)shm_addr;

	while(1){
		printf("command > ");
		scanf("%c", &command);
		getchar();
		switch(command){
			case 's':
			printf("to > ");
			scanf("%ld", &msg.mtype);
			getchar();
			scanf("%[^\n]", msg.mtext);
			getchar();
			msgsnd(snd_qid, &msg, msgsize, 0);
			break;
			case 'r':
			if(msgrcv(rcv_qid,&msg,msgsize,0,IPC_NOWAIT) != -1){
				printf("[%ld] ",msg.mtype);
				printf("%s\n",msg.mtext);
			}else{
				printf("no message\n");
			}
			break;
			case 'c':
			if(msgs->msg_start < checkpoint && checkpoint < msgs->msg_end){
				printf("[%ld] ", (msgs->msgs[checkpoint]).mtype);
				printf("%s\n", (msgs->msgs[checkpoint]).mtext);
			}else if(checkpoint < msgs->msg_end && msgs->msg_end < msgs->msg_start){
				printf("[%ld] ", (msgs->msgs[checkpoint]).mtype);
				printf("%s\n", (msgs->msgs[checkpoint]).mtext);
			}else if(msgs->msg_end < msgs->msg_start && msgs->msg_start < checkpoint){
				printf("[%ld] ", (msgs->msgs[checkpoint]).mtype);
				printf("%s\n", (msgs->msgs[checkpoint]).mtext);
			}else{
				printf("no message\n");
				break;
			}
			if(checkpoint == MAX - 1){
				checkpoint = 0;
			}else{
				checkpoint++;
			}
			break;
			case 'l':
			printf("number > ");
			scanf("%d", &lognum);
			getchar();
			if(lognum >= MAX){
				printf("no message\n");
				break;
			}
			if(msgs->msg_start < lognum && lognum < msgs->msg_end){
				printf("[%ld] ", (msgs->msgs[lognum]).mtype);
				printf("%s\n", (msgs->msgs[lognum]).mtext);
			}else if(lognum < msgs->msg_end && msgs->msg_end < msgs->msg_start){
				printf("[%ld] ", (msgs->msgs[lognum]).mtype);
				printf("%s\n", (msgs->msgs[lognum]).mtext);
			}else if(msgs->msg_end < msgs->msg_start && msgs->msg_start < lognum){
				printf("[%ld] ", (msgs->msgs[lognum]).mtype);
				printf("%s\n", (msgs->msgs[lognum]).mtext);
			}else{
				printf("no message\n");
			}
			break;
			default:
			break;
		}
	}
}