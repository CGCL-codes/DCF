/*
 * dynamicbloomfilter.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: liaoliangyi
 */


#include "dynamicbloomfilter.h"


using namespace std;

int LinkList::num = 0;

DynamicBloomFilter::DynamicBloomFilter(const size_t item_num, const double fp, const size_t exp_block_num){
	capacity = item_num;

	single_capacity = capacity/exp_block_num; //s=6 7680 s=12 3840 s=24 1920 s=48 960 s=96 480

	false_positive = fp;
	single_false_positive = 1-pow((1-this->false_positive), (double)single_capacity/this->capacity);

	counter = 0;
	bits_num = (int) ceil(single_capacity * (1/log(2.0)) * log(1/single_false_positive)/log(2.0)) * 4;
//		this->bits_num=(int)ceil(single_capacity*log(single_false_positive)/log(0.61285))*4;
	hash_num = (int) ceil((bits_num/4)/single_capacity * log(2.0));

	curSBF = new CountingBloomFilter(single_capacity, single_false_positive);
	nextSBF = NULL;
	sbf_list = new LinkList(single_capacity, single_false_positive);
	sbf_list->cf_pt = curSBF;
	sbf_list->tail_pt = curSBF;
	sbf_list->num = 1;
}

DynamicBloomFilter::DynamicBloomFilter(int n,  int m){
	false_positive = 0; //not used
	capacity = n;
	single_capacity = 1024;
	single_false_positive = 0;//not used
	counter = 0;
	bits_num = m;
	hash_num = 7;

	curSBF = new CountingBloomFilter(1024, m);
	nextSBF = NULL;
	sbf_list = new LinkList(1024, m);
	sbf_list->cf_pt = curSBF;
	sbf_list->tail_pt = curSBF;
	sbf_list->num = 1;
}

DynamicBloomFilter::~DynamicBloomFilter(){
	delete curSBF;
	delete nextSBF;
	delete sbf_list;
}

bool DynamicBloomFilter::insertItem(const char* item){
	unsigned long int* hash_val = generateHashVal(item);

	if(curSBF->item_num >= curSBF->capacity){
		curSBF = getNextSBF(curSBF);
	}
	//duplicate filtering
//	if(!this->queryItem(item)){
//		if(curSBF->insertItem(hash_val)){
//			this->counter += 1;
//		}
//	}

	if(curSBF->insertItem(hash_val)){
		this->counter += 1;
	}

	return true;
}

CountingBloomFilter* DynamicBloomFilter::getNextSBF(CountingBloomFilter* curSBF){
	if(curSBF == this->sbf_list->tail_pt){
		nextSBF = new CountingBloomFilter(single_capacity, single_false_positive);
		curSBF->next = nextSBF;
		nextSBF->front = curSBF;
		sbf_list->tail_pt = nextSBF;
		sbf_list->num++;
	}else{
		nextSBF = curSBF->next;
		if(nextSBF->item_num >= nextSBF->capacity){
			nextSBF = getNextSBF(nextSBF);
		}
	}
	return nextSBF;
}

bool DynamicBloomFilter::queryItem(const char* item){
	unsigned long int* hash_val = generateHashVal(item);

	CountingBloomFilter* query_pt = sbf_list->cf_pt;
	for(int i = 0; i<sbf_list->num; i++){
		if(query_pt->queryItem(hash_val) == true){
			return true;
		}
		query_pt = query_pt->next;
	}
	return false;
}

bool DynamicBloomFilter::deleteItem(const char* item){
	unsigned long int* hash_val = generateHashVal(item);
	CountingBloomFilter* query_pt = sbf_list->cf_pt;
	CountingBloomFilter* delete_pt = sbf_list->cf_pt;
	int counter = 0;

	for(int i = 0; i<sbf_list->num; i++){
		if(query_pt->queryItem(hash_val) == true){
			counter += 1;
		}
		query_pt = query_pt->next;
	}

	if(counter == 1){
		for(int i = 0; i<sbf_list->num; i++){
			if(delete_pt->queryItem(hash_val) == true){
				delete_pt->deleteItem(hash_val);
				this->counter -= 1;
				return true;
			}
			delete_pt = delete_pt->next;
		}
	}
	return false;
}

unsigned long int* DynamicBloomFilter::generateHashVal(const char* item){
	unsigned long int* hash_value = new unsigned long int[hash_num];
	unsigned long long int* value0 = (unsigned long long int*)HashFunc::sha1(item);
	unsigned long long int hv0 = *value0;
	unsigned long long int* value1 = value0;
	unsigned long long int hv1 = *value1;

	hash_value[0] = (unsigned long int)(hv0>>32) % (bits_num/4);
	hash_value[1] = (unsigned long int)(hv1 & 0xFFFFFFFF) % (bits_num/4);

	for(int i = 2; i<hash_num; i++) {
		hash_value[i] = (hash_value[0] + i*hash_value[1] + i*i) % (bits_num/4);
	}

	return hash_value;
}

float DynamicBloomFilter::size_in_mb(){
	return bits_num * sbf_list->num/ 8.0 / 1024 / 1024;
}

