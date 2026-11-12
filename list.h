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




int loadWorkFromFile(void);


void addNode(work* new_work);
Node** find_id(int id);
void delete_Node(Node** link);



Node* find_by_title(Node* p, const char* title);// 2. 作品名查找 (重名 可能多条)
Node* find_by_author(Node* p, const char* title);// 3. 按作者名查 (同作者 可能多条)
Node* find_by_student_id(Node* p, const char* title);// 4. 按学号查 (同作者 可能多条)
Node* find_by_keyword(Node* p, const char* title);// 5. 按关键词查 (模糊匹配，多条) -> 核心用到 strstr












