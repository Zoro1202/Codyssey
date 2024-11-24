#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// 최대 계좌 수
#define MAX_USERS 100
// 최대 히스토리 갯수
#define MAX_HISTORY 20
// 폭
#define WIDTH 120
// 높이
#define HEIGHT 25

#define FILENAME "accounts.json"

typedef struct Transaction {
    char date[20];
    char description[100];
    long long amount;
    long long balanceAfter;  // 거래 후 잔액
} Transaction;

typedef struct User {
    char name[50];
    char accountNumber[11];
    char openDate[20];
    long long balance;
    Transaction history[MAX_HISTORY];
    int historyCount;
} User;

User users[MAX_USERS];
int userCount = 0;
int selectedUser = 0;
int menuState = 0;  // 0: 메인 메뉴, 1: 사용자 상세 메뉴

//사용자 메뉴 히스토리 일람
int historyOffset = 0;  // 히스토리 표시 시작 인덱스
int historyCursor = 0;   // 현재 커서 위치

void ClearScreen() {
    system("cls");  // 콘솔 화면 지우기
}

void DrawUIBorder() {
    for (int i = 0; i < WIDTH; i++) printf("=");
    printf("\n");

    for (int i = 0; i < HEIGHT - 2; i++) {
        printf("|");
        for (int j = 0; j < WIDTH - 2; j++) printf(" ");
        printf("|\n");
    }

    for (int i = 0; i < WIDTH; i++) printf("=");
    printf("\n");
}

void PrintCenteredText(int row, const char *text) {
    int padding = (WIDTH - strlen(text)) / 2;
    printf("\033[%d;%dH%s", row, padding, text);
}

void GetCurrentDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t); //컴퓨터 시간
    strftime(buffer, 20, "%Y-%m-%d %H:%M", tm_info);
}

void FormatNumberWithCommas(char *buffer, long long number) {
    char temp[50];
    sprintf(temp, "%lld", number);

    int len = strlen(temp);
    int commaCount = (len - 1) / 3;
    int newLen = len + commaCount;

    buffer[newLen] = '\0';
    int tempIndex = len - 1;
    int bufferIndex = newLen - 1;
    int count = 0;

    while (tempIndex >= 0) {
        buffer[bufferIndex--] = temp[tempIndex--];
        if (++count == 3 && tempIndex >= 0) {
            buffer[bufferIndex--] = ',';
            count = 0;
        }
    }
}

void AddTransaction(User *user, long long amount, const char *description) {
    if (user->historyCount >= MAX_HISTORY) {
        // 가장 오래된 기록 삭제 queue, FIFO
        for (int i = 1; i < MAX_HISTORY; i++) {
            user->history[i - 1] = user->history[i];
        }
        user->historyCount--;  // 기록 수 하나 줄임
    }

    // 새로운 거래 기록 추가
    GetCurrentDate(user->history[user->historyCount].date);
    strcpy(user->history[user->historyCount].description, description);
    user->history[user->historyCount].amount = amount;
    user->history[user->historyCount].balanceAfter = user->balance;
    user->historyCount++;  // 기록 수 증가
}

void AddUser() {
    if (userCount >= MAX_USERS) return;

    char name[50];
    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 사용자 추가 =====");
    printf("\033[%d;%dH고객 이름 입력: ", HEIGHT - 2, WIDTH - 30);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    strcpy(users[userCount].name, name);
    sprintf(users[userCount].accountNumber, "%010d", userCount + 1);
    GetCurrentDate(users[userCount].openDate);
    users[userCount].balance = 0;
    users[userCount].historyCount = 0;

    userCount++;
}

