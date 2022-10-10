#include "pch.h"
#include "Device.h"

void Device::Init()
{
#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif // _DEBUG

	// DXGI 持失
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	// Device 持失
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));


}