#ifndef RECORD_H
#define RECORD_H

class Record
{
    const int __c_id;
    int __purchases;
    int __stock_amount;

public:

    /**
        C'tor for the record.
    */
    Record(int id, int stock_amount);

    /**
        Executes a record purchase.
        @retval The record's price.
        @return The price of the record.
        @details increments the record's amount of purchases by 1.
    */
    int purchase();

};

#endif