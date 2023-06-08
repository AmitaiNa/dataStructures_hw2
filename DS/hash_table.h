#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "avl_tree.h"

template <class T>
class hash_table
{

    class hash_function
    {
        int __table_size;

        public:

            /**
                C'tor for the hash table function object.
            */
            hash_function(int table_size) : __table_size(table_size)
            {}

            /**
                Returns the given key's corresponding index in the has table array.
                @param key The key whose index should be calculated.
                @retval The hash table index of the given key.
            */
            int operator()(int key) const
            {
                return key % __table_size;
            }
    };

    /**
        Doubles the size of the hash table's array.
    */
    void resize()
    {
        // TODO
    }

    AVL_Tree<T>* __array;
    int __size;
    hash_function* __hash_function;

    const int INITIAL_HT_ARRAY_SIZE = 8;

public:

    /**
        C'tor for the hash table.
    */
    hash_table() : __array(new AVL_Tree<T>[INITIAL_HT_ARRAY_SIZE]), __size(0), __hash_function(new hash_function(INITIAL_HT_ARRAY_SIZE))
    {}

    /**
        Returns a pointer to the object with the given key.
        @param key The key to search for.
        @retval A pointer to the requested T type object.
    */
    T* find(int key) const
    {
        // TODO
    }

    /**
        Inserts an object to the hash table.
        @param object The object to be inserted.
        @retval True - if the insertion was successful.
        @retval False - if the object already exists in the hash table.
    */
    bool insert(const T& object)
    {
        // TODO
    }

};

#endif