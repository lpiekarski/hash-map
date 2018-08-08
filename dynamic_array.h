/** @file       dynamic_array.h
    @brief      Dynamic array interface.
    @author     £ukasz Piekarski <wookashp98@gmail.com>
    @date       24.07.2018
*/

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <cstdlib>

/** @brief      Dynamic array class.
    @param T    Contained data type. Dynamic array contains 
                const pointers of that type.
*/
template<class T>
class DynamicArray {
protected:

    /** @brief      Array of const pointers to elements stored in the dynamic array structure.
    */
    T const ** arr;

    /** @brief      Number of elements in @p arr.
    */
    size_t sz;

    /** @brief      Size of @p arr.
    */
    size_t arr_size;

public:

    /** @brief      Coefficient by which the @p arr array expands if needed.
    */
    static size_t const SIZE_UP_SCALE = 2;

    /** @brief      If there is less than one per @p SIZE_DOWN_TRIGGER elements in @p arr array it will shrink.
    */
    static size_t const SIZE_DOWN_TRIGGER = 4;

    /** @brief      Coefficient by which the @p arr array shrinks if needed.
    */
    static size_t const SIZE_DOWN_SCALE = 2;

    /** @brief      Creates a new structure.
                    Creates a new DynamicArray - representation of the empty dynamic array.
    */
    DynamicArray();

    /** @brief      Adds element to the dynamic array.
                    Adds @p el to the dynamic array with
                    an index as the biggest present in the array increased by one.
       @param el    Pointer to the element to add.
    */
    void push_back(T const * el);

    /** @brief      Removes the element from array.
                    Removes the element with index @p idx from the dynamic array.
                    Elements that had greater index than the one removed have their index decreased by one.
        @param idx  Index of the element to remove.
        @return     Removed element.
    */
    T const * remove(size_t idx);

    /** @brief      Array element acquisition.
                    Acquires the element with index @p idx from the dynamic array and returns it.
                    If there is no element with index @p idx returns nullptr instead.
        @param idx  Index of the desired element.
        @return     Element with index @p idx from the dynamic array.
    */
    T const * operator[](size_t idx);

    /** @brief      Sets a value of the element with given index.
                    Changes the value of the element with index @p idx to @p val.
        @param idx  Index of the element to modify.
        @param val  Desired value of the element with index @p idx.
    */
    void set(size_t idx, T const * val);

    /** @brief      Size of the dynamic array.
                    Returns the number of elements in the dynamic array.
        @return     Number of elements in the dynamic array.
    */
    size_t size();

    /** @brief      Deletes the dynamic array.
        @remark     __host__ __device__ function.
    */
    ~DynamicArray();

    /** @brief      Dynamic array assignment.
                    Makes so that the dynamic array contains the same elements as @p d
        @param d    Dynamic array to take the elements from.
        @return     Dynamic array @p d.
    */
    DynamicArray<T> operator=(DynamicArray<T> const & d);
};

#endif /* __DYNAMIC_ARRAY_H__ */