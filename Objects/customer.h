#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{

    const int __id;
    const int __phone_number;
    bool __is_member;
    double __debt;
    double __discount_component;

public:

    /**
        C'tor for the customer.
    */
    Customer(int id, int phone_number);

    /**
        Getter for the member's discount.
        @retval The member's discount.
    */
    double get_member_discount_component() const;

    /**
        Setter for the member's discount.
        @param calculated_discount The amount to discount.
    */
    void set_member_discount_component(double calculated_discount);

    /**
        Getter for the member's debt.
        @retval The member's debt.
    */
    double get_member_debt() const;

    /**
        Add the given discount to a common ancestor.
        @param amount The amount of discount to add.
    */
    void add_discount(double amount);

    /**
        Returns the customer's member status.
        @retval True - if the customer is a member.
        @retval False - else.
    */
    bool is_member();

    /**
        Cast to int operator. returns the customer's id.
        Needed for use with trees.
    */
    operator int() const;

};

#endif