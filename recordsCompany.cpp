#include "recordsCompany.h"

RecordsCompany::RecordsCompany() : __records_piles(nullptr), __records(nullptr)
{}

RecordsCompany::~RecordsCompany()
{
    if(__records_piles!=nullptr)
        delete __records_piles;
    if(__records!=nullptr)
    {
        for (int i = 0; i < __records_amount; ++i)
        {
            delete __records[i];
        }
        delete[] __records;
    }
}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records)
{
    if(number_of_records<0)
        return StatusType::INVALID_INPUT;
    try
    {
        if(__records_piles!=nullptr)
            delete __records_piles;
        if(__records!=nullptr)
        {
            for (int i = 0; i < __records_amount; ++i)
            {
                delete __records[i];
            }
            delete[] __records;
        }
        __member_ptr_tree.reset_debt();
        __records_amount = number_of_records;
        __records_piles = new Record_union_DB(records_stocks,number_of_records);
        __records = new Record*[number_of_records];
        for(int i=0 ; i< number_of_records ; i++)
        {
            __records[i] = new Record(i, records_stocks[i]);
        }
        return StatusType::SUCCESS;
    }
    catch(...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType RecordsCompany::addCostumer(int c_id, int phone)
{
    if(c_id<0 || phone<0)
        return StatusType::INVALID_INPUT;
    try
    {
        Customer *newCustomerPTR = __customer_table.insert(Customer(c_id, phone));
        if(newCustomerPTR==nullptr)
            return StatusType::ALREADY_EXISTS;
        return StatusType::SUCCESS;
    }
    catch(...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

Output_t<int> RecordsCompany::getPhone(int c_id)
{
    if (c_id < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Customer* customer = __customer_table.find(c_id);
    if (customer == nullptr)
    {
        return StatusType::DOESNT_EXISTS;
    }
    return customer->get_phone();
}

StatusType RecordsCompany::makeMember(int c_id)
{
    if (c_id < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Customer* customer_ptr = __customer_table.find(c_id);
    if(customer_ptr==nullptr)
        return StatusType::DOESNT_EXISTS;
    if (customer_ptr->get_is_member())
    {
        return StatusType::ALREADY_EXISTS;
    }
    customer_ptr->make_member();
    if(__member_ptr_tree.insert(customer_ptr) == nullptr)
        throw SHOULDNT_GET_HERE();
    __member_ptr_tree.compensateDiscountInPath(customer_ptr);
    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id)
{
    if (c_id < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Customer* customer_ptr = __customer_table.find(c_id);
    if (customer_ptr == nullptr)
    {
        return StatusType::DOESNT_EXISTS;
    }
    return customer_ptr->get_is_member();
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id)
{
    if (c_id < 0 || r_id < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Customer* customer_ptr = __customer_table.find(c_id);
    if (customer_ptr == nullptr || r_id >= __records_amount)
    {
        return StatusType::DOESNT_EXISTS;
    }
    int cost = __records[r_id]->purchase();
    if (customer_ptr->get_is_member())
    {
        customer_ptr->set_member_debt(customer_ptr->get_member_debt() + cost);
    }
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount)
{
    if (c_id1 < 0 || c_id2 < c_id1 || amount <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try
    {
        __member_ptr_tree.add_discount(c_id1, c_id2, amount);
    }
    catch(const Member_AVL_Tree::NO_ACTION_PERFORMED&)
    {
        return StatusType::SUCCESS; // no action was performed.
    }
    return StatusType::SUCCESS; // successful operation.
}

Output_t<double> RecordsCompany::getExpenses(int c_id)
{
    if (c_id < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    double expenses;
    try
    {
        expenses = __member_ptr_tree.get_expenses(Customer(c_id));
    }
    catch(const Member_AVL_Tree::NO_MEMBER_WITH_SUCH_ID&)
    {
        return StatusType::DOESNT_EXISTS;
    }
    return expenses;
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2)
{
    return __records_piles->putOnTop(r_id1, r_id2);
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight)
{
    return __records_piles->getPlace(r_id, column, hight);
}