// test_ghep.cpp - Ghép code của Tường (đọc CSV, tôpô, chia kỳ) với code của Tuấn (tổ hợp)
// Mỗi dòng in ra: [OK] hoặc [SAI] để nhìn là biết.
#include "../include/graph.h"
#include "../include/scheduler.h"
#include "../include/combinatorics.h"
#include <fstream>

static int tong = 0, loi = 0;

static void check(const string &ten, bool dung)
{
    tong++;
    if (!dung)
        loi++;
    cout << (dung ? "[OK]  " : "[SAI] ") << ten << endl;
}

// Thứ tự order có hợp lệ không? (đủ n môn, không lặp, mọi tiên quyết đứng trước)
static bool orderHopLe(const Graph &g, const vector<int> &order)
{
    if ((int)order.size() != g.n)
        return false;
    vector<int> pos(g.n, -1);
    for (int i = 0; i < g.n; i++)
    {
        if (order[i] < 0 || order[i] >= g.n || pos[order[i]] != -1)
            return false;
        pos[order[i]] = i;
    }
    for (int v = 0; v < g.n; v++)
        for (int p : g.courses[v].prereqs)
            if (pos[p] >= pos[v])
                return false;
    return true;
}

// Kế hoạch có hợp lệ không? (mỗi môn đúng 1 lần, tiên quyết ở kỳ trước, tín chỉ <= maxC)
static bool planHopLe(const Graph &g, const Plan &plan, int maxC)
{
    vector<int> ky(g.n, -1);
    for (int k = 0; k < (int)plan.size(); k++)
    {
        int tc = 0;
        for (int v : plan[k])
        {
            if (ky[v] != -1)
                return false; // một môn xuất hiện hai lần
            ky[v] = k;
            tc += g.courses[v].credits;
        }
        if (tc > maxC)
            return false;
    }
    for (int v = 0; v < g.n; v++)
    {
        if (ky[v] == -1)
            return false; // thiếu môn
        for (int p : g.courses[v].prereqs)
            if (ky[p] >= ky[v])
                return false;
    }
    return true;
}

int main()
{
    // ===== A. Đọc CSV (Tường) =====
    cout << "--- A. Doc CSV ---" << endl;
    Graph g;
    bool docDuoc = loadCourses("data/courses.csv", g);
    check("loadCourses doc duoc file", docDuoc);
    if (!docDuoc)
    {
        cout << "Khong doc duoc data/courses.csv (chay tu thu muc project?)" << endl;
        return 1;
    }
    check("So mon = 50", g.n == 50);
    int dasa = g.idToIndex["DASA230179"];
    int prte = g.idToIndex["PRTE230385"];
    check("CTDL&GT co dung 1 tien quyet", g.courses[dasa].prereqs.size() == 1);
    check("Tien quyet cua CTDL&GT la Ky thuat lap trinh", g.courses[dasa].prereqs[0] == prte);

    Graph rong;
    check("File khong ton tai -> false", !loadCourses("data/khong_co.csv", rong));

    // ===== B. Chu trình và sắp xếp tôpô (Tường) =====
    cout << "--- B. Chu trinh va topo ---" << endl;
    vector<int> cyc;
    check("Du lieu that khong co chu trinh", !hasCycle(g, cyc));
    check("topoKahn hop le", orderHopLe(g, topoKahn(g)));
    check("topoDFS hop le", orderHopLe(g, topoDFS(g)));

    Graph gx = g; // sao chép đồ thị rồi tạo chu trình: DASA -> ... -> PRTE -> DASA
    gx.courses[prte].prereqs.push_back(dasa);
    gx.adj[dasa].push_back(prte);
    check("Them canh loi -> hasCycle = true", hasCycle(gx, cyc));
    check("Co chu trinh -> topoKahn rong", topoKahn(gx).empty());
    check("Co chu trinh -> topoDFS rong", topoDFS(gx).empty());
    check("Co chu trinh -> minSemestersByPrereq = -1", minSemestersByPrereq(gx) == -1);

    // ===== C. Đường găng và chia kỳ (Tường) =====
    cout << "--- C. Duong gang va chia ky ---" << endl;
    int minSem = minSemestersByPrereq(g);
    cout << "      so ky toi thieu theo tien quyet = " << minSem << endl;
    check("minSemestersByPrereq = 4", minSem == 4);

    Plan p28 = buildSemesters(g, 28);
    cout << "      so ky khi toi da 28 TC = " << p28.size() << endl;
    check("buildSemesters(28) hop le", planHopLe(g, p28, 28));
    check("So ky >= 6 (150 TC / 28)", p28.size() >= 6);
    Plan p22 = buildSemesters(g, 22);
    cout << "      so ky khi toi da 22 TC = " << p22.size() << endl;
    check("buildSemesters(22) hop le", planHopLe(g, p22, 22));
    check("Tran 2 TC -> Plan rong", buildSemesters(g, 2).empty());

    // ===== D. Tổ hợp (Tuấn) trên đồ thị thật =====
    cout << "--- D. To hop (Tuan) ---" << endl;
    vector<int> bi = affectedCourses(g, dasa);
    cout << "      rot CTDL&GT keo theo:";
    for (int v : bi)
        cout << " " << g.courses[v].id;
    cout << endl;
    check("Rot CTDL&GT -> 8 mon bi anh huong", bi.size() == 8);

    vector<int> sub = {prte, dasa, g.idToIndex["DBSY240184"], g.idToIndex["OOPR230279"]};
    check("countTopoOrders 4 mon (KTLT, CTDL, CSDL, OOP) = 3", countTopoOrders(g, sub) == 3);

    // done = mọi môn ở tầng 0, 1, 2 -> chỉ còn 6 môn tầng 3 đủ điều kiện
    vector<int> level = courseLevels(g);
    vector<bool> done(g.n, false);
    for (int v = 0; v < g.n; v++)
        done[v] = (level[v] <= 2);
    check("feasibleSets (toi da 9 TC) = 41 nhom", feasibleSets(g, done, 9).size() == 41);
    check("feasibleSets (toi da 28 TC) = 63 nhom", feasibleSets(g, done, 28).size() == 63);

    cout << "=== " << (tong - loi) << "/" << tong << " dung ===" << endl;
    return loi == 0 ? 0 : 1;
}
