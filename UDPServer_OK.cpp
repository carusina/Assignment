#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

char buf[BUFSIZE + 1];
typedef struct Client
{
	SOCKET sock;
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
};

void *send_msg(void * arg) {
	Client *client = (Client *)arg;

	while(1) {
		char ch[2];
		buf[0] = '\0';
		while((ch[0] = getch()) != '\n') {
			strcat(buf, ch);
		}
		printf("server: %s\n", buf);

		// '\n' 문자 제거
		int len = (int)strlen(buf);

		// 데이터 보내기
		int retval = sendto(client->sock, buf, len, 0,
			(struct sockaddr *)&client->clientaddr, sizeof(client->clientaddr));
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			break;
		}
	}
	pthread_exit(NULL);
}


void *recv_msg(void *arg) {
	Client *client = (Client *)arg;

	while(1) {
		// 데이터 받기
		buf[0] = '\0';

		client->addrlen = sizeof(client->clientaddr);
		int retval = recvfrom(client->sock, buf, BUFSIZE, 0,
			(struct sockaddr *)&client->clientaddr, &client->addrlen);
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			break;
		}

		// 받은 데이터 출력
		buf[retval] = '\0'; // 받은 데이터의 끝을 명시
		printf("client: %s\n", buf);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int retval;
	Client client;
	pthread_t sendT;
	pthread_t recvT;

	// 소켓 생성
	client->sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (client->sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(client->sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 클라이언트와 데이터 통신
	pthread_create(&sendT, NULL, send_msg, (void *)&client);
	pthread_create(&recvT, NULL, recv_msg, (void *)&client);

	pthread_join(sendT, NULL);
	pthread_join(recvT, NULL);

	// 소켓 닫기
	close(sock);
	return 0;
}
