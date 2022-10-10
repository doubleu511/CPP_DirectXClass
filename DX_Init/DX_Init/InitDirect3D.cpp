#include "../../Common/d3dApp.h"
#include <DirectXColors.h>

using namespace DirectX;

class initdirect3D : public D3DApp
{
public:
	initdirect3D(HINSTANCE hInstance);
	~initdirect3D();

	virtual bool Initialize()override;
	
private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;
};	

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	try
	{
		initdirect3D theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

initdirect3D::initdirect3D(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

initdirect3D::~initdirect3D()
{
}

bool initdirect3D::Initialize()
{
	if (!D3DApp::Initialize())
		return false;
	// 여기서 초기화
	return true;
}

void initdirect3D::OnResize()
{
	D3DApp::OnResize();
	// 추가 구현
}

void initdirect3D::Update(const GameTimer& gt)
{

}

void initdirect3D::Draw(const GameTimer& gt)
{
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	FlushCommandQueue();
}