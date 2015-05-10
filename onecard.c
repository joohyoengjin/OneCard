#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STACK_SIZE 13 // 처음에 카드를 7장씩 받으며 최대 카드 수는 13장을 넘을 수 없기로 한다.
#define TRUE 1
#define FALSE 0

// 카드의 타입은 int형으로 정의
typedef int element;

// 프로그램 소스코드 상에서 ♤(1) ♡(2) ♧(3) ◇(4)로 표기
// A(1), J(11), Q(12), K(13)으로 표기
// 한자리 수는 0# 형태로 표기.(#은 각 숫자)
// 예 : ♧8 카드는 308, ◇K 카드는 413 으로 표기
// 카드 목록
element cardlist[4][13]={
    {101,102,103,104,105,106,107,108,109,110,111,112,113},
    {201,202,203,204,205,206,207,208,209,210,211,212,213},
    {301,302,303,304,305,306,307,308,309,310,311,312,313},
    {401,402,403,404,405,406,407,408,409,410,411,412,413}
};
////////////////////////////////////////////자료구조 시작 부분////////////////////////////////////////////
//////////////////////////////////////////////스택 시작 부분//////////////////////////////////////////////
typedef struct {
    element stack[MAX_STACK_SIZE];
    int top;
} StackType;

// 스택 초기화 함수
void init(StackType *s)
{
    s->top = -1;
}

// 공백 상태 검출 함수
int is_empty(StackType *s)
{
    return (s->top == -1);
}

// 포화 상태 검출 함수
int is_full(StackType *s)
{
    return (s->top == (MAX_STACK_SIZE -1));
}

// 삽입 함수
void push(StackType *s, element item)
{
    if(is_full(s))
    {
        printf("스택 포화 에러\n");
        return;
    }
    else s->stack[++(s->top)] = item;
}

// 삭제 함수
element pop(StackType *s)
{
    if(is_empty(s)) // 스택이 공백이 되면 게임이 끝나므로 실제로 호출될 일은 없다.
    {
        printf("스택 공백 에러\n");
        exit(1);
    }
    else return s->stack[(s->top)--];
}
//////////////////////////////////////////////스택 종료 부분//////////////////////////////////////////////

