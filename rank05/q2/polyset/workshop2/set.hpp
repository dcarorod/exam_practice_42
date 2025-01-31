#pragma once

#include "searchable_array_bag.hpp"
#include "searchable_tree_bag.hpp"

class set : public searchable_array_bag, public searchable_tree_bag {
private:
    set() {}

public:
    ~set() {}
    set(const set& other) :
        searchable_array_bag(other),
        searchable_tree_bag(other) {
            size = other.size;
            data = other.data;
            tree = other.tree;
        }
    set& operator=(const set& other) {
        if (this != &other) {
            searchable_array_bag::operator=(other);
            searchable_tree_bag::operator=(other);
        }
        return *this;
    }

    bool has(int num) {
        if (size > 0)
            return searchable_array_bag::has(num);
        return searchable_tree_bag::has(num);
    }
    void insert(int num) {
        if (size > 0) {
            if (!has(num))
                searchable_array_bag::insert(num);
        }
        if (!has(num))
            searchable_tree_bag::insert(num);
    }
    void insert(int* items, int count) {
        for (int i = 0; i < count; i++)
            insert(items[i]);
    }
    void print() const {
        if (size > 0)
            searchable_array_bag::print();
        else
            searchable_tree_bag::print();
    }
    void clear() {
        if (size > 0)
            searchable_array_bag::clear();
        else
            searchable_tree_bag::clear();

    }
};