#ifndef HASHFUNCTIONS_H_
#define HASHFUNCTIONS_H_

#include<openssl/evp.h>

class HashFunc{
public:
	HashFunc();
	~HashFunc();
	static char* sha1(const char* key);
	static char* md5(const char* key);
};

#endif //HASHFUNCTIONS_H_
