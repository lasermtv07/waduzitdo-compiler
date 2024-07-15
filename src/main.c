/* WADUZITDO to NASM compiler
 * released under the UNLICENSE software license
 * (c) lasermtv07,2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
//dont use windows, untested
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif
//---
#include "structs.c"
#include "parser.c"
#include "cli.c"

char* itoa(int n){
	char*m=malloc(64);
	m[0]='\0';
	int p=1;
	int pri=0;
	//separates invidivual digits
	while((n/p)!=0){
		p*=10;
		strcat(m,char2string((((n%p)-pri)/(p/10))+0x30));
		pri=n%p;
	}
	// *sighs*reverses string
	char* o=malloc(64);
	o[0]='\0';
	for(int i=strlen(m)-1;i>=0;i--){
		strcat(o,char2string(m[i]));
	}
	return o;
}

char* removeNewlinesAndEscape(char* n){
	char* o=malloc(strlen(n)*2+2);
	o[0]='\0';
	for(int i=0;i<strlen(n);i++){
		if(n[i]!='\''){
			if(n[i]!='\n') strcat(o,char2string(n[i]));
		}
		else strcat(o," ");
	}
	return o;
}


int main(int argc,char**argv){
	struct cli a;
	parseCLI(&a,argc,argv);
	if(access(a.input,F_OK)!=0){
		printf("Error: file does not exist!\n");
		exit(0);
	}

	struct command t;
	FILE* prog=fopen(a.input,"r");
	char buff[10240];

	//variables regarding the DATA section (constants)
	char* data=malloc(1024);
	const char* data_pre=" db '";
	const char* data_post="', 0Ah\n";
	int datac=1;

	//bss
	const char* bss="SECTION .bss\nbuff: resb 1024";
	
	char* functions=malloc(2048);
	functions[0]='\0';
	strcat(functions,"read:\nmov eax,3\nmov ebx,0\nmov ecx,buff\nmov edx,1024\nint 0x80\nret ;\n");
	strcat(functions,"stop:\nmov eax,1\nmov ebx,0\nint 0x80");
	strcat(functions,"\njstop:\ncmp byte [buff],al\nje stop\nret ;");
	strcat(functions,"\nnstop:\ncmp byte [buff],al\njne stop\nret ;");
	strcat(functions,"\njp0:\njmp _start");
	char* currentContext=malloc(1024);

	int m=-1;

	int jplabels=0;
	char* final=malloc(1024);

	if(prog!=NULL){
		while(fgets(buff,10240,prog)){
			parseLine(&t,buff);
			if(t.jp==1){
				final=realloc(final,strlen(final)+strlen(currentContext)+1024);
				if(jplabels==0){
					strcat(final,"global _start\n_start:");
					strcat(final,currentContext);
				}
				else {
					sprintf(final,"%s\njp%u:%s",final,jplabels,currentContext);
				}
				strcpy(currentContext,"");
				currentContext=realloc(currentContext,1024);
				jplabels++;
			}
			switch(t.com){
				case 't':
					//creates printing function
					data=realloc(data,strlen(data)+strlen(data_pre)+strlen(t.arg)+strlen(data_post)+4);
					strcat(data,"m");
					strcat(data,itoa(datac));
					strcat(data,data_pre);
					strcat(data,removeNewlinesAndEscape(t.arg));
					strcat(data,data_post);
					functions=realloc(functions,strlen(functions)+256);
					strcat(functions,"\nt");
					strcat(functions,itoa(datac));
					strcat(functions,":\nmov edx,");
					strcat(functions,itoa(strlen(removeNewlinesAndEscape(t.arg))+1));
					strcat(functions,"\nmov ecx,m");
					strcat (functions,itoa(datac));
					strcat(functions,"\nmov ebx,1\nmov eax,4\nint 0x80\nret ;\n");
					currentContext=realloc(currentContext,strlen(currentContext)+128);
					if(t.cond==2 || m==-1){
						//calls function in context
						strcat(currentContext,"\ncall t");
						strcat(currentContext,itoa(datac));
					} else {
						//makes function for comparison
						functions=realloc(functions,strlen(functions)+256);
						strcat(functions,"\ncmpt");
						strcat(functions,itoa(datac));
						strcat(functions,"m");
						strcat(functions,itoa(m));
						if(t.cond==1) strcat(functions,"t:\n");
						else strcat(functions,"f:\ncmp byte [buff],'");
						strcat(functions,char2string(m));
						if(t.cond==1) strcat(functions,"'\nje t");
						if(t.cond==0) strcat(functions,"'\njne t");
						strcat(functions,itoa(datac));
						strcat(functions,"\nret ;\n");

						//calls comparison in context. dam. I really should've used sprintf
						strcat(currentContext,"\ncall cmpt");
						strcat(currentContext,itoa(datac));
						strcat(currentContext,"m");
						strcat(currentContext,itoa(m));
						if(t.cond==1) strcat(currentContext,"t\n");
						else strcat(currentContext,"f\n");					
					}

					datac++;
					break;
				case 'a':
					currentContext=realloc(currentContext,strlen(currentContext)+64);
					strcat(currentContext,"\ncall read");
					break;
				case 'm':
					if(strlen(t.arg)>1) m=t.arg[0];
					break;
				case 's':
					currentContext=realloc(currentContext,strlen(currentContext)+64);
					if(t.cond==2) strcat(currentContext,"\ncall stop");
					else {
						//i have discovered sprintf() :D
						sprintf(currentContext,"%s\nmov al,%u\n",currentContext,m);
						if(t.cond==1) strcat(currentContext,"call jstop\n");
						else strcat(currentContext,"call nstop\n");
					}
					break;
				case 'j':
					currentContext=realloc(currentContext,strlen(currentContext)+1024);
					if(t.cond==2){
						if(atoi(t.arg)!=0)sprintf(currentContext,"%s\njmp jp%u",currentContext,atoi(t.arg)+jplabels);
						else sprintf(currentContext,"%s\njmp jp0",currentContext);
					}
					else if(t.cond==1){
						if(atoi(t.arg)!=0)sprintf(currentContext,"%s\ncmp byte [buff],%u\nje jp%u",currentContext,m,atoi(t.arg)+jplabels);
						else sprintf(currentContext,"%s\njmp jp0",currentContext);
					}
					else if(t.cond==0){
						if(atoi(t.arg)!=0)sprintf(currentContext,"%s\ncmp byte [buff],%u\njne jp%u",currentContext,m,atoi(t.arg)+jplabels);
						else sprintf(currentContext,"%s\njmp jp0",currentContext);
					}
			}

		}

	}
	final=realloc(final,strlen(final)+strlen(currentContext)+1024);
	if(jplabels==0){
		strcat(final,"global _start\n_start:");
		strcat(final,currentContext);
	}
	else {
			sprintf(final,"%s\njp%u:%s",final,jplabels,currentContext);
	}
	strcpy(currentContext,"");
	currentContext=realloc(currentContext,1024);
	jplabels++;
	FILE* wr=fopen(a.output,"w");
	fprintf(wr,"SECTION .data\n%s\n%s\nSECTION .text\n%s\n%s",data,bss,functions,final);
	return 0;
}
