#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <functional>
#include "linked_list.h"
#include <type_traits>

template <class T>
class Custom_less
{
    public:
        bool operator()(const T& left, const T& right)
        {
            return left < right;
        }
        bool operator()(int left, const T& right)
        {
            return left < right;
        }
        bool operator()(const T& left, int right)
        {
            return left < right;
        }
};

template<typename H, typename A1, typename A2>
class has_call_operator : public std::false_type {
    template<typename F, typename U1, typename U2>
    static auto test(F* f, U1&& a1, U2&& a2) -> decltype((*f)(std::forward<U1>(a1), std::forward<U2>(a2)), std::true_type());

    template<typename F, typename U1, typename U2>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<H, A1, A2>(nullptr, std::declval<A1>(), std::declval<A2>()))::value;
};



template <class T, class Compare = Custom_less<T>>    //__compare(left, right) = return (left < right)
class AVL_Tree
{
protected:

    class COMPARE_WITH_KEY_NON_ARITHMETIC{};

    Compare __compare;
    struct Tree_Node
    {
        T __data;
        Tree_Node* __left;
        Tree_Node* __right;
        int __height; //1 + max between the sons' height. if a node has no son, its height is counted as -1.

        /**
            C'tor of the node
        */
        Tree_Node(const T& entity) : __data(entity), __left(nullptr), __right(nullptr), __height(0)
        {}
    };
    Tree_Node* __root;
    int __size;

    /**
		Creates a list of node that lead to the requested entity.
        @param entity The entity to find.
        @retval A list which contains the nodes leading to the requested entity (arranged like a stack - requested
        node is in the front, root n the back).
        @retval The front will contain nullptr if the entity is not found.
        @return A list of nodes.
    */
   Linked_List<Tree_Node*> traverse_by_entity(const T& entity);

    /**
		Creates a list of node that lead to the requested entity.
        @param key The key of the requested entity.
        @retval A list which contains the nodes leading to the requested entity (arranged like a stack - requested node is in the front, root n the back).
        @retval The front will contain nullptr if the entity is not found.
        @return A list of nodes.
    */

    template<class U = T,class com = Compare, typename std::enable_if<has_call_operator<com, U, int>::value, bool>::type = true>
    Linked_List<Tree_Node*> traverse_by_key(int key)
    {
        Linked_List<Tree_Node*> path;
        Tree_Node* current_node = __root;
        while (current_node != nullptr)
        {
            path.push_front(current_node);
            if (__compare(current_node->__data, key)) // key is greater than the current node.
            {
                current_node = current_node->__right; //go right.
            }
            else if (__compare(key, current_node->__data)) // key is smaller than the current node.
            {
                current_node = current_node->__left; //go left.
            }
            else                                     // key is equal to the current node.
            {
                return path;                         // key found.
            }
        }
        path.push_front(current_node);              //
        return path;                                // requested key not found.
    }
    template<class U = T,class com = Compare, typename std::enable_if<!has_call_operator<com, U, int>::value, bool>::type = true>
    Linked_List<Tree_Node*> traverse_by_key(int key)
    {throw COMPARE_WITH_KEY_NON_ARITHMETIC();Linked_List<Tree_Node*> purgatory; return purgatory;}

    
    /**
        Calculate the node's balance factor.
        @param node The node.
        @retval The node's balance factor (left son height - right son height).
        @return Integer containing the nodes balance factor.
    */
    static int get_balance_factor(Tree_Node* node);

    /**
        Getter for the node's height
        @param node The node.
        @retval nullptr height is counted as -1.
        @return Integer containing the nodes height.
    */
    static int get_height(Tree_Node* node);

    /**
        Update the node's height by recalculating it.
        @param node The node.
        @return Update the node's height as 1 + max between the sons' height.
        if a node has no son, its height is counted as -1.
    */
    static void update_height(Tree_Node* node);

    /**
        Preform a left-right rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_LR(Tree_Node *current_node);

    /**
        Preform a left-left rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_LL(Tree_Node *current_node);

    /**
        Preform a right-left rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_RL(Tree_Node *current_node);

    /**
        Preform a right-right rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_RR(Tree_Node *current_node);

    /**

    */
    void remove_aux_leaf(Tree_Node* father, Tree_Node* node_to_delete);

