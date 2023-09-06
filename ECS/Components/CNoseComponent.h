#pragma once

#include	"ECS/IComponent.h"

#include	<tgMemoryDisable.h>
#include	<vector>
#include	<tgMemoryEnable.h>

#include	"ECS/Components/Senses/SSmell.h"

typedef std::vector< SSmell* > SmellList;

class CNoseComponent : public IComponent
{
public:

	 CNoseComponent();
	~CNoseComponent();

	void Update( tgFloat DeltaTime, CEntity* Parent ) override;

private:

	SmellList	m_SmellList;

	tgBool		m_SmellDetected;
};

