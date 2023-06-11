OBJS	= mainWet2.o Member_avl_tree.o customer.o record.o recordsCompany.o
OUT	= mainWet2
CC	 = g++
FLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -c 
LFLAGS	 = --std=c++11 -Wall -Werror -pedantic-errors -g -o

all: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(OUT)

mainWet2.o: mainWet2.cpp utilesWet2.h recordsCompany.h
	$(CC) $(FLAGS) mainWet2.cpp

Member_avl_tree.o: DS/Member_avl_tree.cpp DS/avl_tree.h Objects/customer.h
	$(CC) $(FLAGS) DS/Member_avl_tree.cpp

customer.o: Objects/customer.cpp
	$(CC) $(FLAGS) Objects/customer.cpp

record.o: Objects/record.cpp
	$(CC) $(FLAGS) Objects/record.cpp

recordsCompany.o: recordsCompany.cpp utilesWet2.h DS/hash_table.h DS/Member_avl_tree.h DS/Record_union_DB.h Objects/customer.h Objects/record.h
	$(CC) $(FLAGS) recordsCompany.cpp


clean:
	del /f $(OBJS) $(OUT).exe
#	mingw32-make


#powershell commands:
#Get-Content .\example_test.in | .\mainWet2.exe > .\example_test.res
#Get-Content .\example_test.in | .\mainWet2.exe | tee .\example_test.res