objects = src/test.o


test: $(objects)
	g++ -Wall -o test $(objects) -lssl -lcrypto

test.o: src/dynamiccuckoofilter.h


.PHONY : clean
clean :
	rm test $(objects)
