#pragma once

#include "ECS/IComponent.h"

class CHealthComponent : public IComponent
{
public:

	CHealthComponent( tgFloat Health );

	void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ) override;

	void TakeDamage( tgFloat Damage );

private:

	tgFloat m_MaxHealth;
	tgFloat m_CurrentHealth;
};

