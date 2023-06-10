#include "customer.h"

Customer::Customer(int id, int phone_number) : __id(id), __phone_number(phone_number), __is_member(false), __debt(0), __discount(0)
{}

double Customer::get_member_discount() const
{
    return __discount;
}

void Customer::set_member_discount(double calculated_debt)
{
    __discount = calculated_debt;
}

double Customer::get_member_debt() const
{
    return __debt;
}

void Customer::add_discount(double amount)
{
    __discount += amount;
}

Customer::operator int() const
{
    return __id;
}