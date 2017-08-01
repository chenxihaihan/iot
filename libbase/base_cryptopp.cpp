
#include "base_cryptopp.h"
#include "base_convert.h"
#include "base_string.h"

//digest
#include "cryptopp/md5.h"
#include "cryptopp/sha.h"
#include "cryptopp/hmac.h"

//base64
#include "cryptopp/base64.h"

//aes-cbc
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"

//des-cbc
#include "cryptopp/des.h"

//rsa
#include "cryptopp/randpool.h"
#include "cryptopp/rsa.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"
#include "cryptopp/osrng.h"


NS_BASE_BEGIN


X_MD5::X_MD5()
{

}
	
X_MD5::~X_MD5()
{

}



void X_MD5::info()
{
	printf("algorithm name:%s\n", CryptoPP::Weak1::MD5::StaticAlgorithmName());
	printf("digest size:128 bit\n");
}


bool X_MD5::calculate_digest(const std::string src, std::string &digest)
{
	CryptoPP::Weak1::MD5 md5;
	printf("block size:%u byte, digest size:%u byte\n", md5.BlockSize(), md5.DigestSize());
	
	byte buf[md5.DigestSize()];
	memset(buf, 0x0, md5.DigestSize());

	try
	{		
		md5.CalculateDigest(buf, (const byte*)src.c_str(), src.size());
	}	
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_MD5::calculate_digest failed, error:%s\n", e.what());
		return false;
	}	
	/*
	�ȼ����������д���֮��
	md5.Update(src, len);
	md5.Final(buf);

	ע��: �˴�len������message���ַ�����,�������ɵĹ�ϣֵ��һ��һ��
	*/

	digest = bin_to_hexstring((const char*)buf, 16);

	return true;
	
}



//------------------------------------------


X_SHA256::X_SHA256()
{

}
	
X_SHA256::~X_SHA256()
{

}


void X_SHA256::info()
{
	printf("algorithm name:%s\n", CryptoPP::SHA256::StaticAlgorithmName());
	printf("digest size:256 bit\n");

}


bool X_SHA256::calculate_digest(const std::string src, std::string &digest)
{
	
	CryptoPP::SHA256 sha256;
	printf("block size:%u byte, digest size:%u byte\n", sha256.BlockSize(), sha256.DigestSize());

	char buf[sha256.DigestSize()];
	memset(buf, 0x0, sha256.DigestSize());
	
	try
	{	
		sha256.CalculateDigest((byte*)buf, (const byte*)src.c_str(), src.size());
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_SHA256::calculate_digest failed, error:%s\n", e.what());
		return false;
	}

	digest = bin_to_hexstring(buf, sha256.DigestSize());
	
	return true;
	
}



//------------------------------------------


X_SHA1::X_SHA1()
{

}
	
X_SHA1::~X_SHA1()
{

}


void X_SHA1::info()
{
	printf("algorithm name:%s\n", CryptoPP::SHA1::StaticAlgorithmName());
	printf("digest size:160 bit\n");

}


bool X_SHA1::calculate_digest(const std::string src, std::string &digest)
{
	CryptoPP::SHA1 sha1;
	printf("block size:%u byte, digest size:%u byte\n", sha1.BlockSize(), sha1.DigestSize());

	char buf[sha1.DigestSize()];
	memset(buf, 0x0, sha1.DigestSize());
	
	try
	{	
		sha1.CalculateDigest((byte*)buf, (const byte*)src.c_str(), src.size());
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_SHA1::calculate_digest failed, error:%s\n", e.what());
		return false;
	}

	digest = bin_to_hexstring(buf, sha1.DigestSize());
	
	return true;
	
}



//------------------------------------------


X_HMACSHA1::X_HMACSHA1()
{

}
	
X_HMACSHA1::~X_HMACSHA1()
{

}


void X_HMACSHA1::info()
{
	printf("algorithm name: HMACSHA1\n");
	printf("digest size:160 bit\n");
}


bool X_HMACSHA1::calculate_digest(const std::string key, const std::string src, std::string &digest)
{

	return true;
}



