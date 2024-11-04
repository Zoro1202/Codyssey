#include "stdio.h"
#include "stdlib.h"

#define NUM_CANDIDATES 6
#define NUM_FIELDS 5  // 음악 소양, 댄스, 보컬, 비주얼, 전달력

// 후보자 이름과 고유 ID 배열
char* candidate_names[NUM_CANDIDATES] = {"박지연", "Ethan Smith", "김민수", "Lisa Wong", "Tom Johnson", "이준호"};
int candidate_ids[NUM_CANDIDATES] = {123456, 234567, 345678, 456789, 567890, 678901};

// 평가 시트 배열 (고유 ID + 분야 점수 5개 + 총점 = 7개 요소 * 후보자 수)
int scoring_sheet[NUM_CANDIDATES * 7];

// 문자열 비교 함수 (string.h 없이 strcmp 구현)
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// 개행 문자 제거 함수
void remove_newline(char *str) {
    while (*str) {
        if (*str == '\n') {
            *str = '\0';
            break;
        }
        str++;
    }
}

// 심사 결과 입력 함수
void inputScores(int *sheet, int fieldIndex) {
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        int base_index = i * 7;  // 각 후보자의 첫 번째 요소 인덱스

        // 고유 ID 저장
        sheet[base_index] = candidate_ids[i];

        int score;
        do {
            printf("%s: ", candidate_names[i]);
            char input[10];
            fgets(input, sizeof(input), stdin);  // fgets로 입력 받기
            score = atoi(input);
            if (score < 10 || score > 100) {
                printf("잘못된 값입니다. 10에서 100 사이로 입력해주세요.\n");
            }
        } while (score < 10 || score > 100);

        sheet[base_index + fieldIndex + 1] = score;  // 분야별 점수 저장
        printf("------------------------------------\n");
    }
}

// 총점 계산 함수
void calculateTotalScores(int *sheet) {
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        int base_index = i * 7;
        int total = 0;
        for (int j = 1; j <= NUM_FIELDS; j++) {
            total += sheet[base_index + j];
        }
        sheet[base_index + 6] = total;  // 총점 저장
    }
}

// 결과 출력 함수
void printResults(int *sheet) {
    printf("\n입력을 모두 완료했습니다.\n입력하신 내용을 검토하세요!\n");
    printf("------------------------------------\n");
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        int base_index = i * 7;
        printf("%s: %d\n", candidate_names[i], sheet[base_index + 6]);
    }
    printf("------------------------------------\n");
}

int main() {
    char judgeName[50];
    char field[50];
    char submit;
    char input[10];

    do {
        // 심사자 정보 입력
        printf("####################################\n");
        printf("#       오디션 심사 결과 입력       #\n");
        printf("####################################\n");
        printf("> 심사자 이름: ");
        fgets(judgeName, sizeof(judgeName), stdin);
        remove_newline(judgeName);  // 개행 문자 제거

        printf("> 전문 분야: ");
        fgets(field, sizeof(field), stdin);
        remove_newline(field);  // 개행 문자 제거
        printf("++++++++++++++++++++++++++++++++++++\n");

        // 분야 인덱스 설정
        int fieldIndex = -1;
        if (my_strcmp(field, "음악 소양") == 0) fieldIndex = 0;
        else if (my_strcmp(field, "댄스") == 0) fieldIndex = 1;
        else if (my_strcmp(field, "보컬") == 0) fieldIndex = 2;
        else if (my_strcmp(field, "비주얼") == 0) fieldIndex = 3;
        else if (my_strcmp(field, "전달력") == 0) fieldIndex = 4;

        if (fieldIndex == -1) {
            printf("잘못된 전문 분야입니다.\n");
            continue;
        }

        // 점수 입력
        inputScores(scoring_sheet, fieldIndex);

        // 총점 계산
        calculateTotalScores(scoring_sheet);

        // 결과 출력
        printResults(scoring_sheet);

        // 제출 여부 확인
        printf("제출하시겠습니까? (Y/N): ");
        fgets(input, sizeof(input), stdin);
        submit = input[0];  // 첫 번째 문자만 사용
        if (submit == 'Y' || submit == 'y') {
            printf("***최종 제출을 완료했습니다.***\n");
            break;
        } else {
            printf("처음부터 다시 입력합니다.\n");
        }
    } while (1);

    return 0;
}
