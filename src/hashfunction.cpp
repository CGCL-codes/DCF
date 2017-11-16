/*
 * hashfunction.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: liaoliangyi
 */



#include "hashfunction.h"

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
