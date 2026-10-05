#pragma once
#include "common.h"

// Đọc courses.csv (cột: id,name,credits,difficulty,prereqs,school_semester;
// các tiên quyết ngăn cách bằng ';'). Trả false nếu lỗi (không mở được file,
// thiếu cột, số không hợp lệ, trùng mã, tiên quyết không tồn tại).
// schoolSemester giữ ĐÚNG như trong CSV (từ 1). Khi lỗi, g được trả về rỗng.
bool loadCourses(const string& path, Graph& g);

// Trả true nếu có chu trình; cycleNodes là các môn nằm trong một chu trình
// theo thứ tự mũi tên u -> v (mũi tên cuối quay về môn đầu).
bool hasCycle(const Graph& g, vector<int>& cycleNodes);

vector<int> topoKahn(const Graph& g);   // thứ tự tôpô; rỗng nếu có chu trình
vector<int> topoDFS(const Graph& g);    // thứ tự tôpô; rỗng nếu có chu trình
