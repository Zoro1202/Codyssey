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
//유저당 최대 계좌 수
#define MAX_ACCOUNT 5
// 폭
#define WIDTH 120
// 높이
#define HEIGHT 25

#define FILENAME "accounts1.json"

typedef struct Transaction
{
    char date[20];
    char description[100];
    long long amount;
    long long balanceAfter; // 거래 후 잔액
} Transaction;

typedef struct Account
{
    char accountname[50];
    char accountNumber[14];
    char openDate[20];
    long long balance;
    Transaction history[MAX_HISTORY]; //queue 방식
    int historyCount;
} Account;

typedef struct User
{
    char name[50];
    char id[50];
    char password[50];
    Account account[MAX_ACCOUNT]; // 최대치 도달 하면 더 이상 못 만들게
    int accountCount;
} User;

User users[MAX_USERS];
int userCount = 0;
int selectedUser = 0;
int menuState = 0; // 0: 메인 메뉴, 1: 사용자 상세 메뉴

// 사용자 메뉴 히스토리 일람
int historyOffset = 0; // 히스토리 표시 시작 인덱스
int historyCursor = 0; // 현재 커서 위치

// 화면 지우기
void ClearScreen()
{
    // 다 지우기
    system("cls"); // 콘솔 화면 지우기
}

// 사각형 그리기
void DrawUIBorder()
{
    for (int i = 0; i < WIDTH; i++)
        printf("=");
    printf("\n");

    for (int i = 0; i < HEIGHT - 2; i++)
    {
        printf("|");
        for (int j = 0; j < WIDTH - 2; j++)
            printf(" ");
        printf("|\n");
    }

    for (int i = 0; i < WIDTH; i++)
        printf("=");
    printf("\n");
}

/** 메뉴 선택
 * @param selectedOption 선택할 메뉴
 * @param numOptions 최대 옵션 개수
 * @return selectedOption
 */
int HandleMenuInput(int selectedOption, int numOptions)
{
    char input = _getch();

    if (input == 'w' || input == 'W' || input == 72) // W 또는 위 화살표
    {
        if (selectedOption > 0)
            selectedOption--;
    }
    else if (input == 's' || input == 'S' || input == 80) // S 또는 아래 화살표
    {
        if (selectedOption < numOptions - 1)
            selectedOption++;
    }
    else if (input == '\r') // Enter
    {
        return -(selectedOption + 1); // Enter를 눌렀을 때 선택된 옵션을 음수로 반환
    }
    else if (input == 27) // ESC 키
    {
        return -99; // ESC 키 반환 값
    }

    return selectedOption; // 선택된 옵션 유지
}

/** 숫자 입력
 * @param prompt 프롬프트"\033[행;열H"+"할말"
 * @param maxLength 문자열 최대 길이
 */
long long HandleOnlyNumInput(const char *prompt, int maxLength)
{
    char input[50] = {0}; // 입력을 저장할 문자열
    int pos = 0; // 커서위치

    printf("%s", prompt);

    while (1)
    {
        char c = _getch(); // 키 입력 감지

        if (c == 27) // ESC 키
        {
            return -1; // 취소를 나타내는 값 반환
        }
        else if (c == '\r') // Enter 키
        {
            input[pos] = '\0'; // 입력 문자열 종료
            break;
        }
        else if (c >= '0' && c <= '9') // 숫자 입력
        {
            if (pos < maxLength)
            {
                input[pos++] = c;
                printf("%c", c); // 입력된 숫자 화면에 표시
            }
        }
        else if (c == 8 && pos > 0) // Backspace 처리
        {
            pos--;
            printf("\b \b"); // 화면에서 문자 제거
        }
    }

    return atoll(input); // 문자열을 정수로 변환하여 반환
}

/** 문자열 입력
 * @param buffer 입력받을 배열
 * @param bufferSize 사이즈
 * @return 입력취소 = -1, 엔터키 = 0
 */
int HandleStringInput(char *buffer, int bufferSize)
{
    int pos = 0;

    while (1)
    {
        char c = _getch(); // 키 입력 감지

        if (c == 27) // ESC 키
        {
            return -1; // 취소를 나타내는 값 반환
        }
        else if (c == '\r') // Enter 키
        {
            buffer[pos] = '\0'; // 문자열 종료
            return 0; // 정상 입력 완료
        }
        else if (c >= ' ' && c <= '~') // 일반 문자 입력 (ASCII 범위)
        {
            if (pos < bufferSize - 1)
            {
                buffer[pos++] = c;
                printf("%c", c); // 화면에 출력
            }
        }
        else if (c == 8 && pos > 0) // Backspace 처리
        {
            pos--;
            printf("\b \b"); // 화면에서 문자 제거
        }
    }
}
/** 문자열 입력 - 비밀번호용
 * @param buffer 입력받을 배열
 * @param bufferSize 사이즈
 * @return 입력취소 = -1, 엔터키 = 0
 */
