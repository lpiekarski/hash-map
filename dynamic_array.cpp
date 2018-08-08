/** @file       dynamic_array.cpp
    @brief      Dynamic array implementation
    @author     £ukasz Piekarski <wookashp98@gmail.com>
    @date       05.05.2018
*/

#include "dynamic_array.h"

template<class T>
DynamicArray<T>::DynamicArray(void) {
    this->arr = nullptr;
    this->sz = 0;
    this->arr_size = 0;
}

template<class T>
void DynamicArray<T>::push_back(T const *el) {
    if (this->sz + 1 > this->arr_size) {
        if (this->arr_size == 0) {
            this->arr = new T const*[DynamicArray::SIZE_UP_SCALE];
            this->arr_size = DynamicArray::SIZE_UP_SCALE;
        }
        else {
            size_t new_size = DynamicArray::SIZE_UP_SCALE * this->arr_size;
            T const** new_arr = new T const**[new_size];
            for (size_t i = 0; i < this->sz; i++)
                new_arr[i] = this->arr[i];
            delete [] this->arr;
            this->arr = new_arr;
            this->arr_size = new_size;
        }
    }
    this->arr[this->sz] = el;
    this->sz++;
}

template<class T>
T const * DynamicArray<T>::remove(size_t idx) {
    if (idx >= this->sz)
        return nullptr;
    T const *res = this->arr[idx];
    if (SIZE_DOWN_TRIGGER * (this->sz - 1) < this->arr_size) {
        size_t new_size = this->arr_size / DynamicArray::SIZE_DOWN_SCALE;
        if (new_size != 0) {
            T const** new_arr = new T const*[new_size];
            for (size_t i = 0; i < this->sz; i++)
                new_arr[i] = this->arr[i];
            delete [] this->arr;
            this->arr = new_arr;
            this->arr_size = new_size;
        }
        else {
            delete [] this->arr;
            this->arr = nullptr;
            this->arr_size = new_size;
        }
    }
    for (size_t i = idx + 1; i < this->sz; i++) {
        this->arr[i - 1] = this->arr[i];
    }
    this->sz--;
    return res;
}

template<class T>
T const * DynamicArray<T>::operator[](size_t idx) {
    if (idx >= this->sz)
        return nullptr;
    return this->arr[idx];
}

template<class T>
void DynamicArray<T>::set(size_t idx, T const *val) {
    if (this == nullptr || idx >= this->sz)
        return;
    this->arr[idx] = val;
}

template<class T>
size_t DynamicArray<T>::size() {
    return this->sz;
}

template<class T>
DynamicArray<T>::~DynamicArray() {
    if (this->arr != NULL)
        delete [] this->arr;
}

template<class T>
DynamicArray<T> DynamicArray<T>::operator=(DynamicArray<T> const & d) {
    delete [] this->arr;
    this->arr = new T const *[d.arr_size];
    for (size_t i = 0; i < d.arr_size; i++)
        this->arr[i] = d.arr[i];
    this->sz = d.sz;
    this->arr_size = d.arr_size;
    return this;
}
