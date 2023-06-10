#ifndef Record_node_union_DB_H_
#define Record_node_union_DB_H_

#include "../utilesWet2.h"

class Group_reversed_tree; //F.D

class Record_node
{
    public:
        Record_node* __father;
        const int __r_id;
        const int __amount;
        int __height;

        Group_reversed_tree* __R_my_tree;   //will be used only for roots

        /**
            C'tor of the Record_node
        */
        Record_node(int r_id, int amount) : __father(nullptr),
            __r_id(r_id), __amount(amount), __height(0), __R_my_tree(nullptr)
        {}
};

class Group_reversed_tree
{
    public:
    
        Record_node* __root;
        const int __column;
        int __group_nodes_amount;
        int __column_height;
        /**
            C'tor of the Group_reversed_tree.
        */
        Group_reversed_tree(Record_node* first_node, int column) : __root(first_node), __column(column),
            __group_nodes_amount(1), __column_height(first_node->__amount)
        {}
};

class Record_union_DB
{
    private:
        const int __records_amount;
        Record_node** __records;
        Group_reversed_tree** __groups;
    public:
        Record_union_DB(int *Record_nodes_stocks, int number_of_Record_nodes) :
            __records_amount(number_of_Record_nodes),
            __records(new Record_node*[number_of_Record_nodes]),
            __groups(new Group_reversed_tree*[number_of_Record_nodes])
        {
            for(int i=0; i<number_of_Record_nodes; i++)
            {
                __records[i] = new Record_node(i, Record_nodes_stocks[i]);    //new Record_node node
                __groups[i] = new Group_reversed_tree(__records[i], i);    //build new tree from the Record_node node
                __records[i]->__R_my_tree = __groups[i]; //update the root and the only Record_node node about the group tree
            }
        }

        ~Record_union_DB()
        {
            delete[] __records;
            delete[] __groups;
        }

        StatusType getPlace(int r_id, int *column, int *height)
        {
            if(r_id<0 || column==nullptr || height==nullptr)
                return StatusType::INVALID_INPUT;
            if(r_id >= __records_amount)
                return StatusType::DOESNT_EXISTS;
            
            Record_node* root = __records[r_id];
            int sum = 0;// used to update heights after shorting routes
            while(root->__father!=nullptr)    //find the root
            {
                sum+=root->__height;
                root = root->__father;
            }

            Record_node* current = __records[r_id];
            int toSubtract = 0;
            while(current!=root)   //shorting routes for all nodes to root
            {
                Record_node* tempNext = current->__father;
                int tempHeight = current->__height;

                current->__father = root;
                current->__height = sum - toSubtract;

                toSubtract+=tempHeight;
                current = tempNext;
            }
            *column = root->__R_my_tree->__column;
            *height = sum + root->__height;//== __records[r_id]+root->__height after shorting routes
            return StatusType::SUCCESS ;
        }
        
        StatusType putOnTop(int r_id1, int r_id2)
        {
            if(r_id1<0 || r_id2<0)
                return StatusType::INVALID_INPUT;
            if(r_id1>=__records_amount || r_id2>=__records_amount)
                return StatusType::DOESNT_EXISTS;
            if(r_id1==r_id2)
                return StatusType::FAILURE;    //TODO - check if this is the right status
            int r_id1_group, r_id2_group, temp;
            getPlace(r_id1, &r_id1_group, &temp);
            getPlace(r_id2, &r_id2_group, &temp);
            if(r_id1_group == r_id2_group)
                StatusType::FAILURE;

            Group_reversed_tree *bigger, *smaller, *base, *top;
            base = __groups[r_id1_group];
            top = __groups[r_id2_group];
            if(base->__group_nodes_amount >= top->__group_nodes_amount)
            {
                bigger = base;
                smaller = top;
                top->__root->__height += (base->__column_height - base->__root->__height);
            }
            else
            {
                bigger = top;
                smaller = base;
                top->__root->__height += base->__column_height;
                base->__root->__height -= top->__root->__height;
            }
            
            smaller->__root->__R_my_tree = nullptr; //not a root anymore
            smaller->__root->__father = bigger->__root; //connect groups
            bigger->__group_nodes_amount += smaller->__group_nodes_amount;  //update merged amount
            bigger->__column_height += smaller->__column_height;    //update merged size
            __groups[smaller->__column]= nullptr;   //remove from groups array in DB
            delete smaller; //delete group
            return StatusType::SUCCESS;
        }

};

#endif // Record_node_union_DB_H_