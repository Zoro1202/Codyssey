#include <stdio.h>
#include <stdlib.h>

// 후보자 정보의 속성명을 저장하는 배열
const char *member_info[11] = {
    "성명", "생일(YYYY/MM/DD 형식)", "성별(여성이면 F 또는 남성이면 M)", 
    "메일 주소", "국적", "BMI", "주 스킬", "보조 스킬", 
    "한국어 등급(TOPIK)", "MBTI", "소개"
};

// 후보자 6명의 데이터를 저장할 배열 (각 배열은 후보자 정보 11개를 저장)
char candidate01[11][100];
char candidate02[11][100];
char candidate03[11][100];
char candidate04[11][100];
char candidate05[11][100];
char candidate06[11][100];

// 후보자 데이터 배열 포인터 배열 (후보자 데이터를 순차적으로 접근할 수 있도록 함)
char *candidates[6][11] = {
    {candidate01[0], candidate01[1], candidate01[2], candidate01[3], candidate01[4], candidate01[5], candidate01[6], candidate01[7], candidate01[8], candidate01[9], candidate01[10]},
    {candidate02[0], candidate02[1], candidate02[2], candidate02[3], candidate02[4], candidate02[5], candidate02[6], candidate02[7], candidate02[8], candidate02[9], candidate02[10]},
    {candidate03[0], candidate03[1], candidate03[2], candidate03[3], candidate03[4], candidate03[5], candidate03[6], candidate03[7], candidate03[8], candidate03[9], candidate03[10]},
    {candidate04[0], candidate04[1], candidate04[2], candidate04[3], candidate04[4], candidate04[5], candidate04[6], candidate04[7], candidate04[8], candidate04[9], candidate04[10]},
    {candidate05[0], candidate05[1], candidate05[2], candidate05[3], candidate05[4], candidate05[5], candidate05[6], candidate05[7], candidate05[8], candidate05[9], candidate05[10]},
    {candidate06[0], candidate06[1], candidate06[2], candidate06[3], candidate06[4], candidate06[5], candidate06[6], candidate06[7], candidate06[8], candidate06[9], candidate06[10]}
};

// getchar로 문자열 입력받기
void get_input(char *input, int size) {
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && i < size - 1) {
        input[i++] = c;
    }
    input[i] = '\0';  // 문자열 끝에 null 문자 추가
}

int main() {
    int i = 0, j = 0;

    printf("####################################\n");
    printf("     오디션 후보자 데이터 입력\n");
    printf("####################################\n");

    // 후보자 데이터 입력 (while 문 사용)
    while (i < 6) {
        printf("%d번째 후보자의 정보를 입력합니다.\n", i + 1);
        printf("---------------------------------\n");

        for (j = 0; j < 11; j++) {
            printf("%s: ", member_info[j]);
            get_input(candidates[i][j], 100);  // getchar를 이용해 문자열 입력
        }
        printf("=================================\n");
        i++;
    }

    printf("####################################\n");
    printf("     오디션 후보자 데이터 조회\n");
    printf("####################################\n");
    
    for (i = 0; i < 6; i++) {
        // 후보자 데이터 출력 (for 문 사용)
    printf("=============================================================================================\n");
    printf("성   명 | 생   일 | 성별 | 메   일            | 국적 | BMI | 주스킬 | 보조스킬 | TOPIK | MBTI |\n");
    printf("=============================================================================================\n");

        // 성별 가공
        char sex[4] = "남";
        if (candidates[i][2][0] == 'F' || candidates[i][2][0] == 'f') {
            snprintf(sex, sizeof(sex), "여");
        }

        // 후보자 기본 정보 출력
        printf("%s | %s | %s | %s | %s | %s | %s | %s | %s | %s |\n",
               candidates[i][0], candidates[i][1], sex, candidates[i][3], 
               candidates[i][4], candidates[i][5], candidates[i][6], 
               candidates[i][7], candidates[i][8], candidates[i][9]);

        // 소개 출력
        printf("---------------------------------------------------------------------------------------------\n");
        printf("%s\n", candidates[i][10]);
        printf("---------------------------------------------------------------------------------------------\n");
    }
    return 0;
}
