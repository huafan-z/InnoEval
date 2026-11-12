#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Node* listwork_head = NULL;
int MAX_ID = 0;
int WORK_SUM = 0;

int loadWorkFromFile(void){
	char line[512];//单行内容
	int s0, s1, s2, s3, s4;
	work tem_work;//临时work
	FILE* listfile = fopen("worklist.txt", "r");//打开文件
	if (listfile == NULL) {
		printf("[Error] Cannot open file.\n");
		return 0;
	}
	if (fgets(line, sizeof(line), listfile) != NULL) {//读第一行的参数
		line[strcspn(line, "\n")] = '\0';
		sscanf(line, "MAX_ID = %d", &MAX_ID);
	}
	while (fgets(line, sizeof(line), listfile) != NULL) {//读每一行的work内容
		line[strcspn(line, "\n")] = '\0';
		int parsed = sscanf(line, "%d %63s %31s %19s %63s %15s %d %d %d %d %d %d %lf",
			&tem_work.id,
			tem_work.title,
			tem_work.author_name,
			tem_work.student_id,
			tem_work.major,
			tem_work.grade,
			&tem_work.is_published,
			&s0, &s1, &s2, &s3, &s4, 
			&tem_work.avg_score);

		if (parsed == 13) {
			tem_work.scores[0] = s0;
			tem_work.scores[1] = s1;
			tem_work.scores[2] = s2;
			tem_work.scores[3] = s3;
			tem_work.scores[4] = s4;
			// 成功解析 13 个字段，存进链表
			addNode(&tem_work);
		}
		else {
			printf("Data format error on line: \n%s\n", line);
		}
	}
	fclose(listfile);
	return 1;
}
void addNode(work* new_work) {
	Node* new_Node = (Node*)malloc(sizeof(Node));
	if (new_Node == NULL) {
		printf("[Error] Memory allocation failed! Please try again later.\n");
		return;
	}
	new_Node->data = *new_work;
	new_Node->next = listwork_head;
	listwork_head = new_Node;
	WORK_SUM++;
}
Node** find_id(int id) {
	if (!listwork_head) return NULL;
	Node** link = &listwork_head;
	while (*link != NULL) {
		if ((*link)->data.id == id) return link;
		link = &((*link)->next);
	}
	return NULL;
}
void delete_Node(Node** link) {
	Node* target = *link;
	*link = target->next;
	free(target);
	WORK_SUM--;
}
Node* find_by_title(Node* p, const char* title) {
	while (p != NULL) {
		if (strcmp(p->data.title, title) == 0) return p;
		p = p->next;
	}
	return NULL;
}
Node* find_by_author(Node* p, const char* author) {
	while (p != NULL) {
		if (strcmp(p->data.author_name, author) == 0) return p;
		p = p->next;
	}
	return NULL;
}
Node* find_by_student_id(Node* p, const char* stu_id) {
	while (p != NULL) {
		if (strcmp(p->data.student_id, stu_id) == 0) return p;
		p = p->next;
	}
	return NULL;
}
Node* find_by_keyword(Node* p, const char* keyword) {
	while (p != NULL) {
		if (strstr(p->data.title, keyword) != NULL) return p;
		p = p->next;
	}
	return NULL;
}



















