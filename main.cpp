// ─────────────────────────────────────────────────────────────────────────────
//  main.cpp  –  Interactive CLI for the Student Management System
// ─────────────────────────────────────────────────────────────────────────────
#include "StudentManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

// ── Console helpers ───────────────────────────────────────────────────────────
static void pause() {
    std::cout << "\n  Press <Enter> to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int getInt(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Invalid – enter an integer.\n";
    }
}

static double getDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Invalid – enter a number.\n";
    }
}

static std::string getString(const std::string& prompt) {
    std::string val;
    std::cout << prompt;
    std::getline(std::cin, val);
    return val;
}

// ── UI blocks ─────────────────────────────────────────────────────────────────
static void printBanner() {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║        STUDENT MANAGEMENT SYSTEM  ·  v1.0               ║\n";
    std::cout << "  ║    Data Structures & OOP in C++                         ║\n";
    std::cout << "  ║    Linked List  ·  Hash Table  ·  Dynamic Array         ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════════╝\n\n";
}

static void printMenu(int totalStudents) {
    std::cout << "\n  ┌───────────────────────────────────────────┐\n";
    std::cout << "  │               MAIN  MENU                  │\n";
    std::cout << "  ├───────────────────────────────────────────┤\n";
    std::cout << "  │  Students on record : "
              << std::left << std::setw(18) << totalStudents << "│\n";
    std::cout << "  ├───────────────────────────────────────────┤\n";
    std::cout << "  │   [1]  Add New Student                    │\n";
    std::cout << "  │   [2]  View All Students                  │\n";
    std::cout << "  │   [3]  Search Student                     │\n";
    std::cout << "  │   [4]  Update Student                     │\n";
    std::cout << "  │   [5]  Delete Student                     │\n";
    std::cout << "  │   [6]  Add Grades to Student              │\n";
    std::cout << "  │   [7]  Sort & Display                     │\n";
    std::cout << "  │   [8]  Class Statistics                   │\n";
    std::cout << "  │   [9]  Load Sample Dataset                │\n";
    std::cout << "  │   [0]  Exit                               │\n";
    std::cout << "  └───────────────────────────────────────────┘\n";
    std::cout << "  Choice: ";
}

// ── Action: Add Student ───────────────────────────────────────────────────────
static void doAdd(StudentManager& sm) {
    std::cout << "\n  ── ADD NEW STUDENT ────────────────────────\n";
    std::string name  = getString("  Full Name  : ");
    if (name.empty()) { std::cout << "  [!] Name cannot be empty.\n"; return; }
    int         age   = getInt   ("  Age        : ");
    std::string major = getString("  Major      : ");
    if (major.empty()) major = "Undeclared";
    std::string email = getString("  Email      : ");

    int newId = sm.createStudent(name, age, major, email);
    std::cout << "\n  [✓] Student \"" << name
              << "\" added with ID " << newId << "\n";
}

// ── Action: View All ─────────────────────────────────────────────────────────
static void doView(StudentManager& sm) {
    std::cout << "\n  ── ALL STUDENTS  (" << sm.count() << " total)  ──────────────\n\n";
    sm.displayAll();
}

// ── Action: Search ────────────────────────────────────────────────────────────
static void doSearch(StudentManager& sm) {
    std::cout << "\n  ── SEARCH ─────────────────────────────────\n";
    std::cout << "    [1] By Student ID  (O(1) – hash table)\n";
    std::cout << "    [2] By Name        (O(n) – linked-list scan)\n";
    int mode = getInt("  Mode: ");

    if (mode == 1) {
        int id = getInt("  Enter ID: ");
        GradedStudent* s = sm.getById(id);
        if (!s) { std::cout << "  [!] ID " << id << " not found.\n"; return; }
        sm.printHeader();
        s->display();
        sm.printSeparator();
        s->displayDetailed();

    } else if (mode == 2) {
        std::string q = getString("  Name (partial ok): ");
        auto results = sm.searchByName(q);
        if (results.empty()) {
            std::cout << "  [!] No students match \"" << q << "\"\n";
            return;
        }
        std::cout << "\n  Found " << results.size() << " result(s):\n\n";
        sm.printHeader();
        for (auto* s : results) s->display();
        sm.printSeparator();
        if (results.size() == 1) results[0]->displayDetailed();

    } else {
        std::cout << "  [!] Invalid mode.\n";
    }
}