    /**

    */
    void remove_aux_only_son(Tree_Node* father, Tree_Node* node_to_delete, Tree_Node* only_son);

    /**
     * Switch between node_to_delete and the last entity by value using compare -
     * the biggest from all the entities smaller then node_to_delete.
    */
    Tree_Node* remove_aux_two_sons(Tree_Node* father, Tree_Node* node_to_delete);

    /**
		Find the biggest entity in the subtree from root. determined be compare class.
        @param root The node from which to start searching for the maximum.
        @retval ptr to the entity or nullptr if it doesn't exists in the tree - empty tree.
        @return ptr to the max entity if successful else nullptr.
    */
    T* find_max(Tree_Node* root);

public:

    /**
		C'tor of the avl tree.
        @return Initialize an empty avl tree.
    */
    AVL_Tree();

    /**
		D'tor of the avl tree.
    */
    virtual ~AVL_Tree();

    /**
		Find an entity in the tree and return ptr to it's data.
        @param entity The entity to be searched.
        @retval ptr to the entity or nullptr if it doesn't exists in the tree.
        @return ptr to the entity if successful else nullptr.
    */
    T* find(const T& entity); // for trees with a comparator.

    /**
		Extra find func for finding an entity in the tree and return ptr to it's data.
        Will be available only if T is arithmetic - i.e., can be compared directly with double or int.
        @param key The entity key to be searched for.
        @retval ptr to the entity or nullptr if it doesn't exists in the tree.
        @return ptr to the entity if successful else nullptr.
    */
    template<class U = T,class com = Compare, typename std::enable_if<has_call_operator<com, U, int>::value, bool>::type = true>
    T* find(int key)
    {
        Tree_Node* target_node = traverse_by_key(key).pop_front();
        if (target_node == nullptr)
        {
            return nullptr;                         // requested entity not found.
        }
        else
        {
            return &(target_node->__data);
        }
    }
    template<class U = T,class com = Compare, typename std::enable_if<!has_call_operator<com, U, int>::value, bool>::type = true>
    T* find(int key)
    {throw COMPARE_WITH_KEY_NON_ARITHMETIC(); T* purgatory; return purgatory;}

    /**
		Insert an entity into the tree. call for re-balancing if necessary.
        @param entity The entity to be inserted into the tree.
        @retval ptr to the entity or nullptr if it already exists in the tree.
        @return ptr to the entity inserted if successful else nullptr.
    */ 
    virtual T* insert(const T& entity);
   
    /**
		Remove an entity from the tree. call for re-balancing if necessary.
        @param entity The entity to be removed.
        // @retval bool status of the operation. True if successful ; false if it does not exist in the tree.
        @return entity is deleted if successful else nothing and return false.
    */
    bool remove_by_entity(const T& entity);

    /**
		Extra Remove func for removing an entity from the tree. call for re-balancing if necessary.
        Will be available only if T is arithmetic - i.e., can be compared directly with double or int.
        @param key The key of the requested entity.
        @retval bool status of the operation. True if successful ; false if it does not exist in the tree.
        @return entity is deleted if successful else nothing and return false.
    */

