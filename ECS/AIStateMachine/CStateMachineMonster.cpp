#include "CStateMachineMonster.h"

#include <tgCDebugManager.h>

#include "ECS/CEntity.h"
#include "ECS/CWorld.h"
#include "ECS/Components/CPathfindingComponent.h"
#include "ECS/Components/Senses/CHearingComponent.h"
#include "ECS/Components/Senses/CSmellComponent.h"

CStateMachineMonster::CStateMachineMonster()
: m_State( STATE_SEARCH )
, m_SniffCooldown( 2.0f )
, m_WalkIndex( 0.0f )
, m_WaitTime( 2.0f )
, m_Timer( 0.0f )
, m_TimeBetweenPoints( 0.8f )
, m_Searching( false )
, m_SmellDetected( false )
, m_InAttackRange( false )
{

}

CStateMachineMonster::~CStateMachineMonster()
{

}

void CStateMachineMonster::DrawDebug()
{
	tgCDebugManager& rDebug = tgCDebugManager::GetInstance();

	for ( int i = 0; i < m_WalkPath.size(); i++ )
	{
		tgCSphere EntitySphere;
		EntitySphere.Set( m_WalkPath[i], 0.1f );
		rDebug.AddLineSphere( EntitySphere, tgCColor::Red );
	}
}

void CStateMachineMonster::Update(tgFloat DeltaTime, CEntity* Parent)
{
	if( Parent->GetWorld()->GetEntity( "Player" ) )
	{
		DrawDebug();

		switch( m_State )
		{
			case STATE_IDLE:
			{
				m_WalkPath.clear();
				m_WalkIndex = 0;
				m_Timer = 0.0;
				m_Searching = false;

				if( m_WaitTime < 0 )
				{
					if( Parent->GetComponent<CHearingComponent>()->GetHeardNoise() )
					{
						m_State = STATE_FOLLOW_SOUND;
					}
					else if( m_SmellDetected )
					{
						Parent->SetEntityPos(Parent->GetEntityPos());
						m_WalkPath.push_back(Parent->GetEntityPos());
						m_State = STATE_FOLLOW_SMELL;
					}
					else
					{
						Parent->SetEntityPos(Parent->GetEntityPos());
						m_WalkPath.push_back(Parent->GetEntityPos());
						m_State = STATE_SEARCH;
					}
				}

				m_WaitTime -= DeltaTime;

			}
			break;

			case STATE_SEARCH:
			{
				m_WaitTime = 2;
				m_TimeBetweenPoints = 0.6f;

				if( !m_Searching )
				{
					m_WalkPath = Parent->GetComponent< CPathfindingComponent >()->SearhForPlayer( Parent );
					m_Searching = true;
				}

				if( m_Searching )
				{
					Searching( DeltaTime, Parent );
				}

				if( Parent->GetComponent<CHearingComponent>()->GetHeardNoise() )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_WalkPath.clear();
					m_WalkIndex = 0;
					m_Searching = false;
					m_Timer = 0.0;
					m_State = STATE_FOLLOW_SOUND;
				}
				else if( m_SmellDetected )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_WalkPath.clear();
					m_WalkPath.push_back(Parent->GetEntityPos());
					m_WalkIndex = 0;
					m_Searching = false;
					m_Timer = 0.0;
					m_State = STATE_FOLLOW_SMELL;
				}

				tgFloat Distance = (Parent->GetEntityPos() - Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos()).Length();

				if( Distance < 5.0f )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_Searching = false;
					m_State = STATE_ATTACK;
				}

			}
			break;

			case STATE_FOLLOW_SOUND:
			{
				m_WaitTime = 2;
				m_TimeBetweenPoints = 0.25f;

				if ( !m_Searching )
				{
					m_WalkPath = Parent->GetComponent< CPathfindingComponent >()->FollowSound( Parent );
					m_Searching = true;
				}

				if ( m_Searching )
				{
					Searching( DeltaTime, Parent );
				}

				tgFloat Distance = (Parent->GetEntityPos() - Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos()).Length();

				if( Distance < 5.0f )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_Searching = false;
					m_State = STATE_ATTACK;
				}
			}
			break;

			case STATE_FOLLOW_SMELL:
			{
				m_WaitTime = 2.0f;
				m_TimeBetweenPoints = 0.35f;

				if( !m_Searching )
				{
					FollowSmell( DeltaTime, Parent );
					m_Searching = true;
				}
				
				if( m_Searching )
					Searching( DeltaTime, Parent );

				if( Parent->GetComponent<CHearingComponent>()->GetHeardNoise() )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_WalkPath.clear();
					m_WalkIndex = 0;
					m_Searching = false;
					m_Timer = 0.0;
					m_State = STATE_FOLLOW_SOUND;
				}

				tgFloat Distance = (Parent->GetEntityPos() - Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos()).Length();

				if( Distance < 5.0f )
				{
					Parent->SetEntityPos(Parent->GetEntityPos());
					m_Searching = false;
					m_State = STATE_ATTACK;
				}
			}
			break;

			case STATE_ATTACK:
			{
				AttackPlayer( DeltaTime, Parent );

				tgFloat Distance = (Parent->GetEntityPos() - Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos()).Length();

				if( Distance > 8.0f )
				{
					m_InAttackRange = false;

					if( !m_InAttackRange )
					{
						m_Searching = false;
						m_WalkPath.clear();
						m_WalkIndex = 0;
						m_State = STATE_IDLE;
					}
				}
			}
			break;
		}
	}
	
}

