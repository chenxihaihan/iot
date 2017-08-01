
#include "base_convert.h"
#include "base_os.h"


NS_BASE_BEGIN


int hexstring_to_bin(const char* str, char* hex)
{   
	int nRet = 0;

	if(!str || !hex)
	{
		return -1;
	}

	int len = strlen(str);
	if(len % 2)
	{
		return -2;
	}

	int size = len / 2;
	for (int i=0; i < size; i++)    
    {
         char one_byte[3] = {0};
         memcpy(one_byte, &str[i*2], 2);
         one_byte[2] = '\0';
		 /*
		 #include <stdlib.h>
		 long int strtol(const char *nptr, char **endptr, int base);
		 long long int strtoll(const char *nptr, char **endptr, int base);

		������nptr�ַ������ݲ���base��ת���ɳ���������
		����base��Χ��2��36
		strtol()��ɨ�����nptr�ַ���������ǰ��Ŀո��ַ���
		ֱ���������ֻ��������Ųſ�ʼ��ת���������������ֻ��ַ���
		����ʱ('\0')����ת��������������ء�
		������endptr��ΪNULL����Ὣ�����ĵ�һ�����������ַ���ַ��ֵ��endptr
		 */
         hex[i] = strtol(one_byte, NULL, 16);
    }
	
	return nRet;
	
} 




std::string char_to_hexstring(char c)
{
	std::string value = ""; 			   
	char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	value += hex[(c>>4)&0x0F];
	value += hex[c&0x0F];
	return value;
}



//��2����װ��16�����ַ���
std::string bin_to_hexstring(const char *src, unsigned int len)
{
	std::string str = "";
	
	for(unsigned int i=0; i<len; ++i) 
	{
		str += char_to_hexstring(*(src++));
	}

	return str;
}


unsigned long long swap64(unsigned long long ll)
{
	unsigned long long llTmp = \
	  (((ll) >> 56)| \
		(((ll) & 0x00ff000000000000LL) >> 40)| \
		(((ll) & 0x0000ff0000000000LL) >> 24)| \
		(((ll) & 0x000000ff00000000LL) >> 8) | \
		(((ll) & 0x00000000ff000000LL) << 8) | \
		(((ll) & 0x0000000000ff0000LL) << 24)| \
		(((ll) & 0x000000000000ff00LL) << 40)| \
		(((ll) << 56)));

	return llTmp;
}


unsigned long long htonll(unsigned long long ll)
{
	return  isBigEndian() ? ll : swap64(ll);
}


unsigned long long ntohll(unsigned long long ll)
{
	return  isBigEndian() ? ll : swap64(ll);
}


NS_BASE_END
	

