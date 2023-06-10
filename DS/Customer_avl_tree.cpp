#include "Customer_avl_tree.h"

Customer_AVL_Tree::Customer_AVL_Tree() : AVL_Tree<Customer*>()
{}

Customer_AVL_Tree::path_discount_Output Customer_AVL_Tree::traverse_by_customer(Customer *customer_ptr) const
{
    Linked_List<Tree_Node*> path;
    double cumulative_discount = 0;
    Tree_Node *current_node = __root;
    while (current_node != nullptr)
    {
        cumulative_discount += current_node->__data->get_discount_component();
        path.push_front(current_node);
        if (__compare(*(current_node->__data), customer_ptr)) // customer is greater than the current node.
        {
            current_node = current_node->__right; //go right.
        }
        else if (__compare(customer_ptr, *(current_node->__data))) // customer is smaller than the current node.
        {
            current_node = current_node->__left; //go left.
        }
        else                                     // customer is equal to the current node.
        {
            return path_discount_Output(path, cumulative_discount);     // customer found.
        }
    }
    path.push_front(current_node);               //
    return path_discount_Output(path);           // requested customer not found.
}

// Customer_AVL_Tree::path_discount_Output Customer_AVL_Tree::member_traverse_by_key(int key) const
// {
//     Linked_List<Tree_Node*> path;
//     double cumulative_discount = 0;
//     Tree_Node* current_node = __root;
//     while (current_node != nullptr)
//     {
//         cumulative_discount += current_node->__data->get_discount_component();
//         path.push_front(current_node);
//         if (__compare(*(current_node->__data), key)) // key is greater than the current node.
//         {
//             current_node = current_node->__right; //go right.
//         }
//         else if (__compare(key, *(current_node->__data))) // key is smaller than the current node.
//         {
//             current_node = current_node->__left; //go left.
//         }
//         else                                     // key is equal to the current node.
//         {
//             return path_discount_Output(path, cumulative_discount);     // key found.
//         }
//     }
//     path.push_front(current_node);              //
//     return path_discount_Output(path);          // requested key not found.
// }

Customer_AVL_Tree::Tree_Node* Customer_AVL_Tree::find_lowest_common_ancestor(int id1, int id2) const
{   
    if (__root == nullptr)
    {
        throw PERFORMED_ACTION_ON_EMPTY_TREE();
    }
    Tree_Node* current_node = __root;
    while (!((*(current_node->__data) > id1 && *(current_node->__data) < id2) || (*(current_node->__data) == id1 || *(current_node->__data) == id2)))
    {
        if (*(current_node->__data) < id1)
        {
            if (current_node->__right == nullptr)       // lower index is greater than the max index in the tree.
            {
                return nullptr;
            }
            current_node = current_node->__right;
        }
        else
        {
            if (current_node->__left == nullptr)       // upper index is smaller than the min index in the tree.
            {
                return nullptr;
            }
            current_node = current_node->__left;
        }
    }
    return current_node;
}

Customer** Customer_AVL_Tree::insert(Customer* const &customer_ptr)
{
    path_discount_Output path_discount = traverse_by_customer(customer_ptr);            //
    Linked_List<Customer_AVL_Tree::Tree_Node*> path = path_discount.get_path();         //  added usage of path_discount_Output tuple.
    double cumulative_discount = path_discount.get_cumulative_discount();              //
    Customer_AVL_Tree::Tree_Node* current_node = path.pop_front();
    if (current_node != nullptr) // the customer_ptr already exists in the tree.
    {
        return nullptr;
    }

    // the customer_ptr needs to be inserted:
    Customer** direct_ptr_to_data = nullptr;
    if(path.get_length()==0)  //empty tree case
    {
        __root = new Tree_Node(customer_ptr); //insert to root.
        direct_ptr_to_data = &__root->__data;
        __size++;
        (*direct_ptr_to_data)->set_discount_component(-cumulative_discount);    // added to update the inserted member's discount.
        return direct_ptr_to_data;
    }

    current_node = path.pop_front();
    if (__compare(*(current_node->__data), customer_ptr)) // customer is greater than the current node.
    {
        current_node->__right = new Customer_AVL_Tree::Tree_Node(customer_ptr); //insert to the right.
        direct_ptr_to_data = &current_node->__right->__data;
    }
    else if (__compare(customer_ptr, *(current_node->__data)))    // customer is lesser than the current node.
    {                                               // should always execute if got here.
        current_node->__left = new Customer_AVL_Tree::Tree_Node(customer_ptr); //insert to the left.
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
            Customer_AVL_Tree::Tree_Node* new_son = nullptr;
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
                Customer_AVL_Tree::Tree_Node* father = path.front();
                if(__compare(new_son->__data, father->__data))    // put it to the left
                    father->__left = new_son;
                else    // put it to the right
                    father->__right = new_son;
            }
            else    // father is root
                __root = new_son;
            (*direct_ptr_to_data)->set_discount_component(-cumulative_discount);    // added to update the inserted member's discount.
            return direct_ptr_to_data;
        }
        update_height(current_node);
    }
    (*direct_ptr_to_data)->set_discount_component(-cumulative_discount);    // added to update the inserted member's discount.
    return direct_ptr_to_data;
}

