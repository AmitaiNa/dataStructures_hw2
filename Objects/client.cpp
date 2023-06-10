#include "client.h"

Client::Client(int id, int phone_number) : __id(id), __phone_number(phone_number), __is_member(false), __debt(0), __discount(0)
{}

double Client::get_member_discount() const
{
    return __discount;
}

void Client::set_member_cumulative_discount_compensation(double calculated_debt)
{
    __discount = -calculated_debt;
}

Client::operator int() const
{
    return __id;
}