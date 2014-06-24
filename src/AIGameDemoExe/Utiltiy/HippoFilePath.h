/********************************************************************
	created:	2013/10/07
	created:	7:10:2013   17:29
	filename: 	E:\ResCheckModule\ResCheckerCore\HippoFilePath.h
	file path:	E:\ResCheckModule\ResCheckerCore
	file base:	FilePath
	file ext:	h
	author:		sssa2000
	
	purpose:	路径字符串虚拟计算，不管进去的是什么，出来的目录最后必有斜杠，文件不处理；不支持错误格式的输入
*********************************************************************/
#pragma once
#include <string>
#include <vector>

class HippoFilePath
{
public:
	HippoFilePath();
	HippoFilePath(const char* fn);
	HippoFilePath(const std::string& fn);
	~HippoFilePath();
	//还应该有
	HippoFilePath(const HippoFilePath& other);
	HippoFilePath& operator=(const HippoFilePath& other);
	bool operator==(const HippoFilePath& other) const;//操作符==
	bool operator!=(const HippoFilePath& other) const;
	bool operator<(const HippoFilePath& other) const;//操作符<

	void SetFilePath(const std::string& fn);
	//	是否是目录
	bool IsDirectory();
	//	是否是文件
	bool IsFile();
	//	返回文件名+扩展名（不包括目录）
	int GetFileNameWithExtension(std::string& fn);
	//	返回扩展名
	int GetExtensionName(std::string& extension);
	//	返回文件名（不包括扩展名）
	int GetFileNameNoExtension(std::string& fn);
	//	修改扩展名
	int ChangeExtensionName(const char* extension);
	//	返回相对于某个文件夹的相对路径，
	int GetRelativePath(std::string& out,std::string& relative);
	//	返回盘符
	std::string GetDiskName();//盘符要提供最后的斜杠吗？
	//	返回上一级目录名的全路径
	void  GetParentPath(std::string& out);
	//	切割str，获取路径，文件名，后缀
	void Split(std:: string &alldir,std::string  &filename,std::string  &fext );
	void Split( std::string &alldir,std::string  &filewithext );
	void Split( std::vector<std::string>& alldir, std::string& filewithext);

	//获取一个文件路径中的所有目录名，目录名是带斜杠的
	std::string GetFileDir();
	//	返回每一级目录
	int	GetPathAllDir(std::vector<std::string>& vecres);

	//!是否该文件只有文件名，没有路径的目录
	static bool IsOnlyFileName(const std::string& name);	

	const char* c_str();
	//	串联两个路径（不做判断）
	static	HippoFilePath CatTwoPath(const char* lp,const char* rp);
	
	const std::string& GetStdString()const{return m_str;}
	bool isFileExist();//文件是否存在
	//统一斜杠方向和大小写
	static void ProcessPath(std::string& );
	static std::string GetParentDir(const std::string& out);
	
private:	
	//如果是路径不斜杠，文件不做处理
	int RepairPath(std::string& _name,bool isIncludeFileName);
	//是不是绝对路径
	bool IsGlobalPath(const std::string& str);
	//	返回全路径--绝对路径
	std::string CalculateAbsolutePath();
	void OnInit();
	
protected:
private:
	std::string m_str;
};