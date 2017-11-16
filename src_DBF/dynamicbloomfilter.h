#ifndef DYNAMICBLOOMFILTER_H_
#define DYNAMICBLOOMFILTER_H_

#include<string>
#include<iostream>
#include"hashfunctions.h"
#include"countingbloomfilter.h"


class LinkList{
public:
	CountingBloomFilter* cf_pt;
	CountingBloomFilter* tail_pt;
	static int num;
	LinkList(int single_capacity, double single_false_positive){
		cf_pt = new CountingBloomFilter(single_capacity, single_false_positive);
		tail_pt = new CountingBloomFilter(single_capacity, single_false_positive);
	}

	LinkList(int n, int m){
		cf_pt = new CountingBloomFilter(n, m);
		tail_pt = new CountingBloomFilter(n, m);
	}

	~LinkList(){
		delete cf_pt;
		delete tail_pt;
	}
};

class DynamicBloomFilter{
private:
	double false_positive;
	double single_false_positive;
	int capacity;
	int single_capacity;
	int counter;
	int bits_num;
	int hash_num;

	CountingBloomFilter* curSBF;
	CountingBloomFilter* nextSBF;


public:

	// the link list of building blocks
	LinkList* sbf_list;

	DynamicBloomFilter(const size_t capacity, const double false_positive, const size_t exp_block_num = 6);
	DynamicBloomFilter(int n,  int m);
	~DynamicBloomFilter();

	bool insertItem(const char* item);
	CountingBloomFilter* getNextSBF(CountingBloomFilter* curSBF);
	bool queryItem(const char* item);
	bool deleteItem(const char* item);

	unsigned long int* generateHashVal(const char* item);

	float size_in_mb();

};


#endif //DYNAMICBLOOMFILTER_H_

