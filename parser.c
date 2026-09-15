#include <stdio.h>

#define A -4
#define B 2
#define C 3

int main() {

        printf("%i\n", (B*B) - 4 * A * C);

        int x;
        scanf("%i", &x);
        if (x % 2 == 0) {
                printf("even\n");
        } else {
                printf("odd\n");
        }

        int y1; 
        int y2;
        int y3;

        scanf("%i", &y1);
        scanf("%i", &y2);
        scanf("%i", &y3);

        if (y3 > y1 && y3 < y2) {
                printf("yes\n");
        } else {
                printf("no\n");
        }

        int year;
        scanf("%i", &year);
        if (year % 4 == 0 && year % 100 == 0 && year % 400 == 0) {
            printf("yes\n");
        } else {
            printf("no\n");
        }

        int z1;
        int z2;
        int z3;

        scanf("%i", &z1);
        scanf("%i", &z2);
        scanf("%i", &z3);
        
        if (z1 > z2 && z1 > z3) {
            printf("num 1\n");
        } else if (z2 > z3) {
            printf("num 2\n");
        } else {
            printf("num 3\n");
        }


        return 0;
}
