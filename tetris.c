#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

typedef int bool;

//맵 크기
#define MapWidth 10 //임시로 10 x 10 으로 설정
#define MapHeight 20

//방향키 및 스패이스바
#define Arrow 224
#define Up 72
#define Down 80
#define Left 75
#define Right 77
#define Space 32

//key값
#define W -99123 //waitingBlock 값

//논리적 맵 
int map[MapHeight][MapWidth] = { 0 };

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
int createBlock(void);
int fallingBlock(void);
bool checkActiveBlock(void);
void allBlockUnactive(void);

int moveProcess(int key);

//맵 관리 함수
int deleteBlock(void);
void arrayBlock(void);

//랜덤 값 생성
int RandomNum(int range);

int main(void){
    int inp;
    clock_t lastTime = clock(), calOneTick = 0; //마지막 측정 시간, 한틱(블럭이 1칸 떨어지는 시간) 계산용 변수
    clock_t oneTick = 200; // 1틱

    while(1) {
        //입력 처리
        if(kbhit()) { 
            inp = getch();
            if(inp == Arrow) inp = getch(); //방향키는 Arrow + 방향 으로 이루어짐. 즉 방향값을 얻기위해 Arrow 날리는 처리
            moveProcess(inp);
        }

        calOneTick += getLoopTime(lastTime);
        lastTime = clock();

        //계산 처리
        if(checkActiveBlock() == -1) createBlock();
        if(calOneTick > oneTick) {
            calOneTick = 0;
            if(fallingBlock() == -1) allBlockUnactive();
            deleteBlock();
            arrayBlock();
        }

        //출력 처리
        //ClrScr();
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
    int color;
    bool isLine, isBlock;
    
    j = -1; //경계선(line)을 고려해서 j, i 전부 -1부터 시작
    for(y = startY; y < startY + MapHeight + 2; y = y + 1) {
        i = -1;
        for(x = startX; x < startX + 2 * MapWidth + 4; x = x + 2) {
            color = map[j][i] < 0 ? -map[j][i] : map[j][i]; //색상 절댓값으로 받기
            isBlock = (map[j][i] != 0);
            isLine = (x == startX || x == 2 * MapWidth + 4 || y == startY || y == MapHeight + 2);

            if(isLine) setText(x, y, 7, "#");
            else if(isBlock) /*{gotoxy(x,y); printf("%d",map[j][i]);}*/  setText(x, y, color, "□");
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

int createBlock(void) {
    int i, j;
    bool isBlockCollision;
    int tempMap[MapHeight][MapWidth];
    int blockWidth = sizeof(blocks[0][0]) / sizeof(int);
    int blockHeight = sizeof(blocks[0]) / blockWidth / sizeof(int);
    int blockNum = sizeof(blocks) / blockHeight / blockWidth / sizeof(int);
    int randomBlock = RandomNum(blockNum);
    int randomColor = RandomNum(14);

    //tempMap 에 map 가져오기
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            tempMap[i][j] = map[i][j];
        }
    }

    //tempMap에 랜덤 block 생성
    for(i = 0; i < blockHeight; i++) {
        for(j = 0; j < blockWidth; j++) {
            isBlockCollision = (tempMap[i][MapWidth / 2 + j - 1] != 0 && blocks[randomBlock][i][j] != 0);
            if(isBlockCollision) return -1; //블록 생성 실패! -1 반환
            else if(blocks[randomBlock][i][j] == 1) tempMap[i][MapWidth / 2 + j - 1] = -randomColor; //낙하중인 블럭은 -로 구별
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

int fallingBlock(void) {
    int i, j;
    int tempMap[MapHeight][MapWidth];
    bool underIsBlock;

    //tempMap에 map 복사(active 블럭 제외)
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] < 0) tempMap[i][j] = 0;
            else tempMap[i][j] = map[i][j];
        }
    }

    //tempMap에 active 블럭 반영
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] < 0) {
                underIsBlock = (map[i + 1][j] > 0 || i + 1 == MapHeight );
                if(underIsBlock) return -1; //아래가 블럭! 비정상 종료
                else tempMap[i + 1][j] = map[i][j];
            }
        }
    }

    //map에 tempMap 복사
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            map[i][j] = tempMap[i][j];
        }
    }
    
    return 0;
}

bool checkActiveBlock(void) {
    int i, j;

    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] < 0) return 1;
        }
    }
    return -1;
}

void allBlockUnactive(void){
    int i, j;
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            map[i][j] = map[i][j] < 0 ? -map[i][j] : map[i][j];
        }
    }
};

