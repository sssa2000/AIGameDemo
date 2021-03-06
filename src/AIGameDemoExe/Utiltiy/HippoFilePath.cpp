#include "HippoFilePath.h"

#include <algorithm>
#include <assert.h>
#include <io.h>
#include <windows.h>
HippoFilePath::HippoFilePath()
{
	m_str = "";
}

HippoFilePath::HippoFilePath(const char* fn)
{
	if (!fn)
		return;
	m_str=fn;
	if ("" != fn)
	{
		OnInit();
	}	
}


HippoFilePath::HippoFilePath(const std::string& fn):
m_str(fn)
{
	if (fn!="")
	{
		OnInit();
	}	
}
void HippoFilePath::OnInit()
{
	if (m_str.empty())
		return;
	ProcessPath(m_str);
	RepairPath(m_str,IsFile());//对目录的处理
	CalculateAbsolutePath();
}
void HippoFilePath::SetFilePath(const std::string& fn)
{
	m_str=fn;
	OnInit();
}

HippoFilePath::~HippoFilePath()
{

}
HippoFilePath::HippoFilePath(const HippoFilePath& other)
{
	this->m_str=other.m_str;
}
HippoFilePath& HippoFilePath::operator=(const HippoFilePath& other) 
{
	this->m_str=other.m_str;
	return *this;
}
bool HippoFilePath::operator!=(const HippoFilePath& other) const
{
	return !this->operator==(other);
}

const char* HippoFilePath::c_str()
{
	return m_str.c_str();
}


void HippoFilePath::ProcessPath(std::string& dirname)
{
	size_t size=dirname.size();
	for (size_t i=0;i<size;i++)
	{
		char& ch=dirname.at(i);
		if (ch == '/')
		{
			ch='\\';
		}
	}
	//a转小写
	std::transform(dirname.begin(),dirname.end(),dirname.begin(),tolower);
}

int HippoFilePath::RepairPath(std::string& _name,bool isIncludeFileName)
{
	if (isIncludeFileName)
		return 0;
	char& last=_name.at(_name.length()-1);
	if (last != '\\')
	{
		_name.append("\\");
		return 1;
	}
	return 2;
}

bool HippoFilePath::IsDirectory()
{
	//三种情况：文件，目录，既不是文件也不是目录(空或者错误格式)
	//目前对于错误格式不支持。如输入参数为:    ......\\ab\\..\\aaa
	if (!m_str.empty())
	{
		size_t res2=m_str.rfind("\\");
		if (res2 ==-1)
			return false;
		size_t res1=m_str.substr(res2,m_str.length()-1).rfind(".");
		if (res1 ==-1)
			return true;
	}
	return false;
}
bool HippoFilePath::IsFile()
{
	if (m_str.empty())
		return false;
	return !IsDirectory();
}
int HippoFilePath::GetFileNameWithExtension(std::string& fn)
{
	size_t res=m_str.rfind("\\");
	if (res != std::string::npos)
	{
		fn= m_str.substr(res+1,m_str.length()-res+1);
		return 1;
	}
	return -1;
}
int HippoFilePath::GetExtensionName(std::string& extension)
{
	std::string suffix = "";

	size_t res = m_str.rfind(".");
	if (res != std::string::npos)
	{
		suffix= m_str.substr(res +1,m_str.length()-res+1);
		size_t spacePos = suffix.find_last_not_of(' ');
		extension = suffix.substr(0, spacePos + 1);
		return 1;
	}
	return -1;
}

int HippoFilePath::GetFileNameNoExtension(std::string& fn)
{
	size_t res1=m_str.rfind("\\");
	size_t res2=m_str.rfind(".");
	if ((res1 > res2) || (res2==std::string::npos))
		return -1;
	fn= m_str.substr(res1+1,res2-res1-1);
	return 1;
}

int HippoFilePath::ChangeExtensionName(const char* extension)
{
	size_t res=m_str.rfind(".") ;
	if (!IsFile() || !extension || res== std::string::npos)
		return -1;
	std::string tmp(extension);
	m_str=m_str.substr(0,res+1)+tmp;
	return 1;
}

std::string HippoFilePath::CalculateAbsolutePath()
{
	char buf[256];
	GetModuleFileNameA(NULL,buf,256);
	std::string tmp(buf); //-V808
	ProcessPath(tmp);
	tmp=GetParentDir(tmp);

	//还有一种多级目录的..\\..\\..\\需要特殊处理
	if ( (m_str.find("..")!=std::string::npos)||(m_str.find(".\\")!=std::string::npos)||(m_str.find("./")!=std::string::npos))
	{
		if (m_str.substr(0,2)!="./"&&m_str.substr(0,2)!=".\\")// "..\\"
			tmp=GetParentDir(tmp);
		RepairPath(tmp,false);
		size_t res=m_str.find("\\");
		tmp += m_str.substr(res+1);
		m_str=tmp;
	}
	else if (m_str.find(":")!=std::string::npos)
	{
	}
	else
	{
		RepairPath(tmp,false);
		m_str= tmp+m_str;
		
	}

	return m_str;
}