int HandleHiddenStringInput(char *buffer, int bufferSize)
{
    int pos = 0;

    while (1)
    {
        char c = _getch(); // 키 입력 감지

        if (c == 27) // ESC 키
        {
            return -1; // 취소를 나타내는 값 반환
        }
        else if (c == '\r') // Enter 키
        {
            buffer[pos] = '\0'; // 문자열 종료
            return 0; // 정상 입력 완료
        }
        else if (c >= ' ' && c <= '~') // 일반 문자 입력 (ASCII 범위)
        {
            if (pos < bufferSize - 1)
            {
                buffer[pos++] = c;
                printf("*"); // 화면에 출력
            }
        }
        else if (c == 8 && pos > 0) // Backspace 처리
        {
            pos--;
            printf("\b \b"); // 화면에서 문자 제거
        }
    }
}
 
// 현재 시간 저장
// @param buffer "YYYY-MM-DD HH:MM"
void GetCurrentDateTIME(char *buffer)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t); // 컴퓨터 시간
    strftime(buffer, 20, "%Y-%m-%d %H:%M", tm_info);
}

// 현재 시간 저장 - 메뉴 옆에 출력하려고
// @param buffer "YYYY-MM-DD [WEEK]"
void GetCurrentDateWEEK(char *buffer)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t); // 컴퓨터 시간
    strftime(buffer, 25, "%Y-%m-%d [%A]", tm_info);
}

// 타이틀 옆에 시간 출력
// @param row 행(높이)
void PrintCurrentDate(int row)
{
    char buffer[25];
    GetCurrentDateWEEK(buffer);
    printf("\033[%d;%dH%s", row + 1, WIDTH - 26, buffer);
}

// 타이틀 출력(가운데 정렬)
// @param row 행(높이)
// @param text "할말"
void PrintCenteredText(int row, char *text)
{
    int padding = (WIDTH - strlen(text)) / 2;
    printf("\033[%d;%dH%s", row, padding, text); // \033[%d;%dH 열 , 행
    PrintCurrentDate(row); // 타이틀 옆에 날짜 출력
}

// 천의 자리 숫자마다 콤마 찍기
void AddCommas(char *buffer, size_t bufferSize, long long number)
{
    char temp[50];
    snprintf(temp, sizeof(temp), "%lld", number);

    int len = strlen(temp);
    int commaCount = (len - 1) / 3;
    int newLen = len + commaCount;

    if (newLen >= bufferSize)
    {
        buffer[0] = '\0'; // 버퍼가 부족하면 빈 문자열 반환
        return;
    }

    int srcIndex = len - 1;
    int destIndex = newLen - 1;

    buffer[newLen] = '\0'; // 종료 문자

    int groupCounter = 0;
    while (srcIndex >= 0)
    {
        buffer[destIndex--] = temp[srcIndex--];
        groupCounter++;
        if (groupCounter == 3 && srcIndex >= 0)
        {
            buffer[destIndex--] = ',';
            groupCounter = 0;
        }
    }
}

/** 히스토리 추가하기
 * @param account 유저->계좌
 * @param amount 얼마?
 * @param description "설명"
 */
void AddAcountHistory(Account *account, long long amount, const char *description)
{
    // 거래 기록이 최대치를 초과하면 가장 오래된 기록 제거
    if (account->historyCount >= MAX_HISTORY)
    {
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            account->history[i - 1] = account->history[i];
        }
        account->historyCount--; // 기록 수 감소
    }

    // 새로운 거래 기록 추가
    Transaction *newTransaction = &account->history[account->historyCount];
    GetCurrentDateTIME(newTransaction->date);
    strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
    newTransaction->description[sizeof(newTransaction->description) - 1] = '\0'; // 안전하게 NULL 종료
    newTransaction->amount = amount;
    newTransaction->balanceAfter = account->balance; // 현재 잔액 기준으로 잔액 저장
    account->historyCount++; // 기록 수 증가
}

