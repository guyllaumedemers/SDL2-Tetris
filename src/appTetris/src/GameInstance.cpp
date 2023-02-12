#include "../include/GameInstance.h"
#include "../include/LevelManager.h"

GameInstance::GameInstance()
{
	LevelManagerUniquePtr = std::make_shared<LevelManager>();
}

GameInstance::~GameInstance()
{
	LevelManagerUniquePtr = nullptr;
	SetWindowDelegate = nullptr;
}

void GameInstance::Initialize() const
{
	if (!LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->Initialize(SetWindowDelegate);
}

void GameInstance::Update(TextureManager* const TextureManagerPtrArg, SDLManager* const SDLManagerPtrArg) const
{
	if (!SDLManagerPtrArg || !LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->Update(TextureManagerPtrArg, SDLManagerPtrArg);
}

void GameInstance::Clear() const
{
	if (!LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->Clear();
}

void GameInstance::Reset()
{
	if (!LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->Reset(SetWindowDelegate);
}

void GameInstance::PollKeyEvent(int8_t DirX, int8_t DirY) const
{
	if (!LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->PollKeyEvent(DirX, DirY);
}

void GameInstance::PollSpaceKeyEvent() const
{
	if (!LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->PollSpaceKeyEvent();
}
