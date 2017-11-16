objects = test.o dynamiccuckoofilter.o cuckoofilter.o hashfunction.o 

test: $(objects)
	g++ -Wall -o test $(objects) -lssl -lcrypto

hashfunction.o: src/hashfunction.cpp src/hashfunction.h
	g++ -Wall -c src/hashfunction.cpp -lssl -lcrypto

cuckoofilter.o: src/cuckoofilter.cpp src/cuckoofilter.h
	g++ -Wall -c src/cuckoofilter.cpp -lssl -lcrypto

dynamiccuckoofilter.o: src/dynamiccuckoofilter.cpp src/cuckoofilter.h
	g++ -Wall -c src/dynamiccuckoofilter.cpp -lssl -lcrypto

test.o: src/test.cpp src/dynamiccuckoofilter.h
	g++ -Wall -c src/test.cpp -lssl -lcrypto


.PHONY : clean
clean :
	rm *.o
