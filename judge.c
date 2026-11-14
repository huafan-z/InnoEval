#include "judge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "list.h"
#define PAGE_SIZE 10
#define MAX_RESULT 50


int jud_login(void) {





}
void jud_run(void) {
	while (1) {
		printf("==== Welcome Judge! ====\n");
		printf("1. Query works\n");//查询作品
		printf("2. Rate the work\n");//给作品评分
		printf("3. Save the score(s)\n");//保存评分
		printf("0. Exit system \n");
		int choice = safenumber(0, 3, "Please enter your choice (0-3): ");
		switch (choice) {
			case 0:return;
			case 1:Query_jud();break;
			case 2:
			case 3:
		}
	}
}

void Query_jud(void) {
	while (1) {
		system("cls");
		printf("==== Query Works ====\n");
		printf("1. Query By ID\n");
		printf("2. Query By Title\n");
		printf("3. Query By Keyword\n");
		printf("4. Show all\n");
		printf("0. Return\n");
		work outwork[MAX_RESULT];
		int choice = safenumber(0, 4, "Please enter your choice (0-4): ");
		switch (choice) {
			case 0:return;
			case 1:jud_find_id();break;
			case 2: {
				char userstr[64];
				safestr(userstr, sizeof(userstr), "Enter Title: ");
				queryOut(outwork, MAX_RESULT, find_by_title, userstr);
				break;
			}
			case 3: {
				char userstr[64];
				safestr(userstr, sizeof(userstr), "Enter Keyword: ");
				queryOut(outwork, MAX_RESULT, find_by_keyword, userstr);
				break;
			}
			case 4: {
				array_out(outwork, MAX_RESULT);
				if (WORK_SUM <= PAGE_SIZE) printOnePAGE(outwork, WORK_SUM);
				else printPAGE(outwork, WORK_SUM, PAGE_SIZE);
				break;
			}
		}
	}
}

void array_out(work* outwork, int size) {
	Node* p = listwork_head;
	for (int i = 0;i < WORK_SUM && i < size && p != NULL;++i) {
		if(p->data.is_published == 1)
			outwork[i] = p->data;
		p = p->next;
	}
}

void jud_find_id(void) {
	int id = safenumber(0, MAX_ID > 0 ? MAX_ID : 1, "Enter Work ID('0' to return): ");
	if (id == 0) return;
	Node** link = find_id(id);
	if(link) print_work_simple(&(*link)->data);
	else printf("\n[Error] Work ID %d not found.\n", id);
	go_on();
}


