    template<class U = T,class com = Compare, typename std::enable_if<has_call_operator<com, U, int>::value, bool>::type = true>
    bool remove_by_id(int key)
    {
        Linked_List<Tree_Node*> path = traverse_by_key(key);
        Tree_Node* node_to_delete = path.pop_front();
        if (node_to_delete == nullptr) // the key doesn't exists in the tree.
        {
            return false;
        }

        // the key needs to be deleted:
        Tree_Node* father = nullptr;     //case root
        if(path.get_length()!=0)    //not root
            father = path.front();
        while(1)    //LABEL
        {
            Tree_Node* left_son = node_to_delete->__left;
            Tree_Node* right_son = node_to_delete->__right;
            if(right_son == nullptr && left_son == nullptr) //case leaf:
            {
                remove_aux_leaf(father, node_to_delete);
                break;
            }
            else if(right_son == nullptr || left_son == nullptr) //case one son:
            {
                Tree_Node* only_son;
                if(right_son != nullptr)
                    only_son = right_son;
                else
                    only_son = left_son;
                remove_aux_only_son(father, node_to_delete, only_son);
                break;
            }
            else  //case two sons:
            {
                father = remove_aux_two_sons(father, node_to_delete); //switch nodes and get new father
                path = traverse_by_key(father->__data.get_id());
                // jmp LABEL
            }
        }
        delete node_to_delete;
        __size--;
        //rebalancing:
        int length = path.get_length();
        for(int i=length ; i>0 ;i--)
        {
            Tree_Node* current_node = path.pop_front();
            int balance_factor = get_balance_factor(current_node);
            if (balance_factor == 2 || balance_factor == -2)    // we found the first node out of balance
            {
                Tree_Node* new_son = nullptr;
                if (balance_factor == 2)    //L
                {
                    if (get_balance_factor(current_node->__left) == -1)
                    {
                        new_son = rotate_LR(current_node);
                    }
                    else
                    {
                        new_son = rotate_LL(current_node);
                    }
                }
                else    //R
                {
                    if (get_balance_factor(current_node->__right) == 1)
                    {
                        new_son = rotate_RL(current_node);
                    }
                    else
                    {
                        new_son = rotate_RR(current_node);
                    }
                }
                // update father about new son. MAZAL-TOV
                if(i>1) // there are more nodes up the path
                {
                    Tree_Node* father = path.front();
                    if(__compare(new_son->__data, father->__data))    // put it to the left
                        father->__left = new_son;
                    else    // put it to the right
                        father->__right = new_son;
                }
                else    // father is root
                    __root = new_son;
            }
            else
                update_height(current_node);  
        }
        return true;
    }
    template<class U = T,class com = Compare, typename std::enable_if<!has_call_operator<com, U, int>::value, bool>::type = true>
    bool remove_by_id(int key)
    {throw COMPARE_WITH_KEY_NON_ARITHMETIC(); return false;}

    /**
		Find the biggest entity in the subtree from root. determined by compare class.
        @retval ptr to the entity or nullptr if it doesn't exists in the tree - empty tree.
        @return ptr to the max entity if successful else nullptr.
        @details passes root to private find_max.
    */
    T* find_max();

    /**
		Pops the biggest entity in the subtree from root, out of the tree. determined by compare class.
        @retval ptr to the entity or nullptr if it doesn't exists in the tree - empty tree.
        @return ptr to the max entity if successful else nullptr.
        @details uses find_max and remove_by_entity. assumes T has a cpy c'tor.
    */
   T* pop_max();

    /**
		Getter for the tree's size.
        @retval The number of entities in the tree
        @return The size of the tree.
    */
    int get_size();

    /**
		Writes into an array the id's of the tree's entities. sorted in descending order.
        @param output The array the data should be written into
    */
    void descending_inorder(int* const output);

};

//-----------------------AVL_Tree definitions-----------------------

template <class T, class Compare>
AVL_Tree<T, Compare>::AVL_Tree() : __root(nullptr), __size(0)
{}

template <class T, class Compare>
AVL_Tree<T, Compare>::~AVL_Tree()
{
    if (__root == nullptr)
    {
        return;
    }
    Tree_Node* current_node = __root;
    Linked_List<Tree_Node*> path;
    path.push_front(current_node);
    while (path.get_length() != 0)
    {
        current_node = path.pop_front();
        if (current_node->__right != nullptr)
        {
            path.push_front(current_node->__right);
        }
        if (current_node->__left != nullptr)
        {
            path.push_front(current_node->__left);
        }
        delete current_node;
        --__size;
    }
}

template <class T, class Compare>
T* AVL_Tree<T, Compare>::find(const T& entity)
{
    Tree_Node* target_node = traverse_by_entity(entity).pop_front();
    if (target_node == nullptr)
    {
        return nullptr;                         // requested entity not found.
    }
    else
    {
        return &(target_node->__data);
    }
}

