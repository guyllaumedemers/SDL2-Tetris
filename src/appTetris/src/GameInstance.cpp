#include "../include/GameInstance.h"

void GameInstance::Initialize(SDLManager* const SDLManagerPtrArg) const
{
	if (!SDLManagerPtrArg || !LevelManagerUniquePtr)
	{
		return;
	}

	LevelManagerUniquePtr->Initialize(SDLManagerPtrArg, SetWindowEvent);
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
