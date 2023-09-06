#pragma once

#include "ECS/IComponent.h"

#include "Navigation/SNode.h"

#include <tgMemoryDisable.h>
#include <vector>
#include "random"
#include <tgMemoryEnable.h>
#include "ECS/CEntity.h"

typedef std::vector < SNode >	Path;
typedef std::vector	< SNode* >	SNodeList;
typedef std::vector < tgCV3D >	WalkPath;

class CPathfindingComponent : public IComponent
{
public:

	enum EResult
	{
		PATH_FOUND,
		PATH_NOT_FOUND,
	};

	// Constructor / Destructor
	 CPathfindingComponent();
	~CPathfindingComponent();

	//////////////////////////////////////////////////////////////////////////

	void		Update			( tgFloat DeltaTime, CEntity* Parent ) override;

	//////////////////////////////////////////////////////////////////////////

	Path*		GetPath			( SNode* Start, SNode* Goal );

	//////////////////////////////////////////////////////////////////////////

	WalkPath	SearhForPlayer	( CEntity* Parent );
	WalkPath	FollowSound		( CEntity* Parent );

	//////////////////////////////////////////////////////////////////////////

private:

	SNodeList		m_OpenList;
	SNodeList		m_ClosedList;
	SNodeList		m_StartGoal;
	SNodeList		m_Path;
	WalkPath		m_WalkPath;

	SNode*			m_pStart;
	SNode*			m_pGoal;

	EResult			m_Result;
};

