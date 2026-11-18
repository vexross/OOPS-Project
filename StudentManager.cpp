// ─────────────────────────────────────────────────────────────────────────────
//  StudentManager.cpp  –  CRUD, sort, search, display implementations
// ─────────────────────────────────────────────────────────────────────────────
#include "StudentManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

// ────────────────────────────────────────────────────────────────────────────
//  Lifecycle
// ────────────────────────────────────────────────────────────────────────────
StudentManager::StudentManager() : idCounter(1001) {}

StudentManager::~StudentManager() { deleteAll(); }

// ────────────────────────────────────────────────────────────────────────────
//  Helper: convert string to lowercase (for case-insensitive name index)
// ────────────────────────────────────────────────────────────────────────────
std::string StudentManager::toLower(const std::string& s) const {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return r;
}

// ────────────────────────────────────────────────────────────────────────────
//  CREATE
// ────────────────────────────────────────────────────────────────────────────
int StudentManager::createStudent(const std::string& name, int age,
                                   const std::string& major,
                                   const std::string& email)
{
    int newId = idCounter++;
    GradedStudent* s = new GradedStudent(newId, name, age, major, email);

    list.insertBack(s);             // O(1)  – linked list tail insert
    byId.insert(newId, s);          // O(1)  – hash table
    byNameIdx.insert(toLower(name), s); // O(1) – hash table

    return newId;
}

// ────────────────────────────────────────────────────────────────────────────
//  READ
// ────────────────────────────────────────────────────────────────────────────
GradedStudent* StudentManager::getById(int id) {
    GradedStudent** pp = byId.get(id);  // O(1) hash lookup
    return pp ? *pp : nullptr;
}

GradedStudent* StudentManager::getByExactName(const std::string& name) {
    GradedStudent** pp = byNameIdx.get(toLower(name));  // O(1)
    return pp ? *pp : nullptr;
}

// Partial-match search traverses the linked list  O(n)
std::vector<GradedStudent*>
StudentManager::searchByName(const std::string& query) const {
    std::string q = toLower(query);
    return list.findAll([&](GradedStudent* const& s) {
        return toLower(s->getName()).find(q) != std::string::npos;
    });
}

std::vector<GradedStudent*> StudentManager::getAllStudents() const {
    return list.toVector();
}

// ────────────────────────────────────────────────────────────────────────────
//  UPDATE
// ────────────────────────────────────────────────────────────────────────────
bool StudentManager::updateStudent(int id,
                                    const std::string& name, int age,
                                    const std::string& major,
                                    const std::string& email)
{
    GradedStudent* s = getById(id);
    if (!s) return false;

    // Remove stale name entry, then re-insert with new name
    byNameIdx.remove(toLower(s->getName()));

    s->setName(name);
    s->setAge(age);
    s->setMajor(major);
    s->setEmail(email);

    byNameIdx.insert(toLower(name), s);
    return true;
}

bool StudentManager::addGrade(int id, double grade) {
    GradedStudent* s = getById(id);
    if (!s) return false;
    s->addGrade(grade);
    return true;
}

bool StudentManager::addGrades(int id, const std::vector<double>& grades) {
    GradedStudent* s = getById(id);
    if (!s) return false;
    for (double g : grades) s->addGrade(g);
    return true;
}

// ────────────────────────────────────────────────────────────────────────────
//  DELETE
// ────────────────────────────────────────────────────────────────────────────
bool StudentManager::deleteStudent(int id) {
    GradedStudent* s = getById(id);
    if (!s) return false;

    byNameIdx.remove(toLower(s->getName()));  // remove from name index
    byId.remove(id);                          // remove from ID index
    list.remove([id](GradedStudent* const& st){
        return st->getId() == id;             // remove from linked list  O(n)
    });
    delete s;                                 // free heap object
    return true;
}

void StudentManager::deleteAll() {
    // Delete each heap-allocated GradedStudent through the list
    list.forEach([](GradedStudent*& s){ delete s; });
    list.clear();
    byId.clear();
    byNameIdx.clear();
}

