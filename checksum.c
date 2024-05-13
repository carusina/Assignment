#include <stdio.h>
#include <string.h>

void char_as_binary(char input, char* result) {
    for(int i = 0 ; i < 8 ; i++ ) {
        char shift_byte = 0x01 << (7-i);
        if (shift_byte & input) strcat(result, "1");
        else strcat(result, "0");
    }
}

void char_array_as_binary(char* input, int N, char* result) {
    char sum[9] = "00000000\0";
    char temp[8];

    for(int i = 0; i < N; i++) {
        temp[0] = '\0';
        char_as_binary(input[i], temp);
        printf("temp: %s\n", temp);

        int is_up = 0;
        for(int j = 7; j >= 0; j--) {
            printf("j: %d, t: %c, s: %c \n",j, temp[j], sum[j]);
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
        result[8] = '\0';
        printf("result: %s\n", result);
        for(int j = 0; j < 8; j++) {
            sum[j] = result[7-j];
        }
        printf("sum: %s\n", sum);
    }
    strcpy(result, sum);
}


void Printf_char_as_binary(char input)
{
    int i;
    printf("input(Hex): %.2x , (Binary) :");
    for( i = 0 ; i < 8 ; i++ ) {
        char shift_byte = 0x01 << (7-i);
        if (shift_byte & input) printf("1");
        else printf("0");
    }
    printf("\r\n");
}


int main() {
    char str[1] = "a";
    char result[9];

    char_array_as_binary(str,1, result);


    Printf_char_as_binary('a');
    // Printf_char_as_binary('b');
}
