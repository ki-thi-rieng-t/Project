#include "graph.h"
#include "combinatorics.h"

int main()
{
    Graph g;
    if (!loadCourses("data/courses.csv", g))
    {
        cout << "Khong doc duoc CSV" << endl;
        return 1;
    }
    vector<bool> done(g.n, false);
    vector<vector<int>> f = feasibleSets(g, done, 25);
    cout << "So mon: " << g.n << endl;
    cout << "So tap: " << f.size() << endl;
}