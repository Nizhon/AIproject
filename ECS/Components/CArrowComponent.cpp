#include "CArrowComponent.h"

#include <tgCV3D.h>
#include "ECS/CWorld.h"
#include "CHealthComponent.h"

CArrowComponent::CArrowComponent(tgCV3D ArrowPos, tgCV3D PlayerPos, tgFloat ProjectileSpeed, tgBool Alive)
: m_BulletPos( ArrowPos )
, m_PlayerPos( PlayerPos )
, m_Direction( ArrowPos )
, m_BulletSpeed( ProjectileSpeed )
, m_AliveTime( 0 )
, m_BulletAlive( Alive )
{
}

CArrowComponent::~CArrowComponent()
{
}

void CArrowComponent::Update(tgFloat DeltaTime, CEntity* Parent)
{
	if( Parent->GetWorld()->GetEntity("Player") )
	{
		tgCTransform* pArrow		= Parent->GetComponent<CTransformComponent>()->GetTransfrom();
		CEntity* pPlayer		= Parent->GetWorld()->GetEntity( "Player" );
		CHealthComponent* pHealth	= pPlayer->GetComponent<CHealthComponent>();

		if( m_BulletAlive == true )
			m_AliveTime += DeltaTime;

		pArrow->GetMatrixLocal().Pos = Parent->GetEntityPos() + m_Direction * (m_BulletSpeed * DeltaTime );
		Parent->SetEntityPos( pArrow->GetMatrixLocal().Pos );

		tgFloat Distance = ( pPlayer->GetEntityPos() - pArrow->GetMatrixLocal().Pos ).Length();

		if( Parent->GetComponent<CArrowComponent>()->m_AliveTime >= 3.0f )
		{
			Parent->GetWorld()->DestroyEntity( Parent->GetEntityName() );
		}

		if( Distance < 0.3f && Parent->GetWorld()->GetEntity( "Player" )->GetComponent<CHealthComponent>() )
		{
			Parent->GetWorld()->DestroyEntity( Parent->GetEntityName() );
			pHealth->TakeDamage( 20.0f );
		}
	}
}
