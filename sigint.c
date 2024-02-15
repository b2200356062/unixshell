#include <stdio.h>
#include <signal.h>

void sigint_handler(int signo){
	printf("caught SIGINT\n");
}

int main(){
    signal(SIGINT, sigint_handler);
    while(1);
}