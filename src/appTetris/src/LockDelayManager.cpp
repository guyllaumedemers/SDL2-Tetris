#include "../include/LockDelayManager.h"

LockDelayManager::~LockDelayManager()
{
	GenerateTetrominoeOnLockDelegate = nullptr;
}