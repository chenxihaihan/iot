
#include "base_zlib.h"
#include "zlib.h"


NS_BASE_BEGIN

/*
ѹ����(Compression ratio) �ļ�ѹ����Ĵ�С��ѹ��ǰ�Ĵ�С֮�ȣ�
�������100m���ļ�ѹ������90m��ѹ���ʾ���90/100*100%=90%��
ѹ����һ����ԽСԽ�ã�����ѹ��ԽС��ʱ��Խ����

#define Z_OK            0
#define Z_STREAM_END    1

typedef struct z_stream_s {
    Bytef    *next_in;  --- next input byte
    uInt     avail_in;  --- number of bytes available at next_in
    uLong    total_in;  --- total nb of input bytes read so far

    Bytef    *next_out; --- next output byte should be put there   ----- ���buffer
    uInt     avail_out; --- remaining free space at next_out           ------  next_out �ж��ٸ��ֽ��Ƿ���������ֽ�
    uLong    total_out; --- total nb of bytes output so far             ------ ������ֽ���

    char     *msg;      --- last error message, NULL if no error
    struct internal_state FAR *state; --- not visible by applications

    alloc_func zalloc;  --- used to allocate the internal state
    free_func  zfree;   --- used to free the internal state
    voidpf     opaque;  --- private data object passed to zalloc and zfree

    int     data_type;  --- best guess about the data type: binary or text
    uLong   adler;      --- adler32 value of the uncompressed data
    uLong   reserved;   --- reserved for future use
} z_stream;


[����1]: ˵����ѹ�������buffer ��С��С������� �����ж�Ϊ5�ֽ�
# ./test.bin 
zlib version: 1.2.3
---------- Src(len:49) ----------
78 63 70 73 66 64 64 64 64 64 64 64 64 64 64 64 64 64 64 64 64 66 73 64 74 77 72 71 62 62 
62 62 72 77 62 67 66 67 67 67 67 68 68 68 68 68 78 63 70 
ret of deflate:1
---------- Dst(len:38, avail_out:11, total_out:38) ----------
78 ffffff9c ffffffab 48 2e 28 4e 4b 41 03 69 ffffffc5 29 25 ffffffe5 45 ffffff85 49 40 50 54 ffffff9e 
ffffff94 ffffff9e ffffff96 0e 04 19 20 50 ffffff91 5c 00 00 fffffff2 11 13 ffffffff 


---------- Src(len:38) ----------
78 ffffff9c ffffffab 48 2e 28 4e 4b 41 03 69 ffffffc5 29 25 ffffffe5 45 ffffff85 49 40 50 54 ffffff9e 
ffffff94 ffffff9e ffffff96 0e 04 19 20 50 ffffff91 5c 00 00 fffffff2 11 13 ffffffff 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:5) ----------
78 63 70 73 66 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:10) ----------
64 64 64 64 64 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:15) ----------
64 64 64 64 64 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:20) ----------
64 64 64 64 64 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:25) ----------
64 66 73 64 74 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:30) ----------
77 72 71 62 62 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:35) ----------
62 62 72 77 62 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:40) ----------
67 66 67 67 67 
ret of inflate:0
---------- Dst(len:5, avail_out:0, total_out:45) ----------
67 68 68 68 68 
ret of inflate:1
---------- Dst(len:4, avail_out:1, total_out:49) ----------
68 78 63 70



[����2]: 50��1ѹ����ֻ��12���ֽ�(50/12 = 4.17)�����Բ����ڽ�ѹ������
�򵥽����buffer �ߴ綨��Ϊ3������buffer�ߴ�
# ./test.bin 
zlib version: 1.2.3
---------- Src(len:50) ----------
31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 
31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 
ret of deflate:1
---------- Dst(len:12, avail_out:38, total_out:12) ----------

*/


