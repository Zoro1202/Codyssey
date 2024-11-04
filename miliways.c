// UTF-8
// 미완20241104
#include "stdio.h"
#include "stdlib.h"

void input_candidates(char *candidate){
    int size = 50;
    candidate = (char*)malloc(size * sizeof(char));
    int index = 0;
    char c = 0;
    if(candidate == NULL){
        printf("메모리 할당 실패\n");
        return;
    }
    while((c = getchar()) != '\n' && c != EOF){
        //배열 크기 조정하기
            if(index >= size - 1){
                size *= 2; //크기 작을거같으면 두배로 늘려버리기
                candidate = (char *)realloc(candidate, size * sizeof(char));
                // null check
                if(candidate == NULL){
                    printf("메모리 할당 실패\n");
                    return 1;
                }
            }
            candidate[index++] = c;
        index++;
    }
}

int main(){
    // 문제 3의 후보자 6명의 데이터(소개는 제외)에 후보자 고유 ID(문제 5에서 생성), 합격 여부가 포함된 JSON 형식의 문자열을 담은 배열 candidate##_arr로 작성한다.
    char *candidate00_arr;
    char *candidate01_arr;
    char *candidate02_arr;
    char *candidate03_arr;
    char *candidate04_arr;
    char *candidate05_arr;
    
    //예시
    //"이름:박지연,생년월일:20060415,성별:여,이메일:jiyeon@outlook.com,국적:한국BMI:18.5,주특기:댄스,보조특기:작곡,한국어등급:0,MBTI:ENFJ,ID:123456,합격여부:O"
    // getchar() 로 받아서 동적 배열로 늘려야지
    
    


    // candidate##_arr에서 합격한 멤버에 대해 아래 제약 사항의 추가 데이터에 관해 인터뷰하는 코드를 작성한다. 이 때 for 반복문을 사용한다. 
    // 닉네임 : Ariel , 학력 : 중졸 , 키 : 175cm , 몸무게 : 60 , 혈액형 : A , 알러지 : 땅콩 , 취미 : 댄스 연습 작곡 , SNS : Instagram - @Ariel_Kim

    // 

    // candidate##_arr에서 합격자의 기존 기초 정보를 복사해 새로운 milliways##_arr 배열에 복사하고, 인터뷰를 통해 수집된 정보를 추가 한다. 
    char *milliways00_arr;
    char *milliways01_arr;
    char *milliways02_arr;
    char *milliways03_arr;
    char *milliways04_arr;
    char *milliways05_arr;
    // 추가 데이터 중 몸무게는 키와 BMI 값으로 계산하여 채운다. BMI = (체중kg) / ((키m)^2), 기존의 BMI 값은 없애고 새로 넣기?
    // milliways##_arr 내용을 리스트 형식으로 출력한다.
    // (보너스) milliways##_arr 내용을 리스트 형식으로 출력할 때 BMI 값에 따라 저체중, 정상, 과체중, 비만을 표시한다.

    return 0;
}