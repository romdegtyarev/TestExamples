#include <stdio.h>

#include <main.h>


/* Global Internal Variables */
static int VAR1;
/* Global External Variables */
extern int VAR2;
/* Global Exported Variables */
int VAR3;


/* Internal Functions */
static void func();


/* Internal Functions */
/**
 * func1
 */
static void func1() {

    return;
}

/**
 * main
 */
int main() {

    return 1;
}

/* External Functions */
/**
 * func2
 */
extern void func2() {

    return;
}

