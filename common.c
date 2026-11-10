#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"







char safechar(char* prompt) {
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
		if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') return c;
		else printf("\n[Error] Please input 'y' or 'n'! Input again!\n");
	}
}
void safelogin(char* str, int n, char* prompt) {//仅用于login界面
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
void safestr(char *str,int n, char* prompt) {//仅用于char数组
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
int safenumber(int min, int max, char *prompt) {
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
	printf("Press Enter...");
	getchar();
}




int chooserole(void) {
	printf("\n====Welcome to use the system!====\n\n1.admin\n2.judge\n");
	int choice = safenumber(1, 2, "Please choose your identity(1 or 2): ");
	return choice;
}
int is_again(void) {
	char ch = safechar("Do you want to use the system again now?(y/n): ");
	if (ch == 'y' || ch == 'Y') {
		return 1;
	}
	return 0;
}


















