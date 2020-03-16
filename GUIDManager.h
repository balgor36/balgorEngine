#ifndef GUID_MANAGER_H
#define GUID_MANAGER_H

#include "time_bg.h"

class GUIDManager
{
public:
    GUIDManager();
    ~GUIDManager();

    Time& GetTimeRef();
private:
    Time* time;
};

#endif
