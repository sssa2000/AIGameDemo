#include "FxEffectManager.h"
#include "ScopeGuard.h"
#include "ErrReport.h"
#include <d3dx9.h>
#include "Globals.h"
#include "HippoD3d9Device.h"
FxEffectManager::FxEffectManager()
{

}
FxEffectManager::~FxEffectManager()
{

}

EffHandle FxEffectManager::RequireEffectFormFile(const char* fn)
{
	auto itr = m_all_eff.find(std::string(fn));
	if (itr!=m_all_eff.end())
	{
		return itr->second;
	}

	ID3DXEffect* eff = LoadFxFromFile(fn);
	auto res = m_all_eff.insert(std::make_pair(std::string(fn), EffHandle(eff, [&](ID3DXEffect* p){p->Release(); })));
	return res.first->second;
}

ID3DXEffect* FxEffectManager::LoadFxFromFile(const char* fxname)
{
	auto d3d9device = Globals::GetDevice()->GetDeviceD3D9();
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
	dwShaderFlags |= D3DXSHADER_DEBUG;
	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
	ID3DXEffect* peff = 0;
	ID3DXBuffer* perrorbuffer = 0;
	ON_SCOPE_EXIT([&]{if (perrorbuffer) perrorbuffer->Release(); });

	HRESULT v = D3DXCreateEffectFromFileA(d3d9device, fxname, NULL, NULL, dwShaderFlags, NULL, &peff, &perrorbuffer);
	if (v != S_OK)
	{
		const char* err_info = 0;
		if (perrorbuffer)
		{
			err_info = (const char*)perrorbuffer->GetBufferPointer();
			ReportErr("D3DXCreateEffectFromFile %s Failed\n%s", fxname, err_info);
		}
		ReportErr("D3DXCreateEffectFromFile %s Failed", fxname);
	}
	return peff;
}