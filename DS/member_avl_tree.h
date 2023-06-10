#ifndef MEMBER_AVL_TREE
#define MEMBER_AVL_TREE

#include "avl_tree.h"
#include "customer.h"

class Member_AVL_Tree : public AVL_Tree<Customer>
{

// exception for when the id's given to give_discount are both out of the id's in the tree.
class BOTH_IDS_OUT_OF_BOUNDS{};
// exception for when the member than an action is being performed upon doesn't exist.
class NO_MEMBER_WITH_SUCH_ID{};

class path_discount_Output
{

    Linked_List<Member_AVL_Tree::Tree_Node*> __path;
    double __cumulative_discount;

public:
    path_discount_Output(Linked_List<Member_AVL_Tree::Tree_Node*> path, double cumulative_discount = -1) : __path(path), __cumulative_discount(cumulative_discount)
    {}

    Linked_List<Member_AVL_Tree::Tree_Node*> get_path() const {return __path;}

        double get_cumulative_discount() const {return __cumulative_discount;}
    };

    /**
        Preform a left-right rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_LR(Tree_Node *current_node) override;

    /**
        Preform a left-left rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_LL(Tree_Node *current_node) override;

    /**
        Preform a right-left rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_RL(Tree_Node *current_node) override;

    /**
        Preform a right-right rotate in relation to the given node.
        @param node The node.
        @retval Tree_Node* to the new son after rotating.
    */
    Tree_Node* rotate_RR(Tree_Node *current_node) override;


public:

    /**
        C'tor for the member avl tree.
    */
    Member_AVL_Tree();

    /**
		Creates a list of node that lead to the requested customer. along the way, sums the discounts of every member in the search path.
        @param customer The customer to find.
        @retval A tuple which contains the nodes leading to the requested customer (arranged like a stack - requested
        node is in the front, root n the back), and the sum of all the member's discounts that are in the search path.
        @retval The front will contain nullptr if the customer is not found.
        @return A list of nodes and the sum of discounts in the search path.
    */
    path_discount_Output member_traverse_by_customer(const Customer& customer) const;

    /**
		Creates a list of node that lead to the requested customer. along the way, sums the discounts of every member in the search path.
        @param key The customer's key to find.
        @retval A tuple which contains the nodes leading to the requested customer (arranged like a stack - requested
        node is in the front, root n the back), and the sum of all the member's discounts that are in the search path.
        @retval The front will contain nullptr if the customer is not found.
        @return A list of nodes and the sum of discounts in the search path.
    */
    path_discount_Output member_traverse_by_key(int key) const;

    /**
		Find the lowest common ancestor of the given keys.
        @param id1 The lower member id boundary.
        @param id2 The upper member id boundary.
        @retval ptr to the lowest common ancestor's tree node.
        @retval nullptr if both id's are out of the tree's scope.
        @details id1 is promised to be smaller than id2
        @exception PERFORMED_ACTION_ON_EMPTY_TREE.
    */ 
    Member_AVL_Tree::Tree_Node* find_lowest_common_ancestor(int id1, int id2) const;

    /**
		Insert a member into the tree. call for re-balancing if necessary.
        @param customer The member to be inserted into the tree.
        @retval ptr to the member or nullptr if it already exists in the tree.
        @return ptr to the member inserted if successful else nullptr.
        @details after insertion, need to update the inserted member's discount to be the sum of all the discounts in the members's search path.
    */ 
    Customer* insert(const Customer& customer) override;

    /**
		Adds the given discount to all the members whose id is within the given boundaries.
        @param id1 The lower member id boundary.
        @param id2 The upper member id boundary.
        @param amount The amount of discount to give the members.
        @exception BOTH_IDS_OUT_OF_BOUNDS.
    */ 
    void add_discount(int id1, int id2, double amount) const;

    /**
        Gets the expenses of the given member.
        @param id The member's id
        @retval The member's expenses.
        @exception NO_MEMBER_WITH_SUCH_ID.
    */
    double get_expenses(int id) const;

};

#endif