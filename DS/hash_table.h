#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "avl_tree.h"

template <class T>
class hash_table
{

    AVL_Tree<T>** __array;
    int __size;
    int __capacity;

    const int INITIAL_HT_CAPACITY = 8;
    const double MAX_LOAD_FACTOR = 0.5; // when the hash table's load factor gets bigger than the max, the table gets resized.
    const int RESIZE_FACTOR = 2; // the factor by which to scale the array when resizing.

    /**
        Returns the given key's corresponding index in the table array.
        @param key The key whose index should be calculated.
        @retval The hash table index of the given key.
    */
    int get_hashed_index(int key) const
    {
        return key % __capacity;
    }

    /**
        Doubles the size of the hash table's array.
    */
    void resize()
    {
        int new_size = __capacity*RESIZE_FACTOR;
        AVL_Tree<T>** new_array = new AVL_Tree<T>*[new_size];
        for (int i = 0; i < __size; ++i)
        {
            new_array[i] = __array[i];
            __array[i] = nullptr;   // needed so the following "delete[]" won't delete the objects in the trees, but only the allocated memory.
            new_array[new_size-i-1] = nullptr; // TODO can be removed when everything is working. inserted for easier debugging.
        }
        delete[] __array;
        __array = new_array;
        __capacity *= RESIZE_FACTOR;
    }

public:

    /**
        C'tor for the hash table.
    */
    hash_table() : __array(new AVL_Tree<T>*[INITIAL_HT_CAPACITY]), __size(0), __capacity(INITIAL_HT_CAPACITY)
    {
        for (int i = 0; i < INITIAL_HT_CAPACITY; ++i)
        {
            __array[i] = nullptr; // TODO can be removed when everything is working. inserted for easier debugging.
        }
    }

    /**
        D'tor for the hash table.
    */
    ~hash_table()
    {
        delete[] __array;
    }

    /**
        Returns a pointer to the object with the given key.
        @param key The key to search for.
        @retval A pointer to the requested T type object. nullptr if the object is not found.
    */
    T* find(int key) const
    {
        return __array[get_hashed_index(key)].AVL_Tree<T>::find(key);
    }

    /**
        Inserts an object to the hash table.
        @param object The object to be inserted.
        @param key The key of the object to be inserted.
        @retval A pointer to the inserted object. nullptr if an object with the key already exists
    */
    T* insert(const T& object, int key)
    {
        T* retval = __array[get_hashed_index(key)].AVL_Tree<T>::insert(object);
        if (retval == nullptr)
        {
            return nullptr;     // key already exists.
        }
        ++__size;
        if (__size/__capacity > MAX_LOAD_FACTOR)    // check if resizing is needed.
        {
            resize();
        }
        return retval;
    }

};

#endif