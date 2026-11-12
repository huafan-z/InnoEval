#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define PAGE_SIZE 5
char safechar(char* allowed, char* prompt) {//安全输入char,allowed按顺序输入，例：yYnN
	char c;
	int tmp;//清理缓冲区的垃圾桶
	while (1) {
		printf("%s", prompt);
		if (scanf("%c", &c) != 1) {
			printf("\n[Error] Input error. Please try again!\n");
			while ((tmp = getchar()) != '\n' && tmp != EOF);
			continue;
		}
		if (c == '\n') { // 用户直接回车
			printf("\n[Error] Empty input. Please try again!\n");
			continue;
		}
		tmp = getchar();//如果没有第二个字符，tem将为‘n\’
		if (tmp != '\n' && tmp != EOF) {
			printf("\n[Error] Only one char allowed. Please try again!\n");
			while ((tmp = getchar()) != '\n' && tmp != EOF);
			continue;
		}
		if (strchr(allowed, c) != NULL) return c;
		else {
			printf("\n[Error] Invalid input. Use");
			for (const char* p = allowed; *p; ++p) printf(" %c", *p);
			printf(".\n");
		}	
	} 
}
void safelogin(char* str, int n, char* prompt) {//仅用于login界面，安全登录
	while (1) {
		printf("%s", prompt);
		if (fgets(str, n, stdin) == NULL) continue;
		if (strchr(str, '\n') == NULL) {
			int tem;
			while ((tem = getchar()) != '\n' && tem != EOF);
			printf("\n[Error] Input too long. Please try again!\n");
			continue;
		}
		str[strcspn(str, "\n")] = '\0';
		return;
	}
}
void safestr(char *str,int n, char* prompt) {//仅用于char数组，安全输入字符串
	while (1) {
		printf("%s", prompt);
		//判断是否是空字符
		if (fgets(str, n, stdin) == NULL) continue;
		//判断输入长度是否过长
		if (strchr(str, '\n') == NULL) {
			int tem;//垃圾桶
			while ((tem = getchar()) != '\n' && tem != EOF);
			printf("\n[Error] Input too long. Please try again!\n");
			continue;
		}
		if (str[0] == '\n') continue;//用户直接回车
		str[strcspn(str, "\n")] = '\0';
		return;
	}
}
int safenumber(int min, int max, char *prompt) {//安全输入数字
	int num;
	char str[5];
	while (1) {
		safestr(str, sizeof(str), prompt);
		//判断是否是数字
		int is_num = 1;
		for (int i = 0;str[i] != '\0';++i) {
			if (str[i] < '0' || str[i]>'9') {
				is_num = 0;
				break;
			}
		}
		if (!is_num) {
			printf("\n[Error] Invalid format. Digits only. Please input again!\n");
			continue;
		}
		num = atoi(str);//确认是数字，安全输入
		// 范围校验
		if (num < min || num > max) {
			printf("\n[Error] Out of range. Must be between %d and %d !\n", min, max);
			continue;
		}
		return num;
	}
}
void go_on(void) {
	printf("\nPress Enter...");
	getchar();
}
void print_work(const work* w) {//打印一个作品
	printf("\nWork ID: %d\n", w->id);
	printf("Title: %s\n", w->title);
	printf("Author Name: %s\n", w->author_name);
	printf("Student ID: %s\n", w->student_id);
	printf("Major: %s\n", w->major);
	printf("Grade: %s\n", w->grade);
	printf("Publish Status: %s\n", w->is_published ? "Published" : "Unpublished");
	printf("Existing Scores: ");
	for (int i = 0; i < 5; i++) printf(" %.1f ", w->scores[i]);
	printf("\nAverage Score: %.2f\n", w->avg_score);
}
void print_work_simple(const work* w) {//打印一个作品的简单信息
	printf("Work ID: %d   ", w->id);
	printf("Title: %s   ", w->title);
	printf("Author Name: %s\n", w->author_name);
}
int chooserole(void) {
	printf("\n====Welcome to use the system!====\n\n1.admin\n2.judge\n");
	int choice = safenumber(1, 2, "Please choose your identity(1 or 2): ");
	return choice;
}
int is_again(void) {
	char ch = safechar("yn","Do you want to use the system again now?(y/n): ");
	if (ch == 'y') {
		return 1;
	}
	return 0;
}
void printOnePAGE(const work* outWork, int count) {
	while (1) {
		system("cls");
		for (int j = 0;j < count;++j) {
			printf("%d. ", j + 1);
			print_work_simple(&outWork[j]);
		}
		printf("Page 1 | %d work(s) in total\n", count);
		char choice = safechar("qs","[q] Quit  [s] Select: ");
		switch (choice) {
			case 'N': return;
			case 'Y': {
				int choice = safenumber(1, count, "Enter the row number to select: ");
				system("cls");
				print_work(&outWork[choice - 1]);
				go_on();
				continue;
			}
			default:printf("\n[Error] Invalid input. Use Y or N.\n");go_on();continue;
		}
	}
}
void printPAGE(const work* outWork,int count) {
	int i = 0, at_edge;
	while (i < count) {
		system("cls");
		for (int j = 0;j < PAGE_SIZE && i + j < count;++j) {
			printf("%d. ", i + j + 1);
			print_work_simple(&outWork[i + j]);
		}
		printf("Page %d/%d | %d work(s) in total", (i + PAGE_SIZE) / PAGE_SIZE, (count + PAGE_SIZE - 1) / PAGE_SIZE, count);
		if (i == 0) printf(" | First page");
		else if (i + PAGE_SIZE >= count) printf(" | Last page");
		if (at_edge) printf("  *already here*");
		printf("\n");
		at_edge = 0;
		char choice = safechar("pnqs","[p] Prev  [n] Next  [q] Quit  [s] Select: ");
		switch (choice) {
			case 'n':
				if (i != 0)
					i -= PAGE_SIZE;
				else at_edge = 1;
				continue;
			case 'y':
				if (i + PAGE_SIZE < count)
					i += PAGE_SIZE;
				else at_edge = 1;
				continue;
			case 'N': return;
			case 'Y': {
				int choice = safenumber(1, count, "Enter the row number to select: ");
				system("cls");
				print_work(&outWork[choice - 1]);
				go_on();
				continue;
			}
		}
	}
}
int queryOut(work* outWork, int size, Node* (*tool)(Node*, const char*), const char* userstr) {
	Node* p = listwork_head;
	int count = 0;
	while (p != NULL && count < size) {
		p = tool(p, userstr);
		if (!p) break;
		outWork[count++] = p->data;
		p = p->next;
	}
	if (count == size && p != NULL) {
		printf("\n[Warning] Too many results. Only the first %d works are shown.\n", size);
		go_on();
	}
	if (count == 0) return 0;
	else if (count <= PAGE_SIZE) printOnePAGE(outWork, count);
	else printPAGE(outWork, count);
	return 1;
}













