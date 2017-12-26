# lock_test2
system programing assinment6

파일은 총 4개입니다.

컴파일 명령어는 순서대로
gcc -o ipc ipc.c -lpthread
gcc -o client1 client1.c
gcc -o client2 client2.c
gcc -o client3 client3.c 입니다.

서버 파일에 ipc 크리에이트 명령이 있으므로 서버파일을 먼저 실행해 주세요.

서버를 실행한 후 클라이언트를 실행하면 클라이언트 프로세스에서 여러가지 커맨드를 입력할수 있습니다.

command >  라고 되어 있는 화살표 오른쪽에 명령어를 치시면 됩니다.

[명령어 설명]

's' send 명령어 입니다. s를 명령어로 입력한후 to > 가 화면에 뜨면 보낼 대상 클라이언트 번호 1 ~ 3 을 입력합니다. 만약 4를 입력할 경우 전체 메세지가 되어 
shared memory 에 저장됩니다. 보낼 대상을 선택한 후에 메세지를 입력합니다.

'r' recive 명령어 입니다. 가장 먼저 자신에게 보낸 메세지를 출력합니다.

'l' log check 명령어 입니다. 'l' 명령어 후 번호를 입력하면 해당 번호에  해당하는 전체메세지를 출력합니다.

'c' check message 명령어 입니다. 확인하지 않은 가장 먼저 저장된 전체메세지를 확인합니다.
 log check 명령어와 중복되지는 않습니다. 
