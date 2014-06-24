#include <map>
#include <string>
void fun()
{
	std::string key="111";
	std::map<std::string,int> tmpmap;
	tmpmap.insert(std::make_pair(key,1));
}