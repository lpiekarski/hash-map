/** @file       hash_map.cuh
    @brief      Hash map interface.
    @author     £ukasz Piekarski <wookashp98@gmail.com>
    @date       03.08.2018
*/

#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include "dynamic_array.h"

/** @brief          Hash map class.
    @param T_key    Key data type.
    @param T_value  Stored values data type.
                    HashMap stores const pointers of that type as values.
*/
template < class T_key, class T_value >
class HashMap {
private:
    /** @brief      Pointer to a dynamic array containing dynamic arrays of keys.
    */
    DynamicArray < DynamicArray < T_key > > * keys;

    /** @brief      Pointer to a dynamic array containing dynamic arrays of values.
    */
    DynamicArray < DynamicArray < T_value > > * values;

    /** @brief      Number of bindings in the hash map.
    */
    size_t elements_num;

    /** @brief      Pointer to a hash function.
    */
    size_t (* hash_func) (T_key const *, size_t);

    /** @brief      Pointer to a key comparator.
    */
    int (* key_cmp) (T_key const *, T_key const *);
public:

    /** @brief      Size of the @p keys and @p values arrays in the newly created hash map.
    */
    static size_t const DEFAULT_SIZE = 1;

    /** @brief      Coefficient by which the @p values and @p keys arrays expand when needed.
    */
    static size_t const SIZE_UP_SCALE = 2;

    /** @brief      If there is less than one per @p SIZE_DOWN_TRIGGER elements in @p keys and @p values arrays
                    they will shrink given opportunity.
    */
    static size_t const SIZE_DOWN_TRIGGER = 4;

    /** @brief      Coefficient by which the @p keys and @p values arrays shrink when needed.
    */
    static size_t const SIZE_DOWN_SCALE = 2;


    /** @brief      Creates a new hash map.
                    Hash numbers in that map are created by @p hash_func function.
        @param key_comp
                    Pointer of the key comparator function.
        @param hash_func
                    Pointer of the key hashing function.
    */
    HashMap(int (* key_comp) (T_key const *, T_key const *), size_t (* hash_func) (T_key const *, size_t));

    /** @brief      Adds a new binding.
                    Adds the <tt>(key, value)</tt> binding to the hash map.
                    If @p key has already value bound in the hash map the value is overriden with @p value.
        @param key  Key of the binding to add.
        @param value
                    Value of the binding to add.
        @return     @p false if the old value has been overriden,
                    @p true otherwise.
    */
    bool add(T_key const * key, T_value const * value);

    /** @brief      Removes the existing binding from the hash map.
                    Removes the binding with a @p key key from the hash map.
                    If there was no bindings with a @p key key function doesn't have any effect.
        @param key  Key value of the binding to remove.
        @return     @p nullptr if there was no bindings removed,
                    otherwise a key of the removed binding.
    */
    T_key const * remove(T_key const * key);

    /** @brief      Getter of a value of the given key.
                    Allows access to the values of the stored keys.
        @param key  Key of the binding from which we want to get a value.
        @return     Pointer to value of the binding with @p key key or
                    @p nullptr if there is no binding with such key present in the hash map.
    */
    T_value const * operator[](T_key const * key) const;

    /** @brief      Creates a dynamic array containing all the keys from the hash map.
                    There is no specified order in the dynamic array.
                    Dynamic array should be deleted to prevent memory leaks.
        @return     Pointer to a newly created dynamic array containing all the keys present in the hash map.
    */
    DynamicArray < T_key > * get_keys() const;

    /** @brief      Deletes the hash map.
    */
    ~HashMap();
};


#endif /* __HASH_MAP_H__ */