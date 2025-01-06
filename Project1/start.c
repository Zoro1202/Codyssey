//start.c
#include "std.h"

int main()
{
    // 프로그램 시작 시 JSON 파일에서 데이터 로드
    LoadAccountsFromFile(FILENAME);

    int loggedInUserIndex = -1; // 로그인 상태 추적 (-1: 로그인되지 않음)

    while (1)
    {
        if (loggedInUserIndex == -1) // 로그인되지 않은 상태
        {
            int menuChoice = PrintMainMenu(); // 메인 메뉴 출력 및 선택값 반환

            if (menuChoice == 0) // 로그인
            {
                loggedInUserIndex = UserLoginToAccount(getInstance());
            }
            else if (menuChoice == 1) // 회원가입
            {
                RegisterUser(getInstance());
            }
            else if (menuChoice == 2) // 종료
            {
                printf("프로그램을 종료합니다.\n");
                break;
            }
        }
        else // 로그인된 상태
        {
            UserMenu(&getInstance()->users[loggedInUserIndex]); // 로그인한 사용자 메뉴 호출
            loggedInUserIndex = -1; // 로그아웃 후 초기화
        }
    }
    // 프로그램 종료 시 JSON 파일에 데이터 저장
    SaveAccountsToFile(FILENAME);
    return 0;
}