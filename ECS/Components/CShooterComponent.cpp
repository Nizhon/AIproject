#include "CShooterComponent.h"

#include "CArrowComponent.h"
#include "CTransformComponent.h"
#include "ECS/CWorld.h"
#include "ECS/AIStateMachine/CStateMachineMonster.h"

CShooterComponent::CShooterComponent( tgFloat AttackSpeed, tgFloat AttackRange )
: m_Direction( 0 )
, m_Distance( 0 )
, m_AttackSpeed( AttackSpeed )
, m_AttackRange( AttackRange )
, m_Cooldown( 0 )
{
}

CShooterComponent::~CShooterComponent()
{
}

void CShooterComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	CEntity* pPlayer = Parent->GetWorld()->GetEntity( "Player" );

	if( pPlayer && pPlayer != nullptr )
	{
		if( Parent->GetComponent< CStateMachineMonster >()->InAttackRange() )
		{
			m_Direction	= pPlayer->GetEntityPos() - Parent->GetEntityPos();
			m_Distance	= m_Direction.Normalize();

			if( m_Distance <= m_AttackRange )
			{
				if( m_Cooldown <= 0 )
				{
					CEntity* pBullet = Parent->GetWorld()->CreateEntity( "Bullet" );
					pBullet->AddComponent<CTransformComponent>();
					pBullet->SetEntityPos(Parent->GetEntityPos() + m_Direction );
					tgCV3D PlayerPos = Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos() + tgCV3D( 0,0.6,0 );
					tgBool Alive = true;
					pBullet->AddComponent<CArrowComponent>( m_Direction, PlayerPos, 14.0f, Alive );
				
					m_Cooldown = m_AttackSpeed;
				}
			}
			m_Cooldown -= DeltaTime;
		}
	}
}
