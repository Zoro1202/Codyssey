//User.c
//유저의 계정을 관리, 로그인, 회원가입 등...
#include "std.h"

// 회원가입 (userCount, MAX_USERS, users 필요) dd
void RegisterUser(UserData *userData)
{
    User* users = userData->users;
    int* userCount = &userData->userCount;
    if (*userCount >= MAX_USERS)
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
    fgets(users[*userCount].name, sizeof(users[*userCount].name), stdin); // handleStringInput은 한글은 처리 못함;
    users[*userCount].name[strcspn(users[*userCount].name, "\n")] = 0; // 개행문자 제거

    while (1)
    {
        printf("\033[%d;%dH아이디 입력(ESC로 뒤로가기): ", 5, 5);
        if (HandleStringInput(users[*userCount].id, sizeof(users[*userCount].id)) == -1)
        {
            printf("\033[%d;%dH입력이 취소되었습니다.\n", 8, 5);
            Sleep(2000);
            return;
        }

        // 아이디 중복 확인
        int isDuplicate = 0;
        for (int i = 0; i < *userCount; i++)
        {
            if (strcmp(users[*userCount].id, users[i].id) == 0)
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
    if (HandleHiddenStringInput(users[*userCount].password, sizeof(users[*userCount].password)) == -1)
    {
        printf("\033[%d;%dH입력이 취소되었습니다.\n", 8, 5);
        Sleep(2000);
        return;
    }

    // 초기 계좌 개수 설정
    users[*userCount].accountCount = 0;

    printf("\033[%d;%dH회원가입 완료! 환영합니다, %s님.", 8, 5, users[*userCount].name);
    Sleep(2000);

    // 2. 계좌 개설 여부 확인
    char choice;
    printf("\033[%d;%dH계좌를 개설하시겠습니까? (Y/N): ", 9, 5);
    scanf(" %c", &choice);
    getchar(); // 버퍼 비우기

    if (choice == 'Y' || choice == 'y')
    {
        AddAccount(&users[*userCount]); // 계좌 추가
    }

    (*userCount)++;
}

// 로그인 화면 users userCount
// @return 로그인 취소 = -1, 로그인 성공 = 사용자 인덱스
int UserLoginToAccount(UserData* userData)
{
    User* users = userData->users;
    int* userCount = &userData->userCount;

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
        for (int i = 0; i < *userCount; i++)
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
