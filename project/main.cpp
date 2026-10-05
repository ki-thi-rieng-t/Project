#include "include/common.h"
#include "include/graph.h"
#include "include/scheduler.h"
#include "include/logic.h"
#include "include/balancer.h"
#include "include/output.h"

using namespace std;

int main()
{
    Graph g;
    Plan plan;

    string path = "data/courses.csv";

    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "       COURSE STUDY PLANNER\n";
        cout << "========================================\n";
        cout << "1. Doc du lieu mon hoc\n";
        cout << "2. Kiem tra chu trinh\n";
        cout << "3. Sap xep topo bang Kahn\n";
        cout << "4. Sap xep topo bang DFS\n";
        cout << "5. Test cua Hai Anh\n";
        cout << "6. Xem cap do mon hoc\n";
        cout << "7. So hoc ky toi thieu\n";
        cout << "8. Tao ke hoach hoc tap\n";
        cout << "9. In ke hoach hoc tap\n";
        cout << "10. So sanh voi ke hoach truong\n";
        cout << "11. Can bang tai hoc tap\n";
        cout << "12. Xem cac ke hoach tot nhat\n";
        cout << "0. Thoat\n";
        cout << "----------------------------------------\n";
        cout << "Chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (loadCourses(path, g))
            {
                cout << "\n[OK] Doc du lieu thanh cong.\n";
                cout << "So mon hoc: " << g.n << '\n';

                plan.clear();
            }
            else
            {
                cout << "\n[SAI] Khong doc duoc courses.csv.\n";
            }

            break;
        }

        case 2:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            vector<int> cycleNodes;

            if (hasCycle(g, cycleNodes))
            {
                cout << "\n[SAI] Do thi co chu trinh.\n";

                cout << "Cac mon trong chu trinh:\n";

                for (int v : cycleNodes)
                {
                    if (v >= 0 && v < g.n)
                    {
                        cout << g.courses[v].id << " ";
                    }
                }

                cout << '\n';
            }
            else
            {
                cout << "\n[OK] Do thi khong co chu trinh.\n";
            }

            break;
        }

        case 3:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            vector<int> order = topoKahn(g);

            if (order.empty())
            {
                cout << "\n[SAI] Khong the sap xep topo.\n";
                break;
            }

            cout << "\nThu tu Topological Sort - Kahn:\n";

            for (int v : order)
            {
                cout << g.courses[v].id << " ";
            }

            cout << '\n';

            break;
        }

        case 4:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            vector<int> order = topoDFS(g);

            if (order.empty())
            {
                cout << "\n[SAI] Khong the sap xep topo.\n";
                break;
            }

            cout << "\nThu tu Topological Sort - DFS:\n";

            for (int v : order)
            {
                cout << g.courses[v].id << " ";
            }

            cout << '\n';

            break;
        }

        case 5:
        {
            cout << "\n========================================\n";
            cout << "          TEST CUA HAI ANH\n";
            cout << "========================================\n";

            if (g.n == 0)
            {
                cout << "[SAI] Chua doc courses.csv.\n";
                cout << "Hay chon 1 truoc.\n";
                break;
            }

            cout << "\n[TEST 1] Kiem tra du lieu\n";

            if (g.n > 0)
            {
                cout << "[OK] Co " << g.n
                     << " mon hoc.\n";
            }
            else
            {
                cout << "[SAI] Khong co mon hoc.\n";
            }

            cout << "\n[TEST 2] Tao ke hoach voi 28 TC\n";

            Plan testPlan = buildSemesters(g, 28);

            if (testPlan.empty())
            {
                cout << "[SAI] Khong tao duoc ke hoach.\n";
                break;
            }

            cout << "[OK] Tao ke hoach thanh cong.\n";
            cout << "So hoc ky: "
                 << testPlan.size() << '\n';

            cout << "\n[TEST 3] printPlan()\n";

            printPlan(g, testPlan);

            cout << "[OK] printPlan() hoat dong.\n";

            cout << "\n[TEST 4] comparePlanWithSchool()\n";

            comparePlanWithSchool(g, testPlan);

            cout << "[OK] comparePlanWithSchool() hoat dong.\n";

            cout << "\n[TEST 5] Kiem tra gioi han 28 TC\n";

            bool creditOk = true;

            for (int i = 0;
                 i < (int)testPlan.size();
                 i++)
            {
                int totalCredits = 0;

                for (int v : testPlan[i])
                {
                    if (v >= 0 && v < g.n)
                    {
                        totalCredits +=
                            g.courses[v].credits;
                    }
                }

                cout << "Hoc ky "
                     << i + 1
                     << ": "
                     << totalCredits
                     << " TC\n";

                if (totalCredits > 28)
                {
                    creditOk = false;
                }
            }

            if (creditOk)
            {
                cout << "[OK] Khong hoc ky nao "
                     << "vuot 28 TC.\n";
            }
            else
            {
                cout << "[SAI] Co hoc ky "
                     << "vuot 28 TC.\n";
            }

            cout << "\n========================================\n";
            cout << "       KET THUC TEST HAI ANH\n";
            cout << "========================================\n";

            break;
        }

        case 6:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            vector<int> levels = courseLevels(g);

            if ((int)levels.size() != g.n)
            {
                cout << "\n[SAI] Khong tinh duoc cap do.\n";
                break;
            }

            cout << "\n========== COURSE LEVELS ==========\n";

            for (int i = 0; i < g.n; i++)
            {
                cout << g.courses[i].id
                     << " - "
                     << g.courses[i].name
                     << " : Level "
                     << levels[i]
                     << '\n';
            }

            break;
        }

        case 7:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            int semesters =
                minSemestersByPrereq(g);

            if (semesters < 0)
            {
                cout << "\n[SAI] Do thi co chu trinh.\n";
                cout << "Khong the lap ke hoach hoc tap.\n";
            }
            else
            {
                cout << "\nSo hoc ky toi thieu theo "
                     << "quan he tien quyet: "
                     << semesters
                     << '\n';
            }

            break;
        }

        case 8:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            int maxCredits;

            cout << "\nNhap gioi han tin chi "
                 << "moi hoc ky: ";

            cin >> maxCredits;

            if (maxCredits <= 0)
            {
                cout << "[SAI] Gioi han tin chi "
                     << "phai lon hon 0.\n";
                break;
            }

            plan =
                buildSemesters(
                    g,
                    maxCredits);

            if (plan.empty())
            {
                cout << "\n[SAI] Khong tao duoc "
                     << "ke hoach.\n";
            }
            else
            {
                cout << "\n[OK] Tao ke hoach "
                     << "thanh cong.\n";

                cout << "So hoc ky: "
                     << plan.size()
                     << '\n';

                printPlan(g, plan);
            }

            break;
        }

        case 9:
        {
            if (plan.empty())
            {
                cout << "\n[!] Chua co ke hoach.\n";
                cout << "Hay chon 8 truoc.\n";
                break;
            }

            printPlan(g, plan);

            break;
        }

        case 10:
        {
            if (plan.empty())
            {
                cout << "\n[!] Chua co ke hoach.\n";
                cout << "Hay chon 8 truoc.\n";
                break;
            }

            comparePlanWithSchool(
                g,
                plan);

            break;
        }

        case 11:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            if (plan.empty())
            {
                cout << "\n[!] Chua co ke hoach.\n";
                cout << "Hay chon 8 truoc.\n";
                break;
            }

            int maxCredits;
            int maxDifficulty;

            cout << "\nNhap gioi han tin chi: ";
            cin >> maxCredits;

            cout << "Nhap gioi han do kho: ";
            cin >> maxDifficulty;

            if (maxCredits <= 0 ||
                maxDifficulty <= 0)
            {
                cout << "\n[SAI] Cac gioi han "
                     << "phai lon hon 0.\n";
                break;
            }

            Plan balancedPlan =
                balanceLoad(
                    g,
                    plan,
                    maxCredits,
                    maxDifficulty);

            if (balancedPlan.empty())
            {
                cout << "\n[SAI] Khong tao duoc "
                     << "ke hoach can bang.\n";
            }
            else
            {
                cout << "\n[OK] Da can bang tai "
                     << "hoc tap.\n";

                plan = balancedPlan;

                printPlan(
                    g,
                    plan);
            }

            break;
        }

        case 12:
        {
            if (g.n == 0)
            {
                cout << "\n[!] Hay doc du lieu truoc.\n";
                break;
            }

            int maxCredits;
            int maxDifficulty;
            int k;

            cout << "\nNhap gioi han tin chi: ";
            cin >> maxCredits;

            cout << "Nhap gioi han do kho: ";
            cin >> maxDifficulty;

            cout << "Nhap so ke hoach muon xem: ";
            cin >> k;

            if (maxCredits <= 0 ||
                maxDifficulty <= 0 ||
                k <= 0)
            {
                cout << "\n[SAI] Gia tri khong hop le.\n";
                break;
            }

            vector<Plan> plans =
                topPlans(
                    g,
                    maxCredits,
                    maxDifficulty,
                    k);

            if (plans.empty())
            {
                cout << "\n[SAI] Khong tao duoc "
                     << "ke hoach nao.\n";
                break;
            }

            cout << "\n========== CAC KE HOACH ==========\n";

            for (int i = 0;
                 i < (int)plans.size();
                 i++)
            {
                cout << "\n******** KE HOACH "
                     << i + 1
                     << " ********\n";

                printPlan(
                    g,
                    plans[i]);
            }

            break;
        }

        case 0:
        {
            cout << "\nKet thuc chuong trinh.\n";
            break;
        }

        default:
        {
            cout << "\n[SAI] Lua chon khong hop le.\n";
            break;
        }
        }

    } while (choice != 0);

    return 0;
}
