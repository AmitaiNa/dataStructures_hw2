#include <iostream>
#include "Record_union_DB.h"


void print(Record_union_DB &db)
{
    int column, height;
    StatusType status;
    std::cout << std::endl <<"---status---"<< std::endl;
    for(int i=0;i<4;i++)
    {
        status = db.getPlace(i, &column, &height);
        if (status == StatusType::SUCCESS)
        {
            std::cout << "Record r_id: "<<i<<" is at column: " << column << " and height: " << height << std::endl;
        }
        else
        {
            std::cout << "Failed to get place for Record "<<i<<". Status: " << static_cast<int>(status) << std::endl;
        }
    }
    std::cout <<"---------"<< std::endl<<std::endl;
}

void put(Record_union_DB &db, int base, int top)
{
    StatusType status;
    status = db.putOnTop(base, top);
    if (status == StatusType::SUCCESS) {
        std::cout << "Successfully put Record "<<top<<" on top of Record "<<base<<"." << std::endl;
    } else {
        std::cout << "Failed to put Record "<<top<<" on top of Record "<<base<<". Status: " << static_cast<int>(status) << std::endl;
    }
}



int main() {
    // Create an array of record node stocks
    int recordNodesStocks[] = {10, 5, 8, 12};

    // Create a Record_union_DB object
    Record_union_DB db(recordNodesStocks, 4);

    print(db);
    // Test the getPlace function

    // Test the putOnTop function
    put(db, 1, 3);

    print(db);

    put(db, 1, 2);

    print(db);




    return 0;
}
