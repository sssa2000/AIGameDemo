/*!
 * \file FxEffectManager.h
 * \date 2014/06/02 10:54
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#pragma once
#include <map>
#include <memory>
#include <string>
#include <d3dx9.h>

typedef std::shared_ptr<ID3DXEffect> EffHandle;
typedef std::map<std::string, EffHandle > EffCon;


class FxEffectManager
{
public:
	FxEffectManager();
	~FxEffectManager();

	EffHandle RequireEffectFormFile(const char* fn);
protected:
	ID3DXEffect* LoadFxFromFile(const char* fn);
	EffCon m_all_eff;
private:
};