void Customer_AVL_Tree::add_discount(int id1, int id2, double amount) const
{
    Tree_Node* common_ancestor = find_lowest_common_ancestor(id1, id2); //TODO - nullptr could be ok, if both in bounds but not found
    if (common_ancestor == nullptr)
    {
        throw BOTH_IDS_OUT_OF_BOUNDS();   
    }
    common_ancestor->__data->add_discount(amount);

    // right updates:
    bool has_discount = true;
    Customer_AVL_Tree::Tree_Node* current_node = common_ancestor->__right;
    while (current_node != nullptr)
    {
        if (*(current_node->__data) < id2) // need to go right.
        {
            if (!has_discount)
            {
                current_node->__data->add_discount(amount);
                has_discount = true;
            }
            current_node = current_node->__right;
        }
        else                            // need to go left.
        {
            if (has_discount)
            {
                current_node->__data->add_discount(-amount);
                has_discount = false;
            }
            current_node = current_node->__left;
        }
    }

    // left updates:
    has_discount = true;
    current_node = common_ancestor->__left;
    while (current_node != nullptr)
    {
        if (*(current_node->__data) == id1) // found id1. only relevant for left updates.
        {
            if (!has_discount)
            {
                current_node->__data->add_discount(amount);
            }
            if (current_node->__left != nullptr)
            {
                current_node->__left->__data->add_discount(-amount);
            }
            break;
        }
        if (*(current_node->__data) < id1) // need to go right.
        {
            if (has_discount)
            {
                current_node->__data->add_discount(-amount);
                has_discount = false;
            }
            current_node = current_node->__right;
        }
        else    // need to go left.
        {
            if (!has_discount)
            {
                current_node->__data->add_discount(amount);
                has_discount = true;
            }
            current_node = current_node->__left;
        }
    }
}

double Customer_AVL_Tree::get_expenses(Customer& customer) const
{
    path_discount_Output path_discount = traverse_by_customer(&customer);
    Tree_Node* target = path_discount.get_path().front();
    if (target == nullptr || !(target->__data)->get_is_member())
    {
        throw NO_MEMBER_WITH_SUCH_ID();
    }
    return target->__data->get_member_debt() - path_discount.get_cumulative_discount();
}

