#pragma once
#include "list.h"

extern const char current_judge[20];
extern Node* listwork_head;
extern int UNPUBLISH;
extern int WORK_SUM;
extern int PUBLISH;
extern int MAX_ID;
extern int role;

void safe_password(char* str, int size, const char* prompt);
char safechar(const char* allowed, const char* msg);
void safelogin(char* str, int n, const char* msg);
void safestr(char* str, int n, const char* msg);
int safenumber(int min, int max, char* msg);
void go_on(void);
void print_work(const work* w);
void print_work_simple(const work* w);
void printOnePAGE(const work* outWork, int count);
void printPAGE(const work* outWor, int count, int page_size);
int chooserole(void);
int is_again(void);
int queryOut(work* outWork, int size, Node* (*tool)(Node*, const char*), const char* userstr);
void MoTitle(Node** link);
void MoAuthor(Node** link);
void PubOne(void);
void PubAll(void);
void UnpubOne(void);
void Save(void);
void sortWork(void);
int exit_check_save(void);
