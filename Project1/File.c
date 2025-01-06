//File.c
//파일 관리
//세이브 / 로드
#include "std.h"

// JSON파일 저장 / 로드 
// @param filename 파일 이름 users userCount
void SaveAccountsToFile(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    fprintf(file, "[\n");
    int userCount = (getInstance()->userCount);
    for (int i = 0; i < userCount; i++)
    {
        User* user = getUsers();
        fprintf(file, "  {\n");
        fprintf(file, "    \"name\": \"%s\",\n", user->name);
        fprintf(file, "    \"id\": \"%s\",\n", user->id);
        fprintf(file, "    \"password\": \"%s\",\n", user->password);
        fprintf(file, "    \"accounts\": [\n");

        for (int j = 0; j < user->accountCount; j++)
        {
            Account* account = &user->account[j];
            fprintf(file, "      {\n");
            fprintf(file, "        \"accountNumber\": \"%s\",\n", account->accountNumber);
            fprintf(file, "        \"openDate\": \"%s\",\n", account->openDate);
            fprintf(file, "        \"balance\": %lld,\n", account->balance);
            fprintf(file, "        \"history\": [\n");

            for (int k = 0; k < account->historyCount; k++)
            {
                Transaction* transaction = &account->history[k];
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

// @param filename 파일 이름 users  historyCount  accountCount
void LoadAccountsFromFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("파일을 열 수 없습니다: %s. 초기 데이터로 시작합니다.\n", filename);
        return;
    }

    char line[1024];
    User* currentUser = NULL;
    Account* currentAccount = NULL;
    Transaction* currentTransaction = NULL;
    int inAccounts = 0, inHistory = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "\"name\":"))
        {
            currentUser = &getInstance()->users[getInstance()->userCount++];
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
