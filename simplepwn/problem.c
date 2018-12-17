#include<stdio.h>
#include<stdlib.h>
#include<signal.h>


void hello(){
	char s[32];
	puts("By the way, what's your name?");
	scanf("%32s",s);
	printf("Hello %s\n",s);
}

void vuln(){
	char* cmd = "ls";	
	puts("There are many comands, for example,");
	puts(cmd);
	puts("/bin/sh");
	puts("pwd");	
	puts("and so on.");
	puts("I will execute system(\"ls\").");
	hello();
	system(cmd);
}

//gcc tes.c -fno-stack-protector

int main(){
	int __attribute((aligned(256))) x = 0;	
	int __attribute((aligned(128))) y = 0;	
	int __attribute((aligned(64))) a = 0;		
	
	//init();
	vuln();
	return 0;
}
