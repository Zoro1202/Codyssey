// UTF-8
#include "stdio.h"
#include "stdlib.h"

int main(){
    //심사자 풀은 1차원 배열 하나로 관리, 배열 요소 하나에 완전한 심사자 데이터를 json 형식으로 저장. 배열 이름은 judges_array.
    char *judges_array;
    char project_name[100];
    int judges_pool = 0;
    int select_member = 0;
    //참여 프로젝트 이름 입력받기
    printf("#####################################\n");
    printf("#       심사자 풀 데이터 입력       #\n");
    printf("#####################################\n");
    int index = 0; // 이ㅏㄴ뒉스~
    char c; // 한문자씩 사이즈 체크하면서 받을거
    printf("> 참여 프로젝트: ");
    while((c = getchar()) != '\n' && c != EOF){ // 개행문자(엔터)나 EOF(end of file, ctrl + z) 전까지 입력 받기
        project_name[index++] = c;
    }
    project_name[index] = '\0'; //  뒤에 null문자 넣어서 오 류 해결
    printf("%s", project_name); // test
    //심사 풀 인원 입력받기
    printf("> 심사 풀 인원: ");
    scanf("%d", &judges_pool);
    //선발 멤버 수 입력받기
    printf("> 선발 멤버 수: ");
    scanf("%d", &select_member);

    while(getchar() != '\n'){}// 버퍼 초기화

    //풀 인원 만큼 정보 입력받기. "이름, 성별, 소속, 직함, 전문분야, 메일, 전화" Json 형식으로
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("5명의 심사자 정보 입력을 시작합니다.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");
    //동적 배열로 받는 게 좋을까? 아님 크기를 확 늘려버릴까?
    //배열 크기 나중에 늘려야함
    int size = 20;
    //인덱스 재활용
    index = 0;
    //메모리 할당하기
    judges_array = (char*)malloc(size * sizeof(char));
    // null check
    if(judges_array == NULL){
        printf("메모리 할당 실패 ㅠㅠ..\n");
        return 1;
    }

    // *심사자 1: "이름:김영수,성별:남,소속:JYK 스튜디오,직함:음악 감독 및 프로듀서,전문분야:음악,메일:youngsu@outlook.com,전화:010-1234-5678"
    // "이름:초코맨,성별:바이섹슈얼,소속:CB 스튜디오,직함:초콜렛 만들기 전문가,전문분야:초코 만들기,메일:cman1818@outlook.com,전화:010-3213-1234"
    //입력받기
    for(int i = 1; i<=judges_pool;i++){
        //while(getchar() != '\n'){}// 버퍼 초기화
        printf("*심사자 %d: ", i);
        while ((c = getchar()) != '\n' && c != EOF)
        {
            //배열 크기 조정하기
            if(index >= size - 1){
                size *= 2; //크기 작을거같으면 두배로 늘려버리기
                judges_array = (char *)realloc(judges_array, size * sizeof(char));
                // null check
                if(judges_array == NULL){
                    printf("메모리 할당 실패 ㅠㅠ..\n");
                    return 1;
                }
            }
            judges_array[index++] = c;
        }
        //judges_array[index++] = '\0'; // 뭔가 다른 문자 넣어야함. 이거때매 파싱이 여러번 안됨. 아무도 안쓸거같은 문자뭐있지
        judges_array[index++] = '`';
    }

    //심사자 정보 입력 끝.
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("심사자 풀 입력이 끝났습니다.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");
    //심사자 풀 확인? Y / N
    printf("[PROJECT] 심사자 풀을 확인할까요? ");
    if('Y' == getchar()){
        // Y의 경우 데이터 파싱해서 선발 멤버만큼 출력 [심사자 1], [심사자 2] ...
        
        printf("####################################\n");
        printf("#       심사자 풀 데이터출력       #\n");
        printf("####################################\n");
        //데이터 파싱

        // "이름:김영수,성별:남,소속:JYK 스튜디오,직함:음악 감독 및 프로듀서,전문분야:음악,메일:youngsu@outlook.com,전화:010-1234-5678"
        // 어카지... "랑 , 로 구분하고 이름 : 어쩌고 ':' 이거도 검사

        //인덱스 재활용
        index = 0;
        // 구분자 검사하고 머 문자열 구분 해주는 포인터 저장 변수임
        char *token = judges_array;
        for(int i = 0; i < select_member; i++){
            printf("[심사자 %d]", i+1);
            while (token[index] != '\0' && token[index] != '`'){ 
                if(token[index] == '\"' || token[index] == ','){ 
                    token[index] = '\0'; // '\"' 랑 ',' 구분 해서 문자열 끊기
                    printf("%s\n", token); // 끊어진 문자열 출력 test
                    token = token + index + 1; // 다음 문자열로 이동 ㄱㄱ
                    index = 0;
                } else {
                    index++;
                }
            }
            if(token[index] == '`'){
                token[index] = '\0'; // 대충 255(안쓸거같은 문자)이 다음 심사자와 구분하는 문자임.
                printf("-----------------------------------\n", token); // 벽
                token = token + index + 1; // 다음 문자열로 이동 ㄱㄱ
                index = 0; // 인덱스 초기화
            }
        }
    } else;
    

    //메모리 해제
    free(judges_array);

    printf("\n");

    return 0;
}