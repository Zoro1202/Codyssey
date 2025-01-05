//Transaction.h
#pragma once


void AddAcountHistory(Account* account, long long amount, const char* description);
void ViewAccountHistory(Account* account);
void ProcessTransaction(User* user, int isDeposit);
void HandleTransfer(User* fromUser);
