#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE 4
#define TIMEOUTINTERVER 5

int main(int argc, char *argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // 인터넷 주소 체계의 연결형 서비스
                                                   // 성공적으로 생성시 소켓 디스크립터 반환
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr)); // serveraddr 세팅
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr); // 서버 IP를 AF_INET 기준으로 binary 형태로 변환해서 저장
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); // 서버에 연결 요청, 바로 수락되지 않고
                                                                                // 서버의 대기 큐에서 대기 중이면 블로킹 상태
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;
    
    int window[4] = {0,1,2,3};

    char* packets[8] = {"packet 0", "packet 1", "packet 2", "packet 3", "packet 4", "packet 5", "packet 6", "packet 7"};
    int packets_sendT[8] = {0,0,0,0,0,0,0,0};

    char* acks[8] = {"ACK 0", "ACK 1", "ACK 2", "ACK 3", "ACK 4", "ACK 5", "ACK 6", "ACK 7"};
    char record_acks[3][6] = {'\0', '\0', '\0', '\0', '\0', '\0'};
    int recordIndex = 0;
    int acks_recvT[8] = {0,0,0,0,0,0,0,0};

    char recv_ack[6];

    int time = 0;
    int seq = 0;
    int Rseq = -1;
    int error = 1;
	// 서버와 데이터 통신
	while (window[0] < 7) {
        time++;
        for(int i=0; i<4; i++) {
            if(window[i] == 9) window[i] = 0;
        }
        
        // printf("\n%d\t%d\n", seq, window[WINSIZE-1]);
        if(seq > window[WINSIZE-1]) {
            // printf("\n");   for(int i=0; i<4; i++) printf("%d, ", window[i]);   printf("\n");

            retval = recv(sock, recv_ack, (int)strlen(acks[seq]), MSG_WAITALL);
            recv_ack[retval] = '\0';

            int is_nextseq = 0;
            for(int i = 0; i < WINSIZE; i++) {
                if(strcmp(recv_ack, acks[window[i]]) == 0) {
                    if(window[i] == window[0]) {
                        for(int j = 0; j < WINSIZE; j++) window[j]++;
                        is_nextseq = 1;
                    }
                    Rseq = i;
                    break;
                }
            }
            acks_recvT[Rseq] = time;

            if(is_nextseq == 1) {
                printf("\"%s\" is received. ", recv_ack);

                if(record_acks[0][0] != '\0') {
                    for(int i = 0; i < 3; i++) {
                        if(record_acks[i][0] != '\0') {
                            for(int j = 0; j < WINSIZE; j++) window[j]++;
                            record_acks[i][0] = '\0';
                        }
                    }
                }
            }
            else {
                strcpy(record_acks[recordIndex++], recv_ack);
                printf("\"%s\" is received and recorded.\n", recv_ack);
                // continue;
            }
        }

        if(packets_sendT[window[0]] != 0 && time-packets_sendT[window[0]] >= TIMEOUTINTERVER) {
            printf("%s is timeout.\n", packets[window[0]]);
            
            retval = send(sock, packets[window[0]], (int)strlen(packets[window[0]]), 0);
            packets_sendT[window[0]] = time;
            printf("\"%s\" is transmitted.\n", packets[window[0]]);

            // if(record_acks[0][0] != '\0') {
            //     for(int i = 0; i < 3; i++) {
            //         if(record_acks[i][0] != '\0') {
            //             for(int j = 0; j < WINSIZE; j++) window[j] ++;
            //             record_acks[i][0] = '\0';
            //         }
            //     }
            // }
            continue;
        }
        
        // 오류 가정 시작
        if(seq == 2 && error == 1) {
            packets_sendT[seq] = time;
            printf("\"%s\" is transmitted.\n", packets[seq]);
            seq++;
            error = 0;

            sleep(1);
            continue;
        }
        // 오류 가정 끝
        
        if(seq <= window[WINSIZE-1] || seq == window[0]) {
            retval = send(sock, packets[seq], (int)strlen(packets[seq]), 0);
            packets_sendT[seq] = time;
            printf("\"%s\" is transmitted.\n", packets[seq]);
            seq++;
        }
		
        sleep(1);
	}

	// 소켓 닫기
	close(sock);
	return 0;
}