//------------------------------------------



X_BASE64::X_BASE64()
{

}
	
X_BASE64::~X_BASE64()
{

}


void X_BASE64::info()
{


}


bool X_BASE64::encrypt(const char *pSrc, const unsigned int len, char* &pDst)
{
	CryptoPP::Base64Encoder encoder(NULL, false);
	try
	{	
		encoder.Put((byte*)pSrc, len);
		encoder.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_BASE64::encrypt failed, error:%s\n", e.what());
		return false;
	}
	
    int size = encoder.MaxRetrievable();
    pDst = new char[size+1];
    pDst[size] = '\0';
	
    encoder.Get((byte*)pDst, size);

    return true;
	
}



bool X_BASE64::decrypt(const char *pSrc, char* &pDst)
{
	CryptoPP::Base64Decoder decoder;
	try
	{	
		decoder.Put((byte*)pSrc, strlen(pSrc));
		decoder.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_BASE64::decrypt failed, error:%s\n", e.what());
		return false;
	}
	
    int size = decoder.MaxRetrievable();
    pDst = new char[size+1];
    pDst[size] = '\0';
	
    decoder.Get((byte*)pDst, size);

    return true;

}



//------------------------------------------


X_AES_CBC::X_AES_CBC()
{

}
	
X_AES_CBC::~X_AES_CBC()
{

}


void X_AES_CBC::info()
{
	printf("algorithm name:%s\n", CryptoPP::AES::StaticAlgorithmName());
	printf("algorithm mode:%s\n", CryptoPP::CBC_Mode_ExternalCipher::Encryption::StaticAlgorithmName());
	printf("block size:%u byte\n", CryptoPP::AES::BLOCKSIZE);
	printf("default key len:%u byte, min key len:%u byte, max key len:%u byte\n", 
		CryptoPP::AES::DEFAULT_KEYLENGTH, CryptoPP::AES::MIN_KEYLENGTH, CryptoPP::AES::MAX_KEYLENGTH);
}


bool X_AES_CBC::encrypt(const std::string key, const std::string plaintext, std::string &ciphertext, const std::string iv)
{
	if(key.empty() || plaintext.empty())
	{
		printf("key.empty() || plaintext.empty()\n");
		return false;
	}
	
	//����aes ��Կ
	byte _key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	memset(_key, 0x0, (CryptoPP::AES::DEFAULT_KEYLENGTH));
	unsigned int len = key.size() < (unsigned int)(CryptoPP::AES::DEFAULT_KEYLENGTH) ? key.size() : (CryptoPP::AES::DEFAULT_KEYLENGTH); 
	memcpy(_key, key.c_str(), len);
	
	/*
	���ó�ʼ������iv
	����Ĭ����������ܺͽ��ܱ���ʹ����ͬ�ĳ�ʼ������
	*/
	byte _iv[CryptoPP::AES::BLOCKSIZE];
	memset(_iv, 0x0, (CryptoPP::AES::BLOCKSIZE));
	if(!iv.empty())
	{
		len = iv.size() < (unsigned int)(CryptoPP::AES::BLOCKSIZE) ? iv.size() : (CryptoPP::AES::BLOCKSIZE);  
		memcpy(_iv, iv.c_str(), len);
	}

	//��aes encryption ��cbc mode external cipher ����
	CryptoPP::AES::Encryption aesEncryption;
	aesEncryption.SetKey(_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, _iv);
	
	/*
	CryptoPP �е�����ָ��
	template <class T>
	class member_ptr

	�����StringSink����ʵ��������member_ptr �������ù����ڴ��ͷ�����
	*/
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
	
	try
	{
		//����
		stfEncryptor.Put((byte*)(plaintext.c_str()), plaintext.size());
		stfEncryptor.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_AES_CBC::encrypt failed, error:%s\n", e.what());
		return false;
	}

	return true;
	
}




