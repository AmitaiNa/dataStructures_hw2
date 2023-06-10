#ifndef CLIENT_H
#define CLIENT_H

class Client
{

    int __id;
    int __phone_number;
    bool __is_member;
    double __debt;
    double __discount;

public:

    /**
        C'tor for the client.
    */
    Client(int id, int phone_number);

    /**
        Getter for the member's discount.
        @retval The member's discount.
    */
    double get_member_discount() const;

    /**
        Setter for the member's discount. called when inserting a member to a tree.
        @param calculated_discount The member's discount. calculated to maintain offset from previous member discounts.
        @details Receives the cumulative discount and sets its negative to the member's discount
    */
    void set_member_cumulative_discount(double calculated_discount);

    /**
        Cast to int operator. returns the client's id.
        Needed for use with trees.
    */
    operator int() const;

};

#endif