int moveProcess(int key) {
    int i, j, I, J;
    int tempMap[MapHeight + 3][MapWidth + 3] = { 0 };
    int height = 0, minHeight;
    bool IsBlock;
    int SIGNAL = 0;
    int startx = MapWidth, starty = MapHeight;
    int endx = 0, endy = 0;
    int blockWidth = sizeof(blocks[0][0]) / sizeof(int);
    int blockHeight = sizeof(blocks[0]) / blockWidth / sizeof(int);
    int tempBlock[3][3] = {0};


    //tempMap에 map 복사(active 블럭 제외)
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] < 0) tempMap[i][j] = 0;
            else tempMap[i][j] = map[i][j];
        }
    }

    //tempMap에 active 블럭 반영
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] < 0) {
                switch(key) {
                    //case Up:
                    case Down:
                        IsBlock = (map[i + 1][j] > 0 || i + 1 == MapHeight );
                        if(IsBlock) return -1; //아래가 블럭! 비정상 종료
                        else tempMap[i + 1][j] = map[i][j];
                        break;
                    case Left:
                        IsBlock = (map[i][j - 1] > 0 || j - 1 == -1 );
                        if(IsBlock) return -1; //아래가 블럭! 비정상 종료
                        else tempMap[i][j - 1] = map[i][j];
                        break;
                    case Right:
                        IsBlock = (map[i][j + 1] > 0 || j + 1 == MapWidth );
                        if(IsBlock) return -1; //아래가 블럭! 비정상 종료
                        else tempMap[i][j + 1] = map[i][j];
                        break;
                    case Space:
                        height = 0;
                        while(1){
                            IsBlock = (map[i+height + 1][j] > 0 || i + height + 1 == MapHeight);
                            if(IsBlock) break;
                            height++;
                        }
                        minHeight = minHeight < height ? minHeight : height; 
                    break;
                    case 'z':
                        for(i = 0; i < MapHeight; i++) {
                            for(j = 0; j < MapWidth; j++){
                                if(map[i][j] < 0) {
                                    startx = j < startx ? j : startx;
                                    starty = i < starty ? i : starty;
                                    endx = endx < j ? j : endx;
                                    endy = endy < i ? i : endy;
                                }
                            }
                        }

                        i = 0; I = starty; 
                        while(1){
                            if(I > endy || i > endy - starty) break;
                            j = 0; J = startx;
                            while(1){
                                if(J > endx || j > endx - startx) break;
                                if(map[I][J] < 0) tempBlock[i][j] = map[I][J];
                                else tempBlock[i][j] = 0;
                                J++; j++;
                            }
                            I++; i++;
                        }

                        i = 0; J = startx + endy - starty; 
                        while(1){
                            if(i > endy - starty) break;
                            I = starty; j = 0; 
                            while(1){
                                if(j > endx - startx) break;
                                if(tempBlock[i][j] < 0) {
                                    IsBlock = (tempMap[I][J] > 0);
                                    if(IsBlock) return -1;
                                    else tempMap[I][J] = tempBlock[i][j];
                                }
                                I++; j++;
                            }
                            J--; i++;
                        }
                        SIGNAL = 1;
                    break;
                }
            }
            if(SIGNAL == 1) break;
        }
        if(SIGNAL == 1) break;
    }

    if(key == Space){
        for(i = 0; i < MapHeight; i++) { 
            for(j = 0; j < MapWidth; j++) {
                if(map[i][j] < 0) {
                    tempMap[i + minHeight][j] = map[i][j];
                }
            }
        }
    }

    //map에 tempMap 복사
    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            map[i][j] = tempMap[i][j];
        }
    }
}

int deleteBlock(void) {
    int i, j;
    int checkLine = 0;
    int SIGNAL = 0;
    int n;

    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            if(map[i][j] > 0) checkLine++;
        }
        if(checkLine == MapWidth) {
            for(j = 0; j < MapWidth; j++) {
                map[i][j] = W;
            }
            SIGNAL = 1;
        }
        checkLine = 0;
    }
    return (SIGNAL == 1);
}

void arrayBlock(void) {
    int i, j;
    int tempMap[MapHeight][MapWidth];
    int height = 0;

    for(i = MapHeight - 1; i >= 0; i--){
        for(j = MapWidth - 1; j >= 0; j--){
            if(i-height < 0) tempMap[i][j] = 0; //배열 범위 초과 방지
            else {
            tempMap[i][j] = map[i - height][j];
            }
        }
        if(tempMap[i][0] == W) {i++; height++;} //라인 1번 더 검사
    }

    for(i = 0; i < MapHeight; i++) { 
        for(j = 0; j < MapWidth; j++) {
            map[i][j] = tempMap[i][j];
        }
    }
}