#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Student.h  –  Abstract base class (OOP: Abstraction + Encapsulation)
// ─────────────────────────────────────────────────────────────────────────────
#include <string>
#include <iostream>

class Student {
protected:
    // ── Encapsulation: fields are protected, accessible via interface only ──
    int         id;
    std::string name;
    int         age;

public:
    Student() : id(0), name(""), age(0) {}

    Student(int id, const std::string& name, int age)
        : id(id), name(name), age(age) {}

    virtual ~Student() = default;

    // ── Getters (encapsulation interface) ───────────────────────────────────
    int         getId()   const { return id;   }
    std::string getName() const { return name; }
    int         getAge()  const { return age;  }

    // ── Setters ─────────────────────────────────────────────────────────────
    void setId(int i)                 { id   = i; }
    void setName(const std::string& n){ name = n; }
    void setAge(int a)                { age  = a; }

    // ── Pure virtual interface (Abstraction) ─────────────────────────────────
    //    Forces every concrete subclass to provide its own implementations
    virtual double      getGPA()          const = 0;
    virtual double      getAverageScore() const = 0;
    virtual std::string getLetterGrade()  const = 0;
    virtual void        addGrade(double grade)  = 0;
    virtual void        display()         const = 0;
    virtual void        displayDetailed() const = 0;
};
