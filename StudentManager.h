#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  StudentManager.h  –  CRUD facade over linked list + two hash-table indexes
//
//  Data structures in use:
//    LinkedList<GradedStudent*>            – primary ordered storage
//    HashTable<int,         GradedStudent*>– O(1) lookup by student ID
//    HashTable<std::string, GradedStudent*>– O(1) lookup by (lowercased) name
//    std::vector<double>                  – grade array inside GradedStudent
// ─────────────────────────────────────────────────────────────────────────────
#include "GradedStudent.h"
#include "LinkedList.h"
#include "HashTable.h"
#include <vector>
#include <string>
#include <algorithm>

// Sorting key enum
enum class SortCriteria { BY_ID, BY_NAME, BY_GPA };

class StudentManager {
private:
    // ── Primary storage (preserves insertion / sorted order) ─────────────────
    LinkedList<GradedStudent*>             list;

    // ── Indexes (O(1) direct lookup) ─────────────────────────────────────────
    HashTable<int,         GradedStudent*> byId;       // key: student ID
    HashTable<std::string, GradedStudent*> byNameIdx;  // key: lowercase name

    int idCounter;  // auto-increment ID seed

    // ── Helpers ──────────────────────────────────────────────────────────────
    std::string toLower(const std::string& s) const;

public:
    StudentManager();
    ~StudentManager();

    // ── CREATE ────────────────────────────────────────────────────────────────
    //    Returns the new student's auto-assigned ID
    int  createStudent(const std::string& name, int age,
                       const std::string& major = "Undeclared",
                       const std::string& email = "");

    // ── READ ──────────────────────────────────────────────────────────────────
    GradedStudent*              getById(int id);
    GradedStudent*              getByExactName(const std::string& name);

    // Partial-name search – traverses linked list, returns all matches
    std::vector<GradedStudent*> searchByName(const std::string& query) const;

    // Returns all students in current list order
    std::vector<GradedStudent*> getAllStudents() const;

    // ── UPDATE ────────────────────────────────────────────────────────────────
    bool updateStudent(int id,
                       const std::string& name, int age,
                       const std::string& major, const std::string& email);
    bool addGrade(int id, double grade);
    bool addGrades(int id, const std::vector<double>& grades);

    // ── DELETE ────────────────────────────────────────────────────────────────
    bool deleteStudent(int id);
    void deleteAll();

    // ── SORT (modifies list order in-place) ───────────────────────────────────
    void                        sortBy(SortCriteria c);
    std::vector<GradedStudent*> getSortedCopy(SortCriteria c) const;

    // ── DISPLAY ───────────────────────────────────────────────────────────────
    void printSeparator() const;
    void printHeader()    const;
    void displayAll()     const;
    void displaySorted(SortCriteria c);

    // ── STATISTICS ────────────────────────────────────────────────────────────
    int            count()          const { return list.size(); }
    bool           isEmpty()        const { return list.empty(); }
    double         getClassAverage()const;
    GradedStudent* getTopStudent()  const;
    GradedStudent* getBottomStudent()const;
};
