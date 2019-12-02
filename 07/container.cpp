#include <iostream>
#include <iterator>
#include <vector>
#include <memory>
#include <cassert>
#include <utility>
#include <cstring>
#include <algorithm>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    inline pointer allocate(size_type n) {
        return reinterpret_cast<pointer>(operator new(n * sizeof(value_type)));
    }

    inline void deallocate(pointer p) {
       operator delete(p);
    }

    inline void destroy(pointer p) {
        p->~value_type();
    }

    inline void construct(pointer p, value_type&& val) {
        new (p) value_type(val);
    }
};

template <class T>
class Iterator : public std::iterator<std::forward_iterator_tag, T> {
public:
    using reference = T&;
    using pointer = T*;

    explicit Iterator(pointer ptr) : ptr_(ptr) {}

    bool operator==(const Iterator<T>& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return *ptr_;
    }

    Iterator& operator++() {
        ++ptr_;
        return *this;
    }

private:
    pointer ptr_;
};

template <class T>
class ReverseIterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
    using reference = T&;
    using pointer = T*;

    explicit ReverseIterator(pointer ptr) : ptr_(ptr) {}

    bool operator==(const ReverseIterator<T>& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const ReverseIterator<T>& other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return *ptr_;
    }

    ReverseIterator& operator++() {
        --ptr_;
        return *this;
    }
    
private:
    pointer ptr_;
};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using reverse_iterator = ReverseIterator<T>;

    Vector(size_type size = 0) : size_(size), capacity_(size) {
        data_ = alloc_.allocate(size);
    }

    ~Vector() {
        std::for_each(data_, data_ + size_, 
                [this](value_type& a){ alloc_.destroy(&a); });
    }

    iterator begin() noexcept {
        return iterator(data_);
    }

    iterator end() noexcept {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(data_ + size_ - 1);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(data_ - 1);
    }

    reference operator[](size_type i) {
        return *(iterator(data_ + i));
    }

    const_reference operator[](size_type i) const {
        return *(iterator(data_ + i));
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void resize(size_type n) {
        if (size_ > n) {
            std::for_each(data_ + n, data_ + size_, 
                    [this](value_type& a){ alloc_.destroy(&a); });
            size_ = n;
            return;
        }
        if (size_ < n) {
            if (capacity_ <= n) {
                pointer newData = alloc_.allocate(n);
                for (int i = 0; i < size_; ++i) {
                    alloc_.construct(newData + i, std::move(*(data_ + i)));
                }
                alloc_.deallocate(data_);
                data_ = newData;
                capacity_ = n;
            }
            std::for_each(data_ + size_, data_ + n, [this](value_type& a) {
                alloc_.construct(&a, std::move(value_type{})); 
            });
            size_ = n;
        }
    }

    void reserve(size_type n) {
        if (n > capacity_) {
            pointer newData = alloc_.allocate(n);
            for (int i = 0; i < size_; ++i) {
                alloc_.construct(newData + i, std::move(*(data_ + i)));
            }
            alloc_.deallocate(data_);
            data_ = newData;
            capacity_ = n;
        }
    }

    void clear() {
        std::for_each(data_, data_ + size_, 
                [this](value_type& a){ alloc_.destroy(&a); });
        size_ = 0;
    }

    void push_back(const_reference value) {
        if (capacity_ <= size_) {
            size_type newCap = 2 * capacity_ + 1;
            pointer newData = alloc_.allocate(newCap);
            for (int i = 0; i < size_; ++i) {
                alloc_.construct(newData + i, std::move(*(data_ + i)));
            }
            alloc_.deallocate(data_);
            data_ = newData;
            capacity_ = newCap;
        }
        alloc_.construct(data_ + size_, std::move(value_type{value}));
        ++size_;
    }

    void pop_back() {
        pointer ptr = data_ + size_ - 1;
        alloc_.destroy(ptr);
        --size_;
    }

private:
    pointer data_;
    size_type size_;
    size_type capacity_;
    Alloc alloc_;
};


int main() {
    Vector<int> v;
    assert(v.empty());
    assert(v.size() == 0);

    v.push_back(1);

    assert(!v.empty());
    assert(v.size() == 1);
    assert(v[0] == 1);

    v.pop_back();

    assert(v.empty());
    assert(v.size() == 0);

    v.push_back(3);
    v.push_back(2);
    v.push_back(1);

    assert(!v.empty());
    assert(v.size() == 3);
    assert(v[0] == 3);
    assert(v[1] == 2);
    assert(v[2] == 1);

    auto r = v.rbegin();
    
    assert(r != v.rend());
    assert(*r == 1);
    ++r;
    assert(r != v.rend());
    assert(*r == 2);
    ++r;
    assert(r != v.rend());
    assert(*r == 3);
    ++r;
    assert(r == v.rend());

    auto f = v.begin();
    assert(f != v.end());
    assert(*f == 3);
    ++f;
    assert(f != v.end());
    assert(*f == 2);
    ++f;
    assert(f != v.end());
    assert(*f == 1);
    ++f;
    assert(f == v.end());

    v.reserve(10000);
    assert(v.size() == 3);
    assert(v.capacity() >= 10000);

    const auto c = v.capacity();

    v.resize(2);
    assert(v.size() == 2);
    assert(v.capacity() == c);
    assert(v[0] == 3);
    assert(v[1] == 2);

    v.resize(3);

    assert(v.size() == 3);
    assert(v.capacity() == c);
    assert(v[0] == 3);
    assert(v[1] == 2);
    assert(v[2] == 0);

    v.resize(0);
    assert(v.size() == 0);
    assert(v.begin() == v.end());

    v.resize(2);
    assert(v.size() == 2);
    assert(v[0] == 0);
    assert(v[1] == 0);


    Vector<std::string> s;
    s.push_back("sdfasd");
    
    auto b = s.begin();
    assert(*b == "sdfasd");
    s.resize(2);
    b = s.begin();
    ++b;
    assert(*b == "");
    return 0;
}
