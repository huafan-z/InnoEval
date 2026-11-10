#pragma once
#include "list.h"


extern Node* listwork_head;
extern int MAX_ID;
extern int WORK_SUM;



char safechar(char* msg);
void safelogin(char* str, int n, char* msg);
void safestr(char* str, int n, char* msg);
int safenumber(int min, int max, char* msg);
void go_on(void);



int chooserole(void);
int is_again(void);
