Customer_AVL_Tree::Tree_Node* Customer_AVL_Tree::rotate_LR(Tree_Node* current_node) //current_node = A
{
    Tree_Node* left = current_node->__left;  //save B
    Tree_Node* new_root = left->__right;    //new_root = C
    //---new---
        //C'=C+B+A
        //B'+ C'= B+A  ->  B'=B+A-C'
        //A'+ C'= A -> A'= A-C'
        new_root->__data->set_discount_component(new_root->__data->get_discount_component() + left->__data->get_discount_component() + current_node->__data->get_discount_component());
        left->__data->set_discount_component(left->__data->get_discount_component() + current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        current_node->__data->set_discount_component(current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        //Y'+B'+C'= Y+C+B+A  -> Y'=Y+C+B+A-B'-C'=Y-B'
        //W'+A'+C'= W+C+B+A  -> W'=W+C+B+A-A'-C'=W-A'
        Tree_Node* Y = new_root->__left;
        Tree_Node* W = new_root->__right;
        if(Y!=nullptr)
            Y->__data->set_discount_component(Y->__data->get_discount_component() - left->__data->get_discount_component());
        if(W!=nullptr)
            W->__data->set_discount_component(W->__data->get_discount_component() - current_node->__data->get_discount_component());
    //---------
    left->__right = new_root->__left;    //B->right = C->left (Y)
    new_root->__left = left;    //C->left = B
    current_node->__left = new_root->__right;   //A->left = C->right (W)
    new_root->__right = current_node;   ////C->right = A
    update_height(current_node);    //update A
    update_height(left);    //update B
    update_height(new_root);    //update C
    return new_root;
}

Customer_AVL_Tree::Tree_Node* Customer_AVL_Tree::rotate_LL(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__left;    //new_root = B
    Tree_Node* mid = new_root->__right;  //save Y
    //---new---
        //B'=B+A
        //A'+ B'= A  ->  A'=A-B'
        new_root->__data->set_discount_component(new_root->__data->get_discount_component() + current_node->__data->get_discount_component());
        current_node->__data->set_discount_component(current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        //Y'+A'+B'=Y+B+A  ->  Y'=Y+B+A-B'-A'=Y-A'
        if(mid!=nullptr)
            mid->__data->set_discount_component(mid->__data->get_discount_component() - current_node->__data->get_discount_component());
    //---------
    new_root->__right = current_node;   //B->right = A
    current_node->__left = mid; //A->left = Y
    update_height(current_node);    //update a
    update_height(new_root);    //update B
    return new_root;
}

Customer_AVL_Tree::Tree_Node* Customer_AVL_Tree::rotate_RL(Tree_Node* current_node) //current_node = A
{
    Tree_Node* right = current_node->__right;  //save B
    Tree_Node* new_root = right->__left;    //new_root = C
    //---new---
        //C'=C+B+A
        //B'+ C'= B+A  ->  B'=B+A-C'
        //A'+ C'= A -> A'= A-C'
        new_root->__data->set_discount_component(new_root->__data->get_discount_component() + right->__data->get_discount_component() + current_node->__data->get_discount_component());
        right->__data->set_discount_component(right->__data->get_discount_component() + current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        current_node->__data->set_discount_component(current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        //Y'+A'+C'= Y+C+B+A  -> Y'=Y+C+B+A-A'-C'=Y-A'
        //W'+B'+C'= W+C+B+A  -> W'=W+C+B+A-B'-C'=W-B'
        Tree_Node* Y = new_root->__left;
        Tree_Node* W = new_root->__right;
        if(Y!=nullptr)
            Y->__data->set_discount_component(Y->__data->get_discount_component() - current_node->__data->get_discount_component());
        if(W!=nullptr)
            W->__data->set_discount_component(W->__data->get_discount_component() - right->__data->get_discount_component());
    //---------
    right->__left = new_root->__right;    //B->left = C->right (W)
    new_root->__right = right;    //C->right = B
    current_node->__right = new_root->__left;   //A->right = C->left (Y)
    new_root->__left = current_node;   ////C->left = A
    update_height(current_node);    //update A
    update_height(right);    //update B
    update_height(new_root);    //update C
    return new_root;
}

Customer_AVL_Tree::Tree_Node* Customer_AVL_Tree::rotate_RR(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__right;    //new_root = B
    Tree_Node* mid = new_root->__left;  //save X
    //---new---
        //B'=B+A
        //A'+ B'= A  ->  A'=A-B'
        new_root->__data->set_discount_component(new_root->__data->get_discount_component() + current_node->__data->get_discount_component());
        current_node->__data->set_discount_component(current_node->__data->get_discount_component() - new_root->__data->get_discount_component());
        //X'+A'+B'=X+B+A  ->  X'=X+B+A-B'-A'=X-A'
        if(mid!=nullptr)
            mid->__data->set_discount_component(mid->__data->get_discount_component() - current_node->__data->get_discount_component());
    //---------
    new_root->__left = current_node;   //B->left = A
    current_node->__right = mid; //A->right = X
    update_height(current_node);    //update A
    update_height(new_root);    //update B
    return new_root;
}

void Customer_AVL_Tree::reset_debt()
{
    Tree_Node* current_node = __root;
    Linked_List<Tree_Node*> path;
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
        current_node->__data->set_discount_component(0);
        current_node->__data->set_member_debt(0);
        current_node = current_node->__left;
    }
}