bool X_AES_CBC::decrypt(const std::string key, const std::string ciphertext, std::string &plaintext, const std::string iv)
{
	if(key.empty() || ciphertext.empty())
	{
		printf("key.empty() || ciphertext.empty()\n");
		return false;
	}

	//����aes ��Կ
	byte _key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	memset(_key, 0x0, (CryptoPP::AES::DEFAULT_KEYLENGTH));
	unsigned int len = key.size() < (unsigned int)(CryptoPP::AES::DEFAULT_KEYLENGTH) ? key.size() : (CryptoPP::AES::DEFAULT_KEYLENGTH); 
	memcpy(_key, key.c_str(), len);
	
	/*
	���ó�ʼ������iv
	����Ĭ����������ܺͽ��ܱ���ʹ����ͬ�ĳ�ʼ������
	*/
	byte _iv[CryptoPP::AES::BLOCKSIZE];
	memset(_iv, 0x0, (CryptoPP::AES::BLOCKSIZE));
	if(!iv.empty())
	{
		len = iv.size() < (unsigned int)(CryptoPP::AES::BLOCKSIZE) ? iv.size() : (CryptoPP::AES::BLOCKSIZE);  
		memcpy(_iv, iv.c_str(), len);
	}

	//��aes encryption ��cbc mode external cipher ����
	CryptoPP::AESDecryption aesDecryption;
	aesDecryption.SetKey(_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, _iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(plaintext));
	
	try
	{	
		//����
		stfDecryptor.Put((byte*)(ciphertext.c_str()), ciphertext.size());
		stfDecryptor.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_AES_CBC::decrypt failed, error:%s\n", e.what());
		return false;
	}
	
	return true;

}



//-----------------------------------------


X_DES_CBC::X_DES_CBC()
{

}

	
X_DES_CBC::~X_DES_CBC()
{

}


void X_DES_CBC::info()
{
	printf("algorithm name:%s\n", CryptoPP::DES::StaticAlgorithmName());
	printf("algorithm mode:%s\n", CryptoPP::CBC_Mode_ExternalCipher::Encryption::StaticAlgorithmName());
	printf("block size:%u byte\n", CryptoPP::DES::BLOCKSIZE);
	printf("key len:%u byte\n", CryptoPP::DES::DEFAULT_KEYLENGTH);
}


