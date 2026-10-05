#include "graph.h"
#include <fstream>
#include <sstream>

// Bỏ khoảng trắng và ký tự xuống dòng ở hai đầu một chuỗi
static string clean(string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\r' || s.back() == '\t'))
        s.pop_back();
    int i = 0;
    while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t'))
        i++;
    return s.substr(i);
}

// Chuỗi có toàn chữ số không? (kiểm tra tín chỉ, độ khó, kỳ)
static bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

// Gặp lỗi: xóa sạch đồ thị rồi báo false
static bool fail(Graph& g) {
    g = Graph();
    return false;
}

// Đọc HAI LƯỢT vì tiên quyết ghi bằng mã môn, mà môn tiên quyết có thể nằm ở dòng phía sau.
bool loadCourses(const string& path, Graph& g) {
    ifstream file(path);
    if (!file) return false;           // không mở được file
    g = Graph();                       // bắt đầu từ đồ thị rỗng

    // ----- Bước 1: đọc mọi dòng thành bảng chữ -----
    // rows[i] = { id, name, credits, difficulty, prereqs, school_semester }
    vector<vector<string>> rows;
    string line;
    getline(file, line);               // bỏ dòng tiêu đề
    while (getline(file, line)) {
        if (clean(line).empty()) continue;      // bỏ dòng trống

        vector<string> fields;
        stringstream ss(line);
        string field;
        while (getline(ss, field, ','))
            fields.push_back(clean(field));
        if (fields.size() < 6) return fail(g);  // thiếu cột
        rows.push_back(fields);
    }

    // ----- Bước 2 (lượt 1): tạo các môn, gán chỉ số theo thứ tự dòng -----
    for (int i = 0; i < (int)rows.size(); i++) {
        if (!isNumber(rows[i][2]) || !isNumber(rows[i][3]) || !isNumber(rows[i][5]))
            return fail(g);            // tín chỉ / độ khó / kỳ không phải số
        if (g.idToIndex.count(rows[i][0]) > 0)
            return fail(g);            // trùng mã môn

        Course c;
        c.id = rows[i][0];
        c.name = rows[i][1];
        c.credits = atoi(rows[i][2].c_str());
        c.difficulty = atoi(rows[i][3].c_str());
        c.schoolSemester = atoi(rows[i][5].c_str());   // giữ đúng như CSV (từ 1)
        g.idToIndex[c.id] = i;
        g.courses.push_back(c);
    }
    g.n = (int)rows.size();
    g.adj.assign(g.n, vector<int>());

    // ----- Bước 3 (lượt 2): đổi mã tiên quyết thành chỉ số, nối mũi tên -----
    for (int i = 0; i < g.n; i++) {
        stringstream ss(rows[i][4]);   // ví dụ "A1;B2"
        string part;
        while (getline(ss, part, ';')) {
            part = clean(part);
            if (part.empty()) continue;

            if (g.idToIndex.count(part) == 0)
                return fail(g);        // tiên quyết không tồn tại
            int pre = g.idToIndex[part];

            bool existed = false;      // khai báo trùng thì bỏ qua
            for (int x : g.courses[i].prereqs)
                if (x == pre) existed = true;
            if (!existed) {
                g.courses[i].prereqs.push_back(pre);   // i cần pre
                g.adj[pre].push_back(i);               // học pre thì mở ra i
            }
        }
    }
    return true;
}
