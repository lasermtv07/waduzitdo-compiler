#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct cli {
	char* output;
	char* input;
};
void parseCLI(struct cli*i,int argc,char** argv){
	if(argc<=1){
		printf("Error: argument needed! More info via --help!\n");
		exit(0);
	}
	char *o=malloc(1024);
	o[0]=0x0;
	char *ip=malloc(1024);
	ip[0]=0x0;
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"--help")==0){
			printf("Usage: %s [option] file...\nOptions:\n  -o     Sets output file name\n",argv[0]);
			printf("  -i     Sets input file name\n  --help Displays this help\n");
			printf("Note: This program compiles to assembly, so after complaiton you also need to\n");
			printf("assemble the program with NASM and link it.\n");
			exit(0);
		}
		if(strcmp(argv[i],"-o")==0 && i!=argc-1) strcpy(o,argv[i+1]);
		if(strcmp(argv[i],"-i")==0 && i!=argc-1) strcpy(ip,argv[i+1]);
	}
		if(strlen(ip)==0 && argc>3 && strcmp(argv[argc-2],"-o")!=0) strcpy(ip,argv[argc-1]);
		if(strlen(ip)==0 && argc>=2 && strcmp(argv[1],"-o")!=0) strcpy(ip,argv[1]);
		char* temp;
		char* bu=malloc(1024);
		bu[0]=0x0;
		if(strlen(o)==0){
			strcpy(bu,ip);
			temp=strtok(ip,".");
			strcpy(o,temp);
			strcat(o,".asm");
			strcpy(ip,bu);
		}
		if(strlen(ip)==0){
			printf("Error: No input file found!\n");
			exit(0);
		}
		i->output=o;
		i->input=ip;
}