template <class T, class Compare>
T* AVL_Tree<T, Compare>::insert(const T& entity)
{
    Linked_List<Tree_Node*> path = traverse_by_entity(entity);
    Tree_Node* current_node = path.pop_front();
    if (current_node != nullptr) // the entity already exists in the tree.
    {
        return nullptr;
    }

    // the entity needs to be inserted:
    T* direct_ptr_to_data = nullptr;
    if(path.get_length()==0)  //empty tree case
    {
        __root = new Tree_Node(entity); //insert to root.
        direct_ptr_to_data = &__root->__data;
        __size++;
        return direct_ptr_to_data;
    }

    current_node = path.pop_front();
    if (__compare(current_node->__data, entity)) // entity is greater than the current node.
    {
        current_node->__right = new Tree_Node(entity); //insert to the right.
        direct_ptr_to_data = &current_node->__right->__data;
    }
    else if (__compare(entity, current_node->__data))    // entity is lesser than the current node.
    {                                               // should always execute if got here.
        current_node->__left = new Tree_Node(entity); //insert to the left.
        direct_ptr_to_data = &current_node->__left->__data;
    }
    __size++;

    //rebalancing:
    update_height(current_node);
    int length = path.get_length();
    for(int i=length ; i>0 ;i--)
    {
        current_node = path.pop_front();
        int balance_factor = get_balance_factor(current_node);
        if (balance_factor == 2 || balance_factor == -2)    // we found the first node out of balance
        {
            Tree_Node* new_son = nullptr;
            if (balance_factor == 2)    //L
            {
                if (get_balance_factor(current_node->__left) == -1)
                {
                    new_son = rotate_LR(current_node);
                }
                else
                {
                    new_son = rotate_LL(current_node);
                }
            }
            else    //R
            {
                if (get_balance_factor(current_node->__right) == 1)
                {
                    new_son = rotate_RL(current_node);
                }
                else
                {
                    new_son = rotate_RR(current_node);
                }
            }
            // update father about new son. MAZAL-TOV
            if(i>1) // there are more nodes up the path
            {
                Tree_Node* father = path.front();
                if(__compare(new_son->__data, father->__data))    // put it to the left
                    father->__left = new_son;
                else    // put it to the right
                    father->__right = new_son;
            }
            else    // father is root
                __root = new_son;
            return direct_ptr_to_data;
        }
        update_height(current_node);  
    }
    return direct_ptr_to_data;
}

template <class T, class Compare>
void AVL_Tree<T, Compare>::remove_aux_leaf(Tree_Node* father, Tree_Node* node_to_delete)
{
    if(father==nullptr) //node_to_delete is the last node
        __root = nullptr; 
    else if (father->__right==node_to_delete) // node_to_delete is the right son of her father.
    {
        father->__right = nullptr; //detach the right son.
    }
    else    // node_to_delete is the left son of her father.
    {
        father->__left = nullptr; //detach the left son.
    }
    return;
}

template <class T, class Compare>
void AVL_Tree<T, Compare>::remove_aux_only_son(Tree_Node* father, Tree_Node* node_to_delete, Tree_Node* only_son)
{
    if(father==nullptr) //node_to_delete is root
        __root = only_son; 
    else if (father->__right==node_to_delete) // node_to_delete is the right son of her father.
    {
        father->__right = only_son; //short between the son and the father.
    }
    else    // node_to_delete is the left son of her father.
    {
        father->__left = only_son; //short between the son and the father.
    }
    return;
}

template <class T, class Compare>
typename AVL_Tree<T, Compare>::Tree_Node* AVL_Tree<T, Compare>::remove_aux_two_sons(Tree_Node* node_to_delete_father, Tree_Node* node_to_delete)
{
    T* last_value = find_max(node_to_delete->__left);
    Linked_List<Tree_Node*> path_to_last = traverse_by_entity(*last_value);
    Tree_Node* last = path_to_last.pop_front();   //pop last
    Tree_Node* last_father = path_to_last.front();      
    Tree_Node* last_right_son = last->__right;
    Tree_Node* last_left_son = last->__left;
    Tree_Node* node_to_delete_right_son = node_to_delete->__right;
    Tree_Node* node_to_delete_left_son = node_to_delete->__left;

    //connect node_to_delete_father to last:
    if(node_to_delete_father==nullptr) //node_to_delete is root:
        __root = last; 
    else if (__compare(node_to_delete_father->__data, node_to_delete->__data)) // node_to_delete is greater than the father node.
        node_to_delete_father->__right = last;
    else    // node_to_delete is lesser than the father node.
        node_to_delete_father->__left = last;

    bool case_neighbors = node_to_delete_left_son==last;
    if(case_neighbors)   //neighbors
    {}
    //connect last_father to node_to_delete:
    else if (__compare(last_father->__data, last->__data)) // last is greater than the father node.
        last_father->__right = node_to_delete;
    else    // last is lesser than the father node.
        last_father->__left = node_to_delete;
    
    //connect node_to_delete sons to last:
    last->__right = node_to_delete_right_son;
    if(case_neighbors)   //neighbors
        last->__left = node_to_delete;
    else
    last->__left = node_to_delete_left_son;

    //connect last sons to node_to_delete:
    node_to_delete->__right = last_right_son;
    node_to_delete->__left = last_left_son;
    
    //switch between the nodes heights:
    int temp = node_to_delete->__height;
    node_to_delete->__height = last->__height;
    last->__height = temp;
    
    if(case_neighbors)   //neighbors
        return last;    //the new father of node_to_delete
    else
        return last_father; //the new father of node_to_delete
}

