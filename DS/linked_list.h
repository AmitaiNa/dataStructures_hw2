#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

template <class T>
class Linked_List
{
private:

     struct List_Node;
     class CANNOT_POP_EMPTY_LIST{};
     List_Node* __head;
     int __length;

     /**
          loop throw nodes and delete them
     */
     void destroy_nodes();

public:

     /**
          C'tor of the linked list
     */
     Linked_List();

     /**
               D'tor of the linked list
     */
      ~Linked_List();

     /**
               operator=
     */
     Linked_List<T>& operator=(const Linked_List<T> &other);

     /**
               Get the list's current length.
               @retval The list's length.
     */
     int get_length() const;

     /**
               Inserts a T type object to the front of the list.
               @param T The object to be inserted the the list.
               @retval Pointer to the new node that was created.
     */
     List_Node* push_front(const T& entity);

     /**
               Return a T type object from the front of the list (without popping it).
               @retval a T type from the front of the list.
     */
     T& front() const;

     /**
               Pop a T type object from the front of the list.
               @retval The object removed from the list.
     */
     T pop_front();

     /**
               Remove a given node from the list.
               @param ptr The ptr to the node to be removed from the list.
     */
     void remove_node(const void* ptr);

};

//-----------------------Linked_List definitions-----------------------

template<class T>
Linked_List<T>::Linked_List() : __head(nullptr), __length(0)
{}

template<class T>
Linked_List<T>::~Linked_List()
{
     destroy_nodes();
}

template<class T>
void Linked_List<T>::destroy_nodes()
{
     List_Node* current_node = __head;
     List_Node* next_node;
     while(current_node!=nullptr)
     {
          next_node = current_node->__next;
          delete current_node;
          current_node = next_node;
     }
     __length=0;
     __head=nullptr;
}

template<class T>
Linked_List<T>& Linked_List<T>::operator=(const Linked_List<T> &other)
{
     destroy_nodes();
     Linked_List<T> temp_stack;
     List_Node* head = other.__head;
     while(head!=nullptr)
     {
          temp_stack.push_front(head->__data);
          head = head->__next;
     }
     head = temp_stack.__head;
     while(head!=nullptr)
     {
          push_front(head->__data);
          head = head->__next;
     }
     return *this;
}

template<class T>
typename Linked_List<T>::List_Node* Linked_List<T>::push_front(const T& entity)
{
     List_Node* new_node = new List_Node(entity);
     new_node->__next = __head;
     if(__head != nullptr)
          __head->__previous = new_node;
     __head = new_node;
     ++__length;
     return new_node;
}

template<class T>
T& Linked_List<T>::front() const
{
     if (__head == nullptr)
     {
          throw CANNOT_POP_EMPTY_LIST();
     }
     return __head->__data;
}

template<class T>
T Linked_List<T>::pop_front()
{
     if (__head == nullptr)
     {
          throw CANNOT_POP_EMPTY_LIST();
     }
     List_Node* node_to_pop = __head;
     __head = __head->__next;
     if(__head!=nullptr)
          __head->__previous = nullptr;
     T temp_T = node_to_pop->__data;
     delete node_to_pop;
     --__length;
     return temp_T;
}

template<class T>
void Linked_List<T>::remove_node(const void* ptr)
{
     List_Node* node = (List_Node*)ptr;
     if (__head == nullptr)
     {
          throw CANNOT_POP_EMPTY_LIST();
     }
     if (node->__previous == nullptr) // in case the node to be removed is the first in the list.
     {
          __head = node->__next;
          if (__head != nullptr) // in case the node to be removed is not the only node in the list.
          {
               __head->__previous = nullptr;
          }
     }
     else // in case the node to be removed is not the first.
     {
          node->__previous->__next = node->__next;
          if(node->__next != nullptr)   // in case the node to be removed is not the last in the list.
          {
               node->__next->__previous = node->__previous;
          }
     }
     delete node;
     --__length;
}

template<class T>
int Linked_List<T>::get_length() const
{
     return __length;
}

template <class T>
struct Linked_List<T>::List_Node
{

    T __data;
    List_Node* __next;
    List_Node* __previous;

   /**
        C'tor of the List_Node
   */
   List_Node(const T& entity) : __data(entity), __next(nullptr), __previous(nullptr)
   {}

};

#endif // LINKED_LIST_H_