#include "CNoseComponent.h"

#include <tgCDebugManager.h>

#include "ECS/CEntity.h"
#include "ECS/CWorld.h"
#include "ECS/AIStateMachine/CStateMachineMonster.h"
#include "Senses/CSmellComponent.h"

CNoseComponent::CNoseComponent()
: m_SmellDetected( false )
{
}

CNoseComponent::~CNoseComponent()
{
}

void CNoseComponent::Update(tgFloat DeltaTime, CEntity* Parent)
{
	if( Parent->GetWorld()->GetEntity( "Player" ) && Parent->GetWorld()->GetEntity( "Player" ) != nullptr )
	{
		m_SmellList = Parent->GetWorld()->GetEntity( "Player" )->GetComponent< CSmellComponent >()->GetSmellList();

		tgFloat SmellsDetected = 0;

		for ( int j = 0; j < m_SmellList.size(); j++ )
		{
			tgFloat Distance = ( m_SmellList[j]->m_Pos - Parent->GetEntityPos() ).Length();

			if( Distance < 4.0f )
			{
				m_SmellDetected = true;
				SmellsDetected++;
				Parent->GetComponent< CStateMachineMonster >()->SetSmellDetected( m_SmellDetected );
			}
		}

		if( SmellsDetected == 0 )
		{
			m_SmellDetected = false;
			Parent->GetComponent< CStateMachineMonster >()->SetSmellDetected( m_SmellDetected );
		}
	}
}