template <class T, class Compare>
bool AVL_Tree<T, Compare>::remove_by_entity(const T& entity)
{
    Linked_List<Tree_Node*> path = traverse_by_entity(entity);
    Tree_Node* node_to_delete = path.pop_front();
    if (node_to_delete == nullptr) // the entity doesn't exists in the tree.
    {
        return false;
    }

    // the entity needs to be deleted:
    Tree_Node* father = nullptr;     //case root
    if(path.get_length()!=0)    //not root
        father = path.front();
    while(1)    //LABEL
    {
        Tree_Node* left_son = node_to_delete->__left;
        Tree_Node* right_son = node_to_delete->__right;
        if(right_son == nullptr && left_son == nullptr) //case leaf:
        {
            remove_aux_leaf(father, node_to_delete);
            break;
        }
        else if(right_son == nullptr || left_son == nullptr) //case one son:
        {
            Tree_Node* only_son;
            if(right_son != nullptr)
                only_son = right_son;
            else
                only_son = left_son;
            remove_aux_only_son(father, node_to_delete, only_son);
            break;
        }
        else  //case two sons:
        {
            father = remove_aux_two_sons(father, node_to_delete); //switch nodes and get new father
            path = traverse_by_entity(father->__data);
            // jmp LABEL
        }
    }
    delete node_to_delete;
    __size--;
    //rebalancing:
    int length = path.get_length();
    for(int i=length ; i>0 ;i--)
    {
        Tree_Node* current_node = path.pop_front();
        int balance_factor = get_balance_factor(current_node);
        if (balance_factor == 2 || balance_factor == -2)    // we found the first node out of balance
        {
            Tree_Node* new_son = nullptr;
            if (balance_factor == 2)    //L
            {
                if (get_balance_factor(current_node->__left) == -1)
                {
                    new_son = rotate_LR(current_node);
                }
                else
                {
                    new_son = rotate_LL(current_node);
                }
            }
            else    //R
            {
                if (get_balance_factor(current_node->__right) == 1)
                {
                    new_son = rotate_RL(current_node);
                }
                else
                {
                    new_son = rotate_RR(current_node);
                }
            }
            // update father about new son. MAZAL-TOV
            if(i>1) // there are more nodes up the path
            {
                Tree_Node* father = path.front();
                if(__compare(new_son->__data, father->__data))    // put it to the left
                    father->__left = new_son;
                else    // put it to the right
                    father->__right = new_son;
            }
            else    // father is root
                __root = new_son;
        }
        else
            update_height(current_node);  
    }
    return true;
}

template <class T, class Compare>
T* AVL_Tree<T, Compare>::find_max(Tree_Node* root)
{
    if(root==nullptr)
        return nullptr;
    Tree_Node* current_node = root;
    Tree_Node* last_node = current_node;
    while (current_node != nullptr)
    {
        last_node = current_node;
        current_node = current_node->__right; //go right.
    }
    return &last_node->__data;
}

template <class T, class Compare>
T* AVL_Tree<T, Compare>::find_max()
{
    return find_max(__root);
}

template <class T, class Compare>
int AVL_Tree<T, Compare>::get_size()
{
    return __size;
}

template <class T, class Compare>
void AVL_Tree<T, Compare>::descending_inorder(int* const output)
{
    Tree_Node* current_node = __root;
    Linked_List<Tree_Node*> path;
    int index = 0;
    while (1)
    {
        while (current_node != nullptr)
        {
            path.push_front(current_node);
            current_node = current_node->__right;
        }
        if (path.get_length() == 0)
        {
            break;
        }
        current_node = path.pop_front();
        output[index++] = current_node->__data->get_id();
        current_node = current_node->__left;
    }
}

