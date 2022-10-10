#pragma once

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

public:
	void ResizeWindow(int32 width, int32 height);

	void RenderBegin();
	void RenderEnd();

private:
	WindowInfo _window;

	// 그려질 화면 크기
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<class Device> _device;
	shared_ptr<class SwapChain> _swapChain;
	shared_ptr<class CommandQueue> _cmdQueue;
};

