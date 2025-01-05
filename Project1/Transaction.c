//Transaction.c
//계좌의 거래 내역을 관리.
#include "std.h"
#define USERDATA

/** 히스토리 추가하기
 * @param account 유저->계좌
 * @param amount 얼마?
 * @param description "설명"
 */
void AddAcountHistory(Account* account, long long amount, const char* description)
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
    Transaction* newTransaction = &account->history[account->historyCount];
    GetCurrentDateTIME(newTransaction->date);
    strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
    newTransaction->description[sizeof(newTransaction->description) - 1] = '\0'; // 안전하게 NULL 종료
    newTransaction->amount = amount;
    newTransaction->balanceAfter = account->balance; // 현재 잔액 기준으로 잔액 저장
    account->historyCount++; // 기록 수 증가
}

// 히스토리 출력 (HEIGHT 필요)
void ViewAccountHistory(Account* account)
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
                Transaction* transaction = &account->history[i];
                char buffer[30]; //addCommas 1000 -> 1,000
                AddCommas(buffer, 30, transaction->amount);
                printf("\033[%d;5H[%s] %s | 금액: %s | 잔액: ",
                    4 + i,
                    transaction->date,
                    transaction->description,
                    buffer);
                AddCommas(buffer, 30, transaction->balanceAfter);
                printf("%s",
                    buffer);
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

/** 입출금 화면 HEIGHT
 * @param user 유저
 * @param isDeposit 0 = 입금, 1 = 출금
 */
void ProcessTransaction(User* user, int isDeposit)
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
            char buffer[30];// AddCommas 1000 -> 1,000
            AddCommas(buffer, 30, user->account[i].balance);
            if (i == selectedAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%s", 4 + i, user->account[i].accountNumber, buffer);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%s", 4 + i, user->account[i].accountNumber, buffer);
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

    Account* selectedAccount = &user->account[selectedAccountIndex];

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

// 송금 계좌 선택 userCount, getInstance()->users(이거때문에 좀 복잡해짐) , HEIGHT dd
// @param fromUser 송금할 유저
void HandleTransfer(User* fromUser)
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
            char buffer[30];// AddCommas 1000 -> 1,000
            AddCommas(buffer, 30, fromUser->account[i].balance);
            if (i == fromAccountIndex)
                printf("\033[%d;5H> 계좌번호: %s | 잔액: ₩%s", 4 + i, fromUser->account[i].accountNumber, buffer);
            else
                printf("\033[%d;5H  계좌번호: %s | 잔액: ₩%s", 4 + i, fromUser->account[i].accountNumber, buffer);
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

    Account* fromAccount = &fromUser->account[fromAccountIndex];

    // 2. 수취 사용자 선택
    int targetUserIndex = 0;

    ClearScreen();
    DrawUIBorder();
    while (1)
    {
        PrintCenteredText(2, "===== 송금: 수취 사용자 선택 =====");
        
        for (int i = 0; i < getInstance()->userCount; i++)
        {
            // !!!!!!!!!!!!!!!!!! 여기 참조를 이상하게 하고 있음 !!!!!!!!!!!!!!!!!!!!!! 반드시 고쳐!!!!!!!!!!!!!! 
            //if (&users[i] != fromUser) // 자신 제외
            {
                if (i == targetUserIndex)
                    printf("\033[%d;5H> %s", 4 + i, getInstance()->users[i].name);
                else
                    printf("\033[%d;5H  %s", 4 + i, getInstance()->users[i].name);
            }
        }

        printf("\033[%d;5H(ESC로 취소)", HEIGHT - 2);

        int inputResult = HandleMenuInput(targetUserIndex, getInstance()->userCount);

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

    User* toUser = &getInstance()->users[targetUserIndex];

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
                printf("\033[%d;5H> 계좌번호: %s | 사용자: %s", 4 + i, toUser->account[i].accountNumber, toUser->name);
            else
                printf("\033[%d;5H  계좌번호: %s | 사용자: %s", 4 + i, toUser->account[i].accountNumber, toUser->name);
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
            if (toAccountIndex == fromAccountIndex && fromUser->id == getInstance()->users[targetUserIndex].id) { // 같은 계정 같은 계좌 막기.ㅇㅇ
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

    Account* toAccount = &toUser->account[toAccountIndex];

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
