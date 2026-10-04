// QUY TẮC: không tự ý sửa file này. Cần đổi gì thì báo cả nhóm.
#pragma once
#include <iostream>
#include <cstring>
using namespace std;

const int MAXN = 60;   // số môn tối đa
const int MAXSEM = 12; // số kỳ tối đa trong một kế hoạch
const int MAXPER = 12; // số môn tối đa trong một kỳ

// Một mũi tên (hoặc một phần tử) trong danh sách liên kết
struct EdgeNode
{
    int to;         // chỉ số môn
    EdgeNode *next; // nút kế tiếp, nullptr nếu là nút cuối
};

struct Course
{
    char id[20];        // mã môn, ví dụ "DASA230179"
    char name[100];     // tên môn
    int credits;        // số tín chỉ
    int difficulty;     // độ khó 1..5
    int schoolSemester; // kỳ mà trường xếp (để so sánh)
    EdgeNode *prereqs;  // danh sách các môn TIÊN QUYẾT của môn này
    int preCount;       // số môn tiên quyết
};

struct Graph
{
    int n;                // số môn
    Course courses[MAXN]; // courses[i] là môn có chỉ số i (đánh số từ 0 theo dòng CSV)
    EdgeNode *adj[MAXN];  // adj[u] = danh sách các môn được MỞ RA sau khi học u
};

// Kế hoạch học: kỳ k (k bắt đầu từ 0) gồm count[k] môn, là course[k][0..count[k]-1]
struct Plan
{
    int numSem;
    int count[MAXSEM];
    int course[MAXSEM][MAXPER];
};

// ---------- Các hàm tiện ích dùng chung ----------

// Thêm một nút vào đầu danh sách
inline void addEdge(EdgeNode *&head, int to)
{
    EdgeNode *e = new EdgeNode;
    e->to = to;
    e->next = head;
    head = e;
}

// Giải phóng một danh sách liên kết
inline void freeList(EdgeNode *&head)
{
    while (head != nullptr)
    {
        EdgeNode *tmp = head;
        head = head->next;
        delete tmp;
    }
}

// Khởi tạo đồ thị rỗng (gọi trước khi dùng)
inline void initGraph(Graph &g)
{
    g.n = 0;
    for (int i = 0; i < MAXN; i++)
    {
        g.adj[i] = nullptr;
        g.courses[i].prereqs = nullptr;
        g.courses[i].preCount = 0;
    }
}

// Giải phóng toàn bộ bộ nhớ của đồ thị (gọi khi kết thúc)
inline void freeGraph(Graph &g)
{
    for (int i = 0; i < g.n; i++)
    {
        freeList(g.adj[i]);
        freeList(g.courses[i].prereqs);
        g.courses[i].preCount = 0;
    }
    g.n = 0;
}

// Thêm điều kiện "môn pre là tiên quyết của môn v": cập nhật cả prereqs và adj
inline void addPrereq(Graph &g, int v, int pre)
{
    addEdge(g.courses[v].prereqs, pre);
    g.courses[v].preCount++;
    addEdge(g.adj[pre], v);
}

// Tìm chỉ số môn theo mã, trả -1 nếu không có
inline int findCourse(const Graph &g, const char *id)
{
    for (int i = 0; i < g.n; i++)
        if (strcmp(g.courses[i].id, id) == 0)
            return i;
    return -1;
}

inline void initPlan(Plan &p)
{
    p.numSem = 0;
    for (int k = 0; k < MAXSEM; k++)
        p.count[k] = 0;
}

// LƯU Ý: không gán/sao chép Graph (g2 = g) vì sẽ sao chép con trỏ, không sao chép danh sách.
// Luôn truyền Graph bằng tham chiếu: const Graph& g.