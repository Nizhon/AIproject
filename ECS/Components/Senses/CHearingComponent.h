#pragma once
#include "ECS/IComponent.h"

class CHearingComponent : public IComponent
{
public:

	// Constructor / Destructor
	 CHearingComponent( void );
	~CHearingComponent( void );

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent ) override;

	//////////////////////////////////////////////////////////////////////////

	tgBool GetHeardNoise() { return m_HeardNoise; }

private:

	tgBool m_HeardNoise;

};

