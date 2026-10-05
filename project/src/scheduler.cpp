#include "scheduler.h"
#include "graph.h"

// =====================================================================
// courseLevels: tính "tầng" của từng môn
//   - Môn không cần môn nào trước: tầng 0
//   - Môn khác: tầng = (tầng lớn nhất trong các môn tiên quyết) + 1
// Đi theo thứ tự tôpô nên khi tới môn v, mọi tiên quyết của v đã có tầng đúng.
// =====================================================================
vector<int> courseLevels(const Graph& g) {
    vector<int> order = topoKahn(g);
    if ((int)order.size() != g.n || g.n == 0) return vector<int>();   // chu trình hoặc rỗng

    vector<int> level(g.n, 0);
    for (int v : order) {
        for (int u : g.courses[v].prereqs)      // u là tiên quyết của v
            if (level[u] + 1 > level[v])
                level[v] = level[u] + 1;
    }
    return level;
}

// Số kỳ tối thiểu = tầng lớn nhất + 1 (đường găng)
int minSemestersByPrereq(const Graph& g) {
    if (g.n == 0) return 0;
    vector<int> level = courseLevels(g);
    if (level.empty()) return -1;               // có chu trình

    int maxLevel = 0;
    for (int x : level)
        if (x > maxLevel) maxLevel = x;
    return maxLevel + 1;
}

// =====================================================================
// buildSemesters: chia các môn vào từng học kỳ
// Mỗi kỳ làm như sau:
//   1. Tìm các môn "sẵn sàng": mọi tiên quyết đã học xong ở các kỳ TRƯỚC.
//   2. Xếp chúng theo độ ưu tiên (môn mở ra chuỗi dài hơn thì học trước).
//   3. Bỏ vào kỳ lần lượt, miễn là chưa vượt số tín chỉ tối đa.
//   4. Kết thúc kỳ mới đánh dấu các môn đó là "đã học".
// =====================================================================
static vector<int> height;  // height[i] = chuỗi môn phụ thuộc dài nhất phía sau môn i

// Môn a có được học trước môn b không?
static bool goesFirst(int a, int b) {
    if (height[a] != height[b]) return height[a] > height[b];
    return a < b;           // bằng nhau thì môn có chỉ số nhỏ hơn đi trước
}

Plan buildSemesters(const Graph& g, int maxCredits) {
    Plan plan;
    if (g.n == 0) return plan;

    vector<int> order = topoKahn(g);
    if ((int)order.size() != g.n) return Plan();    // có chu trình

    for (int i = 0; i < g.n; i++)
        if (g.courses[i].credits > maxCredits) return Plan();   // một môn đã vượt trần

    // Tính height: đi từ cuối thứ tự tôpô về đầu
    height.assign(g.n, 0);
    for (int i = g.n - 1; i >= 0; i--) {
        int u = order[i];
        for (int v : g.adj[u])
            if (height[v] + 1 > height[u])
                height[u] = height[v] + 1;
    }

    vector<bool> done(g.n, false);      // done[i] = môn i đã được xếp chưa
    int remaining = g.n;                // số môn còn chưa xếp

    while (remaining > 0) {
        // Bước 1: tìm các môn sẵn sàng
        vector<int> ready;
        for (int v = 0; v < g.n; v++) {
            if (done[v]) continue;
            bool ok = true;
            for (int u : g.courses[v].prereqs)
                if (!done[u]) ok = false;           // còn tiên quyết chưa học
            if (ok) ready.push_back(v);
        }

        // Bước 2: sắp xếp theo độ ưu tiên (chọn dần phần tử đứng đầu)
        for (int i = 0; i < (int)ready.size(); i++)
            for (int j = i + 1; j < (int)ready.size(); j++)
                if (goesFirst(ready[j], ready[i]))
                    swap(ready[i], ready[j]);

        // Bước 3: bỏ vào kỳ cho đến khi đầy
        vector<int> semester;
        int credits = 0;
        for (int v : ready) {
            if (credits + g.courses[v].credits <= maxCredits) {
                semester.push_back(v);
                credits += g.courses[v].credits;
            }
        }

        if (semester.empty()) return Plan();        // không xếp được môn nào => dừng, tránh lặp vô hạn

        // Bước 4: đánh dấu đã học (làm SAU khi chọn xong cả kỳ)
        for (int v : semester) done[v] = true;
        remaining -= semester.size();
        plan.push_back(semester);
    }
    return plan;
}
