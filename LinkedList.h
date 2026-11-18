#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  LinkedList.h  –  Generic singly-linked list (Data Structure)
//
//  Complexity:
//    insertBack / insertFront  O(1)  (tail pointer maintained)
//    remove                    O(n)
//    find / findAll            O(n)
//    toVector / fromVector     O(n)
// ─────────────────────────────────────────────────────────────────────────────
#include <functional>
#include <vector>
#include <iostream>
#include <stdexcept>

// ── Node definition ──────────────────────────────────────────────────────────
template <typename T>
struct ListNode {
    T         data;
    ListNode* next;
    explicit ListNode(const T& val) : data(val), next(nullptr) {}
};

// ── Linked List class ────────────────────────────────────────────────────────
template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;  // O(1) insertBack
    int          count;

public:
    // ── Lifecycle ────────────────────────────────────────────────────────────
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() { clear(); }

    // Disallow copy to avoid double-free
    LinkedList(const LinkedList&)            = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    // ── Insert ───────────────────────────────────────────────────────────────
    void insertBack(const T& data) {
        ListNode<T>* node = new ListNode<T>(data);
        if (!tail) { head = tail = node; }
        else       { tail->next = node; tail = node; }
        ++count;
    }

    void insertFront(const T& data) {
        ListNode<T>* node = new ListNode<T>(data);
        node->next = head;
        if (!head) tail = node;
        head = node;
        ++count;
    }

    // ── Remove first node that satisfies predicate ───────────────────────────
    bool remove(std::function<bool(const T&)> pred) {
        if (!head) return false;

        if (pred(head->data)) {
            ListNode<T>* tmp = head;
            head = head->next;
            if (!head) tail = nullptr;
            delete tmp;
            --count;
            return true;
        }

        ListNode<T>* curr = head;
        while (curr->next) {
            if (pred(curr->next->data)) {
                ListNode<T>* tmp = curr->next;
                curr->next = tmp->next;
                if (tmp == tail) tail = curr;
                delete tmp;
                --count;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // ── Search ───────────────────────────────────────────────────────────────

    // Find first match  →  pointer to data (nullptr if not found)
    T* find(std::function<bool(const T&)> pred) {
        ListNode<T>* curr = head;
        while (curr) {
            if (pred(curr->data)) return &curr->data;
            curr = curr->next;
        }
        return nullptr;
    }

    // Find all matches  →  vector of matching data copies
    std::vector<T> findAll(std::function<bool(const T&)> pred) const {
        std::vector<T> results;
        ListNode<T>* curr = head;
        while (curr) {
            if (pred(curr->data)) results.push_back(curr->data);
            curr = curr->next;
        }
        return results;
    }

    // ── Traversal ────────────────────────────────────────────────────────────
    void forEach(std::function<void(T&)> callback) {
        ListNode<T>* curr = head;
        while (curr) {
            callback(curr->data);
            curr = curr->next;
        }
    }

    void forEachConst(std::function<void(const T&)> callback) const {
        ListNode<T>* curr = head;
        while (curr) {
            callback(curr->data);
            curr = curr->next;
        }
    }

    // ── Conversion to/from std::vector (used by sort) ────────────────────────
    std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(static_cast<size_t>(count));
        ListNode<T>* curr = head;
        while (curr) { result.push_back(curr->data); curr = curr->next; }
        return result;
    }

    // Clears list and rebuilds from sorted vector
    void fromVector(const std::vector<T>& vec) {
        clear();
        for (const auto& item : vec) insertBack(item);
    }

    // ── Utilities ────────────────────────────────────────────────────────────
    void clear() {
        while (head) {
            ListNode<T>* tmp = head;
            head = head->next;
            delete tmp;        // deletes the ListNode only, NOT the T object
        }
        tail  = nullptr;
        count = 0;
    }

    int  size()  const { return count; }
    bool empty() const { return count == 0; }
    ListNode<T>* getHead() const { return head; }
};
