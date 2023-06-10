#include "customer.h"

Customer::Customer(int id, int phone_number) : __id(id), __phone_number(phone_number), __get_is_member(false), __debt(0), __discount_component(0)
{}

double Customer::get_discount_component() const
{
    return __discount_component;
}

void Customer::set_discount_component(double calculated_debt)
{
    __discount_component = calculated_debt;
}

double Customer::get_member_debt() const
{
    return __debt;
}

void Customer::set_member_debt(double newDebt)
{
    __debt=newDebt;
}

void Customer::add_discount(double amount)
{
    __discount_component += amount;
}

bool Customer::get_is_member() const
{
    return __get_is_member;
}

void Customer::make_member()
{
    __get_is_member = true;
}

int Customer::get_phone() const
{
    return __phone_number;
}

Customer::operator int() const
{
    return __id;
}