template <class T, class Compare>
T* AVL_Tree<T, Compare>::pop_max()
{
    T* object_to_pop = find_max();
    if (object_to_pop = nullptr) // empty tree.
    {
        return nullptr;
    }
    T* copy = new T(*(object_to_pop)); // creates a copy of the data within the tree node because remove_by_entity() destroys it. assumes T has a cpy c'tor.
    remove_by_entity(*object_to_pop, false);
    return object_to_pop;
}

//protected:

template <class T, class Compare>
Linked_List<typename AVL_Tree<T, Compare>::Tree_Node*> AVL_Tree<T, Compare>::traverse_by_entity(const T& entity)
{
    Linked_List<Tree_Node*> path;
    Tree_Node* current_node = __root;
    while (current_node != nullptr)
    {
        path.push_front(current_node);
        if (__compare(current_node->__data, entity)) // entity is greater than the current node.
        {
            current_node = current_node->__right; //go right.
        }
        else if (__compare(entity, current_node->__data)) // entity is smaller than the current node.
        {
            current_node = current_node->__left; //go left.
        }
        else                                     // entity is equal to the current node.
        {
            return path;                         // entity found.
        }
    }
    path.push_front(current_node);              //
    return path;                                // requested entity not found.
}

template <class T, class Compare>
int AVL_Tree<T, Compare>::get_balance_factor(Tree_Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    else
    {
        return get_height(node->__left) - get_height(node->__right);
    }
}

template <class T, class Compare>
int AVL_Tree<T, Compare>::get_height(Tree_Node* node)
{
    return node == nullptr ? -1 : node->__height;
}

template <class T, class Compare>
void AVL_Tree<T, Compare>::update_height(Tree_Node* node)
{
    if (node == nullptr)
    {
        return;
    }
    int left = get_height(node->__left);

    int right = get_height(node->__right);
    if(left < right)
        node->__height = 1 + right;
    else
        node->__height = 1 + left;
}

template <class T, class Compare>
typename AVL_Tree<T, Compare>::Tree_Node* AVL_Tree<T, Compare>::rotate_LR(Tree_Node* current_node) //current_node = A
{
    Tree_Node* left = current_node->__left;  //save B
    Tree_Node* new_root = left->__right;    //new_root = C
    left->__right = new_root->__left;    //B->right = C->left (Y)
    new_root->__left = left;    //C->left = B
    current_node->__left = new_root->__right;   //A->left = C->right (W)
    new_root->__right = current_node;   ////C->right = A
    update_height(current_node);    //update A
    update_height(left);    //update B
    update_height(new_root);    //update C
    return new_root;
}

template <class T, class Compare>
typename AVL_Tree<T, Compare>::Tree_Node* AVL_Tree<T, Compare>::rotate_LL(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__left;    //new_root = B
    Tree_Node* mid = new_root->__right;  //save Y
    new_root->__right = current_node;   //B->right = A
    current_node->__left = mid; //A->left = Y
    update_height(current_node);    //update a
    update_height(new_root);    //update B
    return new_root;
}

template <class T, class Compare>
typename AVL_Tree<T, Compare>::Tree_Node* AVL_Tree<T, Compare>::rotate_RL(Tree_Node* current_node) //current_node = A
{
    Tree_Node* right = current_node->__right;  //save B
    Tree_Node* new_root = right->__left;    //new_root = C
    right->__left = new_root->__right;    //B->left = C->right (W)
    new_root->__right = right;    //C->right = B
    current_node->__right = new_root->__left;   //A->right = C->left (Y)
    new_root->__left = current_node;   ////C->left = A
    update_height(current_node);    //update A
    update_height(right);    //update B
    update_height(new_root);    //update C
    return new_root;
}

template <class T, class Compare>
typename AVL_Tree<T, Compare>::Tree_Node* AVL_Tree<T, Compare>::rotate_RR(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__right;    //new_root = B
    Tree_Node* mid = new_root->__left;  //save X
    new_root->__left = current_node;   //B->left = A
    current_node->__right = mid; //A->right = X
    update_height(current_node);    //update A
    update_height(new_root);    //update B
    return new_root;
}

#endif // AVL_TREE_H_