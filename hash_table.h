#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "avl_tree.h"
#include "customer.h"

class Customer_less
{
    public:
        bool operator()(Customer* const &left, Customer* const &right) const
        {
            return *left < *right;
        }
        bool operator()(const int key, Customer* const &right) const
        {
            return key < *right;
        }
        bool operator()(Customer* const &left, const int key) const
        {
            return *left < key;
        }
        bool operator()(const Customer& left, const Customer& right) const
        {
            return left < right;
        }
        bool operator()(const int key, const Customer& right) const
        {
            return key < right;
        }
        bool operator()(const Customer& left, const int key) const
        {
            return left < key;
        }
        bool operator()(const Customer& left, Customer* const &right) const
        {
            return left < *right;
        }
        bool operator()(Customer* const &left, const Customer& right) const
        {
            return *left < right;
        }
};

class Hash_table
{
    const int INITIAL_HT_CAPACITY = 8;
    const double MAX_LOAD_FACTOR = 0.75; // when the hash table's load factor gets bigger than the max, the table gets resized.
    const int RESIZE_FACTOR = 2; // the factor by which to scale the array when resizing.

    AVL_Tree<Customer*, Customer_less>** __array;
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
        AVL_Tree<Customer*, Customer_less>** old_array = __array;
        __size = 0;
        __capacity *= RESIZE_FACTOR;
        __array = new AVL_Tree<Customer*, Customer_less>*[__capacity];
        initialize_pointer_array(__array, __capacity);
        for (int i = 0; i < old_capacity; ++i)
        {
            if(old_array[i]==nullptr)
                continue;
            int treeSize = old_array[i]->get_size();
            Customer** tempArray = new Customer*[treeSize];
            old_array[i]->get_all_entities(tempArray);
            for(int j=0 ; j<treeSize ; j++)
            {
                insert(tempArray[j]);   //insert the Customer through his ptr.
            }
            delete[] tempArray;
            delete old_array[i];
        }
        delete[] old_array;
    }

    /**
        Initializes the given array of pointers to nullptr.
        @param array A pointer to the start of the array to be initialized.
        @param size The size of the given array.
        @exception INVALID_ARGUMENT - received nullptr in array.
    */
    void initialize_pointer_array(AVL_Tree<Customer*, Customer_less>** array, int size)
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
    Hash_table() : __array(new AVL_Tree<Customer*, Customer_less>*[INITIAL_HT_CAPACITY]), __size(0), __capacity(INITIAL_HT_CAPACITY)
    {
        initialize_pointer_array(__array, INITIAL_HT_CAPACITY);
    }

    /**
        D'tor for the hash table.
    */
    ~Hash_table()
    {
        for (int i = 0; i < __capacity; ++i)
        {
            if (__array[i] != nullptr)
            {
                int treeSize = __array[i]->get_size();
                Customer** tempArray = new Customer*[treeSize];
                __array[i]->get_all_entities(tempArray);
                for(int j=0 ; j<treeSize ; j++)
                {
                    delete tempArray[j];    //delete the Customer through his ptr.
                    __size--;
                }
                delete[] tempArray;
                delete __array[i];
            }
        }
        delete[] __array;
    }

    /**
        Returns a pointer to the object with the given key.
        @param key The key to search for.
        @retval A pointer to the requested Customer* type object. nullptr if the object is not found.
    */
    Customer* find(int key) const
    {
        if(__array[get_hashed_index(key)]==nullptr)
            return nullptr;
        Customer** ptrToCustomerPtr= __array[get_hashed_index(key)]->find(key);
        if(ptrToCustomerPtr!=nullptr)
            return *ptrToCustomerPtr;
        else
            return nullptr;
    }

    /**
        Inserts an object to the hash table.
        @param object The object to be inserted.
        @param key The key of the object to be inserted.
        @retval A pointer to the inserted object. nullptr if an object with the key already exists
    */
    bool insert(Customer* const object)
    {
        int hashed_index = get_hashed_index((int)*object);
        if (__array[hashed_index] == nullptr)   // first object to be inserted into the array cell.
        {
            __array[hashed_index] = new AVL_Tree<Customer*, Customer_less>();
        }
        Customer** retval = __array[hashed_index]->insert(object);
        if (retval == nullptr)     // key already exists.
        {
            return false;
        }
        ++__size;
        if ((double)__size/(double)__capacity > MAX_LOAD_FACTOR)    // check if resizing is needed.
        {
            resize();
        }
        return true;
    }

};

#endif