#pragma once

#include <cstddef>      
#include <stdexcept>    

// Шаблонный двунаправленный список
template<typename T>
class MyList {
public:
    //Узел списка 
    struct Node {
        T data;      // Пользовательские данные
        Node* next;  // Указатель на следующий
        Node* prev;  // Указатель на предыдущий
    };

    // Конструкторы / деструктор 

    MyList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    // Копирующий конструктор это глубокая копия всех узлов
    MyList(const MyList& other)
        : m_head(nullptr), m_tail(nullptr), m_size(0) {
        for (Node* cur = other.m_head; cur != nullptr; cur = cur->next) {
            push_back(cur->data);
        }
    }

    // Move-конструктор забирает указатели на head/tail
    MyList(MyList&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    ~MyList() {
        clear();
    }

    // Операторы присваивания

    // Копирующее присваивание
    MyList& operator=(const MyList& other) {
        if (this == &other) return *this;
        MyList tmp(other);  // копируем
        swap(tmp);   // меняемся
        return *this;  // tmp уничтожится и освободит старые узлы
    }

    // Перемещающее присваивание
    MyList& operator=(MyList&& other) noexcept {
        if (this == &other) return *this;
        clear(); // освобождаем свои узлы

        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;

        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
        return *this;
    }

    // Интерфейс 

    void push_back(const T& value) {
        Node* node = new Node{value, nullptr, m_tail};
        if (m_tail) {
            m_tail->next = node;
        } else {
            m_head = node;  
        }
        m_tail = node;
        ++m_size;
    }

    // Вставить в позицию pos 
    void insert(size_t pos, const T& value) {
        if (pos > m_size) throw std::out_of_range("MyList::insert");

        if (pos == m_size) {       
            push_back(value);
            return;
        }
        if (pos == 0) {          
            Node* node = new Node{value, m_head, nullptr};
            m_head->prev = node;
            m_head = node;
            ++m_size;
            return;
        }
        Node* cur = node_at(pos);
        Node* node = new Node{value, cur, cur->prev};
        cur->prev->next = node;
        cur->prev = node;
        ++m_size;
    }

    // Удалить элемент в позиции pos
    void erase(size_t pos) {
        if (pos >= m_size) throw std::out_of_range("MyList::erase");

        Node* cur = node_at(pos);

        if (cur->prev) cur->prev->next = cur->next;
        else           m_head = cur->next;   // удаляем первый

        if (cur->next) cur->next->prev = cur->prev;
        else           m_tail = cur->prev;   // удаляем последний

        delete cur;
        --m_size;
    }

    // Размер
    size_t size() const { return m_size; }

    // Доступ по индексу
    T& operator[](size_t i) { return node_at(i)->data; }
    const T& operator[](size_t i) const { return node_at(i)->data; }

    T& at(size_t i) {
        if (i >= m_size) throw std::out_of_range("MyList::at");
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
        iterator& operator--() { ptr = ptr->prev; return *this; }

        bool operator==(const iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const iterator& o) const { return ptr != o.ptr; }
    };

    //Const-итератор
    struct const_iterator {
        const Node* ptr;

        const T& operator*() const { return ptr->data; }
        const T* operator->() const { return &ptr->data; }
        const T& get() const { return ptr->data; }

        const_iterator& operator++() { ptr = ptr->next; return *this; }
        const_iterator operator++(int) { const_iterator t = *this; ++ptr; return t; }
        const_iterator& operator--() { ptr = ptr->prev; return *this; }

        bool operator==(const const_iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const const_iterator& o) const { return ptr != o.ptr; }
    };

    iterator begin() { return iterator{m_head}; }
    iterator end()   { return iterator{nullptr}; }

    const_iterator begin() const { return const_iterator{m_head}; }
    const_iterator end()   const { return const_iterator{nullptr}; }

private:
    Node* m_head;
    Node* m_tail;
    size_t m_size;

    // Найти узел по индексу (проходом по списку)
    Node* node_at(size_t pos) const {
        if (pos >= m_size) throw std::out_of_range("MyList::node_at");
        Node* cur = m_head;
        for (size_t i = 0; i < pos; ++i) cur = cur->next;
        return cur;
    }

    // Удалить все узлы
    void clear() {
        while (m_head) {
            Node* next = m_head->next;
            delete m_head;
            m_head = next;
        }
        m_tail = nullptr;
        m_size = 0;
    }

    void swap(MyList& other) noexcept {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        std::swap(m_size, other.m_size);
    }
};