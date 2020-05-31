#include <string>


#ifndef LAB1_LIST_H
#define LAB1_LIST_H

template <typename T>
class List {
public:
    class Node {
    public:
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;

        Node() {};
        Node(T const& v) : value(v) {}
    };

    class Iterator {
    public:
        Node* node = nullptr;

        Iterator() {}
        Iterator(Node* n) : node(n) {}

        Iterator operator++(int) {
            Node* last = node;
            node = node->next;
            return Iterator(last);
        }

        Iterator operator--(int) {
            Node* last = node;
            node = node->prev;
            return Iterator(last);
        }

        T& operator*() {
            return node->value;
        }

        T* operator->() {
            return &node->value;
        }

        bool operator==(Iterator const& it) {
            return it.node == node;
        }

        bool operator!=(Iterator const& it) {
            return it.node != node;
        }
    };

private:
    Node* first = nullptr;
    Node* last = nullptr;
    Node _end;
    int length = 0;

public:
    Iterator begin() {
        return first != nullptr ? Iterator(first) : end();
    }

    Iterator end() {
        return Iterator(&_end);
    }

    Iterator add(T const& value) {
        Node* node = new Node(value);
        if (last != nullptr) {
            last->next = node;
            node->prev = last;
        } else {
            first = last = node;
        }
        _end.prev = node;
        node->next = &_end;
        last = node;
        length++;
        return Iterator(node);
    }

    void erase(Iterator iterator) {
        Node* node = iterator.node;
        if (node->next == &_end) {
            last = _end.prev = node->prev;
        }
        if (node->prev == nullptr) {
            first = node->next;
        } else {
            node->prev->next = node->next;
        }
        node->next->prev = node->prev;
        length--;
        delete(node);
    }

    void print() {
        std::cout << "[";
        for (auto it = begin(); it != end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << "]";
    }

    void clear() {
        Node* node = first;
        while (node != nullptr && node != &_end) {
            Node* next = node->next;
            delete(node);
            node = next;
        }
        _end.next = _end.prev = nullptr;
        first = last = nullptr;
        length = 0;
    }

    int get_length() {
        return length;
    }

    ~List() {
        clear();
    }
};

#endif //LAB1_LIST_H