#include "CEntity.h"

#include "CWorld.h"

CEntity::CEntity( void )
: m_pWorld( nullptr )
, m_pParent( nullptr )
{
	
}

CEntity::~CEntity( void )
{
	for ( int i = 0; i < m_ComponentList.size(); i++ )
	{
		delete m_ComponentList[i];
	}

	m_ComponentList.clear();
}

void CEntity::Update( tgFloat DeltaTime)
{
	for ( int i = 0; i < m_ComponentList.size(); i++ )
	{
		m_ComponentList[i]->Update( DeltaTime, m_pParent );
	}
}
