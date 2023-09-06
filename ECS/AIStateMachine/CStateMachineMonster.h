#pragma once

#include "ECS/IComponent.h"

#include <tgMemoryDisable.h>
#include <vector>
#include <tgMemoryEnable.h>
#include "ECS/Components/Senses/SSmell.h"

typedef std::vector< tgCV3D >	WalkPath;
typedef std::vector< SSmell* >	SmellList;

class CStateMachineMonster : public IComponent
{
public:

	enum EState
	{
		STATE_IDLE,
		STATE_SEARCH,
		STATE_FOLLOW_SOUND,
		STATE_FOLLOW_SMELL,
		STATE_ATTACK,

	}; 	// EState


	//////////////////////////////////////////////////////////////////////////

	// Constructor / Destructor
	 CStateMachineMonster();
	~CStateMachineMonster();

	//////////////////////////////////////////////////////////////////////////

	void	DrawDebug			( void );
	void	Update				( tgFloat DeltaTime, CEntity* Parent ) override;

	//////////////////////////////////////////////////////////////////////////

	void	SetSmellDetected	( tgBool SmellDetected )	{ m_SmellDetected = SmellDetected; }

	//////////////////////////////////////////////////////////////////////////

	void	Searching			( tgFloat DeltaTime, CEntity* Parent );
	void	FollowSmell			( tgFloat DeltaTime, CEntity* Parent );
	void	AttackPlayer		( tgFloat DeltaTime, CEntity* Parent );

	//////////////////////////////////////////////////////////////////////////

	tgBool	InAttackRange		( void )	{ return m_InAttackRange; }

	//////////////////////////////////////////////////////////////////////////

private:

	EState			m_State;

	tgFloat			m_SniffCooldown;
	tgFloat			m_WalkIndex;
	tgFloat			m_WaitTime;
	tgFloat			m_Timer;
	tgFloat			m_TimeBetweenPoints;

	tgBool			m_Searching;
	tgBool			m_SmellDetected;
	tgBool			m_InAttackRange;

	WalkPath		m_WalkPath;
};

