#include "combinatorics.h"

long long countTopoOrders(const Graph &g, const vector<int> &subset)
{
    int m = subset.size();
    if (m == 0)
        return 1;
    if (m > 20)
        return -1;

    vector<int> pos(g.n, -1);
    for (int i = 0; i < m; i++)
        pos[subset[i]] = i;

    vector<int> need(m, 0);
    for (int i = 0; i < m; i++)
        for (int p : g.courses[subset[i]].prereqs)
            if (pos[p] != -1)
                need[i] |= (1 << pos[p]);

    vector<long long> dp(1 << m, 0);
    dp[0] = 1;
    for (int mask = 0; mask < (1 << m); mask++)
    {
        if (dp[mask] == 0)
            continue;
        for (int i = 0; i < m; i++)
        {
            if (mask >> i & 1)
                continue;
            if ((need[i] & mask) == need[i])
                dp[mask | (1 << i)] += dp[mask];
        }
    }
    return dp[(1 << m) - 1];
}

static void gen(const Graph &g, const vector<int> &cand, int idx,
                int credits, int maxCredits,
                vector<int> &cur, vector<vector<int>> &out)
{
    if (!cur.empty())
        out.push_back(cur);
    for (int i = idx; i < (int)cand.size(); i++)
    {
        int c = g.courses[cand[i]].credits;
        if (credits + c > maxCredits) // nếu thêm môn này mà vượt maxCredits thì bỏ qua
            continue;
        cur.push_back(cand[i]);
        gen(g, cand, i + 1, credits + c, maxCredits, cur, out);
        cur.pop_back();
    }
}

vector<vector<int>> feasibleSets(const Graph &g, const vector<bool> &done, int maxCredits)
{
    vector<vector<int>> out;
    if ((int)done.size() != g.n)
        return out;

    vector<int> cand;
    for (int v = 0; v < g.n; v++)
    {
        if (done[v])
            continue;
        bool ok = true;
        for (int p : g.courses[v].prereqs)
        {
            if (!done[p])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            cand.push_back(v);
    }
    vector<int> cur;
    gen(g, cand, 0, 0, maxCredits, cur, out);
    return out;
}

vector<int> affectedCourses(const Graph &g, int failedCourse)
{
    vector<int> res;
    if (failedCourse < 0 || failedCourse >= g.n)
        return res;

    vector<bool> seen(g.n, false);
    queue<int> q;
    seen[failedCourse] = true;
    q.push(failedCourse);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : g.adj[u])
        {
            if (seen[v])
                continue;
            seen[v] = true;
            res.push_back(v);
            q.push(v);
        }
    }
    return res;
}