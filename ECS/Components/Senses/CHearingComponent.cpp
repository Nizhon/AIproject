#include "CHearingComponent.h"

#include "ECS/CEntity.h"
#include "ECS/CWorld.h"
#include "tgCDebugManager.h"
#include <tgCLine3D.h>

#include "ECS/Components/CPlayerControllerComponent.h"

CHearingComponent::CHearingComponent()
: m_HeardNoise ( false )
{
}

CHearingComponent::~CHearingComponent()
{
}

void CHearingComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent->GetWorld()->GetEntity("Player") )
	{
		// tgCDebugManager& rDebug = tgCDebugManager::GetInstance();
		tgCV3D PlayerPos		= Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos();
		tgCV3D OwnerPos			= Parent->GetEntityPos();
		tgFloat Distance		= ( PlayerPos - OwnerPos ).Length();
		tgCV3D HalfPoint		= ( PlayerPos + OwnerPos ) / 2;

		/*tgCLine3D DistanceLine( OwnerPos, PlayerPos );

		rDebug.AddLine3D( DistanceLine, tgCColor::Red );

		tgCString T( "%d", DistanceLine );

		rDebug.AddText3D( HalfPoint, tgCColor::White, tgCString( "%f", Distance ) );*/

		tgFloat PlayerSpeedX = Parent->GetWorld()->GetEntity( "Player" )->GetComponent<CPlayerControllerComponent>()->GetSpeedX();
		tgFloat PlayerSpeedZ = Parent->GetWorld()->GetEntity( "Player" )->GetComponent<CPlayerControllerComponent>()->GetSpeedY();

		if( Distance < 14 && PlayerSpeedX >= 4.375f || Distance < 14 && PlayerSpeedX <= -4.375f || Distance < 14 && PlayerSpeedZ >= 4.375f || Distance < 14 && PlayerSpeedZ <= -4.375f )
		{
			m_HeardNoise = true;
		}
		else if( Distance < 8 && PlayerSpeedX >= 1.75f || Distance < 8 && PlayerSpeedX <= -1.75f || Distance < 8 && PlayerSpeedZ >= 1.75f || Distance < 8 && PlayerSpeedZ <= -1.75f )
		{
			m_HeardNoise = true;
		}
		else if( Distance < 5 && PlayerSpeedX >= 1.0 || Distance < 2 && PlayerSpeedX <= -1.0f || Distance < 5 && PlayerSpeedZ >= 1.0f || Distance < 5 && PlayerSpeedZ <= -1.0f )
		{
			m_HeardNoise = true;
		}
		else
		{
			m_HeardNoise = false;
		}

		// rDebug.AddText2D( m_HeardNoise ? tgCColor::Green : tgCColor::Red, m_HeardNoise ? "Have Heard Noise" : "Haven't Heard Noise");
	}
}
