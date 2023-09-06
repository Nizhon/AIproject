
#include "CWorld.h"
#include "tgLog.h"
#include "CEntity.h"
#include "Managers/CWorldManager.h"
#include "tgCDebugManager.h"
#include "AIStateMachine/CStateMachineMonster.h"
#include "Components/CHealthComponent.h"
#include "Components/CModelComponent.h"
#include "Components/CNoseComponent.h"
#include "Components/CPathfindingComponent.h"
#include "Components/CPlayerControllerComponent.h"
#include "Components/CShooterComponent.h"
#include "Components/Senses/CHearingComponent.h"
#include "Components/Senses/CSmellComponent.h"
#include "Navigation/CNavMesh.h"

CWorld::CWorld( void )
: m_MonsterIndex( 0 )
, m_Score( 0 )
, m_ScoreThreshold( 10 )
{
	CWorldManager& rWorldManager	= CWorldManager::GetInstance();
	m_pCollisionWorld				= rWorldManager.LoadWorld( "worlds/CollisionForestFixedMaybe.tfw",	"Collision" );
	m_pNavWorld						= rWorldManager.LoadWorld( "worlds/NavForest.tfw",		"Navigation" );
	
	rWorldManager.SetActiveWorld( m_pCollisionWorld );

	CEntity* pPlayer = CreateEntity( "Player" );
	pPlayer->AddComponent< CTransformComponent >();
	pPlayer->SetEntityPos( tgCV3D( 5,0.3,-20 ) );
	pPlayer->AddComponent< CSmellComponent >( 0.5f );
	pPlayer->AddComponent< CPlayerControllerComponent >();
	pPlayer->AddComponent< CHealthComponent >( 100.0f );

	m_MonsterIndex = 1;

	for ( int i = 0; i < m_MonsterIndex; i++ )
	{
		tgCString Monster = "Monster";

		CEntity* pMonster = CreateEntity( Monster + tgCString("%i", i) );
		pMonster->AddComponent< CTransformComponent >();
		// pMonster->AddComponent< CModelComponent >( "models/droid_big.tfm", pMonster->GetEntityName() );
		pMonster->SetEntityPos(  tgCV3D( 3, 0.3, 26 ) );
		pMonster->AddComponent< CShooterComponent >( 2, 10 );
		pMonster->AddComponent< CNoseComponent >();
		pMonster->AddComponent< CHearingComponent >();
		pMonster->AddComponent< CStateMachineMonster >();
		pMonster->AddComponent< CPathfindingComponent >();
	}

	m_pNavMesh = new CNavMesh( "Navigation" );
}

CWorld::~CWorld( void )
{
	while(!m_EntityList.empty())
	{
		tgCString FirstEntity = m_EntityList[0]->GetEntityName();
		DestroyEntity( FirstEntity );
	}

	delete m_pNavMesh;
	m_pNavMesh = nullptr;

	CWorldManager::GetInstance().DestroyAllWorlds();
}

void CWorld::DrawDebug()
{
	tgCDebugManager& rDebug = tgCDebugManager::GetInstance();

	rDebug.AddText2D( tgCV2D( 5.0f, 50.0f ), tgCColor::White, "" );

	rDebug.AddText2D( tgCColor::White, tgCString("%f", m_Score )  );
	for ( int i = 0; i < m_EntityList.size(); i++ )
	{
		tgCSphere EntitySphere;
		EntitySphere.Set( m_EntityList[i]->GetEntityPos(), 0.3f );
		if( m_EntityList[i]->GetEntityName() == "Player" )
			rDebug.AddLineSphere( EntitySphere, tgCColor::Lime );
		else if( m_EntityList[i]->GetEntityName() == "Bullet" )
			rDebug.AddLineSphere( EntitySphere, tgCColor::Black );
		else
			rDebug.AddLineSphere( EntitySphere, tgCColor::Aqua );
	}
}

void CWorld::Update( tgFloat DeltaTime )
{
	DrawDebug();

	if( GetEntity( "Player" ) )
	{
		m_Score += DeltaTime;
	}

	if( m_Score > m_ScoreThreshold )
	{
		tgFloat Temp = m_MonsterIndex;
		m_MonsterIndex += 0;

		for ( int i = Temp; i < m_MonsterIndex; i++ )
		{
			tgCString Monster = "Monster";
			CEntity* pMonster = CreateEntity( Monster + tgCString("%i", i) );
			pMonster->AddComponent< CTransformComponent >();
			// pMonster->AddComponent< CModelComponent >( "models/droid_big.tfm", pMonster->GetEntityName() );
			pMonster->SetEntityPos(  tgCV3D( 3, 0.3, 26 ) );
			pMonster->AddComponent< CShooterComponent >( 2, 10 );
			pMonster->AddComponent< CNoseComponent >();
			pMonster->AddComponent< CHearingComponent >();
			pMonster->AddComponent< CStateMachineMonster >();
			pMonster->AddComponent< CPathfindingComponent >();
		}
		m_ScoreThreshold += 10;
	}

	for ( int i = 0; i < m_EntityList.size(); i++ )
	{
		m_EntityList[i]->Update( DeltaTime );
	}
}

void CWorld::DestroyEntity( tgCString Name )
{
	for ( int i = 0; i < m_EntityList.size(); i++ )
	{
		if( m_EntityList[i]->GetEntityName() == Name )
		{
			delete m_EntityList[i];
			m_EntityList.erase( m_EntityList.begin() + i );
		}
	}
}

CEntity* CWorld::CreateEntity( tgCString Name )
{
	CEntity* Entity = new CEntity;
	Entity->SetWorld( this );
	Entity->SetEntityName( Name );
	Entity->SetComponentParent( Entity );
	m_EntityList.push_back( Entity );
	return Entity;
}

CEntity* CWorld::GetEntity( tgCString Name )
{
	for( CEntity* Entity : m_EntityList )
	{
		if( Entity->GetEntityName() == Name )
		{
			return Entity;
		}
	}
	return nullptr;
}
