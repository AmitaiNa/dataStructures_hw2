#ifndef UNION_H_
#define UNION_H_

class Union
{
    private:
        class Reversed_tree
        {
            public:
                class Node
                {
                    public:
                        Node* __father;
                        Node* __root;
                        int __index;

                        /**
                            C'tor of the node
                        */
                        Node(int index = 0) : __father(nullptr), __root(nullptr), __index(index)
                        {}
                };
            
                Node* __root;
                int __size;
                int __index;
                /**
                    C'tor of the Reversed_tree.
                */
                Reversed_tree(Node* root = nullptr, int index = 0) : __root(root), __size(1), __index(index)
                {}
        };

        int __size;
        Reversed_tree::Node** __members;
        Reversed_tree** __groups;
    public:
        Union(int size) : __size(size) , __members(new Reversed_tree::Node*[size]), __groups(new Reversed_tree*[size])
        {
            for(int i=0; i<size; i++)
            {
                __members[i] = new Reversed_tree::Node(i);
                __groups[i] = new Reversed_tree(__members[i], i);
            }
        }

        Reversed_tree* find(int index)
        {
            
        }
};

#endif // UNION_H_