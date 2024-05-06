#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE 4

int main(int argc, char *argv[])
{
	int retval;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); // 인터넷 주소 체계의 연결형 서비스
                                                          // 성공적으로 생성시 소켓 디스크립터 반환
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr)); // serveraddr 세팅
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소
	serveraddr.sin_port = htons(SERVERPORT); // 포트 번호
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); // 소켓에 IP와 포트 할당
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN); // 연결 요청 대기 상태, SOMAXCONN만큼의 연결 요청 대기 큐
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	socklen_t addrlen;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen); // 대기중인 클라이언트의 연결 요청을 수락
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr)); // 클라이언트 IP를 AF_INET를 기준으로 binary에서
                                                                      // 알아볼 수 있게 변환해서 저장
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
        
        int window[WINSIZE] = {0,1,2,3};

        char* packets[8] = {"packet 0", "packet 1", "packet 2", "packet 3", "packet 4", "packet 5", "packet 6", "packet 7"};
		char buf[3][9] = {'\0', '\0', '\0'};
		int bufIndex = 0;

        char* acks[8] = {"ACK 0", "ACK 1", "ACK 2", "ACK 3", "ACK 4", "ACK 5", "ACK 6", "ACK 7"};

        char recv_packet[9];

        int time = 0;
        int seq = 0;
		int Rseq = 0;
		// 클라이언트와 데이터 통신
		while (window[0] < 8) {
			for(int i=0; i<4; i++) {
				if(window[i] == 9) window[i] = 0;
			}
            time++;

			retval = recv(client_sock, recv_packet, (int)strlen(packets[seq]), 0);
			recv_packet[retval] = '\0';

			if(strcmp(packets[seq], recv_packet) == 0) {
				printf("\"%s\" is received. ", recv_packet);

				if(buf[0][0] != '\0') {
					printf("%s, ", recv_packet);
					
					for(int i = 1; i < 4; i++) {
						if(i != 3 && buf[i] != '\0') {
							printf("%s, ", buf[i-1]);
							buf[i-1][0] = '\0';
						}
						else {
							printf("and %s are delivered. ", buf[i-1]);
							
							buf[i-1][0] = '\0';
							for(int j = 0; j < WINSIZE; j++) window[j] += i;
							bufIndex = 0;
							break;
						}
					}
					int tmp = seq;
					retval = send(client_sock, acks[tmp], (int)strlen(acks[tmp]), 0);
					printf("\"%s\" is transmitted.\n", acks[tmp]);
					for(int i = 0; i < WINSIZE; i++) window[i]++;
					seq = Rseq;
					seq++;

					continue;
				}
				retval = send(client_sock, acks[seq], (int)strlen(acks[seq]), 0);
				printf("\"%s\" is transmitted.\n", acks[seq]);

				for(int i = 0; i < WINSIZE; i++) window[i]++;
				seq++;
			}
			else {
				Rseq = seq;
				strcpy(buf[bufIndex++], recv_packet);
				
				printf("\"%s\" is received and buffered. ", recv_packet);

				for(int i = 0; i < WINSIZE; i++) {
					if(strcmp(recv_packet, packets[window[i]]) == 0) break;
					Rseq++;
				}

				retval = send(client_sock, acks[Rseq], (int)strlen(acks[Rseq]), 0);
				printf("\"%s\" is transmitted.\n", acks[Rseq]);
			}
            sleep(1);
		}

		// 소켓 닫기
		close(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}

	// 소켓 닫기
	close(listen_sock);
	return 0;
}
