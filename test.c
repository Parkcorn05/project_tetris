 #include <stdio.h>
 #include <conio.h>

 //방향키 및 스패이스바
#define Arrow 224
#define Up 72
#define Down 80
#define Left 75
#define Right 77
#define Space 32

int moveProcess(int key) {
          switch(key) {
                    //case Up:
                    case Down:
                        printf("Down");
                        break;
                    case Left:
                        printf("Left");
                        break;
                    case Right:
                        printf("Right");
                        break;
                    case Space:
                        printf("Space");
                        break;
                    case 'z':
                    break;
                }
}

 int main(void) {
    while(1) {
            //입력 처리
        if(kbhit) { 
            int inp = getch();
            if(inp == Arrow) inp = getch(); //방향키는 Arrow + 방향 으로 이루어짐. 즉 방향값을 얻기위해 Arrow 날리는 처리
            moveProcess(inp);
        }
    }
 }

