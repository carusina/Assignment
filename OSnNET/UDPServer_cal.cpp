#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    3072

int main(int argc, char *argv[])
{
	int retval;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0); // 인터넷 주소 체계의 비연결형 서비스
												  // 성공적으로 생성시 소켓 디스크립터 반환
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr)); // serveraddr 초기화
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소
	serveraddr.sin_port = htons(SERVERPORT); // 포트 번호
	retval = bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); // 소켓에 IP와 포트 할당
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 데이터 통신에 사용할 변수
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트와 데이터 통신
	while (1) {
		FILE *fp = NULL;
		// 데이터 받기
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *)&clientaddr, &addrlen); // 클라이언트로 부터 최대 BUFSIZE byte 만큼의
																							// 데이터를 받아 buf에 저장
																							// 받은 데이터의 byte 리턴
																							// 한번에 하나의 UDP 데이터만 읽을 수 있음
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			break;
		}

		// 받은 데이터 출력
		buf[retval] = '\0'; // 받은 데이터의 끝 명시
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr)); // 클라이언트 IP를 AF_INET를 기준으로 binary에서
                                                                      // 알아볼 수 있게 변환해서 저장
		printf("[UDP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		char fname[20];
		if(strncmp("request", buf, 7) == 0) {
			printf("The server received a request from a client\n");

			strcpy(fname, buf+9);
			fname[retval-10] = '\0';
			
			if((fp = fopen(fname, "r")) == NULL) {
				printf("Cannt Open File\n");
				break;
			}
			char c;
			retval = 0;
			while((c = fgetc(fp)) != EOF) {
				buf[retval++] = c;
			}
		}

		// 데이터 보내기
		retval = sendto(sock, buf, retval, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr)); // 클라이언트에게 retval byte만큼의
																								   // buf를 전달
																								   // 보낸 데이터의 byte 리턴
																								   // 소켓의 지역 IP 주소와 포트 번호가
																								   // 결정되지 않은 상태면 암시적인 bind 역할
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			break;
		}
		if(fp != NULL) {
			printf("The server sent \"%s\" to the client\n", fname);
			fclose(fp);
		}
	}
	// 소켓 닫기
	close(sock);
	return 0;
}
