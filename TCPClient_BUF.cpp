#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE 4
#define TIMEOUTINTERVER 0.5
#define SENDINTERVER 0.05
#define PAYLOADSIZE 6
#define CHECKSUMSIZE 8
#define PACKINDICATOR 2
#define MAXSEQNUM 50
#define MAXPACKET 50

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
    
    int window[WINSIZE] = {0,1,2,3};

    char packets[MAXPACKET][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE+1];
    int packets_sendT[MAXPACKET]; for(int i=0; i<MAXPACKET; i++) packets_sendT[i]=0;
    char checksum[CHECKSUMSIZE+1];
    char send_contents[PAYLOADSIZE];
    FILE* fp = fopen("text.txt", "r");

    int last_seq = 0;
    while(1) {
        int finish = read_contents(fp, send_contents);
        char_array_as_binary(send_contents, PAYLOADSIZE, checksum);
        sprintf(packets[last_seq], "%2d%s%s", last_seq, checksum, send_contents);
        packets[last_seq++][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE] = '\0';

        if(finish == 1) {
            fclose(fp);
            break;
        }
    }
    
    char acks[MAXPACKET][10];
    int acks_recvT[MAXPACKET]; for(int i=0; i<MAXPACKET; i++) acks_recvT[i]=0;
    int ack_recvT = 0;

    char recv_ack[10];
    char last_ack[10] = "NULL";

    int time = 0;
    int seq = 0;
    int seqNum = 0;
    int is_dupl = 0;
    int is_EOF = 0;
	// 서버와 데이터 통신
    for(; seq < 4; seq++) {
        time += SENDINTERVER;

        retval = send(sock, packets[seq], (int)strlen(packets[seq]), 0);
        packets_sendT[seq] = time;
        printf("packet %d is transmitted ", seq);
        for(int i = PACKINDICATOR+CHECKSUMSIZE; i < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; i++) {
            if(i == PACKINDICATOR+CHECKSUMSIZE) printf("(");
            printf("%c", packets[seq][i]);
            if(packets[seq][i] == ' ') i+=2;
            if(i == PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE-1) printf(").\n");
        }
        seqNum += (int)strlen(packets[seq]);
        sprintf(acks[seq], "ACK = %d\0", seqNum);
        sleep(SENDINTERVER);
    }
    
	while (window[0] < MAXSEQNUM) {
        time += SENDINTERVER*2;
        for(int i=0; i<WINSIZE; i++) {
            if(window[i] == MAXSEQNUM+1) window[i] = 0;
        }

        if(seq >= window[0]) {
            retval = recv(sock, recv_ack, (int)strlen(acks[window[0]]), MSG_WAITALL);
            recv_ack[retval] = '\0';

            if(strcmp(recv_ack, last_ack) != 0) {
                printf("(%s) is received. ", recv_ack);
                strcpy(last_ack, recv_ack);
                acks_recvT[seq-WINSIZE] = time;
                is_dupl = 0;

                for(int i = 0; i < WINSIZE; i++) window[i]++;
                if(seq == last_seq-1) {
                    printf("\n");
                    sleep(SENDINTERVER*2);
                    if(strcmp(acks[seq-1], recv_ack) == 0) break;
                    continue;
                }
            }
            else if(is_dupl == 0) {
                printf("(%s) is received and ignored.\n", recv_ack);
                is_dupl = 1;
            }
        }

        if((packets_sendT[window[0]] != 0 && time-packets_sendT[window[0]] >= TIMEOUTINTERVER) && acks_recvT[window[0]] == 0) {
            seq = window[0];
            printf("packet %d is timeout.\n", seq);
        }
        
		if(seq <= window[WINSIZE-1] || seq == window[0]) {
            if(seq == MAXSEQNUM) {
                sleep(SENDINTERVER*2);
                continue;
            }

            if(seq != last_seq-1) {
                retval = send(sock, packets[seq], (int)strlen(packets[seq]), 0);
                packets_sendT[seq] = time;
                printf("packet %d is transmitted ", seq);
                for(int i = PACKINDICATOR+CHECKSUMSIZE; i < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; i++) {
                    if(i == PACKINDICATOR+CHECKSUMSIZE) printf("(");
                    if(packets[seq][i] == 'E' && packets[seq][i+1] == 'O' && packets[seq][i+2] == 'F') {
                        printf(").\n");
                        break;
                    }
                    else printf("%c", packets[seq][i]);
                    if(packets[seq][i] == ' ' || packets[seq][i] == '.') i+=2;
                    if(i == PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE-1) printf(").\n");
                }
                
                seqNum += (int)strlen(packets[seq]);
                sprintf(acks[seq], "ACK = %d\0", seqNum);
                seq++;
            }
        }

        sleep(SENDINTERVER*2);
	}

	// 소켓 닫기
	close(sock);
	return 0;
}
