#pragma once
#include "list.h"


extern Node* listwork_head;
extern int MAX_ID;
extern int WORK_SUM;



char safechar(char* allowed, char* msg);
void safelogin(char* str, int n, char* msg);
void safestr(char* str, int n, char* msg);
int safenumber(int min, int max, char* msg);
void go_on(void);
void print_work(const work* w);
void printOnePAGE(const work* outWork, int count);
void printPAGE(const work* outWor, int count);
int chooserole(void);
int is_again(void);
int queryOut(work* outWork, int size, Node* (*tool)(Node*, const char*), const char* userstr);














