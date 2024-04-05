#include <sys/types.h> // basic type definitions
#include <sys/socket.h> // socket(), AF_INET, ...
#include <arpa/inet.h> // htons(), htonl(), ...
#include <netdb.h> // gethostbyname(), ...
#include <unistd.h> // close(), ...
#include <fcntl.h> // fcntl(), ...
#include <pthread.h> // pthread_create(), ...
#include <poll.h> // poll()
#include <sys/epoll.h> // epoll()

#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strerror(), ...
#include <errno.h> // errno

#include <pthread.h>
#include <termio.h>
int getch(void) {
	int ch;
	
	struct termios buf1;
	struct termios save;
	
	tcgetattr(0, &save);
	buf1 = save;
	
	buf1.c_lflag &= ~(ICANON|ECHO);
	buf1.c_cc[VMIN] = 1;
	buf1.c_cc[VTIME] = 0;
	
	tcsetattr(0, TCSAFLUSH, &buf1);
	
	ch = getchar();
	
	tcsetattr(0, TCSAFLUSH, &save);
	
	return ch;
}

// Windows 소켓 코드와 호환성을 위한 정의
typedef int SOCKET;
#define SOCKET_ERROR   -1
#define INVALID_SOCKET -1

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
