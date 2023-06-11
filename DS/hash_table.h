#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "avl_tree.h"

template <class T>
class Hash_table
{
    const int INITIAL_HT_CAPACITY = 8;
    const double MAX_LOAD_FACTOR = 0.75; // when the hash table's load factor gets bigger than the max, the table gets resized.
    const int RESIZE_FACTOR = 2; // the factor by which to scale the array when resizing.

    AVL_Tree<T>** __array;
    int __size;     // total number of objects in the HT.
    int __capacity;     // maximum capacity of the array.


    // exception for unexpected behaviour from resize.
    class POPPED_EMPTY_TREE{};
    // exception for passing nullptr to initialize_pointer_array.
    class INVALID_ARGUMENT{};

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
        @exception POPPED_EMPTY_TREE - popped an empty tree.
    */
    void resize()
    {
        int old_capacity = __capacity;
        AVL_Tree<T>** old_array = __array;
        __capacity *= RESIZE_FACTOR;
        __array = new AVL_Tree<T>*[__capacity];
        initialize_pointer_array(__array, __capacity);
        for (int i = 0; i < old_capacity; ++i)
        {
            while (__array[i] != nullptr && __array[i]->get_size() != 0)
            {
                T* relocated_T_ptr = __array[i]->find_max();
                if (relocated_T_ptr == nullptr)
                {
                    throw POPPED_EMPTY_TREE();   // shouldn't get here. popped an empty tree.
                }
                insert(*relocated_T_ptr);
                __array[i]->remove_by_entity(*relocated_T_ptr);
            }
        }
        delete[] old_array;
    }

    /**
        Initializes the given array of pointers to nullptr.
        @param array A pointer to the start of the array to be initialized.
        @param size The size of the given array.
        @exception INVALID_ARGUMENT - received nullptr in array.
    */
    void initialize_pointer_array(AVL_Tree<T>** array, int size)
    {
        if (array == nullptr)
        {
            throw INVALID_ARGUMENT();   // shouldn't get here. received nullptr in array.
        }
        for (int i = 0; i < size; ++i)
        {
            array[i] = nullptr;
        }
    }

public:

    /**
        C'tor for the hash table.
    */
    Hash_table() : __array(new AVL_Tree<T>*[INITIAL_HT_CAPACITY]), __size(0), __capacity(INITIAL_HT_CAPACITY)
    {
        initialize_pointer_array(__array, INITIAL_HT_CAPACITY);
    }

    /**
        D'tor for the hash table.
    */
    ~Hash_table()
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
        return __array[get_hashed_index(key)]->find(key);
    }

    /**
        Inserts an object to the hash table.
        @param object The object to be inserted.
        @param key The key of the object to be inserted.
        @retval A pointer to the inserted object. nullptr if an object with the key already exists
    */
    T* insert(const T& object)
    {
        int hashed_index = get_hashed_index((int)object);
        if (__array[hashed_index] == nullptr)   // first object to be inserted into the array cell.
        {
            __array[hashed_index] = new AVL_Tree<T>();
        }
        T* retval = __array[hashed_index]->insert(object);
        if (retval == nullptr)     // key already exists.
        {
            return nullptr;
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