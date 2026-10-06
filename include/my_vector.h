#pragma once

#include <cstddef>    
#include <stdexcept>  
#include <utility>     
#include <algorithm>   

// Шаблонный класс T — тип хранимых элементов (будет int)
template<typename T>
class MyVector {
public:
    //Конструкторы / деструктор 

    MyVector()
        : m_data(nullptr), m_size(0), m_capacity(0) {}

    // Конструктор копирования 
    MyVector(const MyVector& other)
        : m_data(nullptr), m_size(0), m_capacity(0) {
        reserve(other.m_size);
        for (size_t i = 0; i < other.m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
        m_size = other.m_size;
    }

    // Move-конструктор — забирает буфер у other
    MyVector(MyVector&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    ~MyVector() {
        delete[] m_data;
    }

    //Операторы присваивания

    // Копирующее присваивание
    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this;   // защита от самоприсваивания

        MyVector tmp(other);  
        swap(tmp); 
        return *this;  // tmp уничтожится и освободит старый буфер
    }

    // Перемещающее присваивание
    MyVector& operator=(MyVector&& other) noexcept {
        if (this == &other) return *this;

        delete[] m_data;  // освобождаем свой буфер

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    // Интерфейс

    void push_back(const T& value) {
        if (m_size == m_capacity) {
            size_t new_cap = (m_capacity == 0) ? 1 : (m_capacity * 3 + 1) / 2;
            reserve(new_cap);
        }
        m_data[m_size++] = value;
    }

    void insert(size_t pos, const T& value) {
        if (pos > m_size) {
            throw std::out_of_range("MyVector::insert: pos > size");
        }
        if (m_size == m_capacity) {
            size_t new_cap = (m_capacity == 0) ? 1 : (m_capacity * 3 + 1) / 2;
            reserve(new_cap);
        }
        // Сдвигаем элементы [pos, size) на один вправо
        for (size_t i = m_size; i > pos; --i) {
            m_data[i] = m_data[i - 1];
        }
        m_data[pos] = value;
        ++m_size;
    }

    void erase(size_t pos) {
        if (pos >= m_size) {
            throw std::out_of_range("MyVector::erase: pos >= size");
        }
        for (size_t i = pos; i + 1 < m_size; ++i) {
            m_data[i] = m_data[i + 1];
        }
        --m_size;
    }

    // Размер
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    // Доступ по индексу
    T& operator[](size_t i) { return m_data[i]; }
    const T& operator[](size_t i) const { return m_data[i]; }

    // Метод at() — с проверкой границ
    T& at(size_t i) {
        if (i >= m_size) throw std::out_of_range("MyVector::at");
        return m_data[i];
    }

    // Итератор

    struct iterator {
        T* ptr;

        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }

        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator t = *this; ++ptr; return t; }

        bool operator==(const iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const iterator& o) const { return ptr != o.ptr; }
    };
    //Const-итератор 
    struct const_iterator {
        const T* ptr;

        const T& operator*() const { return *ptr; }
        const T* operator->() const { return ptr; }

        const_iterator& operator++() { ++ptr; return *this; }
        const_iterator operator++(int) { const_iterator t = *this; ++ptr; return t; }

        bool operator==(const const_iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const const_iterator& o) const { return ptr != o.ptr; }
    };

    iterator begin() { return iterator{m_data}; }
    iterator end()   { return iterator{m_data + m_size}; }

    const_iterator begin() const { return const_iterator{m_data}; }
    const_iterator end()   const { return const_iterator{m_data + m_size}; }

private:
    T* m_data;          // Буфер
    size_t m_size;      // Сколько элементов сейчас
    size_t m_capacity;  // Сколько влезет без перевыделения

    // Выделять память под new_capacity элементов
    void reserve(size_t new_capacity) {
        if (new_capacity <= m_capacity) return;

        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

    // Обмен содержимым с другим вектором
    void swap(MyVector& other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }
};