#include "record.h"

Record::Record(int id, int stock_amount) : __id(id), __purchases(0), __stock_amount(stock_amount)
{}

int Record::purchase()
{
    return 100 + __purchases++;
}