int HippoFilePath::GetRelativePath(std::string& out,std::string& dirname)
{
	//得到这个绝对路径相对于相对于给定路径的相对路径，他们之间的关系可能是：a和b不在一个disk，结果为空；a和b在同一个disk
	//都是用绝对路径来比
	if (m_str.empty())
		return -1;

	HippoFilePath fp(dirname);
	std::string relative=fp.c_str();
	std::string src=m_str;
	if (IsFile())
	{
		src=GetParentDir(m_str);
		RepairPath(src,false);
	}
	std::pair<std::string::iterator,std::string::iterator> com_result=std::mismatch(src.begin(),src.end(),relative.begin());
	if (com_result.first == src.end())//
	{
		relative.replace(relative.begin(),com_result.second,".\\");
	}
	else
	{
		std::string str="";
		if (com_result.second == relative.begin())
		{
			out=str;
			return 1;
		}
		std::string::iterator itr=com_result.first;
		while(itr != src.end())
		{
			if (*itr=='\\')
				str += "..\\";
			++itr;
		}
		//判断com_result.second左边第一个字符是不是\,如果不是移动迭代器
		itr=com_result.second;
		while(*(--itr) != '\\'){}
		relative.replace(relative.begin(),++itr,str);
	}
	out=relative;
	return 1;
}

std::string HippoFilePath::GetDiskName()
{
	//这个时候，存放的必然是绝对路径只需然会前两个字符即可
	if (!m_str.empty() && m_str.length()>3)
		return m_str.substr(0,3);//----
	return "";
}

std::string HippoFilePath::GetParentDir(const std::string& path)
{
	size_t res=path.rfind("\\");
	if (res != std::string::npos)
	{
		return path.substr(0,res);//---
	}
	return "";
}

void HippoFilePath::GetParentPath(std::string& out)
{
	if (IsDirectory())
			out=GetParentDir(GetParentDir(m_str));
	else
		out=GetParentDir(m_str);
	if (!out.empty())
		RepairPath(out,false);//----
}

HippoFilePath HippoFilePath:: CatTwoPath(const char* lp,const char* rp)
{
	if (rp && lp)
	{
		return HippoFilePath((HippoFilePath(lp).c_str())+std::string(rp));
	}
	return HippoFilePath("");
}

bool HippoFilePath::operator==(const HippoFilePath& other) const
{
	if (m_str.length() != other.m_str.length())
	{
		return false;
	}
	return m_str == other.m_str;
}

bool HippoFilePath::operator<(const HippoFilePath& other) const
{
	int a = this->m_str.compare(other.m_str);
	if (a < 0)
	{
		return true;
	}
	return false;
}

bool HippoFilePath::IsOnlyFileName(const std::string& name)
{
	size_t index = name.find("\\");
	if (index != std::string::npos)
	{
		return false;
	}

	index = name.find("/");
	return index == std::string::npos;
}

std::string HippoFilePath::GetFileDir()
{
	std::string DirName = m_str;
	ProcessPath(DirName);
	size_t i = DirName.rfind('\\');
	if (i != std::string::npos)
	{
		DirName = m_str.substr(0, i);
		DirName += '\\';
		return DirName;
	}
	return "";
}

//
void HippoFilePath::Split(std:: string &alldir,std::string  &filename,std::string  &fext )
{
	GetParentPath(alldir);
	GetFileNameNoExtension(filename);
	GetExtensionName(fext);
}
void HippoFilePath::Split( std::string &alldir,std::string  &filewithext )
{
	GetParentPath(alldir);
	GetFileNameWithExtension(filewithext);
}
void HippoFilePath::Split( std::vector<std::string>& alldir, std::string& filewithext)
{
	GetFileNameWithExtension(filewithext);
	size_t last_index=0;
	size_t size=m_str.size();
	//
	for(size_t i=0;i<size;i++)
	{
		if (m_str.at(i)=='\\')
			alldir.push_back(m_str.substr(0,i+1));//输出路径都有 \"
	}
}

bool HippoFilePath::isFileExist()
{
	if (-1 != _access(m_str.c_str(), 0))
	{
		return true;
	}
	return false;
}

