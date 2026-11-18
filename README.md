# Student Management System — C++
### Data Structures · OOP · CRUD

---

## Project Structure

```
sms/
├── Student.h           Abstract base class (pure virtual interface)
├── GradedStudent.h     Concrete student + grades array + GPA engine
├── LinkedList.h        Template singly-linked list (primary storage)
├── HashTable.h         Template hash table w/ chaining (two indexes)
├── StudentManager.h    Manager façade — header
├── StudentManager.cpp  Manager façade — implementation
├── main.cpp            Interactive CLI entry point
├── Makefile            Build script
└── README.md           This file
```

---

## Build & Run

**Requires:** g++ with C++17 support

```bash
# Compile
make

# Compile and run immediately
make run

# Debug build (AddressSanitizer + UBSanitizer)
make debug

# Remove build artifacts
make clean
```

Manual compile (no make):
```bash
g++ -std=c++17 -Wall -O2 -o student_manager main.cpp StudentManager.cpp
./student_manager
```

---

## OOP Design

### Class Hierarchy

```
          ┌──────────────────────────────┐
          │   Student   (abstract)        │  ← Abstraction
          │──────────────────────────────│     Pure virtuals:
          │  # id : int                  │       getGPA()
          │  # name : string             │       getAverageScore()
          │  # age  : int                │       getLetterGrade()
          │──────────────────────────────│       addGrade()
          │  + getId(), getName()...     │       display()
          │  + setName(), setAge()...    │       displayDetailed()
          └──────────────────────────────┘
                        ▲
                  (inheritance)
                        │
          ┌──────────────────────────────┐
          │   GradedStudent  (concrete)   │  ← Inheritance
          │──────────────────────────────│
          │  - grades : vector<double>   │  ← Array (dynamic)
          │  - major  : string           │
          │  - email  : string           │
          │──────────────────────────────│
          │  + addGrade(double)          │
          │  + getAverageScore() [auto]  │  ← Auto-computed
          │  + getGPA()          [auto]  │
          │  + getLetterGrade()  [auto]  │
          └──────────────────────────────┘

          ┌──────────────────────────────────────────────────┐
          │   StudentManager                                  │
          │──────────────────────────────────────────────────│
          │  - list     : LinkedList<GradedStudent*>         │ ← Linked List
          │  - byId     : HashTable<int,    GradedStudent*>  │ ← Dictionary
          │  - byNameIdx: HashTable<string, GradedStudent*>  │ ← Dictionary
          │──────────────────────────────────────────────────│
          │  CRUD + search + sort + statistics               │
          └──────────────────────────────────────────────────┘
```

### OOP Principles Applied

| Principle       | Where                                                        |
|-----------------|--------------------------------------------------------------|
| **Encapsulation** | `Student` fields are `protected`; accessed only via public getters/setters |
| **Abstraction**   | `Student` defines pure virtual functions forcing a clean interface |
| **Inheritance**   | `GradedStudent` extends `Student`, implements all pure virtuals |
| **Polymorphism**  | `StudentManager` stores `GradedStudent*` and calls virtual `display()` |

---

## Data Structures

### 1. Singly Linked List (`LinkedList<T>`)

Primary ordered storage for student records.

```
head → [Student*] → [Student*] → [Student*] → nullptr
                                    ↑ tail (O(1) append)
```

| Operation       | Complexity |
|----------------|------------|
| insertBack      | O(1) (tail pointer) |
| insertFront     | O(1) |
| remove          | O(n) |
| find / findAll  | O(n) |
| toVector        | O(n) |

### 2. Hash Table – Dictionary (`HashTable<K,V>`)

Two instances: one keyed by **student ID** (int), one by **name** (string).  
Uses separate chaining — each bucket is a `std::list<Entry>`.

```
bucket[hash(key)] → [(k0,v0)] → [(k1,v1)] → ...
```

| Operation   | Avg  | Worst |
|-------------|------|-------|
| insert      | O(1) | O(n) |
| get         | O(1) | O(n) |
| remove      | O(1) | O(n) |

### 3. Dynamic Array (`std::vector<double>` inside GradedStudent)

Stores each student's raw grades. Grows on demand.  
Used for GPA and average computation (`std::accumulate`).

Also used as a **sort buffer**: the linked list is dumped to a vector,
sorted with `std::sort` (O(n log n)), then the list is rebuilt.

---

## GPA Scale (4.0)

| Avg %    | GPA | Letter |
|----------|-----|--------|
| ≥ 93     | 4.0 | A      |
| 90 – 92  | 3.7 | A−     |
| 87 – 89  | 3.3 | B+     |
| 83 – 86  | 3.0 | B      |
| 80 – 82  | 2.7 | B−     |
| 77 – 79  | 2.3 | C+     |
| 73 – 76  | 2.0 | C      |
| 70 – 72  | 1.7 | C−     |
| 67 – 69  | 1.3 | D+     |
| 63 – 66  | 1.0 | D      |
| 60 – 62  | 0.7 | D−     |
| < 60     | 0.0 | F      |

---

## Menu Options

```
[1] Add New Student         CREATE  – adds to list + both hash indexes
[2] View All Students       READ    – traverses linked list
[3] Search Student          READ    – O(1) by ID (hash); O(n) by name
[4] Update Student          UPDATE  – O(1) lookup, then update + re-index
[5] Delete Student          DELETE  – removes from list + both indexes, frees heap
[6] Add Grades              UPDATE  – push_back to grades vector
[7] Sort & Display          SORT    – list → vector → std::sort → list
[8] Class Statistics        READ    – traverses list, computes GPA distribution
[9] Load Sample Dataset     CREATE  – 8 pre-built students with grades
[0] Exit
```
