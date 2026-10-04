#include "../include/common.h"

int addCourse(Graph &g, const char *id, const char *name,
              int credits, int difficulty, int schoolSemester)
{
    int i = g.n;
    strcpy(g.courses[i].id, id);
    strcpy(g.courses[i].name, name);
    g.courses[i].credits = credits;
    g.courses[i].difficulty = difficulty;
    g.courses[i].schoolSemester = schoolSemester;
    g.n++;
    return i;
}

int main()
{
    Graph g;
    initGraph(g);

    int ktlt = addCourse(g, "KTLT", "Ky thuat lap trinh", 3, 3, 2);
    int ctdl = addCourse(g, "CTDL", "Cau truc du lieu", 3, 4, 3);
    int oop = addCourse(g, "OOP", "Lap trinh HDT", 3, 3, 3);
    int csdl = addCourse(g, "CSDL", "Co so du lieu", 4, 3, 4);

    addPrereq(g, ctdl, ktlt);
    addPrereq(g, oop, ktlt);
    addPrereq(g, csdl, ctdl);

    cout << "So mon: " << g.n << endl;
    cout << "So tien quyet cua CSDL: " << g.courses[csdl].preCount << endl;

    freeGraph(g);
}
// tạo đồ thị rỗng -> thêm 4 môn -> nối 3 mũi tên môn tiên quyết -> in ra con số để kiểm tra -> dọn dẹp