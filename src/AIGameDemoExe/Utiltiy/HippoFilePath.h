/********************************************************************
	created:	2013/10/07
	created:	7:10:2013   17:29
	filename: 	E:\ResCheckModule\ResCheckerCore\HippoFilePath.h
	file path:	E:\ResCheckModule\ResCheckerCore
	file base:	FilePath
	file ext:	h
	author:		sssa2000
	
	purpose:	·���ַ���������㣬���ܽ�ȥ����ʲô��������Ŀ¼������б�ܣ��ļ���������֧�ִ����ʽ������
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
	//��Ӧ����
	HippoFilePath(const HippoFilePath& other);
	HippoFilePath& operator=(const HippoFilePath& other);
	bool operator==(const HippoFilePath& other) const;//������==
	bool operator!=(const HippoFilePath& other) const;
	bool operator<(const HippoFilePath& other) const;//������<

	void SetFilePath(const std::string& fn);
	//	�Ƿ���Ŀ¼
	bool IsDirectory();
	//	�Ƿ����ļ�
	bool IsFile();
	//	�����ļ���+��չ����������Ŀ¼��
	int GetFileNameWithExtension(std::string& fn);
	//	������չ��
	int GetExtensionName(std::string& extension);
	//	�����ļ�������������չ����
	int GetFileNameNoExtension(std::string& fn);
	//	�޸���չ��
	int ChangeExtensionName(const char* extension);
	//	���������ĳ���ļ��е����·����
	int GetRelativePath(std::string& out,std::string& relative);
	//	�����̷�
	std::string GetDiskName();//�̷�Ҫ�ṩ����б����
	//	������һ��Ŀ¼����ȫ·��
	void  GetParentPath(std::string& out);
	//	�и�str����ȡ·�����ļ�������׺
	void Split(std:: string &alldir,std::string  &filename,std::string  &fext );
	void Split( std::string &alldir,std::string  &filewithext );
	void Split( std::vector<std::string>& alldir, std::string& filewithext);

	//��ȡһ���ļ�·���е�����Ŀ¼����Ŀ¼���Ǵ�б�ܵ�
	std::string GetFileDir();
	//	����ÿһ��Ŀ¼
	int	GetPathAllDir(std::vector<std::string>& vecres);

	//!�Ƿ���ļ�ֻ���ļ�����û��·����Ŀ¼
	static bool IsOnlyFileName(const std::string& name);	

	const char* c_str();
	//	��������·���������жϣ�
	static	HippoFilePath CatTwoPath(const char* lp,const char* rp);
	
	const std::string& GetStdString()const{return m_str;}
	bool isFileExist();//�ļ��Ƿ����
	//ͳһб�ܷ���ʹ�Сд
	static void ProcessPath(std::string& );
	static std::string GetParentDir(const std::string& out);
	
private:	
	//�����·����б�ܣ��ļ���������
	int RepairPath(std::string& _name,bool isIncludeFileName);
	//�ǲ��Ǿ���·��
	bool IsGlobalPath(const std::string& str);
	//	����ȫ·��--����·��
	std::string CalculateAbsolutePath();
	void OnInit();
	
protected:
private:
	std::string m_str;
};