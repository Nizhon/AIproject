#include "CTransformComponent.h"

#include <tgCCollision.h>
#include <tgCLine3D.h>
#include <tgCMesh.h>

#include "ECS/CEntity.h"
#include "Managers/CWorldManager.h"

CTransformComponent::CTransformComponent()
: m_pTransform(new tgCTransform)
{
	
}

CTransformComponent::~CTransformComponent()
{
	delete m_pTransform;
}

void CTransformComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent && Parent != nullptr )
	{

		//////////////////////////////////////////////////////////////////////////
		///
		///		Collision so the Entity don't fall through the world
		///
		//////////////////////////////////////////////////////////////////////////

		tgCTransform* pOwner			= Parent->GetEntityTransform();

		tgCCollision CollisionManager( true );
		CollisionManager.SetType( tgCMesh::EType::TYPE_WORLD );

		tgCLine3D CollisionLine( Parent->GetEntityPos() + tgCV3D( 0, 0.5, 0 ), tgCV3D( Parent->GetEntityPos().x, -0.5, Parent->GetEntityPos().z ) );

		tgCWorld* pWorld = CWorldManager::GetInstance().GetWorld( "Collision" );

		if (CollisionManager.LineAllMeshesInWorld( CollisionLine, *pWorld ) )
		{
			tgCV3D CollisionPoint = CollisionManager.GetLocalIntersection();

			tgCSphere CollisionPointSphere;
			CollisionPointSphere.Set( CollisionPoint, 0.2f );

			pOwner->GetMatrixLocal().Pos = CollisionPoint;
			Parent->SetEntityPos(pOwner->GetMatrixLocal().Pos);
		}
		else
		{
			pOwner->GetMatrixLocal().Pos	= Parent->GetEntityPos();
			Parent->SetEntityPos(pOwner->GetMatrixWorld().Pos);
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//		 Wall collision for the Entity
		//
		//////////////////////////////////////////////////////////////////////////

		CollisionManager.Clear();
		CollisionManager.SetType( tgCMesh::EType::TYPE_WORLD );

		tgCSphere CollisionPlayerSphere;
		CollisionPlayerSphere.Set( Parent->GetEntityPos() + tgCV3D(0, 0.5, 0 ), 0.4f );

		if (CollisionManager.SphereAllMeshesInWorld( CollisionPlayerSphere, *pWorld ) )
		{
			tgCV3D CollisionPointNormal = CollisionManager.GetLocalNormal();
			CollisionManager.GetFraction();

			tgFloat Petrude = 0.4f - ( CollisionManager.GetFraction() * 0.4f );

			pOwner->GetMatrixLocal().Pos += CollisionManager.GetLocalNormal() * Petrude;
			Parent->SetEntityPos(pOwner->GetMatrixLocal().Pos);
		}
	}
}