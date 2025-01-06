//Control.c
// 입력, 출력 관련 기능 관리..
/**
* 최대한 매개 변수로만 받고...
* 전역 변수를 쓰지 말고...
* return 값 아니면 역참조로 반환...
* 역참조도 웬만하면 쓰지 말기...(문자열만 역참조로)
* 상수는 std에 선언. WIDTH, HEIGHT 등
*/
//*********************************************************
#include "std.h"

// 화면 지우기 
void ClearScreen()
{
    // 다 지우기
    system("cls"); // 콘솔 화면 지우기
}

// 사각형 그리기 WIDTH HEIGHT
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
long long HandleOnlyNumInput(const char* prompt, int maxLength)
{
    char input[50] = { 0 }; // 입력을 저장할 문자열
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
int HandleStringInput(char* buffer, int bufferSize)
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
int HandleHiddenStringInput(char* buffer, int bufferSize)
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
void GetCurrentDateTIME(char* buffer)
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t); // 컴퓨터 시간
    strftime(buffer, 20, "%Y-%m-%d %H:%M", tm_info);
}

// 현재 시간 저장 - 메뉴 옆에 출력하려고
// @param buffer "YYYY-MM-DD [WEEK]"
void GetCurrentDateWEEK(char* buffer)
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t); // 컴퓨터 시간
    strftime(buffer, 25, "%Y-%m-%d [%A]", tm_info);
}

// 타이틀 옆에 시간 출력 (외부 상수 WIDTH)
// @param row 행(높이)
void PrintCurrentDate(int row)
{
    char buffer[25];
    GetCurrentDateWEEK(buffer);
    printf("\033[%d;%dH%s", row + 1, WIDTH - 26, buffer);
}

// 타이틀 출력(가운데 정렬) (외부 상수 WIDTH)
// @param row 행(높이)
// @param text "할말"
void PrintCenteredText(int row, char* text)
{
    int padding = (WIDTH - (int)strlen(text)) / 2;
    printf("\033[%d;%dH%s", row, padding, text); // \033[%d;%dH 열 , 행
    PrintCurrentDate(row); // 타이틀 옆에 날짜 출력
}

// 천의 자리 숫자마다 콤마 찍기
void AddCommas(char* buffer, int bufferSize, long long number)
{
    char temp[50];
    int isNegative = 0;

    // 음수 처리
    if (number < 0)
    {
        isNegative = 1;       // 음수 플래그 설정
        number = -number;     // 절대값으로 변환
    }

    snprintf(temp, sizeof(temp), "%lld", number); // 숫자를 문자열로 변환

    int len = (int)strlen(temp);

    // 숫자가 3자리 이하면 쉼표 없이 그대로 반환
    if (len <= 3)
    {
        if (isNegative)
        {
            snprintf(buffer, bufferSize, "-%s", temp);
        }
        else
        {
            snprintf(buffer, bufferSize, "%s", temp);
        }
        return;
    }

    // 새로운 길이를 계산 (쉼표 포함)
    int commaCount = (len - 1) / 3;
    int newLen = len + commaCount;

    if (newLen + isNegative >= bufferSize) // 음수 부호 포함 길이 확인
    {
        buffer[0] = '\0'; // 버퍼가 부족하면 빈 문자열 반환
        return;
    }

    int srcIndex = len - 1;
    int destIndex = newLen - 1;

    buffer[newLen + isNegative] = '\0'; // 종료 문자

    int groupCounter = 0;
    while (srcIndex >= 0)
    {
        buffer[destIndex--] = temp[srcIndex--];
        groupCounter++;
        if (groupCounter == 3 && srcIndex >= 0)
        {
            buffer[destIndex--] = ','; // 그룹 끝에 쉼표 추가
            groupCounter = 0;
        }
    }

    // 음수 부호 추가
    if (isNegative)
    {
        buffer[destIndex] = '-';
    }
}
