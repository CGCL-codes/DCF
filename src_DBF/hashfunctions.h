#include<openssl/evp.h>

class HashFunc{
public:
	HashFunc();
	~HashFunc();
	static char* sha1(const char* key);
	static char* md5(const char* key);
};

char* HashFunc::sha1(const char* key){
	EVP_MD_CTX mdctx;
	unsigned char value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit(&mdctx, EVP_sha1());
	EVP_DigestUpdate(&mdctx, (const void*) key, sizeof(key));
	EVP_DigestFinal_ex(&mdctx, value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	return (char*)value;
}

char* HashFunc::md5(const char* key){
	EVP_MD_CTX mdctx;
	unsigned char value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit(&mdctx, EVP_md5());
	EVP_DigestUpdate(&mdctx, (const void*) key, sizeof(key));
	EVP_DigestFinal_ex(&mdctx, value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);

	return (char*)value;
}
