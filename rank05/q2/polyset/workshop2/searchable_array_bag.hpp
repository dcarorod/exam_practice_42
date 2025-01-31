#pragma once

#include "searchable_bag.hpp"
#include "array_bag.hpp"

class searchable_array_bag : public searchable_bag, public array_bag
{
public:
    virtual bool has(int num) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == num)
                return true;
        }
        return false;
    }

    searchable_array_bag() : searchable_bag(), array_bag() {}
    ~searchable_array_bag() {}
    searchable_array_bag(const searchable_array_bag& other) :
        searchable_bag(other),
        array_bag(other) {
            size = other.size;
            data = other.data;
        }
    searchable_array_bag operator=(const searchable_array_bag& other) {
        if (this != &other) {
            searchable_bag::operator=(other);
            array_bag::operator=(other);
        }
        return *this;
    }
};