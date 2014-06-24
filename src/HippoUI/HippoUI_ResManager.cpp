#include "HippoUI_ResManager.h"
#include "HippoUI_ResFactory.h"
#include "HippoUI_Global.h"
#include "HippoUI_Define.h"
#include "ErrReport.h"
#include "tixml/tinyxml.h"
#include <algorithm>

HippoUI_ResManager::HippoUI_ResManager()
{

}
HippoUI_ResManager::~HippoUI_ResManager()
{
	auto itr=m_all_state_textures.begin();
	auto itrend=m_all_state_textures.end();
	while (itr!=itrend)
	{
		delete itr->second;
		++itr;
	}
	m_all_state_textures.clear();
}


void HippoUI_ResManager::LoadUIStyleFromFile(const char* filename)
{
	//read xml 
	TiXmlDocument doc;
	bool b=doc.LoadFile(filename);
	if(!b)
	{
		ReportErr("Load UI Style File: %s Failed\n%s",filename,doc.ErrorDesc());
		return;
	}

	TiXmlElement* pRoot=doc.FirstChildElement();
	TiXmlElement* pControl=pRoot->FirstChildElement("Control");
	while (pControl)
	{
		TiXmlElement* pShader=pControl->FirstChildElement("ShaderFx");
		TiXmlElement* pTexture=pControl->FirstChildElement("Texture");
		if(pShader)
		{
			const char* s=pShader->Attribute("state");
			UISTATE state=ParseState(s);
			if(state!=UISTATE_END)
			{
				const char* path=pShader->Attribute("file");
				AddShaderResourceFromFile(path,state);
			}
		}

		while(pTexture)
		{
			const char* s=pTexture->Attribute("state");
			const char* rect=pTexture->Attribute("rect");
			UISTATE state=ParseState(s);
			if(state!=UISTATE_END)
			{
				const char* path=pTexture->Attribute("file");
				AddTextureResourceFromFile(path,state,rect);
			}
			pTexture=pTexture->NextSiblingElement("Texture");
		}
		pControl=pControl->NextSiblingElement("Control");
	}
}
UISTATE HippoUI_ResManager::ParseState(const char* s)
{
	for (int i=0;i<UISTATE_END;++i)
	{
		if(strcmp(strUISTATE[i],s)==0)
			return (UISTATE)i;
	}
	return UISTATE_END;
}

void HippoUI_ResManager::AddShaderResourceFromFile(const char* path,UISTATE state)
{

	//m_all_shaders.push_back(std::make_pair(state,std::make_shared()));
}
void HippoUI_ResManager::AddTextureResourceFromFile(const char* path,UISTATE state,const char* rect)
{
	HippoUI_TextureResource* p=HippoUI_Global::GetIns()->GetResFactory()->CreateTextureFromFile(path);
	TextureResHandle handle(p);
	m_all_textures.insert(std::make_pair(path,handle));

	HippoUI_TextureProxy* proxy=CreateTextureProxy(handle,rect);
	m_all_state_textures.push_back(std::make_pair(state,proxy));

}




HippoUI_TextureProxy* HippoUI_ResManager::CreateTextureProxy(TextureResHandle& tex_handle,const char* srect)
{
	HippoUI_Rect _rect;
	_rect.SetRectFromString(srect);
	HippoUI_TextureProxy* p=new HippoUI_TextureProxy(tex_handle,_rect);
	return p;
}

HippoUI_TextureProxy* HippoUI_ResManager::GetTextureProxy(UISTATE s)
{
	auto itr=std::find_if(
		m_all_state_textures.begin(),
		m_all_state_textures.end(),
		[&](std::pair<UISTATE,HippoUI_TextureProxy*>& _pair)->bool{return _pair.first==s;}
	);
	if (itr!=m_all_state_textures.end())
	{
		return itr->second;
	}
	return 0;
}