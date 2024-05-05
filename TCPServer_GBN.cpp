#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE    4
#define TIMEOUTINTERVER 5

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
	char buf[BUFSIZE + 1];

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

		int time = 0;
		int window[WINSIZE] = {0,1,2,3};
		char* packets[8] = {"packet 0", "packet 1", "packet 2", "packet 3", "packet 4", "packet 5", "packet 6", "packet 7"};
		int is_dropped[8] = {0,0,0,0,0,0,0,0};
		char recv_packet[9];
		char* acks[8] = {"ACK 0", "ACK 1", "ACK 2", "ACK 3", "ACK 4", "ACK 5", "ACK 6", "ACK 7"};


		int cnt = 0;
		int p_drop = 0;
		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기
			retval = recv(client_sock, recv_packet, BUFSIZE, 0); // 클라이언트으로 부터 촤대 BUFSIZE byte만큼의 데이터를 받아 buf에 저장
                                                         		// 받은 데이터의 byte 리턴
			recv_packet[retval] = '\0';

			if(strcmp(recv_packet, packets[cnt]) == 0) {
				if(is_dropped[cnt] == 1) {
					printf("\"%s\" is received and delivered. ", recv_packet);
				}
				else {
					printf("\"%s\" is received. ", recv_packet);
				}
				retval = send(client_sock, acks[cnt], (int)strlen(acks[cnt]), 0);
				printf("\"%s\" is transmitted.\n", acks[cnt]);
				cnt++;
			}
			else {
				printf("\"%s\" is received and dropped. ", recv_packet);
				retval = send(client_sock, acks[cnt-1], (int)strlen(acks[cnt]), 0);
				printf("\"%s\" is retransmitted.\n", acks[cnt-1]);
				is_dropped[cnt+p_drop] = 1;
				is_dropped[cnt+p_drop+1] = 1;
				p_drop++;
			}
			sleep(3);
			if(cnt >= 8) break;
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