int Zlib::zip_mem(char *pSrc, const int src_len, 
					      char* &pDst, int &dst_len, const int level)
{
	int nRet = 0;

	if ((NULL == pSrc) || (src_len < 1))
	{
		printf("(NULL == pSrc) || (src_len < 1)\n");
		return -1;
	}

	pDst = NULL;
	dst_len = 0;

	printf("---------- Src(len:%d) ----------\n", src_len);
	for (int i=0; i<src_len; i++)
	{
		printf("%02x ", pSrc[i]);
	}
	printf("\n");
	

	/*
	Ϊѹ����ʼ���ڲ���״̬��zalloc��zfree��opaque�ֶα���
	�ڵ���֮ǰ��ʼ�������zalloc��zfree����ʼ��ΪZ_NULL��
	deflateInit��������Ƕ�ʹ��Ĭ�ϵķ��亯����
	*/
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
 
	/*
	ѹ������(-1 ~ 9)
	��ѹ������ѹ���ٶȿ쵫��ѹ���ʸ�
	��ѹ������ѹ���ٶ�������ѹ���ʵ�
	zlib ��Z_DEFAULT_COMPRESSION=-1�� �ṩ�õ�ѹ���ٶȺ�ѹ���ʣ� ��ͬ��6
	ѹ���������0�� ��ѹ��
	*/
	nRet = deflateInit_(&strm, level, ZLIB_VERSION, sizeof(z_stream));
	if (nRet != Z_OK)
	{
		printf("deflateInit_ failed, error code:%d\n", nRet);
		return nRet;
	}

	//����buf �ĳ��Ⱥ��׵�ַ
	strm.avail_in = src_len;
	strm.next_in = (Bytef*)pSrc;

	pDst = (char*)malloc(src_len+1);
	//pDst = new char[src_len+1];
	if(NULL == pDst)
	{
		printf("new failed, byte size:%d, errno:%d, errmsg:%s\n", src_len, errno, strerror(errno));
		return -1;
	}
	
	memset(pDst, 0x0, (src_len+1));
	strm.avail_out = src_len+1;
	strm.next_out = (Bytef*)pDst;

	/*
	  deflate ���������ܵ�ѹ�����ݣ�
	  �����뻺��Ϊ�ջ��������������ʱ��, deflateִ�н������ء�
	*/
	nRet = deflate(&strm, Z_FINISH);
	if ((Z_STREAM_END == nRet) || (Z_OK == nRet))
	{
		printf("ret of deflate:%d\n", nRet);
		
		nRet = Z_OK;
		
		dst_len = src_len + 1 - strm.avail_out;

		printf("---------- Dst(len:%d, avail_out:%u, total_out:%lu) ----------\n", 
			dst_len, strm.avail_out, strm.total_out);
		
		for (int i=0; i<dst_len; i++)
		{
			printf("%02x ", pDst[i]);
		}
		printf("\n");
	}
	else
	{
		printf("deflate failed, error code:%d", nRet);
		if(strm.msg)
		{
			printf(", error msg:%s", strm.msg);
		}
		printf("\n");

		DELETE_POINTER_ARR(pDst);
	}

	/* clean up and return */
	deflateEnd(&strm);
	
	return nRet;
	
}





int Zlib::unzip_mem(char *pSrc, const int src_len, 
						     char* &pDst, int &dst_len)
{
	int nRet = 0;

	if ((NULL == pSrc) || (src_len < 1))
	{
		printf("(NULL == pSrc) || (src_len < 1)\n");
		return -1;
	}

	pDst = NULL;
	dst_len = 0;

	printf("---------- Src(len:%d) ----------\n", src_len);
	for (int i=0; i<src_len; i++)
	{
		printf("%02x ", pSrc[i]);
	}
	printf("\n");
	

	/* allocate inflate state */
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	nRet = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
	if (nRet != Z_OK)
	{
		printf("deflateInit_ failed, error code:%d\n", nRet);
		return nRet;
	}

	strm.avail_in = src_len;
	strm.next_in = (Bytef*)pSrc;

	char *pDstNew = NULL;
	int multiple = 1;
	do 
	{
		/*
		ȱʡ���buffer�ĳ���Ϊ���볤�ȵ�3�������ǿ�������ռ�
		�ڽ�ѹ���̻��ǲ��������ʱ���Ҫ������Ӧ�Ĵ���
		*/
		char *pTmp = (char*)malloc(3*src_len);
		//char *pTmp = new char[3*src_len];
		if(NULL == pTmp)
		{
			printf("new pTmp failed, byte size:3*%d, errno:%d, errmsg:%s\n", 
				src_len, errno, strerror(errno));
			return -1;
		}		
		memset(pTmp, 0x0, (3*src_len)); 
		
		strm.avail_out = 3*src_len;
		strm.next_out = (Bytef*)pTmp;

		nRet = inflate(&strm, Z_NO_FLUSH);
		if ((Z_STREAM_END == nRet) || (Z_OK == nRet))
		{
			printf("ret of inflate:%d\n", nRet);

			if(Z_STREAM_END == nRet)
			{
				nRet = Z_OK;
			}

			dst_len = 3*src_len - strm.avail_out;

			printf("---------- Dst(len:%d, avail_out:%u, total_out:%lu) ----------\n", 
				dst_len, strm.avail_out, strm.total_out);
			
			for (int i=0; i<dst_len; i++)
			{
				printf("%02x ", pTmp[i]);
			}
			printf("\n");

			/*
			void *realloc(void *ptr, size_t size);
			
			ʹ��realloc ע���
			1. reallocʧ�ܵ�ʱ�򣬷���NULL
			2. reallocʧ�ܵ�ʱ��ԭ�����ڴ治�ı䣬Ҳ���ǲ�free��move��(����ط������׳���)
			3. ����ԭ�����ڴ���滹���㹻��ʣ���ڴ�Ļ���
			        realloc���ڴ�=ԭ�����ڴ�+ʣ���ڴ�,realloc���Ƿ���ԭ���ڴ�ĵ�ַ; 
			   ����ԭ�����ڴ����û���㹻��ʣ���ڴ�Ļ���
			       realloc�������µ��ڴ棬Ȼ���ԭ�����ڴ����ݿ��������ڴ��
			   ԭ�����ڴ潫��free��,realloc�������ڴ�ĵ�ַ
			4. ���sizeΪ0��Ч����ͬ��free()������NULL
			5. ���ݸ�realloc��ָ���������ǰͨ��malloc(), calloc(), ��realloc()�����
			6. ���ݸ�realloc��ָ�����Ϊ�գ���ͬ��malloc
			7. ���size С�ڵ���ԭ���Ĵ�С������ԭ���ĵ�ַ
			*/

			//������һ����ʱ����pNewTmp�� ��ֹreallocʧ�ܺ��pDstNew ��ֵΪNULL
			char *pNewTmp = (char*)realloc(pDstNew, (multiple*3*src_len));
			if(NULL == pNewTmp)
			{	
				printf("realloc pDstNew failed, byte size:%d*3*%d, errno:%d, errmsg:%s\n", 
					multiple, src_len, errno, strerror(errno));

				DELETE_POINTER_ARR(pTmp);
				DELETE_POINTER(pDstNew);
				return -1;
			}
			
			pDstNew = pNewTmp;
			memcpy((pDstNew+((multiple-1)*3*src_len)), pTmp, (3*src_len));
			
			multiple++;

			DELETE_POINTER_ARR(pTmp);
		
		}
		else
		{
			printf("inflate failed, error code:%d", nRet);
			if(strm.msg)
			{
				printf(", error msg:%s", strm.msg);
			}
			printf("\n");
		
			break;
		}

	} while (strm.avail_out == 0);
	

	dst_len = strm.total_out;
	pDst = pDstNew;
		
	/* clean up and return */
	inflateEnd(&strm);

	return nRet;
	
}




