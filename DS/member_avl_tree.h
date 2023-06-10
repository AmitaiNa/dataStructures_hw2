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
    path_discount_Output(Linked_List<AVL_Tree<Client>::Tree_Node*> path, double cumulative_discount = -1) : __path(path), __cumulative_discount(cumulative_discount)
    {}

    Linked_List<AVL_Tree<Client>::Tree_Node*> get_path() const {return __path;}

    double get_cumulative_discount() const {return __cumulative_discount;}
};

public:

    /**
        C'tor for the member avl tree.
    */
    Member_AVL_Tree();

    /**
		Creates a list of node that lead to the requested client. along the way, sums the discounts of every member in the search path.
        @param client The client to find.
        @retval A tuple which contains the nodes leading to the requested client (arranged like a stack - requested
        node is in the front, root n the back), and the sum of all the member's discounts that are in the search path.
        @retval The front will contain nullptr if the client is not found.
        @return A list of nodes and the sum of discounts in the search path.
    */
    path_discount_Output member_traverse_by_client(const Client& client);

    /**
		Find the lowest common ancestor of the given keys.
        @param id1 The lower member id boundary.
        @param id2 The upper member id boundary.
        @retval ptr to the lowest common ancestor's tree node.
        @retval nullptr if both id's are out of the tree's scope.
        @details id1 is promised to be smaller than id2
        @exception PERFORMED_ACTION_ON_EMPTY_TREE.
    */ 
    AVL_Tree<Client>::Tree_Node* find_lowest_common_ancestor(int id1, int id2);

    /**
		Insert a member into the tree. call for re-balancing if necessary.
        @param client The member to be inserted into the tree.
        @retval ptr to the member or nullptr if it already exists in the tree.
        @return ptr to the member inserted if successful else nullptr.
        @details after insertion, need to update the inserted member's discount to be the sum of all the discounts in the members's search path.
    */ 
    Client* insert(const Client& client) override;

    /**
		Adds the given discount to all the members whose id is within the given boundaries.
        @param id1 The lower member id boundary.
        @param id2 The upper member id boundary.
        @param amount The amount of discount to give the members.
        @details Adds the given discount to the lowest common ancestor of the given keys.
    */ 
    void add_discount(int id1, int id2, double amount);

};

#endif