bool X_DES_CBC::encrypt(const std::string key, const std::string plaintext, std::string &ciphertext, const std::string iv)
{
	if(key.empty() || plaintext.empty())
	{
		printf("key.empty() || plaintext.empty()\n");
		return false;
	}

	//����des ��Կ
	byte _key[CryptoPP::DES::DEFAULT_KEYLENGTH];
	memset(_key, 0x0, (CryptoPP::DES::DEFAULT_KEYLENGTH));
	unsigned int len = key.size() < (unsigned int)(CryptoPP::DES::DEFAULT_KEYLENGTH) ? key.size() : (CryptoPP::DES::DEFAULT_KEYLENGTH); 
	memcpy(_key, key.c_str(), len);
	
	/*
	���ó�ʼ������iv
	����Ĭ����������ܺͽ��ܱ���ʹ����ͬ�ĳ�ʼ������
	*/
	byte _iv[CryptoPP::DES::BLOCKSIZE];
	memset(_iv, 0x0, (CryptoPP::DES::BLOCKSIZE));
	if(!iv.empty())
	{
		len = iv.size() < (unsigned int)(CryptoPP::DES::BLOCKSIZE) ? iv.size() : (CryptoPP::DES::BLOCKSIZE);  
		memcpy(_iv, iv.c_str(), len);
	}

	//��des encryption ��cbc mode external cipher ����
	CryptoPP::DES::Encryption desEncryption;
	desEncryption.SetKey(_key, CryptoPP::DES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(desEncryption, _iv);
	
	/*
	CryptoPP �е�����ָ��
	template <class T>
	class member_ptr

	�����StringSink����ʵ��������member_ptr �������ù����ڴ��ͷ�����
	*/
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
	
	try
	{
		//����
		stfEncryptor.Put((byte*)(plaintext.c_str()), plaintext.size());
		stfEncryptor.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_DES_CBC::encrypt failed, error:%s\n", e.what());
		return false;
	}

	return true;
	
}




bool X_DES_CBC::decrypt(const std::string key, const std::string ciphertext, std::string &plaintext, const std::string iv)
{
	if(key.empty() || ciphertext.empty())
	{
		printf("key.empty() || ciphertext.empty()\n");
		return false;
	}

	//����des ��Կ
	byte _key[CryptoPP::DES::DEFAULT_KEYLENGTH];
	memset(_key, 0x0, (CryptoPP::DES::DEFAULT_KEYLENGTH));
	unsigned int len = key.size() < (unsigned int)(CryptoPP::DES::DEFAULT_KEYLENGTH) ? key.size() : (CryptoPP::DES::DEFAULT_KEYLENGTH); 
	memcpy(_key, key.c_str(), len);
	
	/*
	���ó�ʼ������iv
	����Ĭ����������ܺͽ��ܱ���ʹ����ͬ�ĳ�ʼ������
	*/
	byte _iv[CryptoPP::DES::BLOCKSIZE];
	memset(_iv, 0x0, (CryptoPP::DES::BLOCKSIZE));
	if(!iv.empty())
	{
		len = iv.size() < (unsigned int)(CryptoPP::DES::BLOCKSIZE) ? iv.size() : (CryptoPP::DES::BLOCKSIZE);  
		memcpy(_iv, iv.c_str(), len);
	}

	//��aes encryption ��cbc mode external cipher ����
	CryptoPP::DESDecryption desDecryption;
	desDecryption.SetKey(_key, CryptoPP::DES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(desDecryption, _iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(plaintext));
	
	try
	{	
		//����
		stfDecryptor.Put((byte*)(ciphertext.c_str()), ciphertext.size());
		stfDecryptor.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_DES_CBC::decrypt failed, error:%s\n", e.what());
		return false;
	}
	
	return true;

}




//------------------------------------------



X_RSA_OAEP::X_RSA_OAEP()
{
}



X_RSA_OAEP::~X_RSA_OAEP()
{

}


void X_RSA_OAEP::info()
{
	printf("algorithm name:%s\n", CryptoPP::RSA::StaticAlgorithmName());
	printf("algorithm mode:%s\n", CryptoPP::RSAES_OAEP_SHA_Decryptor::StaticAlgorithmName().c_str());
}



bool X_RSA_OAEP::generate_rsa_key(const std::string &key_path, const std::string &key_name)
{
	/*
	���㹫����Կ��˽����Կ�� �������ɵ��ı��б���
	������Կ��˽����Կ ����16�����ַ���
	ע�⣺HexEncoder�Ĺ��캯���д��� �� new ��FileSink �Ѷ���
	�ö�����HexEncoder�����ͷš�����ʹ�ð취�� ��HexEncoder�����а�������
	����ָ�����õĳ�Ա����member_ptr<FileSink>����������FileSink �Ѷ���
	*/
	
	/*
	����˽����Կ
	ʹ����������������ѡ����������pq�� ���ڼ���˽����ԿSK={d,n}
	*/
	unsigned int key_len = 1024; //key len=1024,  inblock len=86,  outblock len=256
	CryptoPP::AutoSeededRandomPool random_pool;
	CryptoPP::RSAES_OAEP_SHA_Decryptor oaep_sha_decryptor(random_pool, key_len);
	try
	{
		//����˽����Կ�ļ�
		std::string private_key_file = format("%s/%s_private.key", key_path.c_str(), key_name.c_str());
		CryptoPP::HexEncoder private_hex_encoder(new CryptoPP::FileSink(private_key_file.c_str()));	
		oaep_sha_decryptor.DEREncode(private_hex_encoder);
		private_hex_encoder.MessageEnd();

	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_RSA::RSAES_OAEP_SHA_Decryptor::DEREncode failed, error:%s\n", e.what());
		return false;
	}
	

	/*
	���ɹ�����Կ
	������Կ���ɺ�˽����Կ��һ�Եģ� ��Ҫʹ����������˽����Կǰѡ���eֵ��
	�������ɹ�����ԿPK={e,n}, ������Ҫ��priv����pub
	*/
	CryptoPP::RSAES_OAEP_SHA_Encryptor oaep_sha_encryptor(oaep_sha_decryptor);	
	try
	{
		//���ɹ�����Կ�ļ�
		std::string public_key_file = format("%s/%s_public.key", key_path.c_str(), key_name.c_str());
		CryptoPP::HexEncoder public_hex_encoder(new CryptoPP::FileSink(public_key_file.c_str()));
		oaep_sha_encryptor.DEREncode(public_hex_encoder);
		public_hex_encoder.MessageEnd();
	}
	catch(CryptoPP::Exception &e)	
	{	
		printf("X_RSA::RSAES_OAEP_SHA_Encryptor::DEREncode failed, error:%s\n", e.what());
		return false;
	}	
	
	return true;

}




bool X_RSA_OAEP::encrypt(const std::string public_key_file, const std::string plaintext, std::string &ciphertext)
{
	ciphertext = "";

	std::string tmp = plaintext;
	unsigned int size = tmp.size();
	while(size > 86)
	{
		std::string inblock = "";
		std::string outblock = "";
		inblock = tmp.substr(0, 86);
		tmp.erase(0, 86);
		
		try
		{	
			CryptoPP::FileSource file_source(public_key_file.c_str(), true, new CryptoPP::HexDecoder);
			CryptoPP::RSAES_OAEP_SHA_Encryptor oaep_sha_encryptor(file_source);
			
			CryptoPP::AutoSeededRandomPool random_pool;
			CryptoPP::StringSource(inblock, true, 
				new CryptoPP::PK_EncryptorFilter(random_pool, oaep_sha_encryptor, new CryptoPP::HexEncoder(new CryptoPP::StringSink(outblock))));

			ciphertext += outblock;
		}
		catch(CryptoPP::Exception &e)	
		{	
			printf("X_RSA::encrypt failed, error:%s\n", e.what());
			ciphertext = "";
			return false;
		}

		size = tmp.size();
		
	}	

	if(size > 0)
	{
		std::string inblock = "";
		std::string outblock = "";
		try
		{	
			CryptoPP::FileSource file_source(public_key_file.c_str(), true, new CryptoPP::HexDecoder);
			CryptoPP::RSAES_OAEP_SHA_Encryptor oaep_sha_encryptor(file_source);
			
			CryptoPP::AutoSeededRandomPool random_pool;
			CryptoPP::StringSource(tmp, true, 
				new CryptoPP::PK_EncryptorFilter(random_pool, oaep_sha_encryptor, new CryptoPP::HexEncoder(new CryptoPP::StringSink(outblock))));

			ciphertext += outblock;
		}
		catch(CryptoPP::Exception &e)	
		{	
			printf("X_RSA::encrypt failed, error:%s\n", e.what());
			ciphertext = "";
			return false;
		}
		
	}
		
	return true;
	
}




bool X_RSA_OAEP::decrypt(const std::string private_key_file, const std::string ciphertext, std::string &plaintext)
{
	plaintext = "";

	std::string tmp = ciphertext;
	unsigned int size = tmp.size();
	while(size >= 256)
	{
		std::string inblock = "";
		std::string outblock = "";
		inblock = tmp.substr(0, 256);
		tmp.erase(0, 256);
		
		try
		{	
			CryptoPP::FileSource file_source(private_key_file.c_str(), true, new CryptoPP::HexDecoder);
			CryptoPP::RSAES_OAEP_SHA_Decryptor oaep_sha_decryptor(file_source);
			
			CryptoPP::AutoSeededRandomPool random_pool;
			CryptoPP::StringSource(inblock.c_str(), true, 
				new CryptoPP::HexDecoder(new CryptoPP::PK_DecryptorFilter(random_pool, oaep_sha_decryptor, new CryptoPP::StringSink(outblock))));

			plaintext += outblock;
		}
		catch(CryptoPP::Exception &e)	
		{	
			printf("X_RSA::decrypt failed, error:%s\n", e.what());
			plaintext = "";
			return false;
		}


		size = tmp.size();
		
	}

	return true;
	
}



NS_BASE_END

