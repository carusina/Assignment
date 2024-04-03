#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char *argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0); // 인터넷 주소 체계의 비연결형 서비스
												  // 성공적으로 생성시 소켓 디스크립터 반환
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓 주소 구조체 초기화
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr)); // serveraddr 초기화
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr); // 서버 IP를 AF_INET 기준으로 binary 형태로 변환해서 저장
	serveraddr.sin_port = htons(SERVERPORT);

	// 데이터 통신에 사용할 변수
	struct sockaddr_in peeraddr;
	socklen_t addrlen;
	char buf[BUFSIZE + 1];
	int len;

	// 서버와 데이터 통신
	while (1) {
		int isRequest = 0;
		char fname[20];

		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = sendto(sock, buf, (int)strlen(buf), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); // 서버에게 buf byte만큼의
																											 // buf를 전달
                                                       														 // 보낸 데이터의 byte 리턴
																											 // 소켓의 지역 IP 주소와 포트 번호가
																											 // 결정되지 않은 상태면 암시적인 bind 역할
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			break;
		}
		printf("[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);
		if(strncmp("request", buf, 7) == 0) {
			isRequest = 1;
			strcpy(buf+9, fname);
			fname[retval-1] = '\0';
		}

		// 데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *)&peeraddr, &addrlen); // 서버 부터 최대 BUFSIZE byte 만큼의
																						  // 데이터를 받아 buf에 저장
																					      // 받은 데이터의 byte 리턴
																						  // 한번에 하나의 UDP 데이터만 읽을 수 있음
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			break;
		}

		// 송신자의 주소 체크
		if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) { // 데이터를 받은 주소와 서버의 주소를 비교
			printf("[오류] 잘못된 데이터입니다!\n");
			break;
		}
		
		if(isRequest == 1) {
			FILE *fp;
			printf("The client received “novel.txt” from the server.\n");

			if((fp = fopen(fname, "w")) == NULL) {
				printf("Cannt Open File\n");
				break;
			}

			fputs(buf, fp);
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// 소켓 닫기
	close(sock);
	return 0;
}
