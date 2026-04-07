#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

typedef int bool;

//맵 크기
#define MapWidth 20 //임시로 10 x 10 으로 설정
#define MapHeight 20

//방향키 및 스패이스바
#define Arrow 224
#define Up 72
#define Down 80
#define Left 75
#define Right 77
#define Space 32

//논리적 맵 
int map[MapHeight][MapWidth];

//블럭 모양
int blocks[][3][3] = {{{1, 1, 1},  //□ □
                       {1, 0, 1},  //  □ 
                       {1, 1, 0}}, 
                      {{1, 1, 0},  //□ □
                       {1, 1, 0},  //□ □
                       {0, 0, 0}},
                      {{1, 1, 1},  //□ □ □
                       {0, 0, 0},
                       {0, 0, 0}},
                      {{1, 1, 1},  //□ □ □
                       {0, 0, 1},  //    □ 
                       {0, 0, 0}},
                      {{1, 1, 1},  //□ □ □
                       {0, 1, 0},  //  □ 
                       {0, 0, 0}}
                    };

//출력 설정 함수
void gotoxy(int x, int y);
void textcolor(int colorNum);
void setText(int x, int y, int colorNum, char* string);

//맵 출력 함수
void drawGame(void); //출력
void ClrScr(void); //삭제

//오브젝트 동작 함수
clock_t getLoopTime(clock_t lastTime);
int makeBlock(void);

//랜덤 값 생성
int RandomNum(int range);

int main(void){
    int inp;
    clock_t lastTime = clock(), calOneTick = 0; //마지막 측정 시간, 한틱(블럭이 1칸 떨어지는 시간) 계산용 변수
    clock_t oneTick = 500; // 1틱 = 0.5초

    while(1) {
        //입력 처리
        if(!kbhit) { 
            inp = getch();
            if(inp == Arrow) inp = getch(); //방향키는 Arrow + 방향 으로 이루어짐. 즉 방향값을 얻기위해 Arrow 날리는 처리
            switch(inp) {
                case Up:
                case Down:
                case Left:
                case Right:
                case Space:
                case 'z':
                break;
            }
        }
        calOneTick += getLoopTime(lastTime);
        lastTime = clock();

        //계산 처리
        makeBlock();
        if(calOneTick > 500) {

        }

        //출력 처리
        ClrScr();
        drawGame();
    }
    return 0;
}

void gotoxy(int x, int y){
    COORD Pos;
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void textcolor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void setText(int x, int y, int colorNum, char* string){
    gotoxy(x, y);
    textcolor(colorNum);
    printf("%s", string);
    return;
}

void drawGame(void){
    int x, y;
    int i, j;
    int startX = 2, startY = 1;
    bool isLine, isBlock;
    
    j = -1; //경계선(line)을 고려해서 j, i 전부 -1부터 시작
    for(y = startY; y < startY + MapHeight + 2; y = y + 1) {
        i = -1;
        for(x = startX; x < startX + 2 * MapWidth + 4; x = x + 2) {
            isBlock = (map[j][i] == 1);
            isLine = (x == startX || x == 2 * MapWidth + 4 || y == startY || y == MapHeight + 2);

            if(isLine) setText(x, y, 7, "#");
            else if(isBlock) setText(x, y, 7, "□");
            else setText(x, y, 7, " ");
            i++;
        }
        j++;
    }
    return;
}

void ClrScr(void){
    system("cls");
    return;
}

clock_t getLoopTime(clock_t lastTime){
    return clock() - lastTime;
}

int RandomNum(int range) {
    srand(time(NULL));
    return rand() % range;
}

int makeBlock(void) {
    int i, j;
    bool isBlockCollision;
    int tempMap[MapHeight][MapWidth];
    int blockWidth = sizeof(blocks[0][0]) / sizeof(int);
    int blockHeight = sizeof(blocks[0]) / blockWidth / sizeof(int);
    int blockNum = sizeof(blocks) / blockHeight / blockWidth / sizeof(int);
    int randomNum = RandomNum(blockNum);

    //tempMap 에 map 가져오기
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            tempMap[i][j] = map[i][j];
        }
    }

    //tempMap에 랜덤 block 생성
    for(i = 0; i < blockHeight; i++) {
        for(j = 0; j < blockWidth; j++) {
            isBlockCollision = (tempMap[i][MapWidth / 2 + j - 1] == 1 && blocks[randomNum][i][j] == 1);
            if(isBlockCollision) return -1; //블록 생성 실패! -1 반환
            else tempMap[i][MapWidth / 2 + j - 1] = blocks[randomNum][i][j];
        }
    }

    //map에 tempMap 가져오기
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            map[i][j] = tempMap[i][j];
        }
    }

    return 0;
}
