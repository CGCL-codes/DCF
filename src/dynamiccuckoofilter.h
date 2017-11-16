#ifndef DYNAMICCUCKOOFILTER_H_
#define DYNAMICCUCKOOFILTER_H_


#include"cuckoofilter.h"
#include"linklist.h"
#include<list>
#include<math.h>
#include<iostream>



class DynamicCuckooFilter{
private:

	int capacity;
	int single_capacity;

	int single_table_length;

	double false_positive;
	double single_false_positive;

	double fingerprint_size_double;
	int fingerprint_size;

	Victim victim;

	CuckooFilter* curCF;
	CuckooFilter* nextCF; //it's temporary. uses getNextCF() to get the next CF;

public:

	//record the items inside DCF
	int counter;

	// the link list of building blocks CF1, CF2, ...
	LinkList* cf_list;

	//construction & distruction functions
	DynamicCuckooFilter(const size_t capacity, const double false_positive, const size_t exp_block_num = 6);
	~DynamicCuckooFilter();

	//insert & query & delete functions
	bool insertItem(const char* item);
	CuckooFilter* getNextCF(CuckooFilter* curCF);
	bool failureHandle(Victim &victim);
	bool queryItem(const char* item);
	bool deleteItem(const char* item);

	//compaction
	bool compact();
	void sort(CuckooFilter** cfq, int queue_length);
	bool remove(CuckooFilter* cf_remove);

	//generate 2 bucket addresses
	void generateIF(const char* item, size_t &index, uint32_t &fingerprint, int fingerprint_size, int single_table_length);
	void generateA(size_t index, uint32_t fingerprint, size_t &alt_index, int single_table_length);

	//get info of DCF
	int getFingerprintSize();
	float size_in_mb();

	//extra function to make sure the table length is the power of 2
	uint64_t upperpower2(uint64_t x);

	void info();
};







#endif //DYNAMICCUCKOOFILTER_H
