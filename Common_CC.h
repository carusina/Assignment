#include <sys/types.h> // basic type definitions
#include <sys/socket.h> // socket(), AF_INET, ...
#include <arpa/inet.h> // htons(), htonl(), ...
#include <netdb.h> // gethostbyname(), ...
#include <unistd.h> // close(), ...
#include <fcntl.h> // fcntl(), ...
#include <pthread.h> // pthread_create(), ...
#include <poll.h> // poll()
// #include <sys/epoll.h> // epoll()

#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strerror(), ...
#include <errno.h> // errno

// Windows 소켓 코드와 호환성을 위한 정의
typedef int SOCKET;
#define SOCKET_ERROR   -1
#define INVALID_SOCKET -1
#define PAYLOAD 10

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	char *msgbuf = strerror(errno);
	printf("[%s] %s\n", msg, msgbuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	char *msgbuf = strerror(errno);
	printf("[%s] %s\n", msg, msgbuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
    char *msgbuf = strerror(errcode);
    printf("[오류] %s\n", msgbuf);
}


/*--------------------------*/
void char_as_binary(char input, char* result) {
    for(int i = 0 ; i < 8 ; i++ ) {
        char shift_byte = 0x01 << (7-i);
        if (shift_byte & input) strcat(result, "1");
        else strcat(result, "0");
    }
}

int add_binary(char temp[], char sum[], char result[]) {
    int is_up = 0;

    for(int j = 7; j > 0; j--) {
        if(temp[j] != sum[j]) {
            if(is_up == 1) {
                result[j] = '0';
                is_up = 1;
            }
            else {
                result[j] = '1';
            }
        }
        else {
            if(temp[j] == '0') {
                if(is_up == 1) {
                    result[j] = '1';;
                    is_up = 0;
                }
                else {
                    result[j] = '0';
                }
            }
            else {
                if(is_up == 1) {
                    result[j] = '1';
                    is_up = 1;
                }
                else {
                    result[j] = '0';
                    is_up = 1;
                }
            }
        }
    }
    if(temp[0] != sum[0]) {
        if(is_up == 1) {
            result[0] = '0';
            is_up = 0;
            return 1;
        }
        else {
            result[0] = '1';
            return 0;
        }
    }
    else {
        if(temp[0] == '1') {
            if(is_up == 1) {
                result[0] = '1';
                is_up = 0;
                return 1;
            }
            else {
                result[0] = '0';
                return 1;
            }
        }
        else {
            if(is_up == 1) {
                result[0] = '1';
                is_up = 0;
                return 0;
            }
            else {
                result[0] = '0';
                return 0;
            }
        }
    }
}

void complemnet(char result[]) {
    for(int i = 0; i < 8; i++) {
        if(result[i] == '0') result[i] = '1';
        else result[i] = '0';
    }
}

//인자: 입력값, 크기, 저장할 곳
void char_array_as_binary(char* input, int N, char* result) {
    char temp[9];
    char sum[10] = "00000000\0"; // 원래 9

    for(int i = 0; i < N; i++) {
        temp[0] = '\0';
        char_as_binary(input[i], temp);
        temp[8] = '\0';

        int is_over = 0;
        is_over = add_binary(temp, sum, result);


        result[8] = '\0';
        strcpy(sum, result);

        while(is_over == 1) {
            strcpy(temp, "00000001\0");
            is_over = add_binary(temp, sum, result);
            strcpy(sum, result);
        }
    }
	complemnet(sum);
    strcpy(result, sum);
}

int read_contents(FILE* fp, char pack[]) {
    char c;
    for(int index = 0; index < PAYLOAD; index++) {
        c = fgetc(fp);
        if(c == EOF) {
            if(index == 0) strcpy(pack, "EOFEOF");
            return 1;
        }
        pack[index] = c;
        if(c == '.') {
            if(index != PAYLOAD) {
                pack[++index] = 'E';
                pack[++index] = 'O';
                pack[++index] = 'F';
            }
        }
    }
    return 0;
}