void SortUsers() {
    User temp;
    for (int i = 0; i < userCount - 1; i++) {
        for (int j = 0; j < userCount - i - 1; j++) {
            if (users[j].balance < users[j + 1].balance) {
                // Swap users[j] and users[j + 1]
                temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}

void TransferMoney(User *from, User *to) {
    long long amount;
    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 계좌 이체 =====");
    printf("\033[%d;%dH받는 사용자: %s (%s)", 4, 5, to->name, to->accountNumber);
    printf("\033[%d;%dH보낼 금액 입력: ", HEIGHT - 2, WIDTH - 30);
    scanf("%lld", &amount);
    getchar();

    if (amount <= 0) {
        printf("\033[%d;%dH유효한 금액을 입력하세요.", HEIGHT - 3, WIDTH - 30);
        Sleep(2000);
        return;
    }

    if (from->balance < amount) {
        printf("\033[%d;%dH잔액이 부족합니다.", HEIGHT - 3, WIDTH - 30);
        Sleep(2000);
        return;
    }

    // 잔액 처리
    from->balance -= amount;
    to->balance += amount;

    // 기록 추가
    char fromDescription[100], toDescription[100];
    sprintf(fromDescription, "송금: %s (%s)", to->name, to->accountNumber);
    sprintf(toDescription, "입금: %s (%s)", from->name, from->accountNumber);

    AddTransaction(from, -amount, fromDescription);  // 송금 기록
    AddTransaction(to, amount, toDescription);       // 입금 기록

    // 완료 메시지
    for (int i = 3; i > 0; i--) {
        printf("\033[%d;%dH이체 완료! %d초 후 돌아갑니다...", HEIGHT - 3, WIDTH - 50, i);
        Sleep(1000);
    }

    SortUsers();

}

void PrintMainMenu() {
    ClearScreen();
    DrawUIBorder();

    PrintCenteredText(2, "===== 메인 메뉴 =====");

    int row = 4;  // 시작 행
    for (int i = 0; i < userCount; i++) {
        char formattedBalance[50];
        FormatNumberWithCommas(formattedBalance, users[i].balance);

        if (i == selectedUser) {
            printf("\033[%d;%dH> %s (₩%s)", row, 5, users[i].name, formattedBalance);
        } else {
            printf("\033[%d;%dH  %s (₩%s)", row, 5, users[i].name, formattedBalance);
        }
        row++;
    }

    if (userCount == selectedUser) {
        printf("\033[%d;%dH> [사용자 추가]", row, 5);
    } else {
        printf("\033[%d;%dH  [사용자 추가]", row, 5);
    }

    // 저장 및 종료 버튼 추가
    if (selectedUser == userCount + 1) {
        printf("\033[%d;%dH> [저장 및 종료]", row + 2, 5);
    } else {
        printf("\033[%d;%dH  [저장 및 종료]", row + 2, 5);
    }

    // 툴팁
    printf("\033[%d;%dHW: 위로 이동, S: 아래로 이동, Enter: 선택", HEIGHT - 1, 2);
}

void PrintUserMenu() {
    ClearScreen();
    DrawUIBorder();

    User *user = &users[selectedUser];
    PrintCenteredText(2, user->name);

    char formattedBalance[50];
    FormatNumberWithCommas(formattedBalance, user->balance);

    printf("\033[4;5H계좌번호: %s", user->accountNumber);
    printf("\033[5;5H개설일: %s", user->openDate);
    printf("\033[6;5H현재 잔액: ₩%s", formattedBalance);
    printf("\033[8;5H입금: Q, 출금: E, 이체: D, 뒤로가기: A + Enter");

    int visibleLines = HEIGHT - 12;  // 화면에 표시 가능한 히스토리 줄 수
    int startRow = 10;

    // 히스토리 출력
    for (int i = 0; i < visibleLines; i++) {
        int index = historyOffset + i;  // 현재 출력할 히스토리의 인덱스
        if (index >= user->historyCount) break;  // 히스토리 끝까지 출력 완료

        char formattedTransaction[50];
        FormatNumberWithCommas(formattedTransaction, abs(user->history[index].amount));  // 절대값 사용

        if (index == historyCursor) {
            // 현재 커서 위치 강조
            printf("\033[%d;5H> [%d] %s | %s | 금액: ₩%s | 잔액: ₩%lld", startRow + i,
                   index + 1,  // 인덱스는 1부터 시작
                   user->history[index].date,
                   user->history[index].description,
                   formattedTransaction,
                   user->history[index].balanceAfter);
        } else {
            printf("\033[%d;5H  [%d] %s | %s | 금액: ₩%s | 잔액: ₩%lld", startRow + i,
                   index + 1,  // 인덱스는 1부터 시작
                   user->history[index].date,
                   user->history[index].description,
                   formattedTransaction,
                   user->history[index].balanceAfter);
        }
    }
}

void HandleTransfer(User *from) {
    int targetIndex;
    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 송금 =====");
    for (int i = 0; i < userCount; i++) {
        if (&users[i] != from) {
            printf("\033[%d;5H%d. %s (%s)", 4 + i, i + 1, users[i].name, users[i].accountNumber);
        }
    }
    printf("\033[%d;%dH송금할 사용자 번호 선택: ", HEIGHT - 2, WIDTH - 30);
    scanf("%d", &targetIndex);
    getchar();

    if (targetIndex < 1 || targetIndex > userCount || &users[targetIndex - 1] == from) {
        printf("\033[%d;%dH잘못된 선택입니다.", HEIGHT - 3, WIDTH - 30);
        return;
    }

    TransferMoney(from, &users[targetIndex - 1]);
}

void HandleTransaction(User *user, int isDeposit) {
    char input[50];
    long long amount;
    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, isDeposit ? "===== 입금 =====" : "===== 출금 =====");

    printf("\033[%d;%dH현재 잔액: ₩", 4, 5);
    char formattedBalance[50];
    FormatNumberWithCommas(formattedBalance, user->balance);
    printf("%s", formattedBalance);

    printf("\033[%d;%dH금액 입력: ", HEIGHT - 2, WIDTH - 30);
    fgets(input, sizeof(input), stdin); // 입력을 문자열로 받음
    input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거
    // 1234a -> 1234a \0
    int isNum = 0;
        // 받은 문자열의 끝까지 검사. , 숫자 이외의 문자가 나오면 isNum = 0으로 바꿈.
    for(int i = 0; input[i] != '\0' ; i ++){
            // 48 <= input[i] <= 57
        if((input[i] >= '0' && input[i] <= '9'))
            isNum = 1;
        else if(!(input[i] >= '0' && input[i] <= '9')){
            isNum = 0;
            break;
        }
    }

    // TODO - 0 이하일때 예외 처리 말고 문자 값이 들어왔을 때도 처리.
    if ((isNum == 0) || (sscanf(input, "%lld", &amount) != 1 || amount <= 0)) {
        printf("\033[%d;%dH유효한 금액을 입력하세요.", HEIGHT - 3, WIDTH - 30);
        Sleep(2000);
        return;
    }

    if (isDeposit) {
        user->balance += amount;
        char description[100];
        sprintf(description, "입금");
        AddTransaction(user, amount, description);
    } else {
        if (user->balance < amount) {
            printf("\033[%d;%dH잔액이 부족합니다.", HEIGHT - 3, WIDTH - 30);
            Sleep(2000);
            return;
        }
        user->balance -= amount;
        char description[100];
        sprintf(description, "출금");
        AddTransaction(user, -amount, description);
    }

    // 거래 완료 메시지와 카운트다운
    for (int i = 3; i > 0; i--) {
        printf("\033[%d;%dH거래 완료! %d초 후 돌아갑니다...", HEIGHT - 3, WIDTH - 50, i);
        Sleep(1000);
    }
}

void SaveAccountsToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "[\n");
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "  {\n");
        fprintf(file, "    \"name\": \"%s\",\n", users[i].name);
        fprintf(file, "    \"accountNumber\": \"%s\",\n", users[i].accountNumber);
        fprintf(file, "    \"openDate\": \"%s\",\n", users[i].openDate);
        fprintf(file, "    \"balance\": %lld,\n", users[i].balance);
        fprintf(file, "    \"history\": [\n");

        for (int j = 0; j < users[i].historyCount; j++) {
            fprintf(file, "      {\n");
            fprintf(file, "        \"date\": \"%s\",\n", users[i].history[j].date);
            fprintf(file, "        \"description\": \"%s\",\n", users[i].history[j].description);
            fprintf(file, "        \"amount\": %lld,\n", users[i].history[j].amount);
            fprintf(file, "        \"balanceAfter\": %lld\n", users[i].history[j].balanceAfter);
            fprintf(file, "      }%s\n", (j < users[i].historyCount - 1) ? "," : "");
        }

        fprintf(file, "    ]\n");
        fprintf(file, "  }%s\n", (i < userCount - 1) ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
}

void LoadAccountsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;  // 파일이 없으면 초기 데이터로 시작

    char line[1024];
    int inHistory = 0;  // history 배열 내부인지 여부 확인

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"name\":")) {
            sscanf(line, " \"name\": \"%[^\"]\",", users[userCount].name);
        } else if (strstr(line, "\"accountNumber\":")) {
            sscanf(line, " \"accountNumber\": \"%[^\"]\",", users[userCount].accountNumber);
        } else if (strstr(line, "\"openDate\":")) {
            sscanf(line, " \"openDate\": \"%[^\"]\",", users[userCount].openDate);
        } else if (strstr(line, "\"balance\":")) {
            sscanf(line, " \"balance\": %lld,", &users[userCount].balance);
        } else if (strstr(line, "\"history\": [")) {
            inHistory = 1;  // history 배열 시작
        } else if (inHistory && strstr(line, "\"date\":")) {
            sscanf(line, " \"date\": \"%[^\"]\",", users[userCount].history[users[userCount].historyCount].date);
        } else if (inHistory && strstr(line, "\"description\":")) {
            sscanf(line, " \"description\": \"%[^\"]\",", users[userCount].history[users[userCount].historyCount].description);
        } else if (inHistory && strstr(line, "\"amount\":")) {
            sscanf(line, " \"amount\": %lld,", &users[userCount].history[users[userCount].historyCount].amount);
        } else if (inHistory && strstr(line, "\"balanceAfter\":")) {
            sscanf(line, " \"balanceAfter\": %lld,", &users[userCount].history[users[userCount].historyCount].balanceAfter);
            users[userCount].historyCount++;  // 기록 개수 증가
        } else if (strstr(line, "]")) {  // history 배열 종료
            inHistory = 0;
        } else if (strstr(line, "}")) {
            // 사용자 정보를 다 읽었으면 다음 사용자로 이동
            if (!inHistory && (users[userCount].historyCount > 0 || strlen(users[userCount].name) > 0)) {
                userCount++;
            }
        }
    }

    fclose(file);
}

