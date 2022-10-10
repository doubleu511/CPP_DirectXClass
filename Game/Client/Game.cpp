#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Arithmetic.h"
using namespace MathLibrary;

void Game::Init(const WindowInfo& window)
{
	GEngine->Init(window);
	double sum = Arithmetic::Add(10, 20);

}

void Game::Update()
{
	GEngine->Render();
}