// ── Action: Update ────────────────────────────────────────────────────────────
static void doUpdate(StudentManager& sm) {
    std::cout << "\n  ── UPDATE STUDENT ─────────────────────────\n";
    int id = getInt("  Student ID: ");
    GradedStudent* s = sm.getById(id);
    if (!s) { std::cout << "  [!] Not found.\n"; return; }

    s->displayDetailed();
    std::cout << "\n  Enter new values (blank = keep current):\n";

    std::string name = getString("  Name  [" + s->getName() + "]: ");
    if (name.empty()) name = s->getName();

    std::string ageStr = getString("  Age   [" + std::to_string(s->getAge()) + "]: ");
    int age = ageStr.empty() ? s->getAge() : std::stoi(ageStr);

    std::string major = getString("  Major [" + s->getMajor() + "]: ");
    if (major.empty()) major = s->getMajor();

    std::string email = getString("  Email [" + s->getEmail() + "]: ");
    if (email.empty()) email = s->getEmail();

    if (sm.updateStudent(id, name, age, major, email))
        std::cout << "\n  [✓] Student " << id << " updated.\n";
}

// ── Action: Delete ────────────────────────────────────────────────────────────
static void doDelete(StudentManager& sm) {
    std::cout << "\n  ── DELETE STUDENT ─────────────────────────\n";
    int id = getInt("  Student ID: ");
    GradedStudent* s = sm.getById(id);
    if (!s) { std::cout << "  [!] Not found.\n"; return; }

    std::cout << "  About to delete: " << s->getName()
              << "  (ID " << id << ")\n";
    std::string confirm = getString("  Type \"yes\" to confirm: ");
    if (confirm == "yes" || confirm == "YES") {
        sm.deleteStudent(id);
        std::cout << "  [✓] Student deleted.\n";
    } else {
        std::cout << "  Cancelled.\n";
    }
}

// ── Action: Add Grades ────────────────────────────────────────────────────────
static void doAddGrades(StudentManager& sm) {
    std::cout << "\n  ── ADD GRADES ─────────────────────────────\n";
    int id = getInt("  Student ID: ");
    GradedStudent* s = sm.getById(id);
    if (!s) { std::cout << "  [!] Not found.\n"; return; }

    std::cout << "  Adding grades for: " << s->getName() << "\n";
    int n = getInt("  How many grades? ");
    for (int i = 0; i < n; ++i) {
        double g = getDouble("  Grade " + std::to_string(i+1) + " [0–100]: ");
        if (sm.addGrade(id, g))
            std::cout << "    → " << g << " added\n";
    }
    std::cout << "\n  New average : " << std::fixed << std::setprecision(2)
              << s->getAverageScore() << "%  "
              << s->getLetterGrade()  << "  (GPA "
              << s->getGPA() << ")\n";
}

// ── Action: Sort ──────────────────────────────────────────────────────────────
static void doSort(StudentManager& sm) {
    std::cout << "\n  ── SORT & DISPLAY ─────────────────────────\n";
    std::cout << "    [1] By ID   (ascending)\n";
    std::cout << "    [2] By Name (alphabetical)\n";
    std::cout << "    [3] By GPA  (highest first)\n";
    int mode = getInt("  Sort by: ");

    SortCriteria c;
    std::string  label;
    switch (mode) {
        case 1: c = SortCriteria::BY_ID;   label = "ID";   break;
        case 2: c = SortCriteria::BY_NAME; label = "Name"; break;
        case 3: c = SortCriteria::BY_GPA;  label = "GPA";  break;
        default: std::cout << "  [!] Invalid choice.\n"; return;
    }

    std::cout << "\n  Sorted by " << label << ":\n\n";
    sm.displaySorted(c);
}

