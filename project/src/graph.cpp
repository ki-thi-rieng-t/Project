#include "graph.h"

// (static = chỉ dùng trong file này, để không đụng tên với file của bạn khác)

// =====================================================================
// 1. hasCycle: tìm chu trình
// Ý tưởng: đi sâu (DFS) và tô màu từng môn
//   0 = chưa đi tới
//   1 = đang đi (môn nằm trên đường đang đi)
//   2 = đã đi xong
// Nếu từ môn u đi tới một môn màu 1 thì ta đã đi vòng về chỗ cũ => có chu trình.
// =====================================================================
static vector<int> color;
static vector<int> path;        // các môn trên đường đang đi
static vector<int> cycleList;   // chu trình tìm được
static bool found;

static void dfsCycle(const Graph& g, int u) {
    color[u] = 1;
    path.push_back(u);

    for (int v : g.adj[u]) {
        if (found) return;

        if (color[v] == 1) {
            // v đang nằm trên đường đi => chu trình là từ v đến cuối đường
            int start = 0;
            while (path[start] != v) start++;
            for (int i = start; i < (int)path.size(); i++)
                cycleList.push_back(path[i]);
            found = true;
            return;
        }
        if (color[v] == 0) dfsCycle(g, v);
    }

    path.pop_back();    // đi xong u, lùi lại một bước
    color[u] = 2;
}

bool hasCycle(const Graph& g, vector<int>& cycleNodes) {
    color.assign(g.n, 0);
    path.clear();
    cycleList.clear();
    found = false;

    for (int i = 0; i < g.n && !found; i++)
        if (color[i] == 0) dfsCycle(g, i);

    cycleNodes = cycleList;
    return found;
}

// =====================================================================
// 2. topoKahn: sắp xếp tôpô kiểu "gỡ dần"
// Ý tưởng:
//   - Môn nào không cần môn nào học trước (bậc vào = 0) thì xếp được ngay.
//   - Xếp xong một môn, coi như đã học: các môn sau nó bớt đi 1 tiên quyết.
//   - Môn nào hết tiên quyết thì được xếp tiếp. Lặp lại cho đến hết.
//   - Nếu cuối cùng còn môn chưa xếp được => có chu trình.
// =====================================================================
vector<int> topoKahn(const Graph& g) {
    vector<int> indeg(g.n);         // indeg[i] = số tiên quyết của môn i chưa học
    queue<int> ready;               // hàng đợi: các môn đã sẵn sàng để xếp
    vector<int> order;              // thứ tự kết quả

    for (int i = 0; i < g.n; i++) {
        indeg[i] = g.courses[i].prereqs.size();
        if (indeg[i] == 0) ready.push(i);
    }

    while (!ready.empty()) {
        int u = ready.front();      // lấy một môn ra
        ready.pop();
        order.push_back(u);         // xếp nó vào thứ tự

        for (int v : g.adj[u]) {    // v là môn được mở ra sau u
            indeg[v]--;             // v bớt một tiên quyết
            if (indeg[v] == 0)      // hết tiên quyết => sẵn sàng
                ready.push(v);
        }
    }

    if ((int)order.size() != g.n) return vector<int>();   // còn môn kẹt => có chu trình
    return order;
}

// =====================================================================
// 3. topoDFS: sắp xếp tôpô kiểu "đi sâu"
// Ý tưởng: đi sâu hết các môn phụ thuộc vào u trước, rồi mới ghi u.
// Ghi xong thì đảo ngược danh sách: môn ghi sau cùng sẽ đứng đầu.
// =====================================================================
static vector<bool> seen;
static vector<int> finish;          // các môn theo thứ tự "đi xong"

static void dfsTopo(const Graph& g, int u) {
    seen[u] = true;
    for (int v : g.adj[u])
        if (!seen[v]) dfsTopo(g, v);
    finish.push_back(u);            // mọi môn phụ thuộc u đã được ghi rồi
}

vector<int> topoDFS(const Graph& g) {
    vector<int> cycle;
    if (hasCycle(g, cycle)) return vector<int>();

    seen.assign(g.n, false);
    finish.clear();
    for (int i = 0; i < g.n; i++)
        if (!seen[i]) dfsTopo(g, i);

    reverse(finish.begin(), finish.end());
    return finish;
}
