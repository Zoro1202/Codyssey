//data.c
// 전역 데이터 관리 (싱글톤 패턴)
#include "std.h"

static UserData instance;

UserData* getInstance()
{
    return &instance; // 
}

User* getUsers() {
    return &getInstance()->users;  // users만 반환
}

void setUsers(const User* user, int userCount) {
    UserData* instance = getInstance();
    memcpy(instance->users, user, sizeof(User));           // 데이터 복사 (배열이라 memcpy 사용)
    instance->userCount = userCount;
}