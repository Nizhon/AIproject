#include "CSmellComponent.h"

#include <tgCDebugManager.h>
#include <tgCSphere.h>

#include "ECS/CEntity.h"
#include "ECS/CWorld.h"
#include "ECS/AIStateMachine/CStateMachineMonster.h"

CSmellComponent::CSmellComponent( tgFloat Timer )
: m_SmellIndex( 0 )
, m_Timer( Timer )
, m_Cooldown( 0 )
, m_SmellDetected ( false )
{
}

CSmellComponent::~CSmellComponent()
{
	while(!m_SmellList.empty())
	{
		int i = 0;
		delete m_SmellList[i];
		m_SmellList.erase(m_SmellList.begin());
	}
}

void CSmellComponent::DrawDebug( void )
{
	tgCDebugManager& rDebug = tgCDebugManager::GetInstance();
	for ( int i = 0; i < m_SmellList.size(); i++ )
	{
		tgCSphere SmellSphere;
		SmellSphere.Set( m_SmellList[i]->m_Pos, 4.0f );
		rDebug.AddLineSphere( SmellSphere, tgCColor::Green );
	}
}

void CSmellComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent->GetWorld()->GetEntity( "Player" ) && Parent->GetWorld()->GetEntity( "Player" ) != nullptr )
	{
		DrawDebug();

		if( m_Cooldown <= 0 )
		{
			SSmell* Smell		= new SSmell;
			Smell->m_Pos		= Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos();
			Smell->m_LifeTime 	= 0;
			m_SmellList.push_back( Smell );
			m_Cooldown = m_Timer;
		}

		for ( int i = 0; i < m_SmellList.size(); i++ )
		{
			m_SmellList[i]->m_LifeTime += DeltaTime;

			if(m_SmellList[i]->m_LifeTime >= 8.0f)
			{
				delete m_SmellList[i];
				m_SmellList.erase( m_SmellList.begin() + i );
			}
		}

		m_Cooldown -= DeltaTime;
	}
}
