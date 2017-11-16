#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H

#include<string>
#include<openssl/evp.h>

using namespace std;

class HashFunc{
public:
	HashFunc();
	~HashFunc();
	static std::string sha1(const char* key);
	static std::string md5(const char* key);
};

#endif //HASHFUNCTION_H
