#ifndef COUNTINGBLOOMFILTER_H_
#define COUNTINGBLOOMFILTER_H_

#include<math.h>


class CountingBloomFilter{
private:
	char* bits;
	int byte_num;
	int bits_num;
	int hash_num;

public:
	CountingBloomFilter* next;
	CountingBloomFilter* front;
	int capacity;
	int item_num;

	CountingBloomFilter(int capacity, double false_positive);
	CountingBloomFilter(int n, int m);
	~CountingBloomFilter();

	bool insertItem(unsigned long int* hash_value);
	bool queryItem(unsigned long int* hash_value);
	bool deleteItem(unsigned long int* hash_value);

	bool write(unsigned long int hash_value, int counter);
	int read(unsigned long int hash_value);
};

#endif //COUNTINGBLOOMFILTER_H_
