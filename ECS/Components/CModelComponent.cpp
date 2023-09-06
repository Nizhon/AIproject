#include "CModelComponent.h"
#include "Managers/CModelManager.h"
#include "ECS/CEntity.h"

CModelComponent::CModelComponent( const char* pFilePath, const tgCString& rName )
: m_pModel( nullptr )
{
	m_pModel = CModelManager::GetInstance().LoadModel( pFilePath, rName, true );
}

CModelComponent::~CModelComponent( void )
{
	CModelManager::GetInstance().DestroyModel( m_pModel );
}

void CModelComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent && Parent != nullptr )
	{
		m_pModel->GetTransform().GetMatrixLocal() = Parent->GetEntityTransform()->GetMatrixWorld();
		m_pModel->Update();
	}
}

