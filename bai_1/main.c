#include <stdio.h>

int add (int a, int b);
int sub (int a, int b);

int main() {
    int a = 10;
    int b = 6;
    printf("Add : %d + %d = %d \n", a, b, add(a, b));
    printf("Sub : %d - %d = %d \n", a, b, sub(a, b));
    return 0;
}