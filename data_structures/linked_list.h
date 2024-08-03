#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "wet2util.h"
#include <iostream>

template <typename T>
class List {
public:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
    ~List();

    output_t<Node*> add(const T& value);
    output_t<T> first() const;
    StatusType remove(Node* node);
    void printList() const;
    int size() const {
        return m_size;
    }

    // Overload the << operator for Key
    friend std::ostream& operator<<(std::ostream& os, const List& key) {
        os << "List: [";
        Node* iterator = key.m_head;
        if (iterator == nullptr) {
            os << "Empty; ]";
            return os;
        }
        while (iterator!=nullptr) {
            os << " ";
            os << *(iterator->data) << ";";
            iterator=iterator->next;
        }
        os << " ] ";
        return os;
    }
private:
    Node* m_head;
    Node* m_tail;
    int m_size;
};

template <typename T>
List<T>::~List() {
    Node* current = m_head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
output_t<typename List<T>::Node*> List<T>::add(const T& value) {
    try {
        Node* newNode = new Node(value);
        if (m_head == nullptr) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            newNode->prev = m_tail;
            m_tail = newNode;
        }
        ++m_size;
        return newNode;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

template <typename T>
output_t<T> List<T>::first() const {
    if (m_head == nullptr) return StatusType::FAILURE;
    return m_head->data;
}

template <typename T>
StatusType List<T>::remove(Node* node) {
    if (node == nullptr) return StatusType::FAILURE;
    try {
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            m_head = node->next;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            m_tail = node->prev;
        }

        delete node;
        --m_size;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template <typename T>
void List<T>::printList() const {
    Node* current = m_head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

#endif // DOUBLE_LINKED_LIST_H
