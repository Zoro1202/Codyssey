// UTF-8
// 미완20241108
#include "stdio.h"
#include "stdlib.h"

#define FIELDLEN 20
// 정해진 형식이 아니면 안됨 되긴하는데 안됨
//"이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국BMI:18.5,주특기:댄스,보조특기:작곡,한국어등급:0,MBTI:ENFJ,ID:123456,합격여부:O"
//"이름:박지연,닉네임:Ariel,학력:중졸,키:175cm,몸무게:60,혈액형:A,알러지:땅콩,취미:댄스연습작곡,SNS:Instagram-@Ariel_Kim"
typedef struct
{
    // 이름, 생일, 성별, 이메일, 국적, 특기, 보조특기, 한국어등급, 엠비티아이, 아이디, 합격여부
    char *name, *birth, *sex, *email, *country, *skill, *sub, *kor, *mbti, *id, *pass;
    // 추가: 이름, 닉네임, 학력, 키(m), 몸무게(kg), 혈액형, 알러지, 취미, SNS
    char *pname, *aka, *grade, *tall, *weight, *abo, *allergy, *hobby, *sns;
    // char *
} user_field;

// TEST 용 "이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국"
void input_candidates(char **candidate)
{
    int size = 50;
    *candidate = (char *)malloc(size * sizeof(char));
    int index = 0;
    char c = 0;

    // 메모리 할당 실패 체크
    if (*candidate == NULL)
    {
        printf("메모리 할당 실패\n");
        return;
    }

    while ((c = getchar()) != '\n' && c != EOF)
    {
        // 배열 크기 조정하기
        if (index >= size)
        {              // 조건을 index >= size로 변경
            size *= 2; // 크기 작을 거 같으면 두 배로 늘려버리기
            char *temp = (char *)realloc(*candidate, size * sizeof(char));

            // realloc 실패 체크
            if (temp == NULL)
            {
                printf("메모리 할당 실패\n");
                free(*candidate);  // 기존 할당 해제
                *candidate = NULL; // NULL로 설정하여 메모리 해제 표시
                return;
            }

            *candidate = temp; // 재할당 성공 시 포인터 업데이트
        }
        (*candidate)[index++] = c;
    }
    (*candidate)[index] = '\0'; // 문자열 끝에 null 문자 추가
}