// 계좌 개설
// @param user 유저
void AddAccount(User *user)
{
    if (user->accountCount >= MAX_ACCOUNT)
    {
        printf("더 이상 계좌를 추가할 수 없습니다.\n");
        Sleep(2000);
        return;
    }

    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 계좌 추가 =====");
    printf("\033[%d;5H(ESC로 뒤로가기)", HEIGHT - 2);

    // 사용자 비밀번호 확인
    char inputPassword[50];
    printf("\033[4;5H사용자 계정 비밀번호를 입력하세요: ");
    HandleHiddenStringInput(inputPassword, 50);

    if (strcmp(inputPassword, user->password) != 0)
    {
        printf("\033[6;5H비밀번호가 일치하지 않습니다. 계좌 추가를 취소합니다.\n");
        Sleep(2000);
        return;
    }

    // 은행 선택
    const char *bankOptions[] = {
        "하나은행 (0810)",
        "농협은행 (3510)"
    };
    //은행 개수
    const int numOptions = 2;
    // 선택 메뉴
    int selectedOption = 0;

    while (1)
    {
        ClearScreen();
        DrawUIBorder();
        PrintCenteredText(2, "===== 계좌 추가 =====");

        for (int i = 0; i < numOptions; i++)
        {
            if (i == selectedOption)
                printf("\033[%d;5H> %s", 4 + i, bankOptions[i]);
            else
                printf("\033[%d;5H  %s", 4 + i, bankOptions[i]);
        }

        // TODO - 이거 HandleMenuInput 으로 바꾸기
        char input = _getch();
        if (input == 27) // ESC 키
        {
            return; // 뒤로가기
        }
        if (input == 'w' || input == 'W' || input == 72) // W 또는 위 화살표
        {
            if (selectedOption > 0)
                selectedOption--;
        }
        else if (input == 's' || input == 'S' || input == 80) // S 또는 아래 화살표
        {
            if (selectedOption < numOptions - 1)
                selectedOption++;
        }
        else if (input == '\r') // Enter
        {
            break; // 은행 선택 완료
        }
    }

    // 계좌 생성
    Account *newAccount = &user->account[user->accountCount++];
    //은행 코드
    char bankCode[5];

    // 은행별 번호 정하기
    if (selectedOption == 0) // 하나은행
        strcpy(bankCode, "0810");
    else if (selectedOption == 1) // 농협은행
        strcpy(bankCode, "3510");

    //현재 시간 받아오기 null  값을 넣으면 현재 시간임.
    
    //1970년 1월 1일 UTC부터 경과된 초
    time_t now = time(NULL);
    //그걸 연도, 월, 일, 시, 분 등 구조체 정보로 바꿈.
    struct tm *t = localtime(&now);
    // 연도의 마지막 두 자리만 쓰기 (예: 2024 -> 24)
    int year = t->tm_year % 100; 

    // 랜덤 시드
    // 겹치는 경우가 많아서 좀 시드의 복잡성을 더함.
    // 랜덤 시드 = 계좌 수(1~) + (은행 코드(0810 => 810) * 날짜(일, 1 ~ 31)) + 유저 이름에 첫번째 ascii 값(32~127?)
    int randomSeedForAccountnum = (userCount*user->accountCount)+(atoi(bankCode)*t->tm_mday)+(int)user->name[0];
    srand(randomSeedForAccountnum);

    // 랜덤 6자리 번호 생성
    long long randomPart = rand() % (999999 - 100000 + 1) + 100000;

    // 계좌 번호 생성
    sprintf(newAccount->accountNumber, "%s%02d%06lld", bankCode, year, randomPart);

    //현재날짜 구하기
    GetCurrentDateTIME(newAccount->openDate);
    newAccount->balance = 0;
    newAccount->historyCount = 0;

    printf("\033[%d;5H계좌 추가 완료! 계좌번호: %s", HEIGHT - 3, newAccount->accountNumber);
    Sleep(2000);
}

// 사용자의 총 재산
// @param user 유저
long long GetUserTotalBalance(User *user)
{
    long long totalBalance = 0;
    for (int i = 0; i < user->accountCount; i++)
    {
        totalBalance += user->account[i].balance;
    }
    return totalBalance;
}

