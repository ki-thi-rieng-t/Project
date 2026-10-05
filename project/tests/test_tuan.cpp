#include "../include/combinatorics.h"

int addCourse(Graph &g, const char *id, const char *name,
              int credits, int difficulty, int schoolSemester)
{
    Course c;
    c.id = id;
    c.name = name;
    c.credits = credits;
    c.difficulty = difficulty;
    c.schoolSemester = schoolSemester;
    g.courses.push_back(c);
    g.adj.push_back({});
    g.n++;
    return g.n - 1;
}

// pre là tiên quyết của v
void addPrereq(Graph &g, int v, int pre)
{
    g.courses[v].prereqs.push_back(pre);
    g.adj[pre].push_back(v);
}

int main()
{
    Graph g;

    int ktlt = addCourse(g, "KTLT", "Ky thuat lap trinh", 3, 3, 2);
    int ctdl = addCourse(g, "CTDL", "Cau truc du lieu", 3, 4, 3);
    int oop = addCourse(g, "OOP", "Lap trinh HDT", 3, 3, 3);
    int csdl = addCourse(g, "CSDL", "Co so du lieu", 4, 3, 4);

    addPrereq(g, ctdl, ktlt);
    addPrereq(g, oop, ktlt);
    addPrereq(g, csdl, ctdl);

    cout << "So mon: " << g.n << endl;
    cout << "So tien quyet cua CSDL: " << g.courses[csdl].prereqs.size() << endl;

    vector<int> res = affectedCourses(g, ctdl);
    cout << "So mon bi anh huong: " << res.size() << endl;

    vector<int> rong;
    cout << "Subset rong: " << countTopoOrders(g, rong) << endl;

    vector<int> s = {1, 3};
    cout << "{CTDL, CSDL}: " << countTopoOrders(g, s) << endl;

    vector<int> all = {0, 1, 2, 3};
    cout << "Ca 4 mon: " << countTopoOrders(g, all) << endl;

    Graph g2;
    for (int i = 0; i < 4; i++)
        addCourse(g2, "X", "X", 1, 1, 1);
    vector<int> all2 = {0, 1, 2, 3};
    cout << "4 mon roi nhau: " << countTopoOrders(g2, all2) << endl;

    vector<int> res2 = affectedCourses(g, ktlt);
    cout << "Rot KTLT: " << res2.size() << " mon:";
    for (int v : res2)
        cout << " " << g.courses[v].id;
    cout << endl;

    vector<bool> done(4, false);
    vector<vector<int>> a = feasibleSets(g, done, 10);
    cout << "Chua hoc gi, toi da 10 TC: " << a.size() << " nhom (mong doi 1)" << endl;

    done[ktlt] = true;
    vector<vector<int>> b = feasibleSets(g, done, 10);
    cout << "Da hoc KTLT, toi da 10 TC: " << b.size() << " nhom (mong doi 3)" << endl;

    vector<vector<int>> c = feasibleSets(g, done, 3);
    cout << "Da hoc KTLT, toi da 3 TC: " << c.size() << " nhom (mong doi 2)" << endl;

    Graph g3;
    for (int i = 0; i < 21; i++)
        addCourse(g3, "Y", "Y", 1, 1, 1);
    vector<int> big;
    for (int i = 0; i < 21; i++)
        big.push_back(i);
    cout << "21 mon: " << countTopoOrders(g3, big) << " (mong doi -1)" << endl;
}