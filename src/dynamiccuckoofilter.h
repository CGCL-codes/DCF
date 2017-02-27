/*
 * dynamiccuckoofilter.h
 *
 *  Created on: Dec 21, 2016
 *      Author: liaoliangyi
 */

 // This file is the implementation of dynamiccuckoofilter
 // The function "upperpower2" is apart of the source code written by Bin Fan , David G. Andersen and Michael Kaminsky 

#ifndef DYNAMICCUCKOOFILTER_H_
#define DYNAMICCUCKOOFILTER_H_




#include"cuckoofilter.h"
#include"linklist.h"
#include<list>
#include<math.h>
#include<iostream>

using namespace std;


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
	DynamicCuckooFilter(const size_t capacity, const double false_positive, const size_t exp_block_num = 8);
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


DynamicCuckooFilter::DynamicCuckooFilter(const size_t item_num, const double fp, const size_t exp_block_num){

	capacity = item_num;

	single_table_length = upperpower2(capacity/4.0/exp_block_num);//2048 1024 512 256 128 ---!!!---must be the power of 2---!!!---
	single_capacity = single_table_length*0.9375*4;//s=6 1920 s=12 960 s=24 480 s=48 240 s=96 120

	false_positive = fp;
	single_false_positive = 1-pow(1.0-false_positive, ((double)single_capacity/capacity));

	fingerprint_size_double = ceil(log(8.0/single_false_positive)/log(2));
	if(fingerprint_size_double>0 && fingerprint_size_double<=4){
		fingerprint_size = 4;
	}else if(fingerprint_size_double>4 && fingerprint_size_double<=8){
		fingerprint_size = 8;
	}else if(fingerprint_size_double>8 && fingerprint_size_double<=12){
		fingerprint_size = 12;
	}else if(fingerprint_size_double>12 && fingerprint_size_double<=16){
		fingerprint_size = 16;
	}else if(fingerprint_size_double>16 && fingerprint_size_double<=24){
		fingerprint_size = 16;
	}else if(fingerprint_size_double>24 && fingerprint_size_double<=32){
		fingerprint_size = 16;
	}else{
		cout<<"fingerprint out of range!!!"<<endl;
		fingerprint_size = 16;
	}

	counter = 0;


	curCF = new CuckooFilter(single_table_length, fingerprint_size, single_capacity);
	nextCF = new CuckooFilter(single_table_length, fingerprint_size, single_capacity);

	cf_list = new LinkList(single_table_length, fingerprint_size, single_capacity);
	cf_list->cf_pt = curCF;
	cf_list->tail_pt = curCF;
	cf_list->num = 1;
}

DynamicCuckooFilter::~DynamicCuckooFilter(){
//	delete victim;
	delete curCF;
	delete nextCF;
	delete cf_list;
}



bool DynamicCuckooFilter::insertItem(const char* item){
	if(curCF->is_full == true){
		curCF = getNextCF(curCF);
	}

	if(curCF->insertItem(item, victim)){
		counter++;
	}else{
		failureHandle(victim);
		counter++;
	}

	return true;
}

CuckooFilter* DynamicCuckooFilter::getNextCF(CuckooFilter* curCF){
	if(curCF == cf_list->tail_pt){
		nextCF = new CuckooFilter(single_table_length, fingerprint_size, single_capacity);
		curCF->next = nextCF;
		nextCF->front = curCF;
		cf_list->tail_pt = nextCF;
		cf_list->num++;
	}else{
		nextCF = curCF->next;
		if(nextCF->is_full){
			nextCF = getNextCF(nextCF);
		}
	}
	return nextCF;
}

bool DynamicCuckooFilter::failureHandle(Victim &victim){
	nextCF = getNextCF(curCF);
	if(nextCF->insertItem(victim.index, victim.fingerprint,true, victim) == false){
		nextCF = getNextCF(nextCF);
		failureHandle(victim);
	}
	return true;
}

