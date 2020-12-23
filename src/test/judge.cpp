#include <cstdio>
#include <cstdlib>

int main(){
    system("runner.exe < in/in.txt > out/1.txt");
    system("bfRunner.exe < in/in.txt > out/2.txt");
    if(system("fc out\\1.txt out\\2.txt")){
        printf("Failed\n");
    }
    else{
        printf("Succeed\n");
    }
    system("pause");
    return 0;
}
