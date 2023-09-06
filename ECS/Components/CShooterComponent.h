#pragma once
#include "ECS/IComponent.h"

#include <tgCV3D.h>
#include "ECS/CEntity.h"

class CShooterComponent : public IComponent
{
public:

	// Constructor / Destructor
	 CShooterComponent( tgFloat AttackSpeed, tgFloat AttackRange );
	~CShooterComponent();

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ) override;

	//////////////////////////////////////////////////////////////////////////

private:

	tgCV3D		m_Direction;
	tgFloat		m_Distance;
	tgFloat		m_AttackSpeed;
	tgFloat		m_AttackRange;
	tgFloat		m_Cooldown;
};

