#include <stdio.h>
#include <windows.h>
#include <conio.h>

typedef int bool;

//맵 크기
#define MapWidth 10 //임시로 10 x 10 으로 설정
#define MapHeight 10

int map[MapWidth][MapHeight]; //논리적 맵 

//출력 설정 함수
void gotoxy(int x, int y);
void textcolor(int colorNum);
void setText(int x, int y, int colorNum, char* string);

//맵 출력 함수
void drawGame(void);

int main(void){
    drawGame();
    getch();
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
    int startX = 2, startY = 1;
    bool isLine;

    for(y = startY; y < startY + MapHeight + 2; y = y + 1) {
        for(x = startX; x < startX + 2 * MapWidth + 4; x = x + 2) {

            isLine = (x == startX || x == 2 * MapWidth + 4 || y == startY || y == MapHeight + 2);

            if(isLine) setText(x, y, 7, "#");
            else setText(x, y, 7, " ");
        }
    }
    return;
}
