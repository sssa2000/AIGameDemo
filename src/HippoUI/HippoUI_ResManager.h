/********************************************************************
	created:	2012/10/23
	created:	23:10:2012   0:15
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_ResManager.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_ResManager
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "HippoUI_Resource.h"
#include "HippoUI_Define.h"

class HippoUI_TextureProxy;


class HippoUI_ResManager
{
public:
	HippoUI_ResManager();
	~HippoUI_ResManager();
	void LoadUIStyleFromFile(const char* filename);


	HippoUI_TextureProxy* GetTextureProxy(UISTATE s);
protected:
private:
	UISTATE ParseState(const char* s);
	void AddShaderResourceFromFile(const char* path,UISTATE state);
	void AddTextureResourceFromFile(const char* path,UISTATE state,const char* rect);
	HippoUI_TextureProxy* CreateTextureProxy(TextureResHandle& tex_handle,const char* rect);

	std::map<std::string,TextureResHandle > m_all_textures;
	std::map<std::string,ShaderResHandle > m_all_shaders;
	std::vector<std::pair<UISTATE,HippoUI_TextureProxy*> > m_all_state_textures;
	std::vector<std::pair<UISTATE,ShaderResHandle> > m_all_state_shaders;


};