// ────────────────────────────────────────────────────────────────────────────
//  SORT  –  extract to vector → std::sort → rebuild linked list
// ────────────────────────────────────────────────────────────────────────────
static bool cmpId  (GradedStudent* a, GradedStudent* b){ return a->getId()  < b->getId(); }
static bool cmpName(GradedStudent* a, GradedStudent* b){ return a->getName()< b->getName(); }
static bool cmpGPA (GradedStudent* a, GradedStudent* b){ return a->getGPA() > b->getGPA(); }// descending

void StudentManager::sortBy(SortCriteria c) {
    auto vec = list.toVector();
    switch (c) {
        case SortCriteria::BY_ID:   std::sort(vec.begin(), vec.end(), cmpId);   break;
        case SortCriteria::BY_NAME: std::sort(vec.begin(), vec.end(), cmpName); break;
        case SortCriteria::BY_GPA:  std::sort(vec.begin(), vec.end(), cmpGPA);  break;
    }
    list.fromVector(vec);   // clears nodes, rebuilds from sorted vector
}

std::vector<GradedStudent*>
StudentManager::getSortedCopy(SortCriteria c) const {
    auto vec = list.toVector();
    switch (c) {
        case SortCriteria::BY_ID:   std::sort(vec.begin(), vec.end(), cmpId);   break;
        case SortCriteria::BY_NAME: std::sort(vec.begin(), vec.end(), cmpName); break;
        case SortCriteria::BY_GPA:  std::sort(vec.begin(), vec.end(), cmpGPA);  break;
    }
    return vec;
}

// ────────────────────────────────────────────────────────────────────────────
//  DISPLAY
// ────────────────────────────────────────────────────────────────────────────
//  Column widths:  ID(6) | Name(22) | Age(4) | Major(18) | Avg%(7) | Gr(5) | GPA(5)
void StudentManager::printSeparator() const {
    std::cout << "+--------+------------------------+------+--------------------"
                 "+---------+------+------+\n";
}

void StudentManager::printHeader() const {
    printSeparator();
    std::cout << "| " << std::left
              << std::setw(6)  << "ID"
              << "| " << std::setw(22) << "Name"
              << "| " << std::setw(4)  << "Age"
              << "| " << std::setw(18) << "Major"
              << "| " << std::setw(7)  << "Avg %"
              << "| " << std::setw(4)  << "Gr"
              << "| " << std::setw(4)  << "GPA"
              << " |\n";
    printSeparator();
}

void StudentManager::displayAll() const {
    if (list.empty()) {
        std::cout << "  (no students on record)\n";
        return;
    }
    printHeader();
    list.forEachConst([](GradedStudent* const& s){ s->display(); });
    std::cout << "+--------+------------------------+------+--------------------"
                 "+---------+------+------+\n";
}

void StudentManager::displaySorted(SortCriteria c) {
    sortBy(c);
    displayAll();
}

// ────────────────────────────────────────────────────────────────────────────
//  STATISTICS
// ────────────────────────────────────────────────────────────────────────────
double StudentManager::getClassAverage() const {
    if (list.empty()) return 0.0;
    double total = 0.0;
    int    cnt   = 0;
    list.forEachConst([&](GradedStudent* const& s){
        if (s->gradeCount() > 0) { total += s->getAverageScore(); ++cnt; }
    });
    return cnt > 0 ? total / cnt : 0.0;
}

GradedStudent* StudentManager::getTopStudent() const {
    GradedStudent* top = nullptr;
    list.forEachConst([&](GradedStudent* const& s){
        if (!top || s->getGPA() > top->getGPA()) top = s;
    });
    return top;
}

GradedStudent* StudentManager::getBottomStudent() const {
    GradedStudent* bot = nullptr;
    list.forEachConst([&](GradedStudent* const& s){
        if (!bot || s->getGPA() < bot->getGPA()) bot = s;
    });
    return bot;
}
