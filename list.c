#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Node* listwork_head = NULL;
int MAX_ID = 0;
int WORK_SUM = 0;
int UNPUBLISH = 0;

int loadWorkFromFile(void){
	int line_no = 1;
	char line[512];//单行内容
	work tem_work;//临时work
	FILE* listfile = fopen("worklist.txt", "r");//打开文件
	if (listfile == NULL) {
		printf("[Error] Cannot open file.\n");
		return 0;
	}
	if (fgets(line, sizeof(line), listfile) != NULL) {//读第一行的参数
		line[strcspn(line, "\n")] = '\0';
		if (sscanf(line, "MAX_ID = %d", &MAX_ID) != 1) {
			printf("[Error] Invalid header. Using MAX_ID = 0.\n");
			MAX_ID = 0;
		}
	}
	while (fgets(line, sizeof(line), listfile) != NULL) {//读每一行的work内容
		++line_no;
		line[strcspn(line, "\n")] = '\0';
		int parsed = sscanf(line, "%d|%63[^|]|%31[^|]|%19[^|]|%63[^|]|%15[^|]|%d|%lf|%lf|%lf|%lf|%lf|%lf",
			&tem_work.id,
			tem_work.title,
			tem_work.author_name,
			tem_work.student_id,
			tem_work.major,
			tem_work.grade,
			&tem_work.is_published,
			&tem_work.scores[0],
			&tem_work.scores[1],
			&tem_work.scores[2],
			&tem_work.scores[3], 
			&tem_work.scores[4],
			&tem_work.avg_score);
		if (parsed == EOF) continue;
		if (parsed == 13) {
			// 判断是否发布
			if (tem_work.is_published == 0) ++UNPUBLISH;
			// 成功解析 13 个字段，存进链表
			addNode(&tem_work);
		}
		else {
			printf("Data format error on line %d: \n%s\n", line_no, line);
		}
	}
	int ok = 1;
	if (ferror(listfile)) {
		printf("[Error] Read error occurred. Data may be incomplete.\n");
		ok = 0;
	}
	if (fclose(listfile) != 0) {
		printf("[Error] Failed to close file.\n");
		ok = 0;
	}
	return ok;
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
void Publish_All(void) {
	Node** link = &listwork_head;
	while (*link != NULL) {
		if ((*link)->data.is_published == 0)
			(*link)->data.is_published = 1;
		link = &((*link)->next);
	}
	return;
}
void WorkToFile(void) {
	FILE* file = fopen("worklist.tmp", "w");//先存在tem里
	if (!file) return;
	fprintf(file, "MAX_ID = %d\n", MAX_ID);//写第一行
	Node* p = listwork_head;
	while (p != NULL) {//写work
		fprintf(file, "%d|%s|%s|%s|%s|%s|%d|%.1f|%.1f|%.1f|%.1f|%.1f|%.2f\n",
			p->data.id,
			p->data.title,
			p->data.author_name,
			p->data.student_id,
			p->data.major,
			p->data.grade,
			p->data.is_published,
			p->data.scores[0],
			p->data.scores[1],
			p->data.scores[2],
			p->data.scores[3],
			p->data.scores[4],
			p->data.avg_score);
		p = p->next;
	}
	if (ferror(file)) {//写入失败
		printf("[Error] Write error occurred.\n");
		fclose(file);
		return;
	}
	if (fflush(file) != 0) {//刷入硬盘失败
		printf("[Error] Flush failed.\n");
		fclose(file);
		return;
	}
	if (fclose(file) != 0) {//关闭失败
		printf("\n[Error] Close failed.\n");
		remove("worklist.tmp");
		return;
	}
	remove("worklist.txt");//写文件成功，把tem变成txt
	if (rename("worklist.tmp", "worklist.txt") != 0) {
		printf("\n[Error] Replace failed. Original may be lost.\n");
		remove("worklist.tmp");
		return;
	}
	printf("\n[Success] %d work(s) saved.\n", WORK_SUM);
	return;
}
/*void sort_IDorAver(Node* head, int tem_SUM) {
	Node* mid = head;
	for (int i = 0;i < tem_SUM / 2;++i) mid = mid->next;
	mid->next = NULL;
	tem_SUM /= 2;
	if(head != NULL)
		sort_IDorAver(head, tem_SUM);
	
}
*/









