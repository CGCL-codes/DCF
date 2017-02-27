/*
 * linklist.h
 *
 *  Created on: Jan 4, 2017
 *      Author: liaoliangyi
 */

#ifndef LINKLIST_H_
#define LINKLIST_H_

class LinkList{
public:
	CuckooFilter* cf_pt;
	CuckooFilter* tail_pt;
	int num;
	LinkList(size_t single_table_length, size_t fingerprint_size, double single_capacity){
		cf_pt = new CuckooFilter(single_table_length, fingerprint_size, single_capacity);
		tail_pt = new CuckooFilter(single_table_length, fingerprint_size, single_capacity);
		num = 0;
	}
	~LinkList(){
		delete cf_pt;
		cf_pt = NULL;
		delete tail_pt;
		tail_pt = NULL;
	}
	bool remove(CuckooFilter* cf_remove){
		CuckooFilter* frontCF = cf_remove->front;
		if(frontCF == NULL){
			this->cf_pt = cf_remove->next;
		}else{
			frontCF->next = cf_remove->next;
		}
		cf_remove = NULL;
		num --;
		return true;
	}
};



#endif /* LINKLIST_H_ */
