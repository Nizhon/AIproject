#pragma once
#include "tgSystem.h"

#include "ECS/IComponent.h"

class CModelComponent : public IComponent
{
public:

	// Constructor / Destructor
	 CModelComponent( const char* pFilePath, const tgCString& rName );
	~CModelComponent( void );

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent = nullptr ) override;

	//////////////////////////////////////////////////////////////////////////

private:

	tgCModel*		m_pModel;
};

