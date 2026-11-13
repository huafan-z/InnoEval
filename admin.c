#include "admin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "list.h"
#define MAX_RESULT 50
int adm_login(void) {
	const char* correct_user = "admin";
	const char* correct_pwd = "123456";
	char in_user[20];
	char in_pwd[20];
	//校验账号和密码
	int fail_count = 0;
	while (fail_count < 3) { //错3次退出
		safelogin(in_user, sizeof(in_user), "Username: ");
		safelogin(in_pwd, sizeof(in_pwd), "Password: ");
		//校验（拦截空输入 + 比较内容）
		if (in_user[0] == '\0' || in_pwd[0] == '\0') {
			printf("[Error] Username or password cannot be empty!\n\n");
			continue;
		}
		//strcmp 返回 0 表示完全一致
		if (strcmp(in_user, correct_user) == 0 && strcmp(in_pwd, correct_pwd) == 0) {
			printf("\n[Success] Login successful!\n");
			go_on();
			return 1; // 成功返回 1
		}
		else {
			fail_count++;
			printf("[Error] Invalid username or password. Attempts left: %d\n", 3 - fail_count);
		}
	}
	printf("[Error] Too many failed attempts. Exiting.\n");
	return 0; // 失败返回 0
}
void adm_run(void) {
	while (1) {
		system("cls");
		printf("====Welcome admin!====\n");
		printf("1. Create work(s)\n");//创建作品
		printf("2. Delete work\n");//删除作品
		printf("3. Query works\n");//查询作品
		printf("4. Modify a work\n");//修改作品
		printf("5. Publish work(s) / Unpublish a work\n");//发布/撤回 作品
		printf("6. Save work\n");//保存作品
		printf("7. Calculate the average score\n");//计算平均分
		printf("8. Sorting of works\n");//作品排序
		printf("0. Exit system\n");//退出系统
		int choice = safenumber(0, 8, "Please enter your choice (0-8): ");
		switch (choice) {
			case 0: return;
			case 1: createWork(); break;
			case 2: deleteWork(); break;
			case 3: queryWork(); break;
			case 4: modifyWork(); break;
			case 5: publishWork(); break;
			case 6: saveWork(); break;
			case 7: calcAverageScore(); break;
			case 8: sortWork(); break;
		}
	}
}
work create(void) {
	int is_clear = 0;
	while (1) {
		if (is_clear) {
			system("cls");
			is_clear = 0;
		}
		work tem_work;
		memset(&tem_work, 0, sizeof(work));
		printf("You can input '.' to re-enter!\n\n");
		safestr(tem_work.title, sizeof(tem_work.title), "Title of the Work : ");
		if (tem_work.title[0] == '.') { is_clear = 1; continue; }

		safestr(tem_work.author_name, sizeof(tem_work.author_name), "Author's Name: ");
		if (tem_work.author_name[0] == '.') { is_clear = 1; continue; }

		safestr(tem_work.student_id, sizeof(tem_work.student_id), "Author's student ID: ");
		if (tem_work.student_id[0] == '.') { is_clear = 1; continue; }

		safestr(tem_work.major, sizeof(tem_work.major), "Author's specialty: ");
		if (tem_work.major[0] == '.') { is_clear = 1; continue; }

		safestr(tem_work.grade, sizeof(tem_work.grade), "Author's grade: ");
		if (tem_work.grade[0] == '.') { is_clear = 1; continue; }

		for (int i = 0;i < 5;++i) tem_work.scores[i] = 0.0;
		tem_work.is_published = 0;
		tem_work.avg_score = 0.0;
		return tem_work;
	}
}
void createWork(void) {
	int is_again = 0, i = 1;
	while (i) {
		system("cls");
		printf("==== Create Work(s) ====\n");
		printf("Fill in the following fields.\n");
		printf("Work ID will be assigned automatically.\n\n");
		if (is_again) printf("Please input the informations again!\n");
		work tem_work = create();
		char confirm = safechar("yn", "Confirm this information?(y/n): ");
		if (confirm != 'y') {//用户确认信息
			is_again = 1;
			printf("1. again\n2. return\n");
			int choice = safenumber(1, 2, "Do you want to create again?(1 or 2): ");
			switch (choice) {//取消的话，是否再次创建
				case 1:continue;
				case 2:
					if (i - 1 == 1)
						printf("Cancelled. One work created in total.\n");
					else if (i - 1) {
						printf("Cancelled. %d works created in total.\n", i - 1);
					}
					else printf("Cancelled. No work created this time.\n");
					return;
			}
		}
		tem_work.id = ++MAX_ID;++WORK_SUM;
		addNode(&tem_work);//把创建的tem_work装进链表
		printf("\nCreate Successfully!\n");
		go_on();
		system("cls");
		i++;
		char is_continue = safechar("yn", "Do you want to continue creating? (y/n): ");
		if (is_continue == 'y') continue;
		else {
			if (i - 1 == 1) printf("Cancelled. One work created in total.\n");
			else printf("Cancelled. %d works created in total.\n", i - 1);
			return;
		}

	}
}
void deleteWork(void) {
	system("cls");
	if (!listwork_head) {
		printf("List is empty.\n");
		go_on();
		return;
	}
	printf("==== Delete a Work ====\n");
	printf("Enter the ID of the work you want to delete.\n");
	printf("Tip: use [3] Query works first if you are not sure of the ID.\n\n");
	int id = safenumber(0, MAX_ID > 0 ? MAX_ID : 1, "Enter Work ID to delete(enter '0' to return): ");
	if (id == 0) return;
	Node** link = find_id(id);
	if (link) {
		if ((*link)->data.is_published) {
			printf("\n[Error] Work ID %d is published and cannot be deleted.\n", id);
			go_on();
			return;
		}
		print_work(&(*link)->data);
		char confirm = safechar("yn", "\n\nAre you sure to delete this work? (y/n): ");
		if (confirm == 'y') {
			delete_Node(link);
			printf("\n[Success] Deleted ID %d.\n", id);
		}
		else printf("\n[Info] Deletion cancelled.\n");
	}
	else printf("\n[Error] Work ID %d not found.\n", id);
	go_on();
}
void queryWork(void) {
	system("cls");
	if (!listwork_head) {
		printf("List is empty.\n");
		go_on();
		return;
	}
	while (1) {
		system("cls");
		printf("\n==== Query Works ====\n");
		printf("1. Query by ID\n");//ID查找
		printf("2. Query by Title\n");//作品名查找
		printf("3. Query by Author Name\n");//作者名查找
		printf("4. Query by Student ID\n");//作者学号查找
		printf("5. Query by Keyword\n");//作品名关键词查找
		printf("0. Return\n");
		int choice = safenumber(0, 5, "Select a query method (0-5): ");
		printf("\n");
		work outWork[MAX_RESULT];
		switch (choice) {
			case 0:return;
			case 1: {//找到直接打印并允许退出查询
				int id = safenumber(1, MAX_ID > 0 ? MAX_ID : 1, "Enter Work ID: ");
				Node** link = find_id(id);
				if (link) print_work(&(*link)->data);
				else printf("Work with ID %d not found.\n", id);
				go_on();
				break;
			}
			case 2: {//2、3、4、5都要经过分页层
				char title[64];
				safestr(title, sizeof(title), "Enter Title: ");
				if (!queryOut(outWork, MAX_RESULT, find_by_title, title))
					printf("No work titled \"%s\".\n", title);
				break;
			}
			case 3: {
				char author[32];
				safestr(author, sizeof(author), "Enter Author Name: ");
				if (!queryOut(outWork, MAX_RESULT, find_by_author, author))
					printf("No work by author \"%s\".\n", author);
				break;
			}
			case 4: {
				char stu_id[20];
				safestr(stu_id, sizeof(stu_id), "Enter Student ID: ");
				if (!queryOut(outWork, MAX_RESULT, find_by_student_id, stu_id))
					printf("No work with student ID \"%s\".\n", stu_id);
				break;
			}
			case 5: {
				char keyword[64];
				safestr(keyword, sizeof(keyword), "Enter Keyword: ");
				if (!queryOut(outWork, MAX_RESULT, find_by_keyword, keyword))
					printf("No work matching \"%s\".\n", keyword);
				break;
			}
		}
	}
}
void modifyWork(void) {
	system("cls");
	if (!listwork_head) {
		printf("List is empty.\n");
		go_on();
		return;
	}
	while (1) {
		system("cls");
		printf("==== Modify a Work ====\n");
		printf("Enter the ID of the work you want to modify.\n");
		printf("Tip: use [3] Query works first if you are not sure of the ID.\n");
		printf("Enter 0 to cancel.\n\n");
		int id = safenumber(0, MAX_ID > 0 ? MAX_ID : 1, "Work's ID: ");
		if (id == 0) return;
		Node** link = find_id(id);
		if (link) {
			if ((*link)->data.is_published) {
				printf("\n[Error] Work ID %d is published and cannot be modified.\n", id);
				go_on();
				return;
			}
			int i = 1;
			while (i == 1) {
				system("cls");
				printf("\nCurrent work information:\n");
				print_work_simple(&(*link)->data);
				printf("\nWhat would you like to modify?\n");
				printf("1. Title\n");
				printf("2. Author's information\n");
				printf("0. Cancel\n");
				int choice = safenumber(0, 2, "Select a field to modify(0 - 2): ");
				switch (choice) {
					case 0:i = 0;break;
					case 1:MoTitle(link);break;
					case 2:MoAuthor(link);break;
				}
			}
		}
	}
}
void publishWork(void) {
	system("cls");
	if (!listwork_head) {
		printf("List is empty.\n");
		go_on();
		return;
	}
	while (1) {
		system("cls");
		printf("\n==== Publish / Unpublish ====\n");
		printf("1. Publish one work\n");
		printf("2. Publish all unpublished\n");
		printf("3. Unpublish one work\n");
		printf("0. Back\n");
		int choice = safenumber(0, 3, "Select an option (0-3): ");
		switch (choice) {
			case 0:return;
			case 1:PubOne();break;
			case 2:PubAll();break;
			case 3:UnpubOne();break;
		}
		go_on();
	}
}









void saveWork(void) {
	system("cls");
	if (!listwork_head) {
		printf("\n[Info] No work to save.\n");
		go_on();
		return;
	}
	printf("\n[Warning] Saving will overwrite the file. Deleted works cannot be recovered.\n");
	char c = safechar("yn", "Save now? (y/n): ");
	if (c == 'n') {
		printf("[Info] Save Cancel.");
		return;
	}
	WorkToFile();
	go_on();
	return;
}








void calcAverageScore(void) {}
void sortWork(void) {}












