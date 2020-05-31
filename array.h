#include <iostream>
#include <stdlib.h>
#include <functional>


#ifndef SEM4LAB2_ARRAY_H
#define SEM4LAB2_ARRAY_H

template <typename T>
class Array {
    static const int REALLOCATION_ELEMENTS = 16;

    T* memory_span = nullptr;
    int size = 0;
    int allocated_size = 0;

    void ensure_size(int size) {
        if (allocated_size < size) {
            int old_size = allocated_size;
            while (allocated_size < size) {
                allocated_size += REALLOCATION_ELEMENTS;
            }

            if (memory_span != nullptr) {
                T* new_memory_span = new T[allocated_size];
                for (int i = 0; i < old_size; i++) {
                    new_memory_span[i] = memory_span[i];
                }
                delete[] (memory_span);
                memory_span = new_memory_span;
            } else {
                memory_span = new T[allocated_size];
            }
        }
    }

public:
    Array() {

    }

    Array(Array<T> const& other) {
        ensure_size(other.size);
        size = other.size;
        for (int i = 0; i < size; i++) {
            memory_span[i] = other.memory_span[i];
        }

        std::vector<int> a = {1, 2};
    }

    Array(Array<T>&& other) {
        memory_span = other.memory_span;
        size = other.size;
        allocated_size = other.allocated_size;
        other.memory_span = nullptr;
        other.size = 0;
        other.allocated_size = 0;
    }

    Array<T>& operator= (Array<T> const& other) {
        ensure_size(other.size);
        size = other.size;
        for (int i = 0; i < size; i++) {
            memory_span[i] = other.memory_span[i];
        }
        return *this;
    }

    Array<T>& operator= (Array<T>&& other) {
        memory_span = other.memory_span;
        size = other.size;
        allocated_size = other.allocated_size;
        other.memory_span = nullptr;
        other.size = 0;
        other.allocated_size = 0;
        return *this;
    }

    inline int length() {
        return size;
    }

    inline T& operator[] (int index) {
        return memory_span[index];
    }

    T& add(T const& elem) {
        ensure_size(size + 1);
        return memory_span[size++] = elem;
    }

    void resize(int size) {
        if (size > this->size) {
            ensure_size(size);
            this->size = size;
        } else {
            if (size == 0) {
                clear();
            } else {
                this->size = size;
            }
        }
    }

    bool contains(T const& value) {
        for (int i = 0; i < size; i++) {
            if (memory_span[i] == value) {
                return true;
            }
        }
        return false;
    }

    bool any(std::function<bool(T const&)> check) {
        for (int i = 0; i < size; i++) {
            if (check(memory_span[i])) {
                return true;
            }
        }
        return false;
    }

    bool all(std::function<bool(T const&)> check) {
        for (int i = 0; i < size; i++) {
            if (!check(memory_span[i])) {
                return false;
            }
        }
        return true;
    }

    void clear() {
        if (memory_span != nullptr) {
            delete[] (memory_span);
            memory_span = nullptr;
        }
        size = allocated_size = 0;
    }

    ~Array() {
        clear();
    }
};

#endif
