#include <stdio.h>
 
int main(){ 
 
        char* string[30];
        string[0]="String 1";
        string[1]="String 2";
        string[2]="String 3";
        string[3]="String 4";
        string[4]="String 5";

        //print all strings
        for(int loop=0;loop<5;loop++){
            printf("%s\n",string[loop]);
        }
        return 0;
}