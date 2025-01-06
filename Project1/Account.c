//Account.c
//유저의 계좌, 송금(입출금) 관련 관리
#include "std.h"


// 계좌 개설 (usercount 필요)
// @param user 유저
void AddAccount(UserData* userData)
{
    User* user = userData->users;
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
    const char* bankOptions[] = {
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

        printf("\033[%d;5H(ESC로 뒤로가기)", HEIGHT - 2); //뒤로가기

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
    Account* newAccount = &user->account[user->accountCount++];
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
    struct tm* t = localtime(&now);
    // 연도의 마지막 두 자리만 쓰기 (예: 2024 -> 24)
    int year = t->tm_year % 100;

    // 랜덤 시드
    // 겹치는 경우가 많아서 좀 시드의 복잡성을 더함.
    // 랜덤 시드 = 계좌 수(1~) + (은행 코드(0810 => 810) * 날짜(일, 1 ~ 31)) + 유저 이름에 첫번째 ascii 값(32~127?)
    int randomSeedForAccountnum = (userData->userCount * user->accountCount) + (atoi(bankCode) * t->tm_mday) + (int)user->name[0];
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

// 계좌 출력 - 히스토리 창 접근 HEIGHT 필요
// @param user 유저
void ViewAccounts(User* user)
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
            char buffer[30];// AddCommas 1000 -> 1,000
            AddCommas(buffer, 30, user->account[i].balance);
            if (i == selectedAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%s", 4 + i, user->account[i].accountNumber, buffer);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%s", 4 + i, user->account[i].accountNumber, buffer);
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
            ClearScreen();
            DrawUIBorder();
        }
        else
        {
            selectedAccountIndex = inputResult; // 선택된 옵션 업데이트
        }
    }
}

