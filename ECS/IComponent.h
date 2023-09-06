#pragma once

#include <tgSystem.h>

class CEntity;

class IComponent
{
public:

	virtual ~IComponent(){};

	virtual void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ){};

private:

};

