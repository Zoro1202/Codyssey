//data.h
#pragma once
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
    long long balanceAfter;
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

typedef struct {
    User users[MAX_USERS];  // User 타입 배열
    int userCount;          // 현재 사용자 수
} UserData;

UserData* getInstance();

User* getUsers();

void setUsers(const User* user, int userCount);
