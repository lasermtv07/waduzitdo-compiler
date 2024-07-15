#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structs.c"

char* char2string(char c){
	char* s=malloc(2);
	s[0]=c;s[1]=0;
	return s;
}

void printCom(struct command s){
	printf(
			"COM\n===\n >condition:%u\n >command:%c\n >arg:%s\n >jump:%u\n",
			s.cond,s.com,s.arg,s.jp
		);
	return;

}

void parseLine(struct command* a,char line[]){
	//split the line into a command and its argument
	char* command=malloc(strlen(line)+1);
	command[0]=0;
	char* arg=malloc(strlen(line)+1);
	arg[0]=0;
	bool sw=false;
	for(int i=0;i<strlen(line);i++){
		if(line[i]==':') sw=true;
		else if(!sw) strcat(command,char2string(line[i]));
		else strcat(arg,char2string(line[i]));
	}
	
	//actually parse the command
	int cond=2;
	char com;
	int jp=0;
	if(strlen(command)>=2){
		if(command[0]=='y' || command[0]=='Y') cond=1;
		if(command[0]=='n' || command[0]=='N') cond=0;
		if(command[0]=='*'){
			jp=1;
			if(strlen(command)==2) com=command[1];
			else if(strlen(command)>=3){
				com=command[2];
				if(command[1]=='y' || command[1]=='Y') cond=1;
				if(command[1]=='n' || command[1]=='N') cond=0;
			}
		}
		com=command[1];
	}
	else if(strlen(command)==1){
		com=command[0];
	}
	else com='/';
	if(com>=65 && com<=90) com+=32;
	//save the parsed data into a struct that was passed as input
	a->cond=cond;
	a->com=com;
	a->arg=arg;
	a->jp=jp;
}
