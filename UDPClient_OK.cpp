#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

char buf[BUFSIZE + 1];
typedef struct Server
{
	SOCKET sock;
	struct sockaddr_in serveraddr;
	struct sockaddr_in peeraddr;
	socklen_t addrlen;
};

void *send_msg(void * arg) {
	Server *server = (Server *)arg;

	while(1) {
		char ch[2];
		buf[0] = '\0';
		while((ch[0] = getch()) != '\n') {
			strcat(buf, ch);
		}
		printf("client: %s\n", buf);

		int len = (int)strlen(buf);

		// 데이터 보내기
		int retval = sendto(server->sock, buf, len, 0,
			(struct sockaddr *)&server->peeraddr, sizeof(server->peeraddr));
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			break;
		}
	}
	pthread_exit(NULL);
}


void *recv_msg(void *arg) {
	Server *server = (Server *)arg;

	while(1) {
		// 데이터 받기
		buf[0] = '\0';

		server->addrlen = sizeof(server->peeraddr);
		int retval = recvfrom(server->sock, buf, BUFSIZE, 0,
			(struct sockaddr *)&server->peeraddr, &server->addrlen);
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			break;
		}

		if (memcmp(&server->peeraddr, &server->serveraddr, sizeof(server->peeraddr))) { // 데이터를 받은 주소와 서버의 주소를 비교
			printf("[오류] 잘못된 데이터입니다!\n");
			break;
		}

		// 받은 데이터 출력
		buf[retval] = '\0'; // 받은 데이터의 끝을 명시
		printf("server: %s\n", buf);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	Server server;
	pthread_t sendT;
	pthread_t recvT;
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 소켓 생성
	server->sock = socket(AF_INET, SOCK_DGRAM, 0); // 인터넷 주소 체계의 비연결형 서비스
												  // 성공적으로 생성시 소켓 디스크립터 반환
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓 주소 구조체 초기화
	memset(&server->serveraddr, 0, sizeof(server->serveraddr)); // serveraddr 초기화
	server->serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &server->serveraddr.sin_addr); // 서버 IP를 AF_INET 기준으로 binary 형태로 변환해서 저장
	server->serveraddr.sin_port = htons(SERVERPORT);

	// 클라이언트와 데이터 통신
	pthread_create(&sendT, NULL, send_msg, (void *)&client);
	pthread_create(&recvT, NULL, recv_msg, (void *)&client);

	pthread_join(sendT, NULL);
	pthread_join(recvT, NULL);

	// 소켓 닫기
	close(sock);
	return 0;
}
