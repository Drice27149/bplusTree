#include <cstdio>
#include <cstdlib>

int main(){
    system("customTest.exe > output/out.txt");
    if(system("fc output\\out.txt output\\expect.txt")){
        printf("Failed\n");
    }
    else{
        printf("Succeed\n");
    }
    return 0;
}
