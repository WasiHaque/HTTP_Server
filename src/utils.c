#include <stdio.h>
#include "utils.h"

void printHello() {
    printf("Hello, World!\n");
}

int add(int a, int b) {
    return a + b;
}

void printSum(int a, int b) {
    printf("The sum of %d and %d is %d\n", a, b, add(a, b));
}