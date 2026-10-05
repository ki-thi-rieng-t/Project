#pragma once
#include "common.h"

vector<int> courseLevels(const Graph& g);    // tầng (từ 0) của từng môn; rỗng nếu có chu trình
int minSemestersByPrereq(const Graph& g);    // số kỳ tối thiểu theo tiên quyết; 0 nếu rỗng; -1 nếu có chu trình
Plan buildSemesters(const Graph& g, int maxCredits); // Plan rỗng nếu lỗi (chu trình, môn vượt trần tín chỉ)