bool DynamicCuckooFilter::queryItem(const char* item){
	size_t index, alt_index;
	uint32_t fingerprint;

	generateIF(item, index, fingerprint, fingerprint_size, single_table_length);
	generateA(index, fingerprint, alt_index, single_table_length);

	CuckooFilter* query_pt = cf_list->cf_pt;
	for(int count = 0; count<cf_list->num; count++){
		if(query_pt->queryImpl(index, fingerprint)){
			return true;
		}else if(query_pt->queryImpl(alt_index, fingerprint)){
			return true;
		}else{
			query_pt = query_pt->next;
		}
		if(query_pt == 0){
			break;
		}
	}
	return false;
}

bool DynamicCuckooFilter::deleteItem(const char* item){
	size_t index, alt_index;
	uint32_t fingerprint;

	generateIF(item, index, fingerprint, fingerprint_size, single_table_length);
	generateA(index, fingerprint, alt_index, single_table_length);
	CuckooFilter* delete_pt = cf_list->cf_pt;
	for(int count = 0; count<cf_list->num; count++){
		if(delete_pt->queryImpl(index, fingerprint)){
			if(delete_pt->deleteImpl(index, fingerprint)){
				counter--;
				return true;
			}
		}else if(delete_pt->queryImpl(alt_index, fingerprint)){
			if(delete_pt->deleteImpl(alt_index ,fingerprint)){
				counter--;
				return true;
			}
		}else{
			delete_pt = delete_pt->next;
		}
	}
	return false;
}



bool DynamicCuckooFilter::compact(){
	int queue_length = 0;
	CuckooFilter* temp = cf_list->cf_pt;
	for(int count = 0; count<cf_list->num; count++){
		if(!temp->is_full){
			queue_length++;
		}
		temp = temp->next;
	}
	if(queue_length == 0){
		return true;
	}

	CuckooFilter** cfq  = new CuckooFilter*[queue_length];
	int pos = 0;
	temp = cf_list->cf_pt;
	for(int count = 0; count<cf_list->num; count++){
		if(!temp->is_full){
			cfq[pos] = temp;
			pos++;
		}
		temp = temp->next;
	}

	sort(cfq, queue_length);
	for(int i = 0; i<queue_length-1; i++){
		for(int j = queue_length-1; j>i; j--){
			cfq[i]->transfer(cfq[j]);
			if(cfq[i]->is_empty == true){
				cf_list->remove(cfq[i]);
				break;
			}
		}
	}
	if(cfq[queue_length-1]->is_empty == true){
		cf_list->remove(cfq[queue_length-1]);
	}

	return true;
}

void DynamicCuckooFilter::sort(CuckooFilter** cfq, int queue_length){
	CuckooFilter* temp;
	for(int i = 0; i<queue_length-1; i++){
		for(int j = 0; j<queue_length-1-i; j++){
			if(cfq[j]->counter > cfq[j+1]->counter){
				temp = cfq[j];
				cfq[j] = cfq[j+1];
				cfq[j+1] = temp;
			}
		}
	}
}



void DynamicCuckooFilter::generateIF(const char* item, size_t &index, uint32_t &fingerprint, int fingerprint_size, int single_table_length){
	std::string  value = HashFunc::sha1(item);
	uint64_t hv = *((uint64_t*) value.c_str());

	index = ((uint32_t) (hv >> 32)) % single_table_length;
	fingerprint = (uint32_t) (hv & 0xFFFFFFFF);
	fingerprint &= ((0x1ULL<<fingerprint_size)-1);
	fingerprint += (fingerprint == 0);
}

void DynamicCuckooFilter::generateA(size_t index, uint32_t fingerprint, size_t &alt_index, int single_table_length){
	alt_index = (index ^ (fingerprint * 0x5bd1e995)) % single_table_length;
}



int DynamicCuckooFilter::getFingerprintSize(){
	return fingerprint_size;
}

float DynamicCuckooFilter::size_in_mb(){
	return fingerprint_size * 4.0 * single_table_length * cf_list->num / 8 / 1000 / 1000;
}

uint64_t DynamicCuckooFilter::upperpower2(uint64_t x) {
  x--;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  x |= x >> 32;
  x++;
  return x;
}

void info(){

}



#endif //DYNAMICCUCKOOFILTER_H
