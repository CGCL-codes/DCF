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



string  HashFunc::sha1(const char* key){
	EVP_MD_CTX mdctx;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit(&mdctx, EVP_sha1());
	EVP_DigestUpdate(&mdctx, (const void*) key, sizeof(key));
	EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	return std::string((char*)md_value, (size_t)md_len);
}

string HashFunc::md5(const char* key){
	EVP_MD_CTX mdctx;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit(&mdctx, EVP_md5());
	EVP_DigestUpdate(&mdctx, (const void*) key, sizeof(key));
	EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	return std::string((char*)md_value, (size_t)md_len);
}




#endif