void MainLoop() {
    while (1) {
        if (menuState == 0) {
            PrintMainMenu();
        } else if (menuState == 1) {
            PrintUserMenu();
        }

        char input = _getch();
        if (menuState == 0) {
            if (input == 'w' || input == 'W') {
                if (selectedUser > 0) selectedUser--;  // 위로 이동
            } else if (input == 's' || input == 'S') {
                if (selectedUser < userCount + 1) selectedUser++;  // 아래로 이동
            } else if (input == '\r') {  // Enter 키
                if (selectedUser == userCount) {
                    AddUser();
                } else if (selectedUser == userCount + 1) {
                    SaveAccountsToFile(FILENAME);
                    printf("\033[%d;%dH저장 완료! 프로그램 종료 중...", HEIGHT - 3, WIDTH - 50);
                    Sleep(2000);
                    exit(0);
                } else {
                    menuState = 1;  // 상세 메뉴로 이동
                    historyCursor = users[selectedUser].historyCount - 1;  // 기본 커서 위치: 최신 히스토리
                    historyOffset = (historyCursor < HEIGHT - 12)
                                    ? 0 
                                    : historyCursor - (HEIGHT - 12) + 1;  // 최신 항목이 보이도록
                }
            }
        } else if (menuState == 1) {
            User *user = &users[selectedUser];
            if (input == 'q' || input == 'Q') {
                HandleTransaction(user, 1);  // 입금
            } else if (input == 'e' || input == 'E') {
                HandleTransaction(user, 0);  // 출금
            } else if (input == 'd' || input == 'D') {
                HandleTransfer(user);  // 송금
            } else if (input == 'a' || input == 'A') {
                menuState = 0;  // 메인 메뉴로 돌아가기
            } else if (input == 'w' || input == 'W') {
                if (historyCursor > 0) {
                    historyCursor--;
                    if (historyCursor < historyOffset) {
                        historyOffset--;  // 화면 위로 스크롤
                    }
                }
            } else if (input == 's' || input == 'S') {
                if (historyCursor < user->historyCount - 1) {
                    historyCursor++;
                    if (historyCursor >= historyOffset + (HEIGHT - 12)) {
                        historyOffset++;  // 화면 아래로 스크롤
                    }
                }
            }
        }
    }
}

int main() {
    // 프로그램 시작 시 JSON 파일에서 데이터 로드
    LoadAccountsFromFile(FILENAME);
    //정렬
    SortUsers();

    MainLoop();

    // 프로그램 종료 시 JSON 파일에 데이터 저장
    //SaveAccountsToFile(FILENAME);

    return 0;
}
                                          