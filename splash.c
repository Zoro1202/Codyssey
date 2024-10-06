#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void printha(int n, const char *printSomthing, char a);

int main(){
    char input[32];
    char input2[64];

1234-    printf("[현재 날짜를 \"yyyy-mm-dd\" 형식으로 입력하세요]: ");
    scanf("%s", input);

    printf("[당신의 이름을 입력하세요]: ");
    scanf("%s", input2);

    printf("**입력이 정상적으로 처리되었습니다.**\n");
    
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    
    printha(0, "                            [마그라테아 ver 0.1]                           ", '*');
    printha(1, "         풀 한 포기 없는 황무지에서 반짝이는 행성을 만들내는 곳 마그라테아,", '*');
    printha(2, "    사람들이 보지 못하는 잠재력을 찾고 전문가의 손길을 더해 보석을 빗는 곳,", '*');
    printha(3, "            마그라테아에 오신걸 환영합니다.                                ", '*');
    printha(4, "                                                                           ", '*');
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("[사용자]: %s                                   [실행 시간]:",input2);

    char *temp = strtok(input, "-");
    int count = 0;

    while (temp != NULL)
        {
            if(count == 0)
                printf("%s년 ", temp);
            else if(count == 1)
                printf("%s월 ", temp);
            else if(count == 2)
                printf("%s일 ", temp);
            else;
            temp = strtok(NULL, "-");
            count++;
        }
    
    printf("\n=================================================================================");

}

void printha(int n, const char *printSomthing, char a){
    for (int i = 0; i <= n; i++) {
        printf("%c",a);
    }
        printf("%s", printSomthing);
    for (int i = 5; i > n; i--){
        printf("%c",a);
    }
    printf("\n");
}