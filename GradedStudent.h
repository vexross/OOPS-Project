#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  GradedStudent.h  –  Concrete class (OOP: Inheritance + Polymorphism)
//  Inherits Student, adds grades array, GPA auto-computation, letter grades
// ─────────────────────────────────────────────────────────────────────────────
#include "Student.h"
#include <vector>
#include <numeric>
#include <iomanip>
#include <sstream>

class GradedStudent : public Student {
private:
    // ── Array data structure: dynamic grade storage ─────────────────────────
    std::vector<double> grades;
    std::string         major;
    std::string         email;

public:
    // ── Constructors ─────────────────────────────────────────────────────────
    GradedStudent()
        : Student(), major("Undeclared"), email("") {}

    GradedStudent(int id, const std::string& name, int age,
                  const std::string& major = "Undeclared",
                  const std::string& email = "")
        : Student(id, name, age), major(major), email(email) {}

    // ── Grade management (array: push_back) ──────────────────────────────────
    void addGrade(double grade) override {
        if (grade >= 0.0 && grade <= 100.0)
            grades.push_back(grade);
        else
            std::cerr << "  [!] Invalid grade " << grade
                      << " – must be in range [0, 100]\n";
    }

    void clearGrades() { grades.clear(); }

    // ── GPA / Average computation (auto-computed, never stored) ──────────────
    double getAverageScore() const override {
        if (grades.empty()) return 0.0;
        return std::accumulate(grades.begin(), grades.end(), 0.0)
               / static_cast<double>(grades.size());
    }

    // 4.0 scale with +/- modifiers
    double getGPA() const override {
        double avg = getAverageScore();
        if (avg >= 93) return 4.0;
        if (avg >= 90) return 3.7;
        if (avg >= 87) return 3.3;
        if (avg >= 83) return 3.0;
        if (avg >= 80) return 2.7;
        if (avg >= 77) return 2.3;
        if (avg >= 73) return 2.0;
        if (avg >= 70) return 1.7;
        if (avg >= 67) return 1.3;
        if (avg >= 63) return 1.0;
        if (avg >= 60) return 0.7;
        return 0.0;
    }

    std::string getLetterGrade() const override {
        double avg = getAverageScore();
        if (avg >= 93) return "A";
        if (avg >= 90) return "A-";
        if (avg >= 87) return "B+";
        if (avg >= 83) return "B";
        if (avg >= 80) return "B-";
        if (avg >= 77) return "C+";
        if (avg >= 73) return "C";
        if (avg >= 70) return "C-";
        if (avg >= 67) return "D+";
        if (avg >= 63) return "D";
        if (avg >= 60) return "D-";
        return "F";
    }

    // ── Field accessors ──────────────────────────────────────────────────────
    std::string               getMajor()    const { return major;  }
    std::string               getEmail()    const { return email;  }
    const std::vector<double>&getGrades()   const { return grades; }
    int                       gradeCount()  const { return static_cast<int>(grades.size()); }

    void setMajor(const std::string& m) { major = m; }
    void setEmail(const std::string& e) { email = e; }

    // ── Display: table row (matches StudentManager header) ───────────────────
    void display() const override {
        std::cout
            << "| " << std::left  << std::setw(6)  << id
            << "| " << std::left  << std::setw(22) << name.substr(0, 22)
            << "| " << std::right << std::setw(3)  << age << " "
            << "| " << std::left  << std::setw(16) << major.substr(0, 16) << "  "
            << "| " << std::right << std::setw(6)
                    << std::fixed << std::setprecision(2) << getAverageScore() << " "
            << "| " << std::left  << std::setw(4) << getLetterGrade() << " "
            << "| " << std::right << std::setw(4)
                    << std::setprecision(2) << getGPA() << " "
            << "|\n";
    }

    // ── Display: full detail card ─────────────────────────────────────────────
    void displayDetailed() const override {
        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║           STUDENT DETAIL CARD            ║\n";
        std::cout << "  ╠══════════════════════════════════════════╣\n";
        std::cout << "  ║  ID      : " << std::left << std::setw(30) << id   << "║\n";
        std::cout << "  ║  Name    : " << std::setw(30) << name               << "║\n";
        std::cout << "  ║  Age     : " << std::setw(30) << age                << "║\n";
        std::cout << "  ║  Major   : " << std::setw(30) << major              << "║\n";
        std::cout << "  ║  Email   : " << std::setw(30)
                  << (email.empty() ? "N/A" : email)                          << "║\n";
        std::cout << "  ╠══════════════════════════════════════════╣\n";
        // Grades
        std::ostringstream gs;
        if (grades.empty()) {
            gs << "(none)";
        } else {
            for (size_t i = 0; i < grades.size(); ++i) {
                gs << grades[i];
                if (i + 1 < grades.size()) gs << "  ";
            }
        }
        // Wrap long grade strings
        std::string gstr = gs.str();
        if (gstr.size() <= 30) {
            std::cout << "  ║  Grades  : " << std::setw(30) << gstr           << "║\n";
        } else {
            std::cout << "  ║  Grades  : " << std::setw(30) << gstr.substr(0,27)+"..." << "║\n";
        }
        std::cout << "  ╠══════════════════════════════════════════╣\n";
        std::cout << "  ║  Average : " << std::setw(30)
                  << (std::to_string((int)getAverageScore()) + "." +
                      std::to_string((int)(getAverageScore()*100)%100) + "%") << "║\n";
        std::cout << "  ║  Grade   : " << std::setw(30) << getLetterGrade()   << "║\n";
        std::ostringstream gpas;
        gpas << std::fixed << std::setprecision(2) << getGPA() << " / 4.0";
        std::cout << "  ║  GPA     : " << std::setw(30) << gpas.str()         << "║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";
    }
};
