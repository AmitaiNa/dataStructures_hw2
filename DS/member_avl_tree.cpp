#include "member_avl_tree.h"

Member_AVL_Tree::Member_AVL_Tree() : AVL_Tree<Client>()
{}

Member_AVL_Tree::path_discount_Output Member_AVL_Tree::member_traverse_by_client(const Client& client)
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

AVL_Tree<Client>::Tree_Node* Member_AVL_Tree::find_lowest_common_ancestor(int id1, int id2)
{   
    if (__root == nullptr)
    {
        throw PERFORMED_ACTION_ON_EMPTY_TREE();
    }
    AVL_Tree<Client>::Tree_Node* current_node = __root;
    while (!((current_node->__data > id1 && current_node->__data < id2) || (current_node->__data == id1 || current_node->__data == id2)))
    {
        if (current_node->__data < id1)
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

Client* Member_AVL_Tree::insert(const Client& client)
{
    path_discount_Output path_discount = member_traverse_by_client(client);            //
    Linked_List<AVL_Tree<Client>::Tree_Node*> path = path_discount.get_path();         //  added usage of path_discount_Output tuple.
    double cumulative_discount = path_discount.get_cumulative_discount();              //
    AVL_Tree<Client>::Tree_Node* current_node = path.pop_front();
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
        direct_ptr_to_data->set_member_cumulative_discount_compensation(cumulative_discount);    // added to update the inserted member's discount.
        return direct_ptr_to_data;
    }

    current_node = path.pop_front();
    if (__compare(current_node->__data, client)) // client is greater than the current node.
    {
        current_node->__right = new AVL_Tree<Client>::Tree_Node(client); //insert to the right.
        direct_ptr_to_data = &current_node->__right->__data;
    }
    else if (__compare(client, current_node->__data))    // client is lesser than the current node.
    {                                               // should always execute if got here.
        current_node->__left = new AVL_Tree<Client>::Tree_Node(client); //insert to the left.
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
            AVL_Tree<Client>::Tree_Node* new_son = nullptr;
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
                AVL_Tree<Client>::Tree_Node* father = path.front();
                if(__compare(new_son->__data, father->__data))    // put it to the left
                    father->__left = new_son;
                else    // put it to the right
                    father->__right = new_son;
            }
            else    // father is root
                __root = new_son;
            direct_ptr_to_data->set_member_cumulative_discount_compensation(cumulative_discount);    // added to update the inserted member's discount.
            return direct_ptr_to_data;
        }
        update_height(current_node);
    }
    direct_ptr_to_data->set_member_cumulative_discount_compensation(cumulative_discount);    // added to update the inserted member's discount.
    return direct_ptr_to_data;
}

void Member_AVL_Tree::add_discount(int id1, int id2, double amount)
{

}


Member_AVL_Tree::Tree_Node* Member_AVL_Tree::rotate_LR(Tree_Node* current_node) //current_node = A
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

Member_AVL_Tree::Tree_Node* Member_AVL_Tree::rotate_LL(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__left;    //new_root = B
    Tree_Node* mid = new_root->__right;  //save Y
    new_root->__right = current_node;   //B->right = A
    current_node->__left = mid; //A->left = Y
    update_height(current_node);    //update a
    update_height(new_root);    //update B
    return new_root;
}

Member_AVL_Tree::Tree_Node* Member_AVL_Tree::rotate_RL(Tree_Node* current_node) //current_node = A
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

Member_AVL_Tree::Tree_Node* Member_AVL_Tree::rotate_RR(Tree_Node* current_node) //current_node = A
{
    Tree_Node* new_root = current_node->__right;    //new_root = B
    Tree_Node* mid = new_root->__left;  //save X
    new_root->__left = current_node;   //B->left = A
    current_node->__right = mid; //A->right = X
    update_height(current_node);    //update A
    update_height(new_root);    //update B
    return new_root;
}