// 유저 정렬 // 돈 많은 순서, 버블 정렬 O(n^2)
// TODO - 이진 탐색 구조?로 계좌가 갱신된 유저를 선택적으로 정렬하도록 바꾸기
void SortUsers()
{
    User temp;
    for (int i = 0; i < userCount - 1; i++)
    {
        for (int j = 0; j < userCount - i - 1; j++)
        {
            // 총 계좌 잔액을 기준으로 비교
            long long balance1 = GetUserTotalBalance(&users[j]);
            long long balance2 = GetUserTotalBalance(&users[j + 1]);

            if (balance1 < balance2)
            {
                // Swap users[j] and users[j + 1]
                temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}

// 회원가입
void RegisterUser()
{
    if (userCount >= MAX_USERS)
    {
        printf("더 이상 사용자를 추가할 수 없습니다.\n");
        Sleep(2000);
        return;
    }

    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 회원가입 =====");

    // 1. 사용자 정보 입력
    printf("\033[%d;%dH고객 이름 입력: ", 4, 5);
    fgets(users[userCount].name, sizeof(users[userCount].name), stdin); // handleStringInput은 한글은 처리 못함;
    users[userCount].name[strcspn(users[userCount].name, "\n")] = 0; // 개행문자 제거

    while (1)
    {
        printf("\033[%d;%dH아이디 입력(ESC로 뒤로가기): ", 5, 5);
        if (HandleStringInput(users[userCount].id, sizeof(users[userCount].id)) == -1)
        {
            printf("\033[%d;%dH입력이 취소되었습니다.\n", 8, 5);
            Sleep(2000);
            return;
        }

        // 아이디 중복 확인
        int isDuplicate = 0;
        for (int i = 0; i < userCount; i++)
        {
            if (strcmp(users[userCount].id, users[i].id) == 0)
            {
                isDuplicate = 1;
                break;
            }
        }

        if (isDuplicate)
        {
            printf("\033[%d;%dH이미 존재하는 아이디입니다. 다른 아이디를 입력하세요.\n", 6, 5);
            Sleep(2000);
            printf("\033[%d;%dH                                                       \n", 6, 5);
            printf("\033[%d;%dH                                     \n", 5, 5);
        }
        else
        {
            break; // 중복되지 않으면 루프 종료
        }
    }

    printf("\033[%d;%dH비밀번호 입력(ESC로 뒤로가기): ", 6, 5);
    if (HandleHiddenStringInput(users[userCount].password, sizeof(users[userCount].password)) == -1)
    {
        printf("\033[%d;%dH입력이 취소되었습니다.\n", 8, 5);
        Sleep(2000);
        return;
    }

    // 초기 계좌 개수 설정
    users[userCount].accountCount = 0;

    printf("\033[%d;%dH회원가입 완료! 환영합니다, %s님.", 8, 5, users[userCount].name);
    Sleep(2000);

    // 2. 계좌 개설 여부 확인
    char choice;
    printf("\033[%d;%dH계좌를 개설하시겠습니까? (Y/N): ", 9, 5);
    scanf(" %c", &choice);
    getchar();

    if (choice == 'Y' || choice == 'y')
    {
        AddAccount(&users[userCount]); // 계좌 추가
    }

    userCount++;
}

// 히스토리 출력
void ViewAccountHistory(Account *account)
{
    while (1)
    {
        ClearScreen();
        DrawUIBorder();
        PrintCenteredText(2, "===== 거래 내역 =====");

        if (account->historyCount == 0)
        {
            printf("\033[4;5H거래 내역이 없습니다.\n");
            printf("\033[%d;5H(ESC로 뒤로가기)", HEIGHT - 2);
        }
        else
        {
            // 히스토리 출력
            for (int i = 0; i < account->historyCount; i++)
            {
                Transaction *transaction = &account->history[i];
                printf("\033[%d;5H[%s] %s | 금액: %lld | 잔액: %lld",
                       4 + i, transaction->date, transaction->description, transaction->amount, transaction->balanceAfter);
            }
            printf("\033[%d;5H(ESC로 뒤로가기)", HEIGHT - 2);
        }

        char input = _getch();
        if (input == 27) // ESC 키
        {
            return; // 뒤로가기
        }
    }
}

// 계좌 출력 - 히스토리 창 접근
// @param user 유저
void ViewAccounts(User *user)
{
    if (user->accountCount == 0)
    {
        printf("조회할 계좌가 없습니다.\n");
        Sleep(2000);
        return;
    }

    int selectedAccountIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, "===== 계좌 조회 =====");

        // 계좌 목록 출력
        for (int i = 0; i < user->accountCount; i++)
        {
            if (i == selectedAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%lld", 4 + i, user->account[i].accountNumber, user->account[i].balance);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%lld", 4 + i, user->account[i].accountNumber, user->account[i].balance);
        }

        printf("\033[%d;5H(ESC로 뒤로가기)", HEIGHT - 2);

        // 키 입력 처리
        int inputResult = HandleMenuInput(selectedAccountIndex, user->accountCount);

        if (inputResult == -99) // ESC 키
        {
            return; // 뒤로가기
        }
        else if (inputResult < 0) // Enter
        {
            selectedAccountIndex = -(inputResult + 1);
            ViewAccountHistory(&user->account[selectedAccountIndex]); // 히스토리 보기
        }
        else
        {
            selectedAccountIndex = inputResult; // 선택된 옵션 업데이트
        }
    }
}

/** 입출금 화면
 * @param user 유저
 * @param isDeposit 0 = 입금, 1 = 출금
 */
void ProcessTransaction(User *user, int isDeposit)
{
    if (user->accountCount == 0)
    {
        printf("사용 가능한 계좌가 없습니다.\n");
        Sleep(2000);
        return;
    }

    int selectedAccountIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, isDeposit ? "===== 입금 =====" : "===== 출금 =====");

        for (int i = 0; i < user->accountCount; i++)
        {
            if (i == selectedAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%lld", 4 + i, user->account[i].accountNumber, user->account[i].balance);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%lld", 4 + i, user->account[i].accountNumber, user->account[i].balance);
        }

        printf("\033[%d;5H(ESC로 취소)", HEIGHT - 2);

        int inputResult = HandleMenuInput(selectedAccountIndex, user->accountCount);

        if (inputResult == -99) // ESC 키
        {
            return; // 취소
        }
        else if (inputResult < 0) // Enter 키
        {
            selectedAccountIndex = -(inputResult + 1);
            break; // 계좌 선택 완료
        }
        else
        {
            selectedAccountIndex = inputResult; // 선택된 옵션 업데이트
        }
    }

    Account *selectedAccount = &user->account[selectedAccountIndex];

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, isDeposit ? "===== 입금: 금액 입력 =====" : "===== 출금: 금액 입력 =====");

        long long amount = HandleOnlyNumInput("\033[4;5H금액 입력 (ESC로 취소): ", 10);

        if (amount == -1) // ESC 키로 취소
        {
            return;
        }

        if (amount <= 0)
        {
            printf("\033[%d;5H잘못된 금액입니다. 다시 입력하세요.", HEIGHT - 3);
            Sleep(2000);
            continue;
        }

        if (!isDeposit && selectedAccount->balance < amount)
        {
            printf("\033[%d;5H잔액이 부족합니다. 다시 입력하세요.", HEIGHT - 3);
            Sleep(2000);
            continue;
        }

        // 금액 처리
        if (isDeposit)
        {
            selectedAccount->balance += amount;
            AddAcountHistory(selectedAccount, amount, "입금");
        }
        else
        {
            selectedAccount->balance -= amount;
            AddAcountHistory(selectedAccount, -amount, "출금");
        }

        printf("\033[%d;5H%s 완료!", HEIGHT - 3, isDeposit ? "입금" : "출금");
        Sleep(2000);
        return;
    }
}

