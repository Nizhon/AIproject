#pragma once

#include "ECS/IComponent.h"

#include	<tgCTransform.h>

class CTransformComponent : public IComponent
{
public:

	 CTransformComponent();
	~CTransformComponent();

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ) override;

	//////////////////////////////////////////////////////////////////////////

	tgCTransform* GetTransfrom() { return m_pTransform; }

private:

	tgCTransform* m_pTransform;

};
