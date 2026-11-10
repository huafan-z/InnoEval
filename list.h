#pragma once

typedef struct {
    int id;
    char title[64];
    char author_name[32];
    char student_id[20];
    char major[64];
    char grade[16];
    
    int is_published;
    double scores[5];
    double avg_score;
} work;




typedef struct Node {
    work data;
    struct Node* next;
} Node;




void loadWorkFromFile(void);


void addNode(work* new_work);
Node** find_id(int id);
void delete_Node(Node** link);
void print_work(work* w);



int find_by_id(int id);// 1. ID 查找(返回一条)
int find_by_title(const char* title);// 2. 作品名查找 (重名 可能多条)
int find_by_author(const char* author_name);// 3. 按作者名查 (同作者 可能多条)
int find_by_student_id(const char* stu_id);// 4. 按学号查 (同作者 可能多条)
int find_by_keyword(const char* keyword);// 5. 按关键词查 (模糊匹配，多条) -> 核心用到 strstr












