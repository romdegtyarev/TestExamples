#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char** argv) {
    int (*func1)(int x);
    double (*func2)(int x);

    void *lib;
    lib = dlopen("/home/rom/Eltex/lw5/dinamic/libsqrsqrt.so", RTLD_LAZY);
    func1 = dlsym(lib, "mysqr");
    func2 = dlsym(lib, "mysqrt");

    int a;
    printf("Введите число\n");
    scanf("%d", &a);
    printf("Квадрат числа: %d\nКорень из числа %f\n", (*func1)(a), (*func2)(a));
    dlclose(lib);
    return 0;
}
