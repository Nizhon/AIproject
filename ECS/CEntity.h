#pragma once

#include "tgSystem.h"
#include "IComponent.h"

#include <tgCString.h>
#include <tgCTransform.h>
#include <tgTypes.h>
#include <tgMemoryDisable.h>
#include <vector>
#include <tgMemoryEnable.h>

#include "Components/CTransformComponent.h"

class CWorld;

class CEntity
{
public:


	// Constructor / Destructor
	 CEntity( void );
	~CEntity( void );

	//////////////////////////////////////////////////////////////////////////

	void	Update( tgFloat DeltaTime );

	//////////////////////////////////////////////////////////////////////////

	template < typename Type, typename... TypeArgs >
	Type* AddComponent( TypeArgs&&... rArgs)
	{
		Type* pComponent = new Type( rArgs... );

		AddComponentToList( pComponent );

		return pComponent;
	}

	template < typename Type >
	Type* GetComponent( void )
	{
		for( IComponent* pComponent : m_ComponentList )
		{
			if( &typeid( Type ) == &typeid( *pComponent ) )
				return static_cast< Type* >( pComponent );
		}

		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////

	CWorld*			GetWorld			( void )					{ return m_pWorld; }
	tgCString&		GetEntityName		( void )					{ return m_EntityName; }
	tgCTransform*	GetEntityTransform	( void )					{ return m_pParent->GetComponent<CTransformComponent>()->GetTransfrom(); }
	tgCV3D			GetEntityPos		( void )					{ return m_pParent->GetEntityTransform()->GetMatrixWorld().Pos; }
	tgCString&		GetEntityModel		( void )					{ return m_EntityName; }

	void			AddComponentToList	( IComponent* pComponent )	{ m_ComponentList.push_back( pComponent ); }

	void			SetWorld			( CWorld* World )			{ m_pWorld = World; }
	void			SetEntityName		( tgCString& rName )		{ m_EntityName = rName; }
	void			SetEntityPos		( tgCV3D Pos )				{ m_pParent->GetComponent<CTransformComponent>()->GetTransfrom()->GetMatrixWorld().Pos = Pos; }
	void			SetComponentParent	( CEntity* Parent )			{ m_pParent = Parent; }

	//////////////////////////////////////////////////////////////////////////

private:

	std::vector < IComponent* >	m_ComponentList;

	CWorld*						m_pWorld;

	CEntity*					m_pParent;

	tgCString					m_EntityName;

};