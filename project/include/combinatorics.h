#pragma once
#include "common.h"

// Số thứ tự tôpô của subset; trả -1 nếu subset > 20 môn
long long countTopoOrders(const Graph &g, const vector<int> &subset);

// Các tập môn học được trong một kỳ; tối đa 200000 tập (cắt bớt nếu nhiều hơn)
vector<vector<int>> feasibleSets(const Graph &g, const vector<bool> &done, int maxCredits);

// Các môn bị ảnh hưởng khi rớt failedCourse (không gồm chính nó); rỗng nếu chỉ số sai
vector<int> affectedCourses(const Graph &g, int failedCourse);