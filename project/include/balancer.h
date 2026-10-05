#pragma once

#include "common.h"

int semesterCredits(
    const Graph& g,
    const Plan& plan,
    int semester
);

int semesterDifficulty(
    const Graph& g,
    const Plan& plan,
    int semester
);

Plan balanceLoad(
    const Graph& g,
    const Plan& plan,
    int maxCredits,
    int maxDifficulty
);

vector<Plan> topPlans(
    const Graph& g,
    int maxCredits,
    int maxDifficulty,
    int k
);