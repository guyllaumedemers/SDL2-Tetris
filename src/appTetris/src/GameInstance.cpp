#include "../include/GameInstance.h"
#include "../include/LevelManager.h"

GameInstance::GameInstance()
{
	LevelManagerSharedPtr = std::make_shared<LevelManager>();
}

GameInstance::~GameInstance()
{
	LevelManagerSharedPtr = nullptr;
	SetWindowDelegate = nullptr;
}

void GameInstance::Initialize() const
{
	if (!LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->Initialize(SetWindowDelegate);
}

void GameInstance::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!SDLManagerPtrArg || !TextureManagerPtrArg || !LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
}

void GameInstance::Clear() const
{
	if (!LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->Clear();
}

void GameInstance::RestartGame() const
{
	if (!LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->ResetGame(SetWindowDelegate);
}

void GameInstance::PollKeyEvent(int8_t DirX, int8_t DirY) const
{
	if (!LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->PollKeyEvent(DirX, DirY);
}

void GameInstance::PollSpaceKeyEvent() const
{
	if (!LevelManagerSharedPtr)
	{
		return;
	}

	LevelManagerSharedPtr->PollSpaceKeyEvent();
}
