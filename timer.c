#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

// int main(void){   //실험 1 
//     char inp;
//     clock_t start, end, sum;
//     int i = 0, j = 0;

//     while(1) {
//         sleep(500);
//         start = clock();
//         if(kbhit() == 1) {
//             inp = getch();
//         }
        
//         end = clock();
//         sum += end - start;
//         j++;
//         if(sum > 1000) {i++; sum = 0;}
//         printf("%d - %d회 반복완료\n",i, j);
//     }
//     return 0;
// }

//================================================

int main(void){   //실험 2
    char inp;
    clock_t time = clock(), sum = 0;
    int i = 0, j = 0;
    getch();
    while(1) {
        
        if(kbhit() == 1) {
            inp = getch();
        }

        sum += clock() - time;
        time = clock();
        j++;
        if(sum > 1000) {i++; sum = 0; j = 0;}
        printf("%d - %d회 반복완료\n",i, j);
    }
    return 0;
}