#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE    4
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

	// 서버와 데이터 통신
		int time = 0;
		int window[WINSIZE] = {0,1,2,3};
		char* packets[8] = {"packet 0", "packet 1", "packet 2", "packet 3", "packet 4", "packet 5", "packet 6", "packet 7"};
		int packets_time[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		char recv_ack[6];
		char ack[6];
	
		int cnt = 0;
		int first = 1; // 오류 가정을 위함
		int ack_repeat = 0; // ack 반복 확인
		while(cnt < 8) {
			time++;
			
			int NotTimeOver = 1; // 타임 오버 확인
			if(cnt >= 4) {
				if(time - packets_time[window[0]] >= TIMEOUTINTERVER) {
						printf("%s is timeout.\n", packets[window[0]]);
						cnt = window[0];
						NotTimeOver = 0;
						first = 0;
					}

				if(NotTimeOver) {
					retval = recv(sock, recv_ack, 5, MSG_WAITALL);
					recv_ack[retval] = '\0';
					
					if(strcmp(ack, recv_ack) == 0) {
						if(ack_repeat == 0) {
							printf("\"%s\" is received and ignored.\n", ack);
							ack_repeat = 1;
						}
					}
					else {
						printf("\"%s\" is received. ", recv_ack);
						strcpy(ack, recv_ack);
						ack_repeat = 0;

						for(int i = 0; i < WINSIZE; i++) {
							window[i]++;
						}
					}
				}
			}
			
			if(cnt <= window[WINSIZE-1]) {
				// 오류 가정
				if(cnt == 2 && first == 1) {
					packets_time[cnt] = time;
					printf("\"%s\" is transmitted.\n", packets[cnt]);
					cnt++;
					sleep(3);
					continue;
				}

				retval = send(sock, packets[cnt], (int)strlen(packets[cnt]), 0);
				packets_time[cnt] = time;
				printf("\"%s\" is transmitted.\n", packets[cnt]);
				cnt++;
			}

			sleep(3);
		}

	// 소켓 닫기
	close(sock);
	return 0;
}
