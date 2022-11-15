#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>

#include "array_ptr.h"

using namespace std;

//Вспомогательный класс
class ReserveProxyObj {
public:
    ReserveProxyObj(size_t new_capacity) : capacity_(new_capacity) {

    }
    size_t GetCapacity() const noexcept {
        return capacity_;
    }
private:
    size_t capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : size_(size), capacity_(size), simp_vec(size) {
        fill(simp_vec.Get(), simp_vec.Get() + size, Type());
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size), simp_vec(size) {
        fill(simp_vec.Get(), simp_vec.Get() + size, value);
    }
    
    // Создает вектор заранее заданной емкости с помощью вспомогательного класса обертки
    SimpleVector(const ReserveProxyObj& some_object) {
        Reserve(some_object.GetCapacity());
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : size_(init.size()), capacity_(init.size()), simp_vec(init.size()) {
        copy(init.begin(), init.end(), simp_vec.Get());
    }

    // Конструктор копирования
    SimpleVector(const SimpleVector& other) {
        SimpleVector<Type> tmp(other.GetSize());
        copy(other.begin(), other.end(), tmp.begin());
        swap(tmp);
    }

    SimpleVector(SimpleVector&& other) noexcept {
        simp_vec.swap(other.simp_vec);
        size_ = exchange(other.size_, 0);
        capacity_ = exchange(other.capacity_, 0);
    }

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (simp_vec.Get() == other.simp_vec.Get())
            return *this;
        
        simp_vec.swap(other.simp_vec);
        size_ = exchange(other.size_, 0);
        capacity_ = exchange(other.capacity_, 0);
        return *this;
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this == &rhs)
            return *this;

        SimpleVector<Type> tmp(rhs);
        swap(tmp);

        return *this;
    }    

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return simp_vec[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return simp_vec[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)
            throw out_of_range("Index is out of range"s);
        return simp_vec[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)
            throw out_of_range("Index is out of range"s);
        return simp_vec[index];
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }    

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return simp_vec.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return simp_vec.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return simp_vec.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return simp_vec.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return simp_vec.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return simp_vec.Get() + size_;
    }
    
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size > size_ && new_size <= capacity_) {
            for (size_t i = size_; i < new_size; ++i) {
                simp_vec[i] = move(Type());
            }
        }
        else if (new_size > capacity_) {
            size_t new_capacity = max(capacity_ * 2, new_size);
            SimpleVector<Type> tmp(new_capacity);
            move(simp_vec.Get(), simp_vec.Get() + size_, tmp.begin());
            simp_vec.swap(tmp.simp_vec);
            capacity_ = new_capacity;
        }
        size_ = new_size;
    }

    //Задает емкость вектора
    //Резервирует нужное количество памяти
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            SimpleVector<Type> tmp(new_capacity);
            move(simp_vec.Get(), simp_vec.Get() + size_, tmp.begin());

            simp_vec.swap(tmp.simp_vec);
            capacity_ = exchange(new_capacity, 0);
        }
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_ < capacity_) {
            simp_vec[size_] = item;
        }
        else {
            if (capacity_ == 0) {
                ++capacity_;
            }
            size_t new_capacity = capacity_ * 2;
            SimpleVector<Type> tmp(new_capacity);

            copy(simp_vec.Get(), simp_vec.Get() + size_, tmp.begin());
            tmp[size_] = item;

            simp_vec.swap(tmp.simp_vec);
            capacity_ = new_capacity;
        }
        ++size_;
    }

    void PushBack(Type&& item) {
        if (size_ < capacity_) {
            simp_vec[size_] = move(item);
        }
        else {
            if (capacity_ == 0) {
                ++capacity_;
            }
            size_t new_capacity = capacity_ * 2;
            SimpleVector<Type> tmp(new_capacity);
            move(simp_vec.Get(), simp_vec.Get() + size_, tmp.begin());
            tmp[size_] = move(item);

            simp_vec.swap(tmp.simp_vec);
            capacity_ = exchange(new_capacity, 0);
        }
        ++size_;
    }
    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        if ((pos < simp_vec.Get()) || (pos > simp_vec.Get() + size_))
            throw out_of_range("This position is out of range"s);
        
        auto position = pos - simp_vec.Get();

        if (size_ < capacity_) {
            copy_backward(simp_vec.Get() + position, simp_vec.Get() + size_, simp_vec.Get() + size_ + 1);
            simp_vec[position] = value;
        }
        else {
            if (capacity_ == 0) {
                ++capacity_;
            }
            size_t new_capacity = capacity_ * 2;
            SimpleVector<Type> tmp(new_capacity);

            copy(simp_vec.Get(), simp_vec.Get() + position, tmp.begin());
            tmp[position] = value;

            copy(simp_vec.Get() + position, simp_vec.Get() + size_, tmp.begin() + position + 1);
            simp_vec.swap(tmp.simp_vec);
            capacity_ = new_capacity;
        }
        ++size_;
        return simp_vec.Get() + position;
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        if ((pos < simp_vec.Get()) || (pos > simp_vec.Get() + size_))
            throw out_of_range("This position is out of range"s);

        auto position = pos - simp_vec.Get();

        if (size_ < capacity_) {
            move_backward(simp_vec.Get() + position, simp_vec.Get() + size_, simp_vec.Get() + size_ + 1);
            simp_vec[position] = move(value);
        }
        else {
            if (capacity_ == 0) {
                ++capacity_;
            }
            size_t new_capacity = capacity_ * 2;
            SimpleVector<Type> tmp(new_capacity);

            move(simp_vec.Get(), simp_vec.Get() + position, tmp.begin());
            tmp[position] = move(value);

            move(simp_vec.Get() + position, simp_vec.Get() + size_, tmp.begin() + position + 1);
            simp_vec.swap(tmp.simp_vec);
            capacity_ = exchange(new_capacity, 0);
        }
        ++size_;
        return simp_vec.Get() + position;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!IsEmpty())
            --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (IsEmpty() || ((pos < simp_vec.Get()) || (pos > simp_vec.Get() + size_)))
            throw out_of_range("This position is out of range or vector is empty"s);

        auto position = pos - simp_vec.Get();
        move(simp_vec.Get() + position + 1, simp_vec.Get() + size_, simp_vec.Get() + position);
        --size_;

        return simp_vec.Get() + position;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        simp_vec.swap(other.simp_vec);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> simp_vec;
    Type def_value = Type();
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return ((lhs < rhs) || (lhs == rhs));
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return ((rhs < lhs) || (rhs == lhs));
}
