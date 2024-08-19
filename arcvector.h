#ifndef ARC_VECTOR_WRAPPER_H
#define ARC_VECTOR_WRAPPER_H

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <algorithm>

template <typename T>
class ArcVector {
public:
    // Constructors
    ArcVector();
    ArcVector(size_t size);
    ArcVector(const ArcVector& other);
    ArcVector(ArcVector&& other) noexcept;

    // Destructor
    ~ArcVector();

    // Assignment operators
    ArcVector& operator=(const ArcVector& other);
    ArcVector& operator=(ArcVector&& other) noexcept;

    // Element access
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // Size and capacity
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    // Modifiers
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void clear();

private:
    T* begin_;             // Pointer to the beginning of the allocated memory
    T* end_;               // Pointer to the end of the allocated memory
    T* allocationBegin_;   // Pointer to the start of allocated space
    T* allocationEnd_;     // Pointer to the end of allocated space
    size_t size_;          // Current size of the vector
    size_t capacity_;      // Current capacity of the vector

    void reallocate(size_t new_capacity);
};

// Implementation

template <typename T>
ArcVector<T>::ArcVector() 
    : begin_(nullptr), end_(nullptr), allocationBegin_(nullptr), allocationEnd_(nullptr), size_(0), capacity_(0) {}

template <typename T>
ArcVector<T>::ArcVector(size_t size) 
    : begin_(new T[size]), end_(begin_ + size), allocationBegin_(begin_), allocationEnd_(begin_ + size), size_(size), capacity_(size) {}

template <typename T>
ArcVector<T>::ArcVector(const ArcVector& other)
    : begin_(new T[other.capacity_]), end_(begin_ + other.size_), allocationBegin_(begin_), allocationEnd_(begin_ + other.capacity_), size_(other.size_), capacity_(other.capacity_) {
    std::copy(other.begin_, other.end_, begin_);
}

template <typename T>
ArcVector<T>::ArcVector(ArcVector&& other) noexcept
    : begin_(other.begin_), end_(other.end_), allocationBegin_(other.allocationBegin_), allocationEnd_(other.allocationEnd_), size_(other.size_), capacity_(other.capacity_) {
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.allocationBegin_ = nullptr;
    other.allocationEnd_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
ArcVector<T>::~ArcVector() {
    delete[] begin_;
}

template <typename T>
ArcVector<T>& ArcVector<T>::operator=(const ArcVector& other) {
    if (this != &other) {
        T* new_data = new T[other.capacity_];
        std::copy(other.begin_, other.end_, new_data);
        delete[] begin_;
        begin_ = new_data;
        end_ = begin_ + other.size_;
        allocationBegin_ = begin_;
        allocationEnd_ = begin_ + other.capacity_;
        size_ = other.size_;
        capacity_ = other.capacity_;
    }
    return *this;
}

template <typename T>
ArcVector<T>& ArcVector<T>::operator=(ArcVector&& other) noexcept {
    if (this != &other) {
        delete[] begin_;
        begin_ = other.begin_;
        end_ = other.end_;
        allocationBegin_ = other.allocationBegin_;
        allocationEnd_ = other.allocationEnd_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.begin_ = nullptr;
        other.end_ = nullptr;
        other.allocationBegin_ = nullptr;
        other.allocationEnd_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
T& ArcVector<T>::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return begin_[index];
}

template <typename T>
const T& ArcVector<T>::operator[](size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return begin_[index];
}

template <typename T>
size_t ArcVector<T>::size() const noexcept {
    return size_;
}

template <typename T>
size_t ArcVector<T>::capacity() const noexcept {
    return capacity_;
}

template <typename T>
void ArcVector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    *end_++ = value;
    ++size_;
}

template <typename T>
void ArcVector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    *end_++ = std::move(value);
    ++size_;
}

template <typename T>
void ArcVector<T>::pop_back() {
    if (size_ > 0) {
        --end_;
        --size_;
    }
}

template <typename T>
void ArcVector<T>::clear() {
    end_ = begin_;
    size_ = 0;
}

template <typename T>
void ArcVector<T>::reallocate(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    T* new_end = std::uninitialized_copy(begin_, end_, new_data);
    delete[] begin_;
    begin_ = new_data;
    end_ = new_end;
    allocationBegin_ = begin_;
    allocationEnd_ = begin_ + new_capacity;
    capacity_ = new_capacity;
}

#endif // ARC_VECTOR_WRAPPER_H