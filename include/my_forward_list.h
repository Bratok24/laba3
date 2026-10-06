#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>

// Однонаправленный список — каждый узел хранит только указатель на следующий.
template<typename T>
class MyForwardList {
public:
    struct Node {
        T data;
        Node* next;
    };

    MyForwardList() : m_head(nullptr), m_size(0) {}

    // Копирующий конструктор — глубокая копия
    MyForwardList(const MyForwardList& other)
        : m_head(nullptr), m_size(0) {
        Node* tail = nullptr;
        for (Node* cur = other.m_head; cur != nullptr; cur = cur->next) {
            Node* node = new Node{cur->data, nullptr};
            if (!m_head) m_head = node;
            else         tail->next = node;
            tail = node;
            ++m_size;
        }
    }

    // Move-конструктор — забираем указатель на head
    MyForwardList(MyForwardList&& other) noexcept
        : m_head(other.m_head), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_size = 0;
    }

    ~MyForwardList() { clear(); }

    MyForwardList& operator=(const MyForwardList& other) {
        if (this == &other) return *this;
        MyForwardList tmp(other);
        swap(tmp);
        return *this;
    }

    MyForwardList& operator=(MyForwardList&& other) noexcept {
        if (this == &other) return *this;
        clear();
        m_head = other.m_head;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_size = 0;
        return *this;
    }

    // Добавить в конец (проход до последнего)
    void push_back(const T& value) {
        Node* node = new Node{value, nullptr};
        if (!m_head) {
            m_head = node;
        } else {
            Node* cur = m_head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++m_size;
    }

    // Вставить в позицию pos
    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("MyForwardList::insert");

        if (pos == 0) {
            Node* node = new Node{value, m_head};
            m_head = node;
            ++m_size;
            return;
        }
        // Ищем узел до позиции вставки
        Node* prev = m_head;
        for (size_t i = 0; i < pos - 1; ++i) prev = prev->next;
        Node* node = new Node{value, prev->next};
        prev->next = node;
        ++m_size;
    }

    // Удалить элемент в позиции pos
    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("MyForwardList::erase");

        if (pos == 0) {
            Node* old = m_head;
            m_head = m_head->next;
            delete old;
            --m_size;
            return;
        }
        Node* prev = m_head;
        for (size_t i = 0; i < pos - 1; ++i) prev = prev->next;
        Node* old = prev->next;
        prev->next = old->next;
        delete old;
        --m_size;
    }

    size_t size() const { return m_size; }

    // Доступ по индексу — через проход по списку
    T& operator[](size_t i) { return node_at(i)->data; }
    const T& operator[](size_t i) const { return node_at(i)->data; }

    T& at(size_t i) {
        if (i >= m_size) throw std::out_of_range("MyForwardList::at");
        return node_at(i)->data;
    }

    // Итератор
    struct iterator {
        Node* ptr;

        T& operator*() const { return ptr->data; }
        T* operator->() const { return &ptr->data; }
        T& get() const { return ptr->data; }

        iterator& operator++() { ptr = ptr->next; return *this; }
        iterator operator++(int) { iterator t = *this; ++ptr; return t; }

        bool operator==(const iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const iterator& o) const { return ptr != o.ptr; }
    };

    struct const_iterator {
        const Node* ptr;

        const T& operator*() const { return ptr->data; }
        const T* operator->() const { return &ptr->data; }
        const T& get() const { return ptr->data; }

        const_iterator& operator++() { ptr = ptr->next; return *this; }
        const_iterator operator++(int) { const_iterator t = *this; ++ptr; return t; }

        bool operator==(const const_iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const const_iterator& o) const { return ptr != o.ptr; }
    };

    iterator begin() { return iterator{m_head}; }
    iterator end()   { return iterator{nullptr}; }

    const_iterator begin() const { return const_iterator{m_head}; }
    const_iterator end()   const { return const_iterator{nullptr}; }

private:
    Node* m_head;
    size_t m_size;

    Node* node_at(size_t pos) const {
        if (pos >= m_size) throw std::out_of_range("MyForwardList::node_at");
        Node* cur = m_head;
        for (size_t i = 0; i < pos; ++i) cur = cur->next;
        return cur;
    }

    void clear() {
        while (m_head) {
            Node* next = m_head->next;
            delete m_head;
            m_head = next;
        }
        m_size = 0;
    }

    void swap(MyForwardList& other) noexcept {
        std::swap(m_head, other.m_head);
        std::swap(m_size, other.m_size);
    }
};