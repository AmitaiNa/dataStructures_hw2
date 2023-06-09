#ifndef MEMBER_AVL_TREE
#define MEMBER_AVL_TREE

#include "avl_tree.h"
#include "client.h"

class Member_AVL_Tree : public AVL_Tree<Client>
{

class path_discount_Output
{

    Linked_List<AVL_Tree<Client>::Tree_Node*> __path;
    double __cumulative_discount;

public:
    path_discount_Output(Linked_List<AVL_Tree<Client>::Tree_Node*> path, double cumulative_discount) : __path(path), __cumulative_discount(cumulative_discount)
    {}

    path_discount_Output(Linked_List<AVL_Tree<Client>::Tree_Node*> path) : __path(path)
    {}

    Linked_List<AVL_Tree<Client>::Tree_Node*> get_path() const
    {
        return __path;
    }

    double get_cumulative_discount() const
    {
        return __cumulative_discount;
    }
};

public:

    /**
        C'tor for the member avl tree.
    */
    Member_AVL_Tree() : AVL_Tree<Client>()
    {}

    /**
        D'tor for the member avl tree.
    */
    ~Member_AVL_Tree()
    {}

    /**
		Creates a list of node that lead to the requested client. along the way, sums the discounts of every member in the search path.
        @param client The client to find.
        @retval A tuple which contains the nodes leading to the requested client (arranged like a stack - requested
        node is in the front, root n the back), and the sum of all the member's discounts that are in the search path.
        @retval The front will contain nullptr if the client is not found.
        @return A list of nodes and the sum of discounts in the search path.
    */
    path_discount_Output member_traverse_by_client(const Client& client)
    {
        Linked_List<AVL_Tree<Client>::Tree_Node*> path;
        double cumulative_discount = 0;
        AVL_Tree<Client>::Tree_Node* current_node = __root;
        while (current_node != nullptr)
        {
            cumulative_discount += client.get_member_discount();
            path.push_front(current_node);
            if (__compare(current_node->__data, client)) // client is greater than the current node.
            {
                current_node = current_node->__right; //go right.
            }
            else if (__compare(client, current_node->__data)) // client is smaller than the current node.
            {
                current_node = current_node->__left; //go left.
            }
            else                                     // client is equal to the current node.
            {
                return path_discount_Output(path, cumulative_discount);     // client found.
            }
        }
        path.push_front(current_node);               //
        return path_discount_Output(path);           // requested client not found.
    }

    /**
		Insert a member into the tree. call for re-balancing if necessary.
        @param client The member to be inserted into the tree.
        @retval ptr to the member or nullptr if it already exists in the tree.
        @return ptr to the member inserted if successful else nullptr.
        @details after insertion, need to update the inserted member's discount to be the sum of all the discounts in the members's search path.
    */ 
    Client* insert(const Client& client) override
    {
        path_discount_Output path_discount = member_traverse_by_client(client);     //
        Linked_List<Tree_Node*> path = path_discount.get_path();                //  added usage of path_discount_Output tuple.
        int cumulative_discount = path_discount.get_cumulative_discount();              //
        Tree_Node* current_node = path.pop_front();
        if (current_node != nullptr) // the client already exists in the tree.
        {
            return nullptr;
        }

        // the client needs to be inserted:
        Client* direct_ptr_to_data = nullptr;
        if(path.get_length()==0)  //empty tree case
        {
            __root = new Tree_Node(client); //insert to root.
            direct_ptr_to_data = &__root->__data;
            __size++;
            direct_ptr_to_data->set_member_cumulative_discount(cumulative_discount);    // added to update the inserted member's discount.
            return direct_ptr_to_data;
        }

        current_node = path.pop_front();
        if (__compare(current_node->__data, client)) // client is greater than the current node.
        {
            current_node->__right = new Tree_Node(client); //insert to the right.
            direct_ptr_to_data = &current_node->__right->__data;
        }
        else if (__compare(client, current_node->__data))    // client is lesser than the current node.
        {                                               // should always execute if got here.
            current_node->__left = new Tree_Node(client); //insert to the left.
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
                direct_ptr_to_data->set_member_cumulative_discount(cumulative_discount);    // added to update the inserted member's discount.
                return direct_ptr_to_data;
            }
            update_height(current_node);
        }
        direct_ptr_to_data->set_member_cumulative_discount(cumulative_discount);    // added to update the inserted member's discount.
        return direct_ptr_to_data;
    }

    /**
		Adds the given discount to all the members whose id is within the given boundaries.
        @param id1 The lower member id boundary.
        @param id2 The upper member id boundary
        @param amount The amount of discount to give the members.
        @details Adds the given discount to the lowest common ancestor of the given keys.
    */ 
    void add_discount(int id1, int id2, double amount)
    {

    }

};

#endif