// 송금 계좌 선택
// @param fromUser 송금할 유저
void HandleTransfer(User *fromUser)
{
    if (fromUser->accountCount == 0)
    {
        printf("송금할 계좌가 없습니다. 계좌를 추가해 주세요.\n");
        Sleep(2000);
        return;
    }

    // 1. 송금 계좌 선택
    int fromAccountIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, "===== 송금: 송금 계좌 선택 =====");

        for (int i = 0; i < fromUser->accountCount; i++)
        {
            if (i == fromAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%lld", 4 + i, fromUser->account[i].accountNumber, fromUser->account[i].balance);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%lld", 4 + i, fromUser->account[i].accountNumber, fromUser->account[i].balance);
        }

        printf("\033[%d;5H(ESC로 취소)", HEIGHT - 2);

        int inputResult = HandleMenuInput(fromAccountIndex, fromUser->accountCount);

        if (inputResult == -99) // ESC 키
        {
            return; // 취소
        }
        else if (inputResult < 0) // Enter 키
        {
            fromAccountIndex = -(inputResult + 1);
            break; // 송금 계좌 선택 완료
        }
        else
        {
            fromAccountIndex = inputResult; // 선택된 옵션 업데이트
        }
    }

    Account *fromAccount = &fromUser->account[fromAccountIndex];

    // 2. 수취 사용자 선택
    int targetUserIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, "===== 송금: 수취 사용자 선택 =====");

        for (int i = 0; i < userCount; i++)
        {
            //if (&users[i] != fromUser) // 자신 제외
            {
                if (i == targetUserIndex)
                    printf("\033[%d;5H> %s", 4 + i, users[i].name);
                else
                    printf("\033[%d;5H  %s", 4 + i, users[i].name);
            }
        }

        printf("\033[%d;5H(ESC로 취소)", HEIGHT - 2);

        int inputResult = HandleMenuInput(targetUserIndex, userCount);

        if (inputResult == -99) // ESC 키
        {
            return; // 취소
        }
        else if (inputResult < 0) // Enter 키
        {
            targetUserIndex = -(inputResult + 1);
            break; // 수취 사용자 선택 완료
        }
        else
        {
            targetUserIndex = inputResult; // 선택된 옵션 업데이트
        }
    }

    User *toUser = &users[targetUserIndex];

    // 3. 수취 계좌 선택
    if (toUser->accountCount == 0)
    {
        printf("선택한 사용자에게 계좌가 없습니다.\n");
        Sleep(2000);
        return;
    }

    int toAccountIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, "===== 송금: 수취 계좌 선택 =====");

        for (int i = 0; i < toUser->accountCount; i++)
        {
            //if(fromAccount == toUser->account){ // 송금 계좌 -> 송금 계좌 경우를 방지, 송금 계좌 -> 수취 계좌만 표시 / 선택도
                if (i == toAccountIndex)
                    printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%lld", 4 + i, toUser->account[i].accountNumber, toUser->account[i].balance);
                else
                    printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%lld", 4 + i, toUser->account[i].accountNumber, toUser->account[i].balance);
            //}
        }

        printf("\033[%d;5H(ESC로 취소)", HEIGHT - 2);

        int inputResult = HandleMenuInput(toAccountIndex, toUser->accountCount);

        if (inputResult == -99) // ESC 키
        {
            return; // 취소
        }
        else if (inputResult < 0) // Enter 키
        {
            toAccountIndex = -(inputResult + 1);
            if(toAccountIndex == fromAccountIndex){ // 같은 계좌 막기.ㅇㅇ
                printf("\033[%d;5H같은 계좌로는 송금할 수 없습니다.", HEIGHT - 3);
                Sleep(2000);
                printf("\033[%d;5H                                            ", HEIGHT - 3);
                continue;
            }
            break; // 수취 계좌 선택 완료
        }
        else
        {
            toAccountIndex = inputResult; // 선택된 옵션 업데이트
        }
    }

    Account *toAccount = &toUser->account[toAccountIndex];

    // 4. 송금 금액 입력
    while (1)
    {
        ClearScreen();
        DrawUIBorder();
        PrintCenteredText(2, "===== 송금: 금액 입력 =====");

        long long amount = HandleOnlyNumInput("\033[4;5H송금할 금액 입력 (ESC로 취소): ", 10);
        
        if (amount == -1) // ESC 키
        {
            return; // 취소
        }
        
        if (amount <= 0)
        {
            printf("\033[%d;5H잘못된 금액입니다. 다시 입력하세요.", HEIGHT - 3);
            Sleep(2000);
            continue;
        }

        if (fromAccount->balance < amount)
        {
            printf("\033[%d;5H잔액이 부족합니다. 다시 입력하세요.", HEIGHT - 3);
            Sleep(2000);
            continue;
        }

        // 5. 송금 처리
        fromAccount->balance -= amount;
        toAccount->balance += amount;

        // 거래 기록 추가
        char fromDescription[100], toDescription[100];
        sprintf(fromDescription, "송금: %s (%s)", toUser->name, toAccount->accountNumber);
        sprintf(toDescription, "입금: %s (%s)", fromUser->name, fromAccount->accountNumber);

        AddAcountHistory(fromAccount, -amount, fromDescription);
        AddAcountHistory(toAccount, amount, toDescription);

        printf("\033[%d;5H송금 완료!", HEIGHT - 3);
        Sleep(2000);
        return;
    }
}

