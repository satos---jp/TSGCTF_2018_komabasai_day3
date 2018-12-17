


def esc(s):
	def f(c):
		if c == '\n':
			return '$'
		elif c == '"':
			return '`'
		else:
			return c
	
	s = map(f,s)
	s = ''.join(s)
	return s

head = """
#include<stdio.h>
#define FLAGLEN 17
char* head = 
"""[1:-1]

tail = """
unsigned int myrand(){
	iv = (iv * 314 + 159) % 265358;
	return iv;
}

void myputs(char* s){
	for(; *s!=0; s++){
		if(*s==36){
			putchar(10);
		}
		else if(*s==96){
			putchar(34);
		}
		else{
			putchar(*s);
		}
	}
}

int main(){
	if(cnt==0){
		printf("Here is the flag :: %s%c",flag,10);
		return 0;
	}
	myputs(head);
	printf("%c%s%c;%c",34,head,34,10);
	
	unsigned int p,q;
	p = myrand();
	q = myrand();
	flag[p % FLAGLEN] ^= (q & 255);
	
	printf("unsigned int iv = %u;%c",iv,10);
	printf("unsigned int cnt = %u;%c",cnt-1,10);
	printf("unsigned char flag[FLAGLEN+1] = {%c%c",10,9);
	int i;
	for(i=0;i<FLAGLEN;i++){
		printf("%d, ",flag[i]);
	}
	printf("0%c};%c",10,10);
	
	printf("char* tail = %c%s%c;%c",34,tail,34,10);
	myputs(tail);
	return 0;
}
"""

prog = (
	head + '"' + esc(head) + '";\n' + 
	"unsigned int iv = 1337;\n" + 
	"unsigned int cnt = 1000000;\n" + 
	"unsigned char flag[FLAGLEN+1] = {\n" +
	"\t84, 83, 71, 123, 73, 95, 49, 48, 118, 101, 95, 57, 117, 105, 110, 101, 125, 0\n" +
	"};\n" + 
	"char* tail = \"" + esc(tail) + '";\n' + 
	tail
)



import sys
sys.stdout.write(prog);

