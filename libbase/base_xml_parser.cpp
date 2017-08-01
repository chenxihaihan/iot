
#include "base_xml_parser.h"
#include "base_string.h"


NS_BASE_BEGIN

std::string XML_Node::get_text()
{
	return m_text;
}



std::string XML_Node::get_attr_str(std::string key)
{
	std::string value = "";
	
	std::map<std::string, std::string>::iterator itr = m_mapAttr.find(key);
	if(itr != m_mapAttr.end())
	{
		value = itr->second;
	}

	return value;
}
		


std::string XML_Node::to_string()
{
	std::string str = "";
	if(!m_text.empty())
	{
		str = std::string("Text = ") + m_text + std::string("\n");
	}
	
	if(!m_mapAttr.empty())
	{
		str += "Attr = ";
		
		std::map<std::string, std::string>::iterator itr = m_mapAttr.begin();
		for(; itr != m_mapAttr.end(); itr++)
		{
			str = str + itr->first + std::string(":") + itr->second + std::string(" ");
		}

		str += std::string("\n");
	}
	
	return str;
	

}




/*
enum TiXmlEncoding
{
	TIXML_ENCODING_UNKNOWN,
	TIXML_ENCODING_UTF8,
	TIXML_ENCODING_LEGACY
};
*/
int XML_Parser::parse_file(std::string xmlFile)
{
	int nRet = 0;
	
	if(xmlFile.empty())
	{
		printf("xmlFile is empty.\n");
		return -1;
	}
	
	if (!m_document.LoadFile(xmlFile.c_str()))
	{
		printf("xml file:%s, row:%d, col:%d, error:%s\n",
			xmlFile.c_str(), m_document.ErrorRow(), m_document.ErrorCol(), m_document.ErrorDesc());
		return -2;
	}
	
	/*
	����ѹ��xml�ո񣬽�TiXmlText�ж���ո�ѹ����1���ո�
	�������� ����SetCondenseWhiteSpace�Ƿ����ã�tinyxml ������ѹ��TiXmlText�ո�
	*/
	TiXmlDocument::SetCondenseWhiteSpace(true);

	//m_document.Print();

	return nRet;

}




int XML_Parser::parse_str(std::string xmlStr)
{
	int nRet = 0;

	if(xmlStr.empty())
	{
		printf("xmlStr is empty.\n");
		return -1;
	}
	
	if (!m_document.Parse(xmlStr.c_str()))
	{
		printf("row:%d, col:%d, error:%s\n",
			m_document.ErrorRow(), m_document.ErrorCol(), m_document.ErrorDesc());
		return -2;
	}
	
	/*
	����ѹ��xml�ո񣬽�TiXmlText�ж���ո�ѹ����1���ո�
	�������� ����SetCondenseWhiteSpace�Ƿ����ã�tinyxml ������ѹ��TiXmlText�ո�
	*/
	TiXmlDocument::SetCondenseWhiteSpace(true);

	//m_document.Print();

	return nRet;

}




int XML_Parser::get_nodes(std::string key, std::vector<XML_Node> &vecNode)
{
	if(key.empty())
	{
		printf("key is empty.\n");
		return -1;
	}
	
	std::list<std::string> lstKey;
	split(key, "/\\", lstKey);

	TiXmlElement *root = m_document.RootElement();
	vecNode.clear();
	return get_node(root, lstKey, vecNode);
}





int XML_Parser::get_node(std::string key, XML_Node &node)
{
	int nRet = 0;
	if(key.empty())
	{
		printf("key is empty.\n");
		return -1;
	}

	std::list<std::string> lstKey;
	split(key, "/\\", lstKey);

	std::vector<XML_Node> vecNode;
	TiXmlElement *root = m_document.RootElement();
	nRet = get_node(root, lstKey, vecNode);

	if(!vecNode.empty())
	{
		//ȡ����һ��
		node = vecNode[0];
	}
	
	return nRet;	
	
}




int XML_Parser::get_node(TiXmlElement *father_element, std::list<std::string> &lstKey, 
	std::vector<XML_Node> &vecNode)
{
	int nRet = 0;

	if(father_element == NULL)
	{
		printf("father_element == NULL\n");
		return -1;
	}

	if(lstKey.empty())
	{
		printf("no key.\n");
		return -2;
	}

	//���жϴ���father element
	std::string element_name = "";
	element_name = father_element->Value();
	if(element_name != lstKey.front())
	{
		printf("the value of father_element(%s) isn't same to the front(%s) of lstKey\n",
			element_name.c_str(), (lstKey.front()).c_str());
		return -3;	
	}
	else
	{
		//�ҵ����յ�node
		if(lstKey.size() == 1)
		{
			create_node(father_element, vecNode);
		}
		else
		{
			//�м�node����Ҫ����������node

			//���ų���һ���ڵ�
			lstKey.pop_front();
			
			//��ȡ��һ���ӽڵ�
			TiXmlElement *child_element = father_element->FirstChildElement(); 
			while(child_element) 
			{
				//���TiXmlElement �ı�ǩ����
				element_name = child_element->Value();
				if(element_name == lstKey.front())
				{
					//�ҵ����յ�node
					if(lstKey.size() == 1)
					{
						create_node(child_element, vecNode);						
					}
					else
					{
						//�ҵ��м�node���ݹ����
						nRet = get_node(child_element, lstKey, vecNode);
						if(nRet != 0)
						{
							printf("get_node failed. nRet:%d\n", nRet); 
						}
						return nRet;
					}
					
				}
			
				/*
				û��ƥ�䵽node �󣬾�����һ���ֵ�node ����ƥ��
				*/
				child_element = child_element->NextSiblingElement(); 
				
			}
			

		}

	}

	if(vecNode.empty())
	{
		printf("no found key(%s).\n", lstKey.front().c_str());
		nRet = -4;
	}
	
	return nRet;
	
}





void XML_Parser::create_node(TiXmlElement *element, std::vector<XML_Node> &vecNode)
{
	XML_Node node;
	//�е�TiXmlElement û��Text �� ��Ҫ�ж�һ��
	if(element->GetText() != NULL)
	{
		node.m_text = element->GetText();
	}
	
	//��õ�һ������
	TiXmlAttribute *attr = element->FirstAttribute(); 
	while (attr) 
	{
		std::string attr_key = attr->Name();
		std::string attr_value = attr->Value();
		node.m_mapAttr.insert(std::make_pair(attr_key, attr_value));
		
		//�����һ������
		attr = attr->Next();
	}
	vecNode.push_back(node);

}


NS_BASE_END