// 로그인 화면
// @return 로그인 취소 = -1, 로그인 성공 = 사용자 인덱스
int UserLoginToAccount()
{
    char id[50];
    char password[50];

    while (1)
    {
        ClearScreen();
        DrawUIBorder();
        PrintCenteredText(2, "===== 로그인 =====");

        printf("\033[4;5H아이디 입력 (ESC로 취소): ");
        if (HandleStringInput(id, sizeof(id)) == -1) // ESC 입력 처리
        {
            return -1; // 로그인 취소
        }

        printf("\033[5;5H비밀번호 입력 (ESC로 취소): ");
        if (HandleHiddenStringInput(password, sizeof(password)) == -1) // ESC 입력 처리
        {
            return -1; // 로그인 취소
        }

        // 로그인 검증
        for (int i = 0; i < userCount; i++)
        {
            if (strcmp(users[i].id, id) == 0 && strcmp(users[i].password, password) == 0)
            {
                printf("\033[7;5H로그인 성공! 환영합니다, %s님.", users[i].name);
                Sleep(2000);
                return i; // 로그인 성공
            }
        }

        // 로그인 실패
        printf("\033[7;5H아이디 또는 비밀번호가 잘못되었습니다. 다시 시도하세요.");
        Sleep(2000);
    }

    return -1; // 로그인 실패
}

