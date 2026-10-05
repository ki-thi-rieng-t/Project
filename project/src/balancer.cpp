#include "../include/balancer.h"
#include "../include/logic.h"

using namespace std;

int semesterCredits(const Graph& g, const Plan& plan, int s) {
    if (s < 0 || s >= (int)plan.size()) return 0;

    int total = 0;
    for (int v : plan[s])
        if (v >= 0 && v < g.n)
            total += g.courses[v].credits;

    return total;
}

int semesterDifficulty(const Graph& g, const Plan& plan, int s) {
    if (s < 0 || s >= (int)plan.size()) return 0;

    int total = 0;
    for (int v : plan[s])
        if (v >= 0 && v < g.n)
            total += g.courses[v].difficulty;

    return total;
}

Plan balanceLoad(const Graph& g, const Plan& plan,
                 int maxCredits, int maxDifficulty) {
    if (plan.empty()) return {};

    int semesterCount = plan.size();
    vector<int> courses;
    vector<bool> exists(g.n, false);

    for (const auto& semester : plan) {
        for (int v : semester) {
            if (v >= 0 && v < g.n && !exists[v]) {
                exists[v] = true;
                courses.push_back(v);
            }
        }
    }

    sort(courses.begin(), courses.end(), [&](int a, int b) {
        int pa = g.courses[a].prereqs.size();
        int pb = g.courses[b].prereqs.size();

        if (pa != pb) return pa < pb;
        return a < b;
    });

    Plan result(semesterCount);
    vector<int> semesterOf(g.n, -1);
    vector<bool> placed(g.n, false);

    for (int v : courses) {
        int earliest = 0;
        bool ok = true;

        for (int pre : g.courses[v].prereqs) {
            if (pre < 0 || pre >= g.n || semesterOf[pre] == -1) {
                ok = false;
                break;
            }
            earliest = max(earliest, semesterOf[pre] + 1);
        }

        if (!ok) continue;

        int best = -1;
        int bestDiff = INT_MAX;
        int bestCredits = INT_MAX;

        for (int s = earliest; s < semesterCount; s++) {
            int credits = semesterCredits(g, result, s);
            int diff = semesterDifficulty(g, result, s);

            int newCredits = credits + g.courses[v].credits;
            int newDiff = diff + g.courses[v].difficulty;

            if (newCredits > maxCredits ||
                newDiff > maxDifficulty)
                continue;

            if (newDiff < bestDiff ||
                (newDiff == bestDiff && newCredits < bestCredits)) {
                best = s;
                bestDiff = newDiff;
                bestCredits = newCredits;
            }
        }

        if (best == -1) {
            int lowestDiff = INT_MAX;

            for (int s = earliest; s < semesterCount; s++) {
                int credits = semesterCredits(g, result, s);
                if (credits + g.courses[v].credits > maxCredits)
                    continue;

                int diff = semesterDifficulty(g, result, s);

                if (diff < lowestDiff) {
                    lowestDiff = diff;
                    best = s;
                }
            }
        }

        if (best == -1)
            best = semesterCount - 1;

        result[best].push_back(v);
        placed[v] = true;
        semesterOf[v] = best;
    }

    bool changed = true;

    while (changed) {
        changed = false;

        for (int v : courses) {
            if (placed[v]) continue;

            int earliest = 0;
            bool ok = true;

            for (int pre : g.courses[v].prereqs) {
                if (pre < 0 || pre >= g.n ||
                    semesterOf[pre] == -1) {
                    ok = false;
                    break;
                }

                earliest = max(earliest, semesterOf[pre] + 1);
            }

            if (!ok) continue;

            int best = -1;
            int bestDiff = INT_MAX;

            for (int s = earliest; s < semesterCount; s++) {
                int credits = semesterCredits(g, result, s);
                int diff = semesterDifficulty(g, result, s);

                if (credits + g.courses[v].credits > maxCredits)
                    continue;

                if (diff + g.courses[v].difficulty > maxDifficulty)
                    continue;

                if (diff < bestDiff) {
                    bestDiff = diff;
                    best = s;
                }
            }

            if (best != -1) {
                result[best].push_back(v);
                placed[v] = true;
                semesterOf[v] = best;
                changed = true;
            }
        }
    }

    return result;
}

vector<Plan> topPlans(const Graph& g, int maxCredits,
                      int maxDifficulty, int k) {
    vector<Plan> result;

    if (k <= 0 || g.n == 0)
        return result;

    int semesterCount = 0;

    for (const Course& c : g.courses)
        semesterCount = max(semesterCount, c.schoolSemester);

    if (semesterCount <= 0)
        semesterCount = 1;

    Plan basePlan(semesterCount);

    for (int i = 0; i < g.n; i++) {
        int s = g.courses[i].schoolSemester - 1;

        if (s < 0) s = 0;
        if (s >= semesterCount) s = semesterCount - 1;

        basePlan[s].push_back(i);
    }

    result.push_back(
        balanceLoad(g, basePlan, maxCredits, maxDifficulty)
    );

    for (int variant = 1; variant < k; variant++) {
        Plan candidate = basePlan;

        for (auto& semester : candidate) {
            if (semester.empty()) continue;

            if (variant % 2 == 1) {
                reverse(semester.begin(), semester.end());
            } else {
                rotate(semester.begin(),
                       semester.begin() + 1,
                       semester.end());
            }
        }

        result.push_back(
            balanceLoad(g, candidate, maxCredits, maxDifficulty)
        );
    }

    if ((int)result.size() > k)
        result.resize(k);

    return result;
}