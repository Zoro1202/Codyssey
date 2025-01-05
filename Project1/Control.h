#pragma once
void ClearScreen();
void DrawUIBorder();
int HandleMenuInput(int selectedOption, int numOptions);
long long HandleOnlyNumInput(const char* prompt, int maxLength);
int HandleStringInput(char* buffer, int bufferSize);
int HandleHiddenStringInput(char* buffer, int bufferSize);
void GetCurrentDateTIME(char* buffer);
void GetCurrentDateWEEK(char* buffer);
void PrintCurrentDate(int row);
void PrintCenteredText(int row, char* text);
void AddCommas(char* buffer, size_t bufferSize, long long number);
