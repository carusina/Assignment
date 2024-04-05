#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

char buf[BUFSIZE + 1];
typedef struct Client
{
	SOCKET client_sock;
};

// void *send_msg(void * arg) {
// 	Client *client = (Client *)arg;
// 	SOCKET sock = client->client_sock;

// 	while(1) {
// 		// 데이터 입력
// 		// printf("\n[보낼 데이터] ");
// 		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
// 			break;

// 		// '\n' 문자 제거
// 		int len = (int)strlen(buf);
// 		if (buf[len - 1] == '\n')
// 			buf[len - 1] = '\0';
// 		if (strlen(buf) == 0)
// 			break;

// 		// 데이터 보내기
// 		int retval = send(sock, buf, len, 0); // 클라이언트에게 retval byte만큼의 buf를 전달
// 											  // 보낸 데이터의 byte 리턴
// 		if (retval == SOCKET_ERROR) {
// 			err_display("send()");
// 			break;
// 		}
// 	}
// 	pthread_exit(NULL);
// }

void *send_msg(void * arg) {
	Client *client = (Client *)arg;
	SOCKET sock = client->client_sock;

	while(1) {
		// 데이터 입력
		// printf("\n[보낼 데이터] ");
		char ch[2];
		buf[0] = '\0';
		while((ch[0] = getch()) != '\n') {
			strcat(buf, ch);
		}
		printf("server: %s\n", buf);

		// '\n' 문자 제거
		int len = (int)strlen(buf);
		// if (buf[len - 1] == '\n')
		// 	buf[len - 1] = '\0';
		// if (strlen(buf) == 0)
		// 	break;

		// 데이터 보내기
		int retval = send(sock, buf, len, 0); // 클라이언트에게 retval byte만큼의 buf를 전달
											  // 보낸 데이터의 byte 리턴
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}
	pthread_exit(NULL);
}


void *recv_msg(void *arg) {
	Client *client = (Client *)arg;
	SOCKET sock = client->client_sock;

	while(1) {
		// 데이터 받기
		buf[0] = '\0';
		int retval = recv(sock, buf, BUFSIZE, 0); // 클라이언트으로 부터 촤대 BUFSIZE byte만큼의 데이터를 받아 buf에 저장
												  // 받은 데이터의 byte 리턴
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0'; // 받은 데이터의 끝을 명시
		printf("client: %s\n", buf);
	}
	pthread_exit(NULL);
}


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

	pthread_t sendT;
	pthread_t recvT;

	while (1) {
		Client client;
		// accept()
		addrlen = sizeof(clientaddr);
		client.client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen); // 대기중인 클라이언트의 연결 요청을 수락
		if (client.client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr)); // 클라이언트 IP를 AF_INET를 기준으로 binary에서
                                                                      // 알아볼 수 있게 변환해서 저장
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		// while (1) {
		// 	// 데이터 받기
		// 	retval = recv(client_sock, buf, BUFSIZE, 0); // 클라이언트으로 부터 촤대 BUFSIZE byte만큼의 데이터를 받아 buf에 저장
        //                                                  // 받은 데이터의 byte 리턴
		// 	if (retval == SOCKET_ERROR) {
		// 		err_display("recv()");
		// 		break;
		// 	}
		// 	else if (retval == 0)
		// 		break;

		// 	// 받은 데이터 출력
		// 	buf[retval] = '\0'; // 받은 데이터의 끝을 명시
		// 	printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// 	// 데이터 보내기
		// 	retval = send(client_sock, buf, retval, 0); // 클라이언트에게 retval byte만큼의 buf를 전달
        //                                                 // 보낸 데이터의 byte 리턴
		// 	if (retval == SOCKET_ERROR) {
		// 		err_display("send()");
		// 		break;
		// 	}
		// }

		pthread_create(&sendT, NULL, send_msg, (void *)&client);
		pthread_create(&recvT, NULL, recv_msg, (void *)&client);

		pthread_join(sendT, NULL);
		pthread_join(recvT, NULL);

		// 소켓 닫기
		close(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}

	// 소켓 닫기
	close(listen_sock);
	return 0;
}
