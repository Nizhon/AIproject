#include "CHealthComponent.h"
#include "ECS/CEntity.h"
#include "ECS/CWorld.h"

CHealthComponent::CHealthComponent( tgFloat Health )
: m_MaxHealth( Health )
, m_CurrentHealth( m_MaxHealth )
{
}

void CHealthComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent && Parent != nullptr )
	{
		if( m_CurrentHealth <= 0 )
		{
			Parent->GetWorld()->DestroyEntity( Parent->GetEntityName() );
		}
	}
}

void CHealthComponent::TakeDamage( tgFloat Damage )
{
	m_CurrentHealth -= Damage;
}