#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"
#include "DS/hash_table.h"
#include "DS/customer_avl_tree.h"
#include "DS/Record_union_DB.h"
#include "Objects/customer.h"
#include "Objects/record.h"

class RecordsCompany {
  private:
    int __records_amount;
    Hash_table<Customer>  __customer_table;
    Customer_AVL_Tree __customer_ptr_tree;
    Record_union_DB *__records_piles;
    Record **__records;

    // exception for debugging.
    class SHOULDNT_GET_HERE{};

  public:
    RecordsCompany();
    ~RecordsCompany();
    StatusType newMonth(int *records_stocks, int number_of_records);
    StatusType addCostumer(int c_id, int phone);
    Output_t<int> getPhone(int c_id);
    StatusType makeMember(int c_id);
    Output_t<bool> isMember(int c_id);
    StatusType buyRecord(int c_id, int r_id);
    StatusType addPrize(int c_id1, int c_id2, double  amount);
    Output_t<double> getExpenses(int c_id);
    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *hight);
};

#endif