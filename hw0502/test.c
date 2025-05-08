/* 
간단한 산술 표현식을 위한 재귀 하강 파서
문법:
expr   → term { (+|-) term }
term   → factor { (*|/) factor }
factor → id | num | ( expr )*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

const char *input;
char realText;

// 입력에서 공백이 아닌 다음 문자를 읽어옴
void nextToken()
{
    do
    {
        realText = *input++;  // 공백을 제거하여 lookadhead에 저장
    } while (space(realText)); // 공백 무시
}

// factor를 파싱: id, 숫자, 또는 괄호로 감싸진 표현식
void factor()
{
    // 현재 토큰이 알파벳인 경우 (식별자)
    if (isalpha(realText))
    {
        printf("알파벳: %c\n", realText);
        nextToken();
    }
    // 현재 토큰이 숫자인 경우
    else if (isdigit(realText))
    {
        printf("숫자: %c\n", realText);
        nextToken();
    }
    // 현재 토큰이 '('인 경우 하위 표현식을 파싱
    else if (realText == '(')
    {
        nextToken();
        expr();
        if (realText == ')')
        {
            nextToken();
        }
    }

}

// term → factor { (*| /) factor }
// term을 파싱: factor와 * 또는 / 연산자 및 다른 factor
void term()
{
    factor();
    while (realText == '*' || realText == '/')
    {
        printf("연산자: %c\n", realText);
        nextToken();
        factor();
    }
}


// expr → term { (+| -) term }
// expr을 파싱: term과 + 또는 - 연산자 및 다른 term
void expr()
{
    term();
    while (realText == '+' || realText == '-')
    {
        printf("연산자: %c\n", realText);
        nextToken();
        term();
    }
}
