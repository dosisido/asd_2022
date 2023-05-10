/*
 * Riccardo Rosin 284211
 * Lab 2 Es 1
*/
#include<stdio.h>

int gcd(int a, int b){
    if(a<b) return gcd(b,a);
    if(a==b) return a;
    if(b%2 == 0){
        if(a%2 == 0) return 2 * gcd(a/2, b/2);
        return gcd(a, b/2); 
    }else
        if(a%2 == 0) return gcd(a/2, b);
        else return gcd((a-b)/2, b);
}


int main(int argc, char* argv){
    printf("%d\n", gcd(60, 20));
    return 0;
}