// 문자열 길이 계산
int get_length(char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// candidate##_arr에서 합격한 멤버에 대해 아래 제약 사항의 추가 데이터에 관해 인터뷰하는 코드를 작성한다. 이 때 for 반복문을 사용한다.
// 닉네임 : Ariel , 학력 : 중졸 , 키 : 175cm , 몸무게 : 60 , 혈액형 : A , 알러지 : 땅콩 , 취미 : 댄스 연습 작곡 , SNS : Instagram - @Ariel_Kim
// candidate##_arr에서 합격자의 기존 기초 정보를 복사해 새로운 milliways##_arr 배열에 복사하고, 인터뷰를 통해 수집된 정보를 추가 한다.
void merge_candidate(char *candidate, char **milliways)
{
    // candidate 값 참조한 포인터의 포인터를 가르킴 -> 값
    int size = get_length(candidate);
    *milliways = (char *)malloc(size + 1); // 빈공간을 위한 +1

    for (int i = 0; i < size; i++)
    {
        (*milliways)[i] = candidate[i]; // 복사
    }
    (*milliways)[size] = '\0'; // 빈공간 넣어주기

    // 새로운 값 입력받기
    int index = size;
    char c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // 배열 크기 조정하기
        if (index >= size)
        {               // 조건을 index >= size로 변경
            size += 12; // 크기 조금씩 늘리기 한 +12씩?
            char *temp = (char *)realloc(*milliways, size * sizeof(char));

            // realloc 실패 체크
            if (temp == NULL)
            {
                printf("메모리 할당 실패\n");
                free(*milliways);  // 기존 할당 해제
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
user_field data_parsing(char *candidate)
{
    user_field ud;
    int field_index = 0;
    char **field[FIELDLEN] = {
        &ud.name, &ud.birth, &ud.sex, &ud.email, &ud.country, &ud.skill, &ud.sub, &ud.kor, &ud.mbti, &ud.id, &ud.pass,
        &ud.pname, &ud.aka, &ud.grade, &ud.tall, &ud.weight, &ud.abo, &ud.allergy, &ud.hobby, &ud.sns};
    int index = 0;
    // 구분자 검사하고 머 문자열 구분 해주는 포인터 저장 변수임
    char *token = candidate;
    //printf("[TEST]\n");
    while (token[index] != '\0')
    {
        if (token[index] == '\"' || token[index] == ',')
        {
            token[index] = '\0';   // '\"' 랑 ',' 구분 해서 문자열 끊기
            //printf("%s\n", token); // 끊어진 문자열 출력 test
            if (field_index < FIELDLEN && (token[0] != '\0'))
            {                                  // 빈 값이 들어가버림 방지..
                *field[field_index++] = token; // 끊은 문자열 처음 주소 넣기
            }
            token = token + index + 1; // 다음 문자열로 이동 ㄱㄱ
            index = 0;
        }
        else
        {
            index++;
        }
    }
    //printf("%s\n", token);
    return ud;
}

void print_milliways(user_field userdata)
{
    
    printf("=============================================================================================\n");
    printf("  이름   |   생년월일   | 성별 |           이메일          | 국적 | 주특기 | 보조특기 | 한국어등급 | MBTI | ID | 합격여부 | 닉네임 | 학력 | 키 | 몸무게 | 혈액형 | 알러지 | 취미 | SNS \n");
    printf("=============================================================================================\n");
    printf(" %-5s | %8s | %2s | %20s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n",
           userdata.name+7, userdata.birth+13, userdata.sex+7, userdata.email+10, userdata.country+7, userdata.skill+10, userdata.sub+13, userdata.kor+16, userdata.mbti+5, userdata.id+3, userdata.pass+13,
           userdata.aka+10, userdata.grade+7, userdata.tall+4, userdata.weight+10, userdata.abo+10, userdata.allergy+10, userdata.hobby+7, userdata.sns+4);
    printf("-------------------------------------------------------------------------------------\n");

}



// TEST 용 "이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국"
int main()
{
    // 문제 3의 후보자 6명의 데이터(소개는 제외)에 후보자 고유 ID(문제 5에서 생성), 합격 여부가 포함된 JSON 형식의 문자열을 담은 배열 candidate##_arr로 작성한다.
    
    //첫 입력
    char *candidate00_arr, *candidate01_arr, *candidate02_arr, *candidate03_arr, *candidate04_arr, *candidate05_arr;
    //두번째 & 합친거
    char *milliways00_arr, *milliways01_arr, *milliways02_arr, *milliways03_arr, *milliways04_arr, *milliways05_arr;
    //ㄴ데이터 위치만
    user_field candidate0_poss,candidate1_poss,candidate2_poss,candidate3_poss,candidate4_poss,candidate5_poss;

    //1
    input_candidates(&candidate00_arr);
    merge_candidate(candidate00_arr, &milliways00_arr);
    candidate0_poss = data_parsing(milliways00_arr);
    //2
    input_candidates(&candidate01_arr);
    merge_candidate(candidate01_arr, &milliways01_arr);
    candidate1_poss = data_parsing(milliways01_arr);
    //3
    input_candidates(&candidate02_arr);
    merge_candidate(candidate02_arr, &milliways02_arr);
    candidate2_poss = data_parsing(milliways02_arr);
    //4
    input_candidates(&candidate03_arr);
    merge_candidate(candidate03_arr, &milliways03_arr);
    candidate3_poss = data_parsing(milliways03_arr);
    //5
    input_candidates(&candidate04_arr);
    merge_candidate(candidate04_arr, &milliways04_arr);
    candidate4_poss = data_parsing(milliways04_arr);
    //6
    input_candidates(&candidate05_arr);
    merge_candidate(candidate05_arr, &milliways05_arr);
    candidate5_poss = data_parsing(milliways05_arr);
    
    // 출력
    printf("####################################\n");
    printf("오디션 후보자 데이터 조회 \n");
    printf("####################################\n");
    print_milliways(candidate0_poss);
    print_milliways(candidate1_poss);
    print_milliways(candidate2_poss);
    print_milliways(candidate3_poss);
    print_milliways(candidate4_poss);
    print_milliways(candidate5_poss);

    // 추가 데이터 중 몸무게는 키와 BMI 값으로 계산하여 채운다. BMI = (체중kg) / ((키m)^2), 기존의 BMI 값은 없애고 새로 넣기?
    // milliways##_arr 내용을 리스트 형식으로 출력한다.
    // (보너스) milliways##_arr 내용을 리스트 형식으로 출력할 때 BMI 값에 따라 저체중, 정상, 과체중, 비만을 표시한다.

    free(candidate00_arr);
    free(milliways00_arr);

    return 0;
}