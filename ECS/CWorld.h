#pragma once

#include "tgSystem.h"
#include <tgMemoryDisable.h>
#include <vector>
#include <tgMemoryEnable.h>

#include "Navigation/CNavMesh.h"
#include "Navigation/SNode.h"

class		CEntity;
class		CNavMesh;

typedef std::vector< SNode* > SNodeList;
typedef std::vector< CEntity* > CEntityList;

class CWorld
{
public:

	// Constructor / Destructor
	 CWorld( void );
	~CWorld( void );

	//////////////////////////////////////////////////////////////////////////

	void		DrawDebug				( void );
	void		Update					( tgFloat DeltaTime );
	void		DestroyEntity			( tgCString Name );
	CEntity*	CreateEntity			( tgCString Name );
	CEntity*	GetEntity				( tgCString Name );

	//////////////////////////////////////////////////////////////////////////

	SNodeList	GetNodeList				( void )	{ return m_pNavMesh->GetNodeList(); }
	tgFloat		GetMonsterIndexList		( void )	{ return m_MonsterIndex; }

	//////////////////////////////////////////////////////////////////////////

private:

	CEntityList					m_EntityList;

	CNavMesh*					m_pNavMesh;

	tgCWorld*					m_pCollisionWorld;
	tgCWorld*					m_pNavWorld;

	tgFloat						m_MonsterIndex;
	tgFloat						m_Score;
	tgFloat						m_ScoreThreshold;
};
