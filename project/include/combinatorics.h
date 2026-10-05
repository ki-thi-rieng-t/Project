#pragma once
#include "common.h"

long long countTopoOrders(const Graph &g, const vector<int> &subset);
vector<vector<int>> feasibleSets(const Graph &g, const vector<bool> &done, int maxCredits);
vector<int> affectedCourses(const Graph &g, int failedCourse);