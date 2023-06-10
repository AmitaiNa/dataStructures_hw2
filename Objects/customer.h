#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{

    const int __id;
    const int __phone_number;
    bool __get_is_member;
    double __debt;
    double __discount_component;

public:

    /**
        C'tor for the customer.
    */
    Customer(int id, int phone_number=0);

    /**
        Getter for the member's discount.
        @retval The member's discount.
    */
    double get_discount_component() const;

    /**
        Setter for the member's discount.
        @param calculated_discount The amount to discount. will be replaced.
    */
    void set_discount_component(double calculated_discount);

    /**
        Getter for the member's debt.
        @retval The member's debt.
    */
    double get_member_debt() const;

    /**
        Setter for the member's debt.
        @retval The member's debt.
    */
    void set_member_debt(double newDebt);

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
    bool get_is_member() const;

    /**
        Makes the customer a member.
    */
    void make_member();

    /**
        Getter for the customer's phone number.
        @retval The customers phone number.
    */
    int get_phone() const;

    /**
        Cast to int operator. returns the customer's id.
        Needed for use with trees.
    */
    operator int() const;

};

#endif