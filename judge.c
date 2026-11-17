#include "judge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "list.h"

#define PAGE_SIZE 5
#define MAX_RESULT 50

const char correct_judge[20] = "judge";
char in_judge[20];

static void savescores(void);

int jud_login(void) {
	int fail_count = 0;
	while (fail_count < 3) {
		safelogin(in_judge, sizeof(in_judge), "Your name: ");
		if (in_judge[0] == '\0') {
			printf("[Error] Username cannot be empty!\n\n");
			continue;
		}
		if (strcmp(in_judge, correct_judge) == 0) {
			printf("\n[Success] Login successful!");
			go_on();
			return 1;// 登录成功
		}
		else {
			++fail_count;
			printf("[Error] Invalid username. Attempts left: %d\n", 3 - fail_count);
		}
	}
	printf("[Error] Too many failed attempts. Exiting.\n");
	return 0; // 登录失败
}
void jud_run(void) {
	if (!listwork_head) {
		printf("\n[Error] No works in the system. Judge mode terminated.\n");
		go_on();
		return;
	}
	while (1) {
		system("cls");
		printf("==== Welcome Judge! ====\n\n");
		printf("1. Query works\n");//查询作品
		printf("2. Rate the work\n");//给作品评分
		printf("3. Save the score(s)\n");//保存评分
		printf("0. Exit system \n\n");
		int choice = safenumber(0, 3, "Please enter your choice (0-3): ");
		switch (choice) {
			case 0:
				if (exit_check_save())
					return;
				break;
			case 1: Query_jud();break;
			case 2: Rate_work();break;
			case 3: savescores();break;
		}
	}
}
void Query_jud(void) {
	while (1) {
		system("cls");
		printf("==== Query Works ====\n\n");
		printf("1. Query By ID\n");
		printf("2. Query By Title\n");
		printf("3. Query By Keyword\n");
		printf("4. Show all\n");
		printf("0. Return\n\n");
		work outwork[MAX_RESULT];
		int choice = safenumber(0, 4, "Please enter your choice (0-4): ");
		switch (choice) {
			case 0:return;
			case 1:jud_find_id();break;
			case 2: {
				char userstr[64];
				safestr(userstr, sizeof(userstr), "Enter Title: ");
				int count = 0;
				for (Node* p = listwork_head; p && count < MAX_RESULT; p = p->next) {
					if (p->data.is_published == 1 && strcmp(p->data.title, userstr) == 0) {
						outwork[count++] = p->data;
					}
				}
				if (count == 0) { printf("\nNo published work titled \"%s\".\n", userstr); go_on(); }
				else if (count <= PAGE_SIZE) printOnePAGE(outwork, count);
				else printPAGE(outwork, count, PAGE_SIZE);
				break;
			}
			case 3: {
				char userstr[64];
				safestr(userstr, sizeof(userstr), "Enter Keyword: ");
				int count = 0;
				for (Node* p = listwork_head; p && count < MAX_RESULT; p = p->next) {
					if (p->data.is_published == 1 && strstr(p->data.title, userstr) != NULL) {
						outwork[count++] = p->data;
					}
				}
				if (count == 0) { printf("\nNo published work matching \"%s\".\n", userstr); go_on(); }
				else if (count <= PAGE_SIZE) printOnePAGE(outwork, count);
				else printPAGE(outwork, count, PAGE_SIZE);
				break;
			}
			case 4: {
				array_out(outwork, MAX_RESULT);
				if (PUBLISH <= PAGE_SIZE) printOnePAGE(outwork, PUBLISH);
				else printPAGE(outwork, PUBLISH, PAGE_SIZE);
				break;
			}
		}
	}
}
void array_out(work* outwork, int size) {
	Node* p = listwork_head;
	int count = 0;
	int limit = (PUBLISH < size) ? PUBLISH : size;
	for (int i = 0;i < limit && p != NULL;++i) {
		if (p->data.is_published == 1)
			outwork[count++] = p->data;
		p = p->next;
	}
}
void jud_find_id(void) {
	int id = safenumber(0, MAX_ID, "Enter Work ID('0' to return): ");
	if (id == 0) return;
	Node** link = find_id(id);
	if (link) {
		if ((*link)->data.is_published == 0) printf("\n[Error] This work is not published.\n");
		else print_work_simple(&(*link)->data);
	}
	else printf("\n[Error] Work ID %d not found.\n", id);
	go_on();
}
void Rate_work(void) {
	while (1) {
		system("cls");
		int id = safenumber(0, MAX_ID, "Enter Work ID to Rate(enter '0' to return): ");
		if (id == 0) return;
		Node** link = find_id(id);
		if (link) {
			if ((*link)->data.is_published == 0) {
				printf("\n[Info] Work ID %d is unpublish!", id);
				go_on();
				continue;
			}
			int has_rated = 0;
			for (int i = 0;i < 5;++i) {
				//确认当前评委未点评此作品
				if (strcmp((*link)->data.judge[i], in_judge) == 0) {
					has_rated = 1;
					break;
				}
				if ((*link)->data.judge[i][0] == '\0') {//找到第一个没打分的点
					int score = safenumber(0, 100, "Please enter a score (0-100): ");
					(*link)->data.scores[i] = (double)score;//录入分数
					strcpy_s((*link)->data.judge[i], sizeof((*link)->data.judge[i]), in_judge);//存好谁评的
					double sum = 0.0;//更新平均分
					int i = 0;
					for (;i < 5;++i)
						if ((*link)->data.judge[i][0] != '\0')
							sum += (*link)->data.scores[i];
						else break;
					(*link)->data.avg_score = sum / (double)i;
					printf("\n[Success] Rated successfully.");
					go_on();
					return;//打完分就返回
				}
				if (i == 4) {
					printf("[Error] The scores for this work are full.");
					go_on();
					continue;
				}
			}
			if (has_rated) {
				printf("\n[Error] You have already rated this work.");
				go_on();
				continue;
			}
		}
		else printf("\n[Error] Work ID %d not found.\n", id);
	}
}
static void savescores(void) {
	system("cls");
	if (!listwork_head) {
		printf("\n[Info] No works to save.\n");
		go_on();
		return;
	}
	printf("\n[Warning] Saving will overwrite the file. This action cannot be undone.\n");
	char c = safechar("yn", "Save scores now? (y/n): ");
	if (c == 'n') {
		printf("\n[Info] Save cancelled.\n");
		go_on();
		return;
	}
	Save();
	go_on();
}
