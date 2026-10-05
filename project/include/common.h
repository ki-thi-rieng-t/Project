// QUY TẮC: không tự ý sửa file này. Cần đổi gì thì báo cả nhóm.
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <numeric>
#include <cstdio>
using namespace std;

struct Course
{
    string id, name;
    int credits;         // số tín chỉ
    int difficulty;      // 1..5
    int schoolSemester;  // kỳ trường xếp, đúng như CSV (từ 1)
    vector<int> prereqs; // CHỈ SỐ các môn tiên quyết
};

struct Graph
{
    int n = 0;
    vector<Course> courses;     // courses[i] là môn có chỉ số i (từ 0 theo dòng CSV)
    vector<vector<int>> adj;    // adj[u] = các môn được mở ra sau khi học u
    map<string, int> idToIndex; // "DASA230179" -> chỉ số
};

// Plan[k] = danh sách chỉ số các môn học ở kỳ k (k bắt đầu từ 0)
using Plan = vector<vector<int>>;