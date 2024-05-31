#include "Common.h"

#define SERVERPORT 9000
#define WINSIZE 4
#define RECEIVEINTERVER 1
#define PACKINDICATOR 2
#define CHECKSUMSIZE 8
#define PAYLOADSIZE 10
#define MAXSEQNUM 50
#define MAXPACKET 50
#define MAXBUFPACK 5

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
        
		FILE* fp = fopen("output1.txt", "w");

        int window[WINSIZE] = {0,1,2,3};

        char send_ack[10];
		char last_ack[10];
		char acks[MAXPACKET][10]; for(int i=0; i<MAXPACKET; i++) strcpy(acks[i], "NULL\0");
		int is_bufferd[MAXPACKET]; for(int i=0; i<MAXPACKET; i++) is_bufferd[i]=-1;

		char buf[MAXBUFPACK][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE+1];
        char recv_packet[PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE+1];
		char recv_seq[PACKINDICATOR+1];
		char recv_checksum[CHECKSUMSIZE+1];
		char recv_contents[PAYLOADSIZE+1];

		char checksum[CHECKSUMSIZE+1];
        int time = 0;
        int seq = 0;
		int seqNum = 0;
        int is_buf = 0;
		int is_finish = 0;
		int iter = 0;
		int now_buf = 0;
		int acc = 0;
		// 클라이언트와 데이터 통신
		while (seq < MAXSEQNUM) {
            time += RECEIVEINTERVER;
			if(iter >= MAXBUFPACK) iter = 0;

			recv_packet[0] = '\0';	recv_seq[0] = '\0'; recv_checksum[0] = '\0'; recv_contents[0] = '\0';
			retval = recv(client_sock, recv_packet, PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE, 0);
            recv_packet[retval] = '\0';

			if(now_buf <= MAXBUFPACK) {
				if(now_buf < MAXBUFPACK) {
					strcpy(buf[iter++], recv_packet);
					buf[iter-1][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE] = '\0';
				}
				now_buf++;
			}
			else {
				for(int i = 0; i < PACKINDICATOR; i++) recv_seq[i] = recv_packet[i]; recv_seq[PACKINDICATOR] = '\0';
				printf("packet %d is received and dropped. ", atoi(recv_seq));
				retval = send(client_sock, last_ack, (int)strlen(last_ack), 0);
				printf("(%s) is retransmitted.\n", last_ack);
				sleep(RECEIVEINTERVER);
				continue;	
			}

			for(int i = 0; i < PACKINDICATOR; i++) recv_seq[i] = buf[iter-1][i]; recv_seq[PACKINDICATOR] = '\0';
			for(int i = PACKINDICATOR; i < PACKINDICATOR+CHECKSUMSIZE; i++) recv_checksum[i-PACKINDICATOR] = buf[iter-1][i]; recv_checksum[CHECKSUMSIZE] = '\0';
			for(int i = PACKINDICATOR+CHECKSUMSIZE; i < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; i++) recv_contents[i-(PACKINDICATOR+CHECKSUMSIZE)] = buf[iter-1][i]; recv_contents[PAYLOADSIZE] = '\0';
			if(strcmp(recv_contents, "EOFEOF") == 0) break;
			sleep(RECEIVEINTERVER);

            if(seq != atoi(recv_seq)) {
				if(strcmp(acks[atoi(recv_seq)], "NULL") != 0) {
					printf("packet %d is received and dropped. ", atoi(recv_seq));
					retval = send(client_sock, last_ack, (int)strlen(last_ack), 0);
					printf("(%s) is retransmitted.\n", last_ack);
					now_buf--;
					continue;
				}
                else {
					printf("packet %d is received and buffered ", atoi(recv_seq));
					is_bufferd[atoi(recv_seq)] = iter-1;
					acc++;
					is_buf = 1;
				}
            }
            else {
                printf("packet %d is received ", atoi(recv_seq));

				for(int i = 0; i < PAYLOADSIZE; i++) {
					if(recv_contents[i] == 'E' && recv_contents[i+1] == 'O' && recv_contents[i+2] == 'F') break;
					fputc(recv_contents[i], fp);
				}
            }
			
			char_array_as_binary(recv_contents, PAYLOADSIZE, checksum);
			if(strcmp(checksum, recv_checksum) == 0) printf("and there is no error. ");
			else printf("and there is error. ");

			for(int i = 0; i < PAYLOADSIZE; i++) {
				if(i == 0) printf("(");
				if(recv_contents[i] == 'E' && recv_contents[i+1] == 'O' && recv_contents[i+2] == 'F') {
					printf(") ");
					if(is_buf == 0) is_finish = 1;
					break;
				}
				printf("%c", recv_contents[i]);
				if(i == PAYLOADSIZE-1) printf(") ");
			}

			if(is_bufferd[seq+1] != -1) {
				for(int i = seq+1; i < seq+1+acc; i++) {
					if(i != seq+acc) printf("packet %d, ", i);
					else {
						if(acc != 1) printf("and packet %d are deliverd ", i);
						else printf("packet %d are deliverd ", i);
					}
					for(int j = PACKINDICATOR+CHECKSUMSIZE; j < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; j++) recv_contents[j-(PACKINDICATOR+CHECKSUMSIZE)] = buf[is_bufferd[i]][j]; recv_contents[PAYLOADSIZE] = '\0';
					for(int j = 0; j < PAYLOADSIZE; j++) {
						if(recv_contents[j] == 'E' && recv_contents[j+1] == 'O' && recv_contents[j+2] == 'F') {
							is_finish = 1;
							break;
						}
						fputc(recv_contents[j], fp);
					}
					seqNum += (int)strlen(recv_packet);
					sprintf(acks[i], "ACK = %3d\0", seqNum);

					now_buf--;
					is_bufferd[i] = -1;
				}
				seq += acc;
				acc = 0;
			}
			
			if(is_buf == 1) {
				retval = send(client_sock, last_ack, (int)strlen(last_ack), 0);
                printf("(%s) is retransmitted.\n", last_ack);
			}
			else {
				seqNum += (int)strlen(recv_packet);
				sprintf(send_ack, "ACK = %3d\0", seqNum);
				strcpy(acks[atoi(recv_seq)], send_ack);
				strcpy(last_ack, send_ack);
				retval = send(client_sock, send_ack, (int)strlen(send_ack), 0);
				printf("(%s) is transmitted.\n", send_ack);
				seq++;
				now_buf--;
				is_buf = 0;
			}
			
			if(is_finish == 1) break;
		}

		// 소켓 닫기
		close(client_sock);
		fclose(fp);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}

	// 소켓 닫기
	close(listen_sock);
	return 0;
}
