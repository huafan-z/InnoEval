#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* listwork_head = NULL;
int MAX_ID = 0;
int WORK_SUM = 0;
int UNPUBLISH = 0;
int PUBLISH = 0;

static void reverse_list(void) {
	Node* prev = NULL;
	Node* curr = listwork_head;
	Node* next = NULL;
	while (curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	listwork_head = prev;
}
int loadWorkFromFile(void){
	int line_no = 1;
	char line[512];//单行内容
	work tem_work;//临时work
	memset(&tem_work, 0, sizeof(work));
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
		int parsed = sscanf(line, "%d|%63[^|]|%31[^|]|%19[^|]|%63[^|]|%15[^|]|%d|%lf|%lf|%lf|%lf|%lf|%lf|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|",
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
			&tem_work.avg_score,
			tem_work.judge[0],
			tem_work.judge[1],
			tem_work.judge[2],
			tem_work.judge[3],
			tem_work.judge[4]);
		if (parsed == EOF) continue;
		if (parsed == 18) {
			for (int i = 0;i < 5;++i) {
				if (strcmp(tem_work.judge[i], "None") == 0)
					tem_work.judge[i][0] = '\0';
			}
			// 判断是否发布
			if (tem_work.is_published == 0)
				++UNPUBLISH;
			else ++PUBLISH;
			// 成功解析 18 个字段，存进链表
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
	reverse_list();
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
	++WORK_SUM;
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
int WorkToFile(void) {
	FILE* file = fopen("worklist.tmp", "w");//先存在tem里
	if (!file) return 1;
	fprintf(file, "MAX_ID = %d\n", MAX_ID);//写第一行
	Node* p = listwork_head;
	while (p != NULL) {//写work
		fprintf(file, "%d|%s|%s|%s|%s|%s|%d|%.1f|%.1f|%.1f|%.1f|%.1f|%.2f|%s|%s|%s|%s|%s|\n",
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
			p->data.avg_score,
			p->data.judge[0][0] ? p->data.judge[0] : "None",
			p->data.judge[1][0] ? p->data.judge[1] : "None",
			p->data.judge[2][0] ? p->data.judge[2] : "None",
			p->data.judge[3][0] ? p->data.judge[3] : "None",
			p->data.judge[4][0] ? p->data.judge[4] : "None");
		p = p->next;
	}
	if (ferror(file)) {//写入失败
		fclose(file);
		return 2;
	}
	if (fflush(file) != 0) {//刷入硬盘失败
		fclose(file);
		return 3;
	}
	if (fclose(file) != 0) {//关闭失败
		remove("worklist.tmp");
		return 4;
	}
	remove("worklist.txt");//写文件成功，把tem变成txt
	if (rename("worklist.tmp", "worklist.txt") != 0) {
		remove("worklist.tmp");
		return 5;
	}
	return 0;
}
// 比较函数：返回 1 表示 a 应该排在 b 后面（需要交换）
static int cmp_work(const work* a, const work* b, int method) {// method: 1=按ID升序, 2=按平均分降序
	if (method == 1) return a->id > b->id;
	else return a->avg_score < b->avg_score;
}
// ========== 第一组：冒泡排序（交换数据） ==========
void sort_bubble(int method) {
	if (!listwork_head || !listwork_head->next) return;
	int swapped;
	Node* ptr1;
	Node* lptr = NULL; // 标记已排好序的尾部
	do {
		swapped = 0;
		ptr1 = listwork_head;
		while (ptr1->next != lptr) {
			if (cmp_work(&ptr1->data, &ptr1->next->data, method)) {
				// 直接交换结构体数据（对于几十条数据性能足够）
				work temp = ptr1->data;
				ptr1->data = ptr1->next->data;
				ptr1->next->data = temp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}
// ========== 第二组：归并排序（交换指针） ==========
static Node* merge_list(Node* l1, Node* l2, int method) {// 合并两个有序链表
	Node dummy;
	Node* tail = &dummy;
	dummy.next = NULL;
	while (l1 && l2) {
		if (!cmp_work(&l1->data, &l2->data, method)) {
			tail->next = l1;
			l1 = l1->next;
		}
		else {
			tail->next = l2;
			l2 = l2->next;
		}
		tail = tail->next;
	}
	tail->next = l1 ? l1 : l2;
	return dummy.next;
}
static Node* merge_sort_rec(Node* head, int len, int method) {// 递归拆分
	if (len <= 1) {
		if (head) head->next = NULL;
		return head;
	}
	int left_len = len / 2;
	int right_len = len - left_len;

	Node* right = head;
	for (int i = 0; i < left_len; ++i) right = right->next;

	Node* left = merge_sort_rec(head, left_len, method);
	right = merge_sort_rec(right, right_len, method);

	return merge_list(left, right, method);
}
void sort_merge(int method) {// 归并排序对外接口

	int len = 0;
	for (Node* p = listwork_head; p != NULL; p = p->next) len++;
	listwork_head = merge_sort_rec(listwork_head, len, method);
}