// 선택 정렬
void selectionSort(int a[], int size)
{
    int i,j;
    int min, temp;
    for(i=0; i<size-1; i++)
    {
        min = i; // a[j] ... a[size-1] 사이에 가장 작은 원소의 인덱스(min)를 찾음
        for(j=i+1; j<size; j++)
        {
            if(a[j] < a[min]) min=j;
        }
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}
////////////////////////////////////////////자료구조 종료 부분////////////////////////////////////////////

////////////////////////////////////////원카드 관련 함수 시작 부분////////////////////////////////////////
/////////////////////////////////////////출력 관련 함수 시작 부분/////////////////////////////////////////
// 카드 모양을 출력하기 위한 함수
char* pattern(element item)
{
    switch(item / 100)
    {
        case 1:
            return "♤";
        case 2:
            return "♡";
        case 3:
            return "♧";
        case 4:
            return "◇";
    }
    return "error";
}

// 카드 숫자를 출력하기 위한 함수
int number(element item)
{
    switch(item % 100)
    {
        case 1:
            return 'A';
        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
            return (item % 100);
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
    }
    return FALSE;
}

// 플레이어n+1의 갖고 있는 카드 출력
void cardcheck(StackType *s, int n)
{
    int i;
    
    printf("\t현재 플레이어%d 님이 갖고 있는 카드 입니다.\n\t :", n+1);
    for(i=0; i <= s->top; i++)
    {
        if(number(s->stack[i]) >= 'A' && number(s->stack[i]) <= 'Z')
            printf("%s%c ", pattern(s->stack[i]), number(s->stack[i]));
        else
            printf("%s%d ", pattern(s->stack[i]), number(s->stack[i]));
    }
    printf("\n");
}
/////////////////////////////////////////출력 관련 함수 종료 부분/////////////////////////////////////////

/////////////////////////////////////////탐색 관련 함수 시작 부분/////////////////////////////////////////
// item이 갖고 있는 카드인지 확인
int search(StackType *s, element item)
{
    int i;
    
    for(i=0; i < s->top +1; i++)
    {
        if(s->stack[i] == item)
            return TRUE;
    }
    return FALSE;
}

// item이 갖고 있는 무늬인지 확인
int searchptn(StackType *s, element item)
{
    int i;
    
    for(i=0; i < s->top +1; i++)
    {
        if(s->stack[i]/100 == item/100)
            return TRUE;
    }
    return FALSE;
}

// item이 갖고 있는 숫자인지 확인
int searchnum(StackType *s, element item)
{
    int i;
    
    for(i=0; i < s->top +1; i++)
    {
        if(s->stack[i]%100 == item%100)
            return TRUE;
    }
    return FALSE;
}
/////////////////////////////////////////탐색 관련 함수 종료 부분/////////////////////////////////////////

/////////////////////////////////////카드를 내고 받는 함수 시작 부분//////////////////////////////////////
// 남은 카드가 있는지 검사 후 없으면 다시 리셋
void resetcard(StackType *s, int n, element currentcard)
{
    int i, j;
    
    for(i=0; i<4; i++)
        for(j=0; j<13; j++)
            if(cardlist[i][j] != 0) return; // 남은 카드가 있으면 리턴
    
    for(i=0; i<4; i++) // 남은 카드가 없으면 일단 모두 리셋시킨후
        for(j=0; j<13; j++)
            cardlist[i][j] = (i+1)*100 + (j+1);
				
    cardlist[currentcard/100 -1][currentcard%100 -1] = 0; // 펼쳐진 카드와
    for(i=0; i<n; i++) // 스택에 쌓여있는 카드는 제외시킴
        for(j=0; j<=s[i].top; j++)
            cardlist[s[i].stack[j] / 100 - 1][s[i].stack[j] % 100 - 1] = 0;
    
    printf("남은 카드가 없으므로 카드를 리셋합니다.\n");
}

// 처음 시작할 때 펼쳐지는 카드를 정하는 함수
element resetgame()
{
    int op=0; // while문을 빠져나오기 위한 임시문자
    element pattern, number, item;
    
    srand(time(NULL));
    do
    {
        pattern = rand()%4 + 1; // 무늬를 정함
        number = rand()%13 + 1; // 숫자를 정함
        item = 100*pattern + number;
        if(cardlist[pattern-1][number-1] == item) // 카드가 사용되지 않았음을 의미
        {
            cardlist[pattern-1][number-1]=0; // 카드가 사용되었다는 의미로 임시로 0을 저장
            op=1;
        }
    } while(op==0);
    
    return item;
}

// 카드를 받는 함수
void receive(StackType *s, int turn, int n, element currentcard)
{
    int op=0; // while문을 빠져나오기 위한 임시문자
    element pattern, number, item;
    
    srand(time(NULL));
    do
    {
        resetcard(s, n, currentcard);
        pattern = rand()%4 + 1; // 무늬를 정함
        number = rand()%13 + 1; // 숫자를 정함
        item = 100*pattern + number;
        if(cardlist[pattern-1][number-1] == item) // 카드가 사용되지 않았음을 의미
        {
            if(is_full(&s[turn])) op=1; // 카드가 13장이면 선택된 카드를 사용하지 않음.
            else
            {
                cardlist[pattern-1][number-1]=0; // 카드가 사용되었다는 의미로 임시로 0을 저장
                op=1;
            }
        }
    } while(op==0);
    
    push(&s[turn], item);
}

// 카드를 내는 함수
void putcard(StackType *s, element item)
{
    int i,j;
    element temp;
    
    // 카드의 위치를 i로 찾고 카드를 임시값 temp에 저장
    for(i=0; i <= s->top; i++)
    {
        if(s->stack[i] == item)
        {
            temp = item;
            break;
        }
    }
    
    // i+1번째 부터 스택의 한칸씩 밑으로 내려옴
    for(j=i; j <= s->top; j++)
    {
        s->stack[j] = s->stack[j+1];
    }
    
    s->stack[s->top] = temp;
    
    pop(s);
}
/////////////////////////////////////카드를 내고 받는 함수 종료 부분//////////////////////////////////////
////////////////////////////////////////원카드 관련 함수 종료 부분////////////////////////////////////////

void main()
{
    int n,turn=0; // n : 게임에 참여하는 인원수, turn : 차례를 가리키는 변수
    int i,j; // for 반복문을 위한 변수
    int menu; // 게임 메뉴를 선택하는 변수
    int end=FALSE; // 게임 종료를 알리는 변수(TRUE가 되면 종료)
    int *onecard; // 원카드 외친 여부를 확인하기 위한 변수
    int special=FALSE; // 공격카드를 위한 변수
    int sequence=TRUE; // Q카드에 의한 순서를 따지기 위한 변수
    int seven; // 7카드에 의한 모양바꾸기를 위한 변수
    int exist; // 낼카드가 있는지 확인하기 위한 변수
    StackType *player; // 각 플레이어가 소지한 카드를 스택으로 할당
    element currentcard, item; // currentcard : 현재 펼쳐진 카드, item : 카드를 낼 때 그 카드를 임시로 저장하기 위한 변수
    
    printf("원카드 게임을 시작합니다.\n");
    printf("게임에 참여할 인원수를 입력해 주세요.(2~4명) : ");
    scanf_s("%d",&n);
    
    player=(StackType*)malloc(sizeof(StackType)*n); // 인원수에 맞게 스택 할당
    onecard=(int*)malloc(sizeof(int)*n); // 인원수에 맞게 할당
    
    for(i=0; i<n; i++)
    {
        init(&player[i]);
        onecard[i]=FALSE;
    } // 스택 초기화(top=-1)
    
    currentcard=resetgame(); // 카드 한장을 펼쳐 놓음
    
    printf("\n카드를 7장씩 받습니다.\n\n");
    
    for(i=0; i<n; i++)
    {
        for(j=0; j<7; j++)
            receive(player, i, n, currentcard); // 카드 받기
    }
    
    // 모든 플레이어의 카드 확인
    for(i=0; i<n; i++)
    {
        cardcheck(&player[i], i);
    }
    
    if(number(currentcard)=='A')
    {
        special='A';
        printf("\n\n첫 카드가 A카드 입니다.\n");
        printf("방어카드가 없으면 카드 5장을 받습니다.\n");
    }
    else if(number(currentcard)==2)
    {
        special=2;
        printf("\n\n첫 카드가 2카드 입니다.\n");
        printf("방어카드가 없으면 카드 3장을 받습니다.\n");
    }
    
    // 게임 시작(무한 루프)
    while(1)
    {
        printf("\n\n");
        printf("  ==============================================================\n");
        
        if(number(currentcard) >= 'A' && number(currentcard) <= 'Z')
            printf("\t%s%c 카드가 놓여 있습니다.\n", pattern(currentcard), number(currentcard));
        else
            printf("\t%s%d 카드가 놓여 있습니다.\n", pattern(currentcard), number(currentcard));
        
        printf("\t플레이어%d 님의 차례입니다.\n",turn+1);
        cardcheck(&player[turn], turn);
        printf("  --------------------------------------------------------------\n");
        printf("\t1. 카드 내기\n");
        printf("\t2. 카드 받기\n");
        printf("\t3. 내 카드 크기순으로 정렬하기\n");
        printf("\t4. 모든 플레이어의 카드 다시 보기\n");
        printf("\t5. 카드 내고 원카드 외치기!\n");
        printf("  --------------------------------------------------------------\n");
        printf("\t메뉴를 선택해 주세요. : ");
        scanf_s("%d", &menu);
        printf("  ==============================================================\n\n");
        switch(menu)
        {
            case 1 : // 카드 내기
                if(player[turn].top==0 && onecard[turn]==FALSE) // 카드가 한 장인데 원카드를 외치지 않았으면
                {
                    printf("\n카드가 한장인데 원카드를 외치지 않았으므로\n");
                    printf("벌칙으로 카드를 한 장 받습니다.\n");
                    receive(player, turn, n, currentcard);
                    if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                        printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    else
                        printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    if(sequence) turn++;
                    else turn--;
                }
                else // 일반적인 카드 내는 상황
                {
                    printf("\n♤=1, ♡=2, ♧=3, ◇=4\n");
                    printf("A=01, J=11, Q=12, K=13 으로 입력\n");
                    printf("ex) ♧5=305로 입력\n\n");
                    printf("내실 카드 번호를 입력해 주세요. : ");
                    scanf_s("%d", &item);
                    
                    // 갖고 있지 않는 카드를 입력한 경우
                    if(search(&player[turn], item)==FALSE)
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c는 갖고 있지 않습니다!\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d는 갖고 있지 않습니다!\n", pattern(item), number(item));
                    }
                    
                    // 이전 플레이어가 공격카드를 냈고, 낸 카드가 방어카드가 아니면
                    else if( (special=='A' && number(item)!='A' && number(item)!=3) || (special==2 && number(item)!='A' && number(item)!=2 && number(item)!=3) )
                    {
                        printf("\n이전 플레이어가 공격카드를 냈으므로\n");
                        printf("방어카드를 내셔야 합니다!\n");
                    }
                    
                    // 모양이 같거나 숫자가 같으면
                    else if( (currentcard/100==item/100) || (currentcard%100==item%100) )
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c를 냈습니다.\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d를 냈습니다.\n", pattern(item), number(item));
                        putcard(&player[turn], item);
                        currentcard = item;
                        
                        switch(number(item)) // 특수카드를 낸 경우
                        {
                            case 'A':
                                if(special=='A' || special==2) printf("\n이전 플레이어의 공격을 카드 A로 방어합니다.");
                                special='A';
                                printf("\n카드 A는 공격카드입니다.\n");
                                printf("다음 플레이어가 방어카드가 없으면 카드 5장을 받아야 합니다.\n");
                                if(sequence) turn++;
                                else turn--;
                                break;
                            case 2:
                                if(special==2) printf("\n이전 플레이어의 공격을 카드 2로 방어합니다.");
                                special=2;
                                printf("\n카드 2는 공격카드입니다.\n");
                                printf("다음 플레이어가 방어카드가 없으면 카드 3장을 받아야 합니다.\n");
                                if(sequence) turn++;
                                else turn--;
                                break;
                            case 3:
                                if(special=='A' || special==2)
                                {
                                    special=FALSE;
                                    printf("\n카드 3은 방어카드입니다.\n");
                                    printf("이전 플레이어의 공격이 무효화 됩니다.\n");
                                }
                                if(sequence) turn++;
                                else turn--;
                                break;
                            case 7:
                                printf("\n카드 7은 모양바꾸기 카드입니다.\n");
                                printf("바꾸고 싶은 모양을 입력하세요.\n");
                                printf("♤=1, ♡=2, ♧=3, ◇=4 입력 : ");
                                scanf_s("%d",&seven);
                                currentcard = currentcard%100 + 100*seven;
                                if(sequence) turn++;
                                else turn--;
                                break;
                            case 'J':
                                printf("\n카드 J는 점프카드입니다.\n");
                                if(sequence)
                                {
                                    printf("플레이어%d의 차례를 건너 뜁니다.\n", (turn+1)%n +1);
                                    turn +=2;
                                }
                                else
                                {
                                    printf("플레이어%d의 차례를 건너 뜁니다.\n", (turn-1+n)%n +1);
                                    turn -=2;
                                }
                                break;
                            case 'Q':
                                printf("\n카드 Q는 BACK카드입니다.\n");
                                if(sequence)
                                {
                                    printf("순서가 역방향으로 바뀝니다.\n");
                                    sequence=FALSE;
                                }
                                else
                                {
                                    printf("순서가 정방향으로 바뀝니다.\n");
                                    sequence=TRUE;
                                }
                                if(sequence) turn++;
                                else turn--;
                                break;
                            case 'K':
                                printf("\n카드 K를 내면 한장 더 내실 수 있습니다.\n");
                                exist=FALSE;
                                for(i=0; i<=player[turn].top; i++)
                                {
                                    if( (currentcard/100==player[turn].stack[i]/100) || (currentcard%100==player[turn].stack[i]%100) ) // 낼 카드가 있으면
                                    {
                                        exist=TRUE;
                                        break;
                                    }
                                }
                                
                                if(exist)
                                    printf("카드를 한 장 더 내주세요.\n");
                                else
                                {
                                    printf("카드 K를 내고 낼 카드가 없으면 벌칙으로 카드 한 장을 받게 됩니다.\n");
                                    receive(player, turn, n, currentcard);
                                    if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                                        printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                                    else
                                        printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                                    if(sequence) turn++;
                                    else turn--;
                                }
                                break;
                            default :
                                if(sequence) turn++;
                                else turn--;
                        }//switch
                    } // else if
                    else // 모양도 숫자도 같지 않은 경우
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c는 낼 수 없습니다!!!\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d는 낼 수 없습니다!!!\n", pattern(item), number(item));
                    }
                }
                break;
            case 2 : // 카드 받기
                if(is_full(&player[turn]))
                {
                    printf("\n카드를 13장까지만 받을 수 있습니다.\n");
                    printf("카드를 받지 않고 턴을 넘깁니다.\n");
                }
                
                // 이전플레이어가 A카드를 낸 경우
                else if(special=='A')
                {
                    printf("방어카드가 없으므로 카드를 5장 받습니다.\n");
                    for(i=0; i<5; i++)
                    {
                        if(is_full(&player[turn]))
                        {
                            printf("\n카드를 13장까지만 받을 수 있습니다.\n");
                            printf("카드를 받지 않고 턴을 넘깁니다.\n");
                            break;
                        }
                        else
                        {
                            receive(player, turn, n, currentcard);
                            if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                                printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                            else
                                printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                        }
                    }
                    special=FALSE; // 공격카드효과 해제
                }
                
                // 이전플레이어가 2카드를 낸 경우
                else if(special==2)
                {
                    printf("방어카드가 없으므로 카드를 3장 받습니다.\n");
                    for(i=0; i<3; i++)
                    {
                        receive(player, turn, n, currentcard);
                        if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                            printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                        else
                            printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    }
                    special=FALSE; // 공격카드효과 해제
                }
                
                // 특수카드 효과가 없는 경우
                else
                {
                    printf("\n카드를 한 장 받고 턴을 넘깁니다.\n");
                    receive(player, turn, n, currentcard);
                    if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                        printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    else
                        printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                }
                if(sequence) turn++;
                else turn--;
                break;
            case 3 : // 내 카드 크기순으로 정리하기
                selectionSort(player[turn].stack, player[turn].top+1);
                printf("\n카드가 정렬되었습니다!\n");
                break;
            case 4 : // 모든 플레이어의 카드 다시 보기
                printf("\n");
                for(i=0; i<n; i++)
                {
                    cardcheck(&player[i], i);
                }
                break;
            case 5 : // 카드 내고 원카드 외치기!
                if(player[turn].top==0 && onecard[turn]==FALSE) // 카드가 한 장인데 원카드를 외치지 않았으면
                {
                    printf("\n카드가 한장일 때 미리 원카드를 외쳤어야 합니다!\n");
                    printf("벌칙으로 카드를 한 장 받습니다.\n");
                    receive(player, turn, n, currentcard);
                    if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                        printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    else
                        printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                    if(sequence) turn++;
                    else turn--;
                }
                else
                {
                    printf("\n♤=1, ♡=2, ♧=3, ◇=4\n");
                    printf("A=01, J=11, Q=12, K=13 으로 입력\n");
                    printf("ex) ♧5=305로 입력\n\n");
                    printf("내실 카드 번호를 입력해 주세요. : ");
                    scanf_s("%d", &item);
                    
                    // 갖고 있지 않는 카드를 입력한 경우
                    if(search(&player[turn], item)==FALSE)
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c는 갖고 있지 않습니다!\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d는 갖고 있지 않습니다!\n", pattern(item), number(item));
                    }
                    
                    // 이전 플레이어가 공격카드를 냈고, 낸 카드가 방어카드가 아니면
                    else if( (special=='A' && number(item)!='A') || (special==2 && number(item)!='A' && number(item)!=2) )
                    {
                        printf("\n이전 플레이어가 공격카드를 냈으므로\n");
                        printf("방어카드를 내셔야 합니다!\n");
                    }
                    
                    // 모양이 같거나 숫자가 같으면
                    else if( (currentcard/100==item/100) || (currentcard%100==item%100) )
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c를 냈습니다.\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d를 냈습니다.\n", pattern(item), number(item));
                        putcard(&player[turn], item);
                        currentcard = item;
                        
                        switch(number(item)) // 특수카드를 낸 경우
                        {
                            case 'A':
                                if(special=='A' || special==2) printf("\n이전 플레이어의 공격을 카드 A로 방어합니다.");
                                special='A';
                                printf("\n카드 A는 공격카드입니다.\n");
                                printf("다음 플레이어가 방어카드가 없으면 카드 5장을 받아야 합니다.\n");
                                break;
                            case 2:
                                if(special==2) printf("\n이전 플레이어의 공격을 카드 2로 방어합니다.");
                                special=2;
                                printf("\n카드 2는 공격카드입니다.\n");
                                printf("다음 플레이어가 방어카드가 없으면 카드 3장을 받아야 합니다.\n");
                                break;
                            case 3:
                                if(special=='A' || special==2)
                                {
                                    special=FALSE;
                                    printf("\n카드 3은 방어카드입니다.\n");
                                    printf("이전 플레이어의 공격이 무효화 됩니다.\n");
                                }
                                break;
                            case 7:
                                printf("\n카드 7은 모양바꾸기 카드입니다.\n");
                                printf("바꾸고 싶은 모양을 입력하세요.\n");
                                printf("♤=1, ♡=2, ♧=3, ◇=4 입력 : ");
                                scanf_s("%d",&seven);
                                currentcard = currentcard%100 + 100*seven;
                                break;
                            case 'J':
                                printf("\n카드 J는 점프카드입니다.\n");
                                special='J';
                                if(sequence)
                                {
                                    printf("플레이어%d의 차례를 건너 뜁니다.\n", (turn+1)%n +1);
                                }
                                else
                                {
                                    printf("플레이어%d의 차례를 건너 뜁니다.\n", (turn-1+n)%n +1);
                                }
                                break;
                            case 'Q':
                                printf("\n카드 Q는 BACK카드입니다.\n");
                                if(sequence)
                                {
                                    printf("순서가 역방향으로 바뀝니다.\n");
                                    sequence=FALSE;
                                }
                                else
                                {
                                    printf("순서가 정방향으로 바뀝니다.\n");
                                    sequence=TRUE;
                                }
                                break;
                            case 'K':
                                printf("\n카드 K를 내면 한장 더 내실 수 있습니다.\n");
                                exist=FALSE;//exist 초기화
                                for(i=0; i<=player[turn].top; i++)
                                {
                                    if( (currentcard/100==player[turn].stack[i]/100) || (currentcard%100==player[turn].stack[i]%100) ) // 낼 카드가 있으면
                                    {
                                        exist=TRUE;
                                        break;
                                    }
                                }
                                
                                if(exist)
                                {
                                    special='K';
                                    printf("카드를 한 장 더 내주세요.\n");
                                }
                                else
                                {
                                    printf("카드 K를 내고 낼 카드가 없으면 벌칙으로 카드 한 장을 받게 됩니다.\n");
                                    receive(player, turn, n, currentcard);
                                    if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                                        printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                                    else
                                        printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                                }
                                break;
                            default :
                                break;
                        }//switch
                        
                        if(player[turn].top == 0)
                        {
                            onecard[turn]=TRUE;
                            printf("\n원카드!!!\n");
                            printf("플레이어%d 님의 카드가 한 장 남았습니다!\n",turn+1);
                        }
                        else
                        {
                            printf("\n원카드!!\n");
                            printf("플레이어%d 님은 카드가 두 장 이상이므로 원카드를 외치면 안됩니다.\n",turn+1);
                            printf("벌칙으로 카드를 한 장 받습니다.\n");
                            receive(player, turn, n, currentcard);
                            if(number(player[turn].stack[player[turn].top]) >= 'A' && number(player[turn].stack[player[turn].top]) <= 'Z')
                                printf("카드 %s%c를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                            else
                                printf("카드 %s%d를 받았습니다.\n", pattern(player[turn].stack[player[turn].top]), number(player[turn].stack[player[turn].top]));
                        }
                        if(special=='K')
                            special=FALSE;
                        else if(special=='J')
                        {
                            special=FALSE;
                            if(sequence) turn +=2;
                            else turn -=2;
                        }
                        else if(sequence) turn++;
                        else turn--;
                    }
                    else
                    {
                        if(number(item) >= 'A' && number(item) <= 'Z')
                            printf("\n카드 %s%c는 낼 수 없습니다!!!\n", pattern(item), number(item));
                        else
                            printf("\n카드 %s%d는 낼 수 없습니다!!!\n", pattern(item), number(item));
                    }
                }
                break;
            default :
                printf("\n올바른 메뉴를 입력해 주세요!\n");
                break;
        }//switch
        
        if(turn<0) turn += n;
        else turn = turn % n;
        
        for(i=0; i<n; i++)
        {
            // 카드가 두 장 이상이면 원카드 외친 것을 취소시킴
            if(player[i].top >= 1)
                onecard[i]=FALSE;
            
            // 원카드를 외치고 카드가 없는 상태이면 승리
            if(player[i].top == -1 && onecard[i] == TRUE)
            {
                printf("\n플레이어%d님이 카드를 모두 사용하셨습니다.\n", i+1);
                printf("플레이어%d님이 원카드 게임에서 승리하셨습니다. 축하합니다!\n", i+1);
                end=TRUE;
            }
        }
        if(end) break;
    }//while
    printf("\n원카드 게임을 종료합니다.\n");
    free(player);
    free(onecard);
}//main