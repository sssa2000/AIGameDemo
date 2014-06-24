#include "HippoD3d9Device.h"
#include "ErrReport.h"

HippoD3d9Device::HippoD3d9Device()
{
	m_pD3D=0;
	m_pd3dDevice=0;
	m_bWindowed=true;
	m_maxfps = 0;
	lastTick = 0;
	currentTick = 0;

}
HippoD3d9Device::~HippoD3d9Device()
{
	
}
D3DPRESENT_PARAMETERS& HippoD3d9Device::FillD3dParameter(int w, int h, bool bWindowed)
{
	m_width=w;
	m_height=h;
	m_bWindowed = bWindowed;

	ZeroMemory( &m_d3dpp, sizeof( m_d3dpp ) );
	m_d3dpp.Windowed = m_bWindowed;

	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;// D3DSWAPEFFECT_FLIPEX;// D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferHeight = m_height;
	m_d3dpp.BackBufferWidth = m_width;
	m_d3dpp.BackBufferCount = 3;
	m_d3dpp.FullScreen_RefreshRateInHz = 0;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;// D3DPRESENT_INTERVAL_IMMEDIATE;
	//m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	if (!bWindowed)
	{
		m_d3dpp.FullScreen_RefreshRateInHz = 70;
	}
	return m_d3dpp;
}

void HippoD3d9Device::Init(HWND hwnd,int w,int h,bool bWindowed)
{

	//set up dx
	m_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == (m_pD3D))
	{
		ReportErr("Direct3DCreate9 Failed");
	}
	
	FillD3dParameter(w, h, bWindowed);

	//使用NV的PERFHUD
	unsigned int adapterCount = m_pD3D->GetAdapterCount();
	DWORD adapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
	for (unsigned int adapter = 0; adapter<adapterCount; ++adapter)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = m_pD3D->GetAdapterIdentifier(adapter, 0, &Identifier);
		if (strstr(Identifier.Description,"PerfHUD") != 0)
		{
			adapterToUse = adapter;
			deviceType = D3DDEVTYPE_REF;
			break;
		}
	}

	// Create the D3DDevice
	HRESULT hr = m_pD3D->CreateDevice(
		adapterToUse/*D3DADAPTER_DEFAULT*/, 
		deviceType/*D3DDEVTYPE_HAL*/, 
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp, 
		&m_pd3dDevice );

	if( FAILED(hr) )
	{
		ReportErr("CreateDevice Failed");
	}

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

}

void HippoD3d9Device::CleanUp()
{
	m_pd3dDevice->Release();
	m_pD3D->Release();
}

IDirect3DDevice9* HippoD3d9Device::GetDeviceD3D9()
{
	return m_pd3dDevice;
}
void HippoD3d9Device::SetViewport(int w,int h)
{
	D3DVIEWPORT9 ret;
	ret.X = 0;
	ret.Y = 0;
	ret.Width = w;
	ret.Height = h;
	ret.MinZ = 0.0f;
	ret.MaxZ = 1.0f;
	HRESULT hr = m_pd3dDevice->SetViewport(&ret);
}

void HippoD3d9Device::BeginRender()
{
	m_pd3dDevice->BeginScene();
}

void HippoD3d9Device::EndRender()
{
	//static unsigned int currentTick2 = 0;
	//static unsigned int lastTick2 = currentTick2;
	//currentTick2 = GetTickCount();
	//if (m_maxfps > 0)
	//{
	//	if ((currentTick2 - lastTick2) < inv_maxfps)
	//	{
	//		m_pd3dDevice->EndScene();
	//		return;
	//	}
	//}
	//lastTick2 = currentTick2;
	//Sleep(20);
	 m_pd3dDevice->EndScene();
	 HRESULT r = m_pd3dDevice->Present(0, 0, 0, 0);
	if(r == D3DERR_DEVICELOST)
	{
		ProcessDeviceLost();
	}
	currentTick = GetTickCount();
	m_fps = 1000.f / (currentTick - lastTick);
	lastTick = currentTick;

}

void HippoD3d9Device::ProcessDeviceLost()
{
	int attempt = 0;
	HRESULT hr;
	while(FAILED(hr=m_pd3dDevice->TestCooperativeLevel()))
	{
		// Check if the device needs to be reset.
		if (D3DERR_DEVICENOTRESET == hr) 
			break;

		// If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr )
			Sleep(10);

		if( D3DERR_DRIVERINTERNALERROR == hr )
			Sleep(10);

		Sleep(10);

		if (attempt++ > 10)
		{
			return;
		}
	}


	if (m_pd3dDevice->Reset(&m_d3dpp) != D3D_OK)
		ReportErr("Reset Device Failed");

}


float HippoD3d9Device::GetFPS()
{
	return m_fps;
}
	
void HippoD3d9Device::SetMaxFPS(unsigned int max_fps)
{ 
	m_maxfps = max_fps; 
	inv_maxfps = 0;
	if (max_fps > 0)
		inv_maxfps = 1000.0 / (double)m_maxfps; 
}