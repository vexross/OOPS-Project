#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  HashTable.h  –  Generic dictionary (Data Structure)
//
//  Strategy : Separate chaining (each bucket is a std::list)
//  Complexity:
//    insert / get / remove    O(1) average,  O(n) worst case
//    contains                 O(1) average
// ─────────────────────────────────────────────────────────────────────────────
#include <vector>
#include <list>
#include <functional>
#include <string>
#include <stdexcept>

template <typename K, typename V>
class HashTable {
private:
    // ── Internal key-value entry ─────────────────────────────────────────────
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    int numBuckets;
    int numElements;
    std::vector<std::list<Entry>> buckets;

    // ── Hash dispatcher (delegates to std::hash<K>) ──────────────────────────
    int hashKey(const K& key) const {
        size_t h = std::hash<K>{}(key);
        return static_cast<int>(h % static_cast<size_t>(numBuckets));
    }

public:
    // ── Constructor ──────────────────────────────────────────────────────────
    explicit HashTable(int capacity = 127)
        : numBuckets(capacity), numElements(0), buckets(capacity) {}

    // ── Insert / Update ──────────────────────────────────────────────────────
    //    If key already exists, its value is overwritten (upsert semantics)
    void insert(const K& key, const V& value) {
        int idx = hashKey(key);
        for (auto& e : buckets[idx]) {
            if (e.key == key) { e.value = value; return; }
        }
        buckets[idx].emplace_back(key, value);
        ++numElements;
    }

    // ── Lookup  →  pointer to value (nullptr if not found) ──────────────────
    V* get(const K& key) {
        int idx = hashKey(key);
        for (auto& e : buckets[idx])
            if (e.key == key) return &e.value;
        return nullptr;
    }

    const V* get(const K& key) const {
        int idx = hashKey(key);
        for (const auto& e : buckets[idx])
            if (e.key == key) return &e.value;
        return nullptr;
    }

    // ── Remove  →  true if key was found and erased ──────────────────────────
    bool remove(const K& key) {
        int idx = hashKey(key);
        auto& bucket = buckets[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                --numElements;
                return true;
            }
        }
        return false;
    }

    // ── Existence check ──────────────────────────────────────────────────────
    bool contains(const K& key) const { return get(key) != nullptr; }

    // ── Diagnostics ──────────────────────────────────────────────────────────
    int    size()       const { return numElements; }
    bool   empty()      const { return numElements == 0; }
    double loadFactor() const {
        return static_cast<double>(numElements) / numBuckets;
    }

    void clear() {
        for (auto& b : buckets) b.clear();
        numElements = 0;
    }
};
