OBJS	= mainWet2.o avl_tree.o Customer_avl_tree.o hash_table.o linked_list.o Record_union_DB.o customer.o record.o recordsCompany.o utilesWet2.o
#SOURCE	= mainWet2.cpp DS/Customer_avl_tree.cpp DS/Record_union_DB.cpp Objects/customer.cpp Objects/record.cpp recordsCompany.cpp utilesWet2.cpp
OUT	= mainWet2
CC	 = g++
FLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -c 
LFLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -o

all: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(OUT)

mainWet2.o: mainWet2.cpp utilesWet2.h recordsCompany.h
	$(CC) $(FLAGS) mainWet2.cpp

Customer_avl_tree.o: Customer_avl_tree.cpp DS/avl_tree.h Objects/customer.h
	$(CC) $(FLAGS) Customer_avl_tree.cpp

hash_table.o: DS/avl_tree.h
	$(CC) $(FLAGS) hash_table.h

linked_list.o: DS/linked_list.h
	$(CC) $(FLAGS) linked_list.h

Record_union_DB.o: DS/Record_union_DB.h utilesWet2.h
	$(CC) $(FLAGS) Record_union_DB.h

customer.o: customer.cpp
	$(CC) $(FLAGS) customer.cpp

record.o: record.cpp
	$(CC) $(FLAGS) record.cpp

recordsCompany.o: recordsCompany.cpp utilesWet2.h DS/hash_table.h DS/customer_avl_tree.h DS/Record_union_DB.h Objects/customer.h Objects/record.h
	$(CC) $(FLAGS) recordsCompany.cpp

utilesWet2.o: utilesWet2.h
	$(CC) $(FLAGS) utilesWet2.h

avl_tree.o: DS/avl_tree.h DS/linked_list.h
	$(CC) $(FLAGS) avl_tree.h


clean:
	del /f $(OBJS) $(OUT).exe
	mingw32-make