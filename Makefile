OBJS	= mainWet2.o Customer_avl_tree.o customer.o record.o recordsCompany.o
#SOURCE	= mainWet2.cpp DS/Customer_avl_tree.cpp DS/Record_union_DB.cpp Objects/customer.cpp Objects/record.cpp recordsCompany.cpp utilesWet2.cpp
OUT	= mainWet2
CC	 = g++
FLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -c 
LFLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -o

all: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(OUT)

mainWet2.o: mainWet2.cpp utilesWet2.h recordsCompany.h
	$(CC) $(FLAGS) mainWet2.cpp

Customer_avl_tree.o: DS/Customer_avl_tree.cpp DS/avl_tree.h Objects/customer.h
	$(CC) $(FLAGS) DS/Customer_avl_tree.cpp

customer.o: Objects/customer.cpp
	$(CC) $(FLAGS) Objects/customer.cpp

record.o: Objects/record.cpp
	$(CC) $(FLAGS) Objects/record.cpp

recordsCompany.o: recordsCompany.cpp utilesWet2.h DS/hash_table.h DS/customer_avl_tree.h DS/Record_union_DB.h Objects/customer.h Objects/record.h
	$(CC) $(FLAGS) recordsCompany.cpp


clean:
	del /f $(OBJS) $(OUT).exe
#	mingw32-make