int Zlib::gzip_mem(char *pSrc, const int src_len, 
					      char* &pDst, int &dst_len, const int windowBits, const int memLevel, const int level)
{
	int nRet = 0;

	if ((NULL == pSrc) || (src_len < 1))
	{
		printf("(NULL == pSrc) || (src_len < 1)\n");
		return -1;
	}

	pDst = NULL;
	dst_len = 0;

	printf("---------- Src(len:%d) ----------\n", src_len);
	for (int i=0; i<src_len; i++)
	{
		printf("%02x ", pSrc[i]);
	}
	printf("\n");
	

	/*
	Ϊѹ����ʼ���ڲ���״̬��zalloc��zfree��opaque�ֶα���
	�ڵ���֮ǰ��ʼ�������zalloc��zfree����ʼ��ΪZ_NULL��
	deflateInit��������Ƕ�ʹ��Ĭ�ϵķ��亯����
	*/
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
 
	/*
	ѹ������(-1 ~ 9)
	��ѹ������ѹ���ٶȿ쵫��ѹ���ʸ�
	��ѹ������ѹ���ٶ�������ѹ���ʵ�
	zlib ��Z_DEFAULT_COMPRESSION=-1�� �ṩ�õ�ѹ���ٶȺ�ѹ���ʣ� ��ͬ��6
	ѹ���������0�� ��ѹ��
	*/
	nRet = deflateInit2_(&strm, level, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
	if (nRet != Z_OK)
	{
		printf("deflateInit2_ failed, error code:%d\n", nRet);
		return nRet;
	}

	//����buf �ĳ��Ⱥ��׵�ַ
	strm.avail_in = src_len;
	strm.next_in = (Bytef*)pSrc;

	pDst = (char*)malloc(src_len+1);
	//pDst = new char[src_len+1];
	if(NULL == pDst)
	{
		printf("new failed, byte size:%d, errno:%d, errmsg:%s\n", src_len, errno, strerror(errno));
		return -1;
	}
	
	memset(pDst, 0x0, (src_len+1));
	strm.avail_out = src_len+1;
	strm.next_out = (Bytef*)pDst;

	/*
	  deflate ���������ܵ�ѹ�����ݣ�
	  �����뻺��Ϊ�ջ��������������ʱ��, deflateִ�н������ء�
	*/
	nRet = deflate(&strm, Z_FINISH);
	if ((Z_STREAM_END == nRet) || (Z_OK == nRet))
	{
		printf("ret of deflate:%d\n", nRet);
		
		nRet = Z_OK;
		
		dst_len = src_len + 1 - strm.avail_out;

		printf("---------- Dst(len:%d, avail_out:%u, total_out:%lu) ----------\n", 
			dst_len, strm.avail_out, strm.total_out);
		
		for (int i=0; i<dst_len; i++)
		{
			printf("%02x ", pDst[i]);
		}
		printf("\n");
	}
	else
	{
		printf("deflate failed, error code:%d", nRet);
		if(strm.msg)
		{
			printf(", error msg:%s", strm.msg);
		}
		printf("\n");

		DELETE_POINTER_ARR(pDst);
	}

	/* clean up and return */
	deflateEnd(&strm);
	
	return nRet;
	
}




std::string Zlib::version()
{
	std::string version = zlibVersion();
	return version;
}


NS_BASE_END


