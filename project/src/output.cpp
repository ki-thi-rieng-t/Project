#include "output.h"

void printPlan(const Graph& g, const Plan& plan)
{
    if (plan.empty()){
        cout << "\nKhong co ke hoach hoc tap.\n";
        return;
    }

    cout << "\n========== KE HOACH HOC TAP ==========\n";

    for (int semester = 0; semester < (int)plan.size(); semester++){
        cout << "\n--- Hoc ky " << semester + 1 << " ---\n";

        if (plan[semester].empty()){
            cout << "Khong co mon hoc.\n";
            continue;
        }
        int totalCredits = 0;
        for (int index : plan[semester]){
            if (index < 0 || index >= g.n)
                continue;
            const Course& c = g.courses[index];
            cout << c.id
                 << " | " << c.name
                 << " | " << c.credits << " TC"
                 << " | Do kho: " << c.difficulty
                 << '\n';
            totalCredits += c.credits;
        }
        cout << "Tong tin chi: " << totalCredits << '\n';
    }
    cout << "\n======================================\n";
}

void comparePlanWithSchool(const Graph& g, const Plan& plan){
    if (plan.empty()){
        cout << "\nKhong co ke hoach de so sanh.\n";
        return;
    }
    cout << "\n======= SO SANH VOI KE HOACH TRUONG =======\n";

    for (int semester = 0; semester < (int)plan.size(); semester++){
        cout << "\nHoc ky " << semester + 1 << ":\n";

        if (plan[semester].empty()){
            cout << "Khong co mon hoc.\n";
            continue;
        }

        for (int index : plan[semester]){
            if (index < 0 || index >= g.n)
                continue;

            const Course& c = g.courses[index];

            int schoolSemester = c.schoolSemester;

            cout << c.id << " - " << c.name;

            if (schoolSemester == semester + 1){
                cout << " -> Giong ke hoach truong";
            }
            else{
                cout << " -> Truong xep HK " << schoolSemester;
            }
            cout << '\n';
        }
    }
    cout << "\n============================================\n";
}