// 유저 메뉴 출력
// @param user 유저
void UserMenu(User *user)
{
    int selectedOption = 0; // 현재 선택된 옵션
    const int numOptions = 6; // 메뉴 옵션 개수
    ClearScreen(); // 화면 초기화
    DrawUIBorder();
    while (1)
    {
        
        PrintCenteredText(2, "===== 사용자 메뉴 =====");
        

        // 메뉴 옵션
        const char *menuOptions[] = {
            "계좌 조회",
            "입금",
            "출금",
            "송금",
            "계좌 추가",
            "로그아웃"
        };

        // 메뉴 출력
        for (int i = 0; i < numOptions; i++)
        {
            if (i == selectedOption)
                printf("\033[%d;5H> %s", 4 + i, menuOptions[i]);
            else
                printf("\033[%d;5H  %s", 4 + i, menuOptions[i]);
        }

        // 키 입력 처리
        char input = _getch();
        if (input == 'w' || input == 'W' || input == 72) // W 또는 위 화살표
        {
            if (selectedOption > 0)
                selectedOption--;
        }
        else if (input == 's' || input == 'S' || input == 80) // S 또는 아래 화살표
        {
            if (selectedOption < numOptions - 1)
                selectedOption++;
        }
        else if (input == '\r') // Enter
        {
            ClearScreen(); // 들어갈때 화면 초기화
            DrawUIBorder();
            switch (selectedOption)
            {
            case 0: // 계좌 조회
                ViewAccounts(user);
                break;
            case 1: // 입금
                ProcessTransaction(user, 1); // 1 = 입금
                break;
            case 2: // 출금
                ProcessTransaction(user, 0); // 0 = 출금
                break;
            case 3: // 송금
                HandleTransfer(user);
                break;
            case 4: // 계좌 추가
                AddAccount(user);
                break;
            case 5: // 로그아웃
                return; // 로그아웃 시 메뉴 종료
            }
            ClearScreen(); // 나올때 화면 초기화
            DrawUIBorder();
        }
    }
}

// 메인 메뉴 출력
// @return 메뉴 옵션
int PrintMainMenu()
{
    ClearScreen();
    DrawUIBorder();
    PrintCenteredText(2, "===== 메인 메뉴 =====");

    const char *menuOptions[] = {
        "로그인",
        "회원가입",
        "종료"
    };
    const int numOptions = 3;
    int selectedOption = 0;

    while (1)
    {
        // 메뉴 출력
        for (int i = 0; i < numOptions; i++)
        {
            if (i == selectedOption)
                printf("\033[%d;5H> %s", 4 + i, menuOptions[i]);
            else
                printf("\033[%d;5H  %s", 4 + i, menuOptions[i]);
        }

        // 키 입력 처리
        char input = _getch();
        if (input == 'w' || input == 'W' || input == 72) // W 또는 위 화살표
        {
            if (selectedOption > 0)
                selectedOption--;
        }
        else if (input == 's' || input == 'S' || input == 80) // S 또는 아래 화살표
        {
            if (selectedOption < numOptions - 1)
                selectedOption++;
        }
        else if (input == '\r') // Enter
        {
            return selectedOption; // 선택된 옵션 반환
        }
    }
}

