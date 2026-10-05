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
    string id;
    string name;
    int credits = 0;
    int difficulty = 1;
    int schoolSemester = 1;
    vector<int> prereqs;

    bool isCalculation = false;
};

struct Graph
{
    int n = 0;
    vector<Course> courses;
    vector<vector<int>> adj;
    map<string, int> idToIndex;
};

using Plan = vector<vector<int>>;

struct Schedule
{
    vector<int> courseIndices;

    bool hasConflict = false;
    bool prerequisitesSatisfied = true;

    bool hasFriday = false;
    bool hasPeriod1 = false;

    int softScore = 0;
    int difficultyScore = 0;
    int hardSubjectCount = 0;
    int finalScore = 0;
};