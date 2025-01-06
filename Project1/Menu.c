//Menu.c
//메뉴 출력
#include "std.h"

// 유저 메뉴 출력 dd
// @param user 유저
void UserMenu(User* user)
{
    int selectedOption = 0; // 현재 선택된 옵션
    const int numOptions = 6; // 메뉴 옵션 개수
    ClearScreen(); // 화면 초기화
    DrawUIBorder();
    while (1)
    {

        PrintCenteredText(2, "===== 사용자 메뉴 =====");


        // 메뉴 옵션
        const char* menuOptions[] = {
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
                AddAccount(getInstance());
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

    const char* menuOptions[] = {
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

