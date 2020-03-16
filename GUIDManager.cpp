#include "GUIDManager.h"

GUIDManager::GUIDManager()
{
}

GUIDManager::~GUIDManager()
{

}

Time& GUIDManager::GetTimeRef()
{
    return *time;
}
