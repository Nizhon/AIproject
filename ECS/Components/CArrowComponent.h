#pragma once
#include "ECS/IComponent.h"

#include "ECS/CEntity.h"

class CArrowComponent : public IComponent
{
	public:

	// Constructor / Destructor
	 CArrowComponent( tgCV3D ArrowPos, tgCV3D PlayerPos, tgFloat ProjectileSpeed, tgBool Alive );
	~CArrowComponent();

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ) override;

	//////////////////////////////////////////////////////////////////////////

private:

	tgCV3D m_BulletPos;
	tgCV3D m_PlayerPos;
	tgCV3D m_Direction;

	tgFloat m_BulletSpeed;
	tgFloat m_AliveTime;

	tgBool m_BulletAlive;
};