void CStateMachineMonster::Searching( tgFloat DeltaTime, CEntity* Parent )
{
	tgCV3D& MonsterPos	= Parent->GetEntityTransform()->GetMatrixWorld().Pos;
	tgCV3D GoalPos		= m_WalkPath[ m_WalkIndex + 1 ];

	MonsterPos.InterpolateLinear( m_WalkPath[ m_WalkIndex ], m_WalkPath[ m_WalkIndex + 1 ], m_Timer / m_TimeBetweenPoints );

	tgCV3D WalkPointDirection = GoalPos - MonsterPos;
	WalkPointDirection.Normalize();
	Parent->GetEntityTransform()->GetMatrixWorld().At = WalkPointDirection;
	Parent->GetEntityTransform()->GetMatrixWorld().OrthoNormalize();
				
	m_Timer += DeltaTime;

	if ( m_Timer >= m_TimeBetweenPoints )
	{
		m_Timer = 0;
		m_WalkIndex++;

		if ( m_WalkIndex == m_WalkPath.size() - 1 )
		{
			m_Searching = false;
			m_WalkPath.clear();
			m_WalkIndex = 0;
			m_State = STATE_IDLE;
		}
	}
}

void CStateMachineMonster::FollowSmell( tgFloat DeltaTime, CEntity* Parent )
{
	SmellList SmellList = Parent->GetWorld()->GetEntity( "Player" )->GetComponent< CSmellComponent >()->GetSmellList();

	tgCV3D SmellPoint = SmellList.front()->m_Pos;
	tgFloat	SmellIndex;

	for ( int i = 0; i < SmellList.size(); i++ )
	{
		if( i == 0 )
		{
			for ( SmellIndex = 0; SmellIndex < SmellList.size(); SmellIndex++ )
			{
				tgFloat Distance = ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - SmellList[ SmellIndex ]->m_Pos ).Length();
				tgFloat ClosestPoint = ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - SmellPoint ).Length();

				if ( Distance < ClosestPoint )
				{
					SmellPoint = SmellList[ SmellIndex ]->m_Pos;
					i = SmellIndex;
				}
			}
		}
		else
		{
			SmellPoint = SmellList[i]->m_Pos;
		}

		m_WalkPath.push_back( SmellPoint );
	}
}

void CStateMachineMonster::AttackPlayer(tgFloat DeltaTime, CEntity* Parent)
{
	tgCV3D	LookDirection = Parent->GetEntityPos() - Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos();
	m_WaitTime = 2;
	m_InAttackRange = true;
	Parent->GetEntityTransform()->GetMatrixWorld().At.z =  -LookDirection.z;
	Parent->GetEntityTransform()->GetMatrixWorld().At.x =  -LookDirection.x;
	Parent->GetEntityTransform()->GetMatrixWorld().OrthoNormalize();
}