// ── Action: Statistics ────────────────────────────────────────────────────────
static void doStats(StudentManager& sm) {
    std::cout << "\n  ── CLASS STATISTICS ───────────────────────\n";
    std::cout << "  Total students  : " << sm.count() << "\n";
    std::cout << "  Class average   : " << std::fixed << std::setprecision(2)
              << sm.getClassAverage() << "%\n";

    GradedStudent* top = sm.getTopStudent();
    GradedStudent* bot = sm.getBottomStudent();

    if (top) std::cout << "  Top student     : " << top->getName()
                       << "  (GPA " << top->getGPA() << ")\n";
    if (bot) std::cout << "  Lowest GPA      : " << bot->getName()
                       << "  (GPA " << bot->getGPA() << ")\n";

    // GPA distribution
    auto all = sm.getAllStudents();
    int  A = 0, B = 0, C = 0, D = 0, F = 0;
    for (auto* s : all) {
        double g = s->getGPA();
        if      (g >= 3.7) A++;
        else if (g >= 2.7) B++;
        else if (g >= 1.7) C++;
        else if (g >= 0.7) D++;
        else               F++;
    }
    std::cout << "\n  GPA Distribution:\n";
    std::cout << "    A (≥3.7) : " << A << "\n";
    std::cout << "    B (≥2.7) : " << B << "\n";
    std::cout << "    C (≥1.7) : " << C << "\n";
    std::cout << "    D (≥0.7) : " << D << "\n";
    std::cout << "    F  (<0.7): " << F << "\n";
}

// ── Action: Load Sample Data ──────────────────────────────────────────────────
static void doLoadSample(StudentManager& sm) {
    std::cout << "\n  Loading sample dataset...\n";
    struct Sample { const char* name; int age; const char* major; const char* email;
                    std::vector<double> grades; };
    std::vector<Sample> samples = {
        {"Alice Johnson",  20, "Computer Science", "alice@uni.edu",  {95,88,92,97,91}},
        {"Bob Smith",      21, "Mathematics",      "bob@uni.edu",    {78,82,75,80,85}},
        {"Carol White",    19, "Physics",          "carol@uni.edu",  {91,94,89,96,93}},
        {"David Brown",    22, "Engineering",      "david@uni.edu",  {65,70,68,72,69}},
        {"Eva Martinez",   20, "Computer Science", "eva@uni.edu",    {88,85,90,87,92}},
        {"Frank Lee",      23, "Chemistry",        "frank@uni.edu",  {55,60,58,62,57}},
        {"Grace Kim",      21, "Biology",          "grace@uni.edu",  {100,98,99,97,100}},
        {"Henry Patel",    22, "Economics",        "henry@uni.edu",  {72,76,70,74,78}},
    };
    for (auto& s : samples) {
        int id = sm.createStudent(s.name, s.age, s.major, s.email);
        sm.addGrades(id, s.grades);
        std::cout << "  [✓] " << s.name << " (ID " << id << ")\n";
    }
    std::cout << "\n  " << samples.size() << " students loaded.\n";
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    StudentManager sm;
    printBanner();

    while (true) {
        printMenu(sm.count());

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n";

        switch (choice) {
            case 1: doAdd      (sm); break;
            case 2: doView     (sm); break;
            case 3: doSearch   (sm); break;
            case 4: doUpdate   (sm); break;
            case 5: doDelete   (sm); break;
            case 6: doAddGrades(sm); break;
            case 7: doSort     (sm); break;
            case 8: doStats    (sm); break;
            case 9: doLoadSample(sm);break;
            case 0:
                std::cout << "  Goodbye!\n\n";
                return 0;
            default:
                std::cout << "  [!] Unknown option – try again.\n";
        }
        pause();
    }
}
