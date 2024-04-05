#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

char buf[BUFSIZE + 1];
typedef struct Server
{
	SOCKET server_sock;
};

void *send_msg(void * arg) {
	Server *server = (Server *)arg;
	SOCKET sock = server->server_sock;

	while(1) {
		// 데이터 입력
		// printf("client: ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// else {
		// 	printf("client: %s\n", buf);
		// }

		// '\n' 문자 제거
		int len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

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
	Server *server = (Server *)arg;
	SOCKET sock = server->server_sock;

	while(1) {
		// 데이터 받기
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
		printf("server: %s\n", buf);
	}
	pthread_exit(NULL);
}

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
	int len;
	Server server;
	server.server_sock = sock;
	pthread_t sendT;
	pthread_t recvT;

	// 서버와 데이터 통신
	pthread_create(&sendT, NULL, send_msg, (void *)&server);
	pthread_create(&recvT, NULL, recv_msg, (void *)&server);

	pthread_join(sendT, NULL);
	pthread_join(recvT, NULL);

	// 소켓 닫기
	close(sock);
	return 0;
}
