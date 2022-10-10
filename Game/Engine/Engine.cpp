#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"

void Engine::Init(const WindowInfo& window)
{
	_window = window;
	ResizeWindow(window.width, window.height);

	_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(window, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
}

void Engine::Render()
{
	RenderBegin();

	// TO DO

	RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
}