// JSON파일 저장 / 로드
// @param filename 파일 이름
void SaveAccountsToFile(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    fprintf(file, "[\n");
    for (int i = 0; i < userCount; i++)
    {
        User *user = &users[i];
        fprintf(file, "  {\n");
        fprintf(file, "    \"name\": \"%s\",\n", user->name);
        fprintf(file, "    \"id\": \"%s\",\n", user->id);
        fprintf(file, "    \"password\": \"%s\",\n", user->password);
        fprintf(file, "    \"accounts\": [\n");

        for (int j = 0; j < user->accountCount; j++)
        {
            Account *account = &user->account[j];
            fprintf(file, "      {\n");
            fprintf(file, "        \"accountNumber\": \"%s\",\n", account->accountNumber);
            fprintf(file, "        \"openDate\": \"%s\",\n", account->openDate);
            fprintf(file, "        \"balance\": %lld,\n", account->balance);
            fprintf(file, "        \"history\": [\n");

            for (int k = 0; k < account->historyCount; k++)
            {
                Transaction *transaction = &account->history[k];
                fprintf(file, "          {\n");
                fprintf(file, "            \"date\": \"%s\",\n", transaction->date);
                fprintf(file, "            \"description\": \"%s\",\n", transaction->description);
                fprintf(file, "            \"amount\": %lld,\n", transaction->amount);
                fprintf(file, "            \"balanceAfter\": %lld\n", transaction->balanceAfter);
                fprintf(file, "          }%s\n", (k < account->historyCount - 1) ? "," : "");
            }

            fprintf(file, "        ]\n");
            fprintf(file, "      }%s\n", (j < user->accountCount - 1) ? "," : "");
        }

        fprintf(file, "    ]\n");
        fprintf(file, "  }%s\n", (i < userCount - 1) ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
    printf("데이터가 성공적으로 저장되었습니다: %s\n", filename);
}
// @param filename 파일 이름
void LoadAccountsFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("파일을 열 수 없습니다: %s. 초기 데이터로 시작합니다.\n", filename);
        return;
    }

    char line[1024];
    User *currentUser = NULL;
    Account *currentAccount = NULL;
    Transaction *currentTransaction = NULL;
    int inAccounts = 0, inHistory = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "\"name\":"))
        {
            currentUser = &users[userCount++];
            currentUser->accountCount = 0; // 초기화
            sscanf(line, " \"name\": \"%[^\"]\",", currentUser->name);
        }
        else if (strstr(line, "\"id\":"))
        {
            sscanf(line, " \"id\": \"%[^\"]\",", currentUser->id);
        }
        else if (strstr(line, "\"password\":"))
        {
            sscanf(line, " \"password\": \"%[^\"]\",", currentUser->password);
        }
        else if (strstr(line, "\"accounts\": ["))
        {
            inAccounts = 1;
        }
        else if (inAccounts && strstr(line, "\"accountNumber\":"))
        {
            currentAccount = &currentUser->account[currentUser->accountCount++];
            currentAccount->historyCount = 0; // 초기화
            sscanf(line, " \"accountNumber\": \"%[^\"]\",", currentAccount->accountNumber);
        }
        else if (inAccounts && strstr(line, "\"openDate\":"))
        {
            sscanf(line, " \"openDate\": \"%[^\"]\",", currentAccount->openDate);
        }
        else if (inAccounts && strstr(line, "\"balance\":"))
        {
            sscanf(line, " \"balance\": %lld,", &currentAccount->balance);
        }
        else if (inAccounts && strstr(line, "\"history\": ["))
        {
            inHistory = 1;
        }
        else if (inHistory && strstr(line, "\"date\":"))
        {
            currentTransaction = &currentAccount->history[currentAccount->historyCount++];
            sscanf(line, " \"date\": \"%[^\"]\",", currentTransaction->date);
        }
        else if (inHistory && strstr(line, "\"description\":"))
        {
            sscanf(line, " \"description\": \"%[^\"]\",", currentTransaction->description);
        }
        else if (inHistory && strstr(line, "\"amount\":"))
        {
            sscanf(line, " \"amount\": %lld,", &currentTransaction->amount);
        }
        else if (inHistory && strstr(line, "\"balanceAfter\":"))
        {
            sscanf(line, " \"balanceAfter\": %lld,", &currentTransaction->balanceAfter);
        }
        else if (strstr(line, "]"))
        {
            if (inHistory)
                inHistory = 0; // history 배열 끝
            else if (inAccounts)
                inAccounts = 0; // accounts 배열 끝
        }
        else if (strstr(line, "}"))
        {
            if (!inHistory && !inAccounts && currentUser != NULL)
                currentUser = NULL; // 사용자 정보 끝
        }
    }

    fclose(file);
    printf("데이터가 성공적으로 로드되었습니다: %s\n", filename);
}


void MainLoop()
{
    int loggedInUserIndex = -1; // 로그인 상태 추적 (-1: 로그인되지 않음)

    while (1)
    {
        if (loggedInUserIndex == -1) // 로그인되지 않은 상태
        {
            int menuChoice = PrintMainMenu(); // 메인 메뉴 출력 및 선택값 반환

            if (menuChoice == 0) // 로그인
            {
                loggedInUserIndex = UserLoginToAccount();
            }
            else if (menuChoice == 1) // 회원가입
            {
                RegisterUser();
            }
            else if (menuChoice == 2) // 종료
            {
                printf("프로그램을 종료합니다.\n");
                break;
            }
        }
        else // 로그인된 상태
        {
            UserMenu(&users[loggedInUserIndex]); // 로그인한 사용자 메뉴 호출
            loggedInUserIndex = -1; // 로그아웃 후 초기화
        }
    }

    // 프로그램 종료 시 JSON 파일에 데이터 저장
    SaveAccountsToFile("accounts.json");
}

int main()
{
    // 프로그램 시작 시 JSON 파일에서 데이터 로드
    LoadAccountsFromFile(FILENAME);
    //DrawUIBorder();//test
    // 정렬
    SortUsers();

    MainLoop();

    // 프로그램 종료 시 JSON 파일에 데이터 저장
    SaveAccountsToFile(FILENAME);

    return 0;
}