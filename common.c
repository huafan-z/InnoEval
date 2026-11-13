#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define EPS 1e-9
#define PAGE_SIZE 5
static void Mo_confirm(const char* field, char c) {
	if (c == 'y') printf("\n[Success] %s updated.\n", field);
	else printf("\n[Info] Change cancelled.\n");
	go_on();
}
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
void safestr(char *str, int n, char* prompt) {//仅用于char数组，安全输入字符串
	while (1) {
		printf("%s", prompt);
		//判断是否是空字符
		if (fgets(str, n, stdin) == NULL) continue;
		//用户直接回车
		if (str[0] == '\n') continue;
		//判断输入长度是否过长
		if (strchr(str, '\n') == NULL) {
			int tem;//垃圾桶
			while ((tem = getchar()) != '\n' && tem != EOF);
			printf("\n[Error] Input too long. Please try again!\n");
			continue;
		}
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
		printf("\nPage 1 | %d work(s) in total\n", count);
		char c = safechar("qs","[q] Quit  [s] Select: ");
		switch (c) {
			case 'q': return;
			case 's': {
				int choice = safenumber(1, count, "Enter the row number to select: ");
				system("cls");
				print_work(&outWork[choice - 1]);
				go_on();
				continue;
			}
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
			case 'p':
				if (i != 0)
					i -= PAGE_SIZE;
				else at_edge = 1;
				continue;
			case 'n':
				if (i + PAGE_SIZE < count)
					i += PAGE_SIZE;
				else at_edge = 1;
				continue;
			case 'q': return;
			case 's': {
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
void MoTitle(Node** link) {
	system("cls");
	char new_title[64];
	printf("\nCurrent work information:\n");
	print_work_simple(&(*link)->data);
	printf("\nOld Title: \"%s\" -> ", (*link)->data.title);
	safestr(new_title, sizeof(new_title), "\nNow Title: ");
	printf("Change Title from \"%s\" to \"%s\"?\n", (*link)->data.title, new_title);
	char c = safechar("yn", "Confirm? (y/n): ");
	if (c == 'y') strcpy_s((*link)->data.title, sizeof((*link)->data.title), new_title);
	Mo_confirm("Title", c);
}
void MoAuthor(Node** link) {
	while (1) {
		system("cls");
		char new_title[64];
		printf("\nCurrent work information:\n");
		print_work_simple(&(*link)->data);
		printf("\nWhat would you like to modify?\n");
		printf("\n1. Old Author Name: \"%s\"\n", (*link)->data.author_name);
		printf("\n2. Old Student ID: \"%s\"\n", (*link)->data.student_id);
		printf("\n3. Old Major: \"%s\"\n", (*link)->data.major);
		printf("\n4. Old Grade: \"%s\"\n", (*link)->data.grade);
		printf("\n0. Return\n");
		int choice = safenumber(0, 4, "Select a field to modify(0 - 4): ");
		switch (choice) {
			case 0: return;
			case 1: {
				char name[32];
				MoAu("Author Name", link, name, sizeof(name), (*link)->data.author_name);
				break;
			}
			case 2: {
				char stu_id[20];
				MoAu("Author Student ID", link, stu_id, sizeof(stu_id), (*link)->data.student_id);
				break;
			}
			case 3: {
				char major[64];
				MoAu("Author Major", link, major, sizeof(major), (*link)->data.major);
				break;
			}
			case 4: {
				char grade[16];
				MoAu("Author Grade", link, grade, sizeof(grade), (*link)->data.grade);
				break;
			}
		}
	}
}
void MoAu(const char* field, Node** link, char* new, int size, char* old) {
	system("cls");
	printf("\nCurrent work information:\n");
	print_work_simple(&(*link)->data);
	printf("\nOld Title: \"%s\" -> ", old);
	safestr(new, size, "\nNow Title: ");
	printf("Change Title from \"%s\" to \"%s\"?\n", old, new);
	char c = safechar("yn", "Confirm? (y/n): ");
	if (c == 'y') {
		if (strcmp(field, "Author Name") == 0) strcpy_s((*link)->data.author_name, sizeof((*link)->data.author_name), new);
		else if (strcmp(field, "Student ID") == 0) strcpy_s((*link)->data.student_id, sizeof((*link)->data.student_id), new);
		else if (strcmp(field, "Major") == 0) strcpy_s((*link)->data.major, sizeof((*link)->data.major), new);
		else if (strcmp(field, "Grade") == 0) strcpy_s((*link)->data.grade, sizeof((*link)->data.grade), new);
	}
	Mo_confirm(field, c);
}
void PubOne(void) {
	system("cls");
	int id = safenumber(0, MAX_ID > 0 ? MAX_ID : 1, "Enter Work ID to publish(enter '0' to return): ");
	if (id == 0) return;
	Node** link = find_id(id);
	if (link) {
		if ((*link)->data.is_published) {
			printf("\n[Info] Work ID %d is already published.\n", id);
			go_on();
			return;
		}
		print_work(&(*link)->data);
		char confirm = safechar("yn", "Publish this work? (y/n): ");
		if (confirm == 'y') {
			(*link)->data.is_published = 1;
			--UNPUBLISH;
			printf("\n[Success] Publish ID %d.\n", id);
		}
		else printf("\n[Info] Publish cancelled.\n");
	}
	else printf("\n[Error] Work ID %d not found.\n", id);
	go_on();
}
void PubAll(void) {
	if (UNPUBLISH == 0) {
		printf("\n[Info] No unpublished works.\n");
		return;
	}
	else printf("\n%d unpublished work(s).", UNPUBLISH);
	char c = safechar("yn", " Publish all? (y/n): ");
	if (c == 'y') {
		Publish_All();
		printf("\n[Success] %d work(s) published.\n", UNPUBLISH);
		UNPUBLISH = 0;
	}
	else printf("\n[Info] Publish cancelled.\n");
}
void UnpubOne(void) {
	system("cls");
	int id = safenumber(0, MAX_ID > 0 ? MAX_ID : 1, "Enter Work ID to unpublish: (enter '0' to return): ");
	if (id == 0) return;
	Node** link = find_id(id);
	if (link) {
		if ((*link)->data.is_published == 0) {
			printf("\n[Info] Work ID %d is not published.\n", id);
			return;
		}
		print_work(&(*link)->data);
		double sum = 0.0;
		for (int i = 0;i < 5;++i) sum += (*link)->data.scores[i];
		if (fabs(sum) > EPS) {
			printf("\n[Error] Work ID %d has scores and cannot be unpublished.\n");
			return;
		}
		char confirm = safechar("yn", "Unpublish this work? (y/n): ");
		if (confirm == 'y') {
			(*link)->data.is_published = 0;
			--UNPUBLISH;
			printf("\n[Success] Unpublish ID %d.\n", id);
		}
		else printf("\n[Info] Unpublish cancelled.\n");
	}
	else printf("\n[Error] Work ID %d not found.\n", id);
}






