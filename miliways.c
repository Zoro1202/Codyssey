// UTF-8
// 미완20241108
#include "stdio.h"
#include "stdlib.h"

//닉네임 : Ariel , 학력 : 중졸 , 키 : 175cm , 몸무게 : 60 , 혈액형 : A , 알러지 : 땅콩 , 취미 : 댄스 연습 작곡 , SNS : Instagram - @Ariel_Kim
//"이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국BMI:18.5,주특기:댄스,보조특기:작곡,한국어등급:0,MBTI:ENFJ,ID:123456,합격여부:O"
typedef struct{
    char *name;
    char *birth;
    char *sex;
    char *email;
    char *country;
    //char 
} user_data;
// 아이디어 : 따로 변수를 만들지 않고 그냥 주소값만 저장해놓고 쓰면, 메모리를 아낄수있지않나?
//파싱해서 \0을 넣은 값을 저장할거니까 print를 할거면 그냥 순서대로 받으니까 순서대로 나가게 걍 어쩌고 저쩌고

// TEST 용 "이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국"
void input_candidates(char **candidate){
    int size = 50;
    *candidate = (char*)malloc(size * sizeof(char));
    int index = 0;
    char c = 0;

    // 메모리 할당 실패 체크
    if (*candidate == NULL) {
        printf("메모리 할당 실패\n");
        return;
    }

    while ((c = getchar()) != '\n' && c != EOF) {
        // 배열 크기 조정하기
        if (index >= size) { // 조건을 index >= size로 변경
            size *= 2; // 크기 작을 거 같으면 두 배로 늘려버리기
            char *temp = (char*)realloc(*candidate, size * sizeof(char));
            
            // realloc 실패 체크
            if (temp == NULL) {
                printf("메모리 할당 실패\n");
                free(*candidate); // 기존 할당 해제
                *candidate = NULL; // NULL로 설정하여 메모리 해제 표시
                return;
            }
            
            *candidate = temp; // 재할당 성공 시 포인터 업데이트
        }
        (*candidate)[index++] = c;
    }
    (*candidate)[index] = '\0'; // 문자열 끝에 null 문자 추가
}

// TEST 용 "이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국"
user_data data_parsing(char *candidate){
    
    user_data userdata;
    int field_index = 0;
    char **field[5] = {&userdata.name,&userdata.birth,&userdata.sex,&userdata.email,&userdata.country};
    int index = 0;

    // 구분자 검사하고 머 문자열 구분 해주는 포인터 저장 변수임
    char *token = candidate;
    printf("[TEST]\n");
    while (token[index] != '\0'){ 
        if(token[index] == '\"' || token[index] == ','){ 
            token[index] = '\0'; // '\"' 랑 ',' 구분 해서 문자열 끊기
            printf("%s\n", token); // 끊어진 문자열 출력 test
            if(field_index < 6 && (token[0] != '\0')){ //빈 값이 들어가버림 방지.. 
                *field[field_index++] = token; // 끊은 문자열 처음 주소 넣기
            }
            token = token + index + 1; // 다음 문자열로 이동 ㄱㄱ
            index = 0;
        } else {
            index++;
        }
    }
    printf("%s\n", token);
    return userdata;
}

//문자열 길이 계산
int get_length(char *str){
    int length = 0;
    while(str[length] != '\0'){
        length++;
    }
    return length;
}

    // candidate##_arr에서 합격한 멤버에 대해 아래 제약 사항의 추가 데이터에 관해 인터뷰하는 코드를 작성한다. 이 때 for 반복문을 사용한다. 
    // 닉네임 : Ariel , 학력 : 중졸 , 키 : 175cm , 몸무게 : 60 , 혈액형 : A , 알러지 : 땅콩 , 취미 : 댄스 연습 작곡 , SNS : Instagram - @Ariel_Kim
    // candidate##_arr에서 합격자의 기존 기초 정보를 복사해 새로운 milliways##_arr 배열에 복사하고, 인터뷰를 통해 수집된 정보를 추가 한다. 
void merge_candidate(char *candidate, char **milliways){ 
    //candidate 값 참조한 포인터의 포인터를 가르킴 -> 값
    int size = get_length(candidate);
    *milliways = (char*)malloc(size + 1); // 빈공간을 위한 +1
    
    for(int i = 0; i < size; i++){ 
        (*milliways)[i] = candidate[i]; // 복사
    }
    (*milliways)[size] = '\0'; // 빈공간 넣어주기

    //새로운 값 입력받기
    int index = size;
    char c = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        // 배열 크기 조정하기
        if (index >= size) { // 조건을 index >= size로 변경
            size += 12; // 크기 조금씩 늘리기 한 +12씩?
            char *temp = (char*)realloc(*milliways, size * sizeof(char));
            
            // realloc 실패 체크
            if (temp == NULL) {
                printf("메모리 할당 실패\n");
                free(*milliways); // 기존 할당 해제
                *milliways = NULL; // NULL로 설정하여 메모리 해제 표시
                return;
            }
            
            *milliways = temp; // 재할당 성공 시 포인터 업데이트
        }
        (*milliways)[index++] = c;
    }
    (*milliways)[index] = '\0'; // 문자열 끝에 null 문자 추가

}

// TEST 용 "이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국"
int main(){
    // 문제 3의 후보자 6명의 데이터(소개는 제외)에 후보자 고유 ID(문제 5에서 생성), 합격 여부가 포함된 JSON 형식의 문자열을 담은 배열 candidate##_arr로 작성한다.
    char *candidate00_arr;
    user_data candidate0_poss;
    char *milliways00_arr;
//    char *candidate01_arr;
//    char *candidate02_arr;
//    char *candidate03_arr;
//    char *candidate04_arr;
//    char *candidate05_arr;
    
    //예시
    //"이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국BMI:18.5,주특기:댄스,보조특기:작곡,한국어등급:0,MBTI:ENFJ,ID:123456,합격여부:O"
    // getchar() 로 받아서 동적 배열로 늘려야지 - OK
    
    input_candidates(&candidate00_arr);
    merge_candidate(candidate00_arr, &milliways00_arr);
    candidate0_poss = data_parsing(milliways00_arr);


    // TEST
    printf("%s, %s, %s, %s, %s", candidate0_poss.name,candidate0_poss.birth,candidate0_poss.sex,candidate0_poss.email,candidate0_poss.country);
//    char *milliways01_arr;
//    char *milliways02_arr;
//    char *milliways03_arr;
//    char *milliways04_arr;
//    char *milliways05_arr;
    // 추가 데이터 중 몸무게는 키와 BMI 값으로 계산하여 채운다. BMI = (체중kg) / ((키m)^2), 기존의 BMI 값은 없애고 새로 넣기?
    // milliways##_arr 내용을 리스트 형식으로 출력한다.
    // (보너스) milliways##_arr 내용을 리스트 형식으로 출력할 때 BMI 값에 따라 저체중, 정상, 과체중, 비만을 표시한다.

    free(candidate00_arr);
    free(milliways00_arr);

    return 0;
}