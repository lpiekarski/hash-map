/** @file       hash_map.cu
    @brief      Hash map class implementation.
    @author     £ukasz Piekarski <wookashp98@gmail.com>
    @date       03.08.2018
*/

#include "hash_map.h"

/** @brief      Adds a binding to given key and value dynamic arrays.
                Adds or overrides (if the binding (key->value) is already present in the arrays)
                the binding (key->value) to the dynamic arrays @p keys and @p values.
    @param h    Hash map that will have binding added.
    @param values
                Dynamic array of dynamic arrays of values.
    @param keys Dynamic array of dynamic arrays of keys.
    @param key  Key to add.
    @param value
                Value to add. 
    @return     @p true if there's been created new binding,
                @p false if the old one has been overridden.
*/
template<class T_key, class T_value>
static bool add_binding(HashMap<T_key, T_value> * h, DynamicArray<DynamicArray<T_value> > * values, DynamicArray<DynamicArray<T_key> > * keys, T_key const * key, T_value const * value) {
    size_t key_hash = h->hash_func(key, keys->size());
    for (size_t i = 0; i < keys[key_hash]->size(); i++)
        if (h->key_cmp(key, keys[key_hash][i]) == 0) {
            keys[key_hash]->set(i, value);
            return false;
        }
    keys[key_hash]->push_back(key);
    values[key_hash]->push_back(value);
    return true;
}

template<class T_key, class T_value>
HashMap<T_key, T_value>::HashMap(int(*key_comp)(T_key const*, T_key const*), size_t(*hash_func)(T_key const*, size_t)) {
    this->hash_func = hash_func;
    this->key_cmp = key_comp;
    this->size = HashMap::DEFAULT_SIZE;
    this->elements_num = 0;
    // Create dynamic arrays of keys and values and make their size equal this->size.
    this->keys = new DynamicArray<DynamicArray<T_key> >();
    this->values = new DynamicArray<DynamicArray<T_value> >();
    for (size_t i = 0; i < this->size; i++) {
        this->keys->push_back(new DynamicArray<T_key>);
        this->values->push_back(new DynamicArray<T_value>);
    }
}

template<class T_key, class T_value>
bool HashMap<T_key, T_value>::add(T_key const * key, T_value const * value) {
    // Expand the arrays if the size is appropriate and the hash map doesn't already contain binding with that key.
    if (this->elements_num + 1 > this->keys->size() * HashMap::SIZE_UP_SCALE && this[key] == nullptr) {
        // Create new keys and values dynamic arrays.
        DynamicArray<DynamicArray<T_key> > * new_keys = new DynamicArray<DynamicArray<T_key> >();
        DynamicArray<DynamicArray<T_value> > * new_values = new DynamicArray<DynamicArray<T_value> >();
        size_t new_size = this->keys->size() * HashMap::SIZE_UP_SCALE;
        for (size_t i = 0; i < new_size; i++) {
            new_keys->push_back(new DynamicArray<T_key>());
            new_values->push_back(new DynamicArray<T_value>());
        }
        for (size_t i = 0; i < this->keys->size(); i++) {
            for (size_t j = 0; j < this->keys[i]->size(); j++)
                add_binding(this, new_values, new_keys, new_size, this->keys[i][j], this->values[i][j]);
            delete this->keys[i];
            delete this->values[i];
        }
        delete this->keys;
        delete this->values;
        this->keys = new_keys;
        this->values = new_values;
    }
    bool added = add_binding(this, this->values, this->keys, this->keys->size(), key, value);
    if (added)
        this->elements_num++;
    return added;
}

template<class T_key, class T_value>
T_key const * HashMap<T_key, T_value>::remove(T_key const * key) {
    if (this[key] == nullptr)
        return nullptr;
    T_key const * res = nullptr;
    if ((this->elements_num - 1) * HashMap::SIZE_DOWN_TRIGGER < this->size && this->size > 1) {
        DynamicArray<DynamicArray<T_key> >   * new_keys   = new DynamicArray<DynamicArray<T_key> >();
        DynamicArray<DynamicArray<T_value> > * new_values = new DynamicArray<DynamicArray<T_value> >;
        size_t new_size = this->keys->size() / HashMap::SIZE_DOWN_SCALE;
        for (size_t i = 0; i < new_size; i++) {
            new_keys->push_back(new DynamicArray<T_key>());
            new_values->push_back(new DynamicArray<T_value>());
        }
        for (size_t i = 0; i < this->keys->size(); i++) {
            for (size_t j = 0; j < this->keys[i].size(); j++)
                if (this->key_cmp(this->keys[i][j], key) != 0)
                    add_binding(this, new_values, new_keys, new_size, this->keys[i][j], this->values[i][j]);
                else
                    res = this->keys[i][j];
            delete this->keys[i];
            delete this->values[i];
        }
        delete this->keys;
        delete this->values;
        this->keys = new_keys;
        this->values = new_values;
        this->elements_num--;
        return res;
    }
    size_t key_hash = this->hash_func(key, this->keys->size());
    DynamicArray<T_key> * new_keys = new DynamicArray<T_key>();
    DynamicArray<T_value> * new_values = new DynamicArray<T_value>();
    for (size_t j = 0; j < this->keys[key_hash].size(); j++) {
        if (this->key_cmp(this->keys[key_hash][j], key) != 0) {
            new_keys->push_back(this->keys[key_hash][j]);
            new_values->push_back(this->values[key_hash][j]);
        }
        else {
            res = this->keys[key_hash][j];
        }
    }
    delete this->keys[key_hash];
    delete this->values[key_hash];
    this->keys[key_hash] = new_keys;
    this->values[key_hash] = new_values;
    this->elements_num--;
    return res;
}

template<class T_key, class T_value>
T_value const * HashMap<T_key, T_value>::operator[](T_key const * key) const {
    size_t key_hash = this->hash_func(key, this->keys->size());
    for (size_t i = 0; i < this->keys[key_hash]->size(); i++)
        if (this->key_cmp(this->keys[key_hash][i], key) == 0)
            return this->values[key_hash][i];
    return nullptr;
}

template<class T_key, class T_value>
DynamicArray<T_key> * HashMap<T_key, T_value>::get_keys() const {
    DynamicArray<T_key> * res = new DynamicArray<T_key>();
    for (size_t i = 0; i < this->keys->size(); i++)
        for (size_t j = 0; j < this->keys[i].size(); j++)
            res->push_back(this->keys[i][j]);
    return res;
}

template<class T_key, class T_value>
HashMap<T_key, T_value>::~HashMap() {
    for (size_t i = 0; i < this->size; i++) {
        delete this->keys[i];
        delete this->values[i];
    }
    delete this->keys;
    delete this->values;
}