#include "Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512
#define WINSIZE 4
#define TIMEOUTINTERVER 5
#define SENDINTERVER 0.5
#define PAYLOADSIZE 10
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

    char ENDPACK[PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE+1] = "9999999999EOFEOFEOF\0";
    char packets[MAXPACKET][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE+1];
    int packets_sendT[MAXPACKET]; for(int i=0; i<MAXPACKET; i++) packets_sendT[i]=0;
    char checksum[CHECKSUMSIZE+1];
    char send_contents[PAYLOADSIZE];
    FILE* fp = fopen("input1.txt", "r");

    int last_seq = 0;
    while(1) {
        send_contents[0] = '\0';
        int finish = read_contents(fp, send_contents);
        char_array_as_binary(send_contents, PAYLOADSIZE, checksum);
        sprintf(packets[last_seq], "%2d%s%s", last_seq, checksum, send_contents);
        packets[last_seq++][PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE] = '\0';

        if(finish == 1) {
            fclose(fp);
            break;
        }
    }
    
    char ENDACK[10] = "ACK = END";
    char acks[MAXPACKET][10];
    int acks_recvT[MAXPACKET]; for(int i=0; i<MAXPACKET; i++) acks_recvT[i]=0;
    int ack_recvT = 0;

    char recv_ack[10];
    // char last_ack[10] = "NULL";
    int last_ack_seq = 0;

    int time = 0;
    int seq = 0;
    int cwnd = 1;
    int ssthresh = 999;
    int seqNum = 0;
    int is_dupl = 0;
    int is_EOF = 0;
    int cnt = 0;
    int acc = 0;
    int is_fault = 0;
	while (seq < MAXSEQNUM) {
        cnt = 0;
        while(cnt < cwnd) {
            if(is_EOF == 1) break;

            for(int i = 0; i < seq; i++) {
                if(acks_recvT[i] == 0 && time-packets_sendT[i] >= TIMEOUTINTERVER) {
                    printf("packet %d is timeout.\n", i);
                    time += SENDINTERVER;

                    retval = send(sock, packets[i], (int)strlen(packets[i]), 0);
                    packets_sendT[i] = time;
                    printf("packet %d is retransmitted ", i);
                    for(int j = PACKINDICATOR+CHECKSUMSIZE; j < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; j++) {
                        if(j == PACKINDICATOR+CHECKSUMSIZE) printf("(");
                        if(packets[i][j] == '$') {
                            printf(").\n");
                            break;
                        }
                        printf("%c", packets[i][j]);
                        if(j == PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE-1) printf(").\n");
                    }
                    is_fault = 1;
                    ssthresh = cnt/2;
                    usleep(500000);
                    break;
                }
            }
            if(is_fault == 1) {
                cwnd = 1;
                is_fault = 0;
                break;
            }

            time += SENDINTERVER;
            
            retval = send(sock, packets[seq], (int)strlen(packets[seq]), 0);
            packets_sendT[seq] = time;
            printf("packet %d is transmitted ", seq);
            for(int i = PACKINDICATOR+CHECKSUMSIZE; i < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; i++) {
                if(i == PACKINDICATOR+CHECKSUMSIZE) printf("(");
                if(packets[seq][i] == '$') {
                    printf(").\n");
                    break;
                }
                printf("%c", packets[seq][i]);
                if(i == PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE-1) printf(").\n");
            }

            usleep(500000);
            seqNum += (PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE);
            sprintf(acks[seq], "ACK = %3d\0", seqNum);

            cnt++;
            seq++;
            if(seq == last_seq) is_EOF = 1;
        }
        
        cnt = 0;
        while(cnt < cwnd) {
            for(int i = 0; i < seq; i++) {
                if(acks_recvT[i] == 0 && time-packets_sendT[i] >= TIMEOUTINTERVER) {
                    printf("packet %d is timeout.\n", i);
                    time += SENDINTERVER;

                    retval = send(sock, packets[i], (int)strlen(packets[i]), 0);
                    packets_sendT[i] = time;
                    printf("packet %d is retransmitted ", i);
                    for(int j = PACKINDICATOR+CHECKSUMSIZE; j < PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE; j++) {
                        if(j == PACKINDICATOR+CHECKSUMSIZE) printf("(");
                        if(packets[i][j] == '$') {
                            printf(").\n");
                            break;
                        }
                        printf("%c", packets[i][j]);
                        if(j == PACKINDICATOR+CHECKSUMSIZE+PAYLOADSIZE-1) printf(").\n");
                    }
                    is_fault = 1;
                    ssthresh = cnt/2;
                    usleep(500000);
                    break;
                }
            }
            if(is_fault == 1) {
                cwnd = 1;
                is_fault = 0;
            }

            time += SENDINTERVER*2;

            retval = recv(sock, recv_ack, (int)strlen(acks[0]), MSG_WAITALL);
            recv_ack[retval] = '\0';
            sleep(SENDINTERVER*2);
            for(int i = 0; i < seq; i++) {
                if(strcmp(recv_ack, acks[i]) == 0) {
                    acks_recvT[i] = time;
                    if(i == last_ack_seq) {
                        last_ack_seq += (acc+1);
                        acc = 0;
                        printf("(%s) is received.\n", recv_ack);
                    }
                    else {
                        printf("(%s) is received and recorded.\n", recv_ack);
                        acc++;
                    }
                }
            }
            if(strcmp(recv_ack, acks[seq-1]) == 0) break;

            cnt++;
        }

        if(strcmp(recv_ack, acks[last_seq-1]) == 0) {
            retval = send(sock, ENDPACK, (int)strlen(ENDPACK), 0);

            while(1) {
                retval = recv(sock, recv_ack, (int)strlen(acks[0]), MSG_WAITALL);
                recv_ack[retval] = '\0';

                if(strcmp(recv_ack, ENDACK) == 0) break;
            }
            break;
        }

        if(cwnd < ssthresh) {
            if(cwnd*2 <= ssthresh) cwnd *= 2;
            else cwnd = ssthresh;
        }
        else cwnd++;
	}

	// 소켓 닫기
	close(sock);
	return 0; 
}
