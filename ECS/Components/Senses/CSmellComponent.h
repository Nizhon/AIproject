#pragma once

#include	"ECS/IComponent.h"
#include	<tgMemoryDisable.h>
#include	<vector>
#include	<tgMemoryEnable.h>

#include	"SSmell.h"

typedef std::vector< SSmell* > SmellList;

class CSmellComponent : public IComponent
{
public:

	// Constructor / Destructor
	 CSmellComponent( tgFloat Timer );
	~CSmellComponent();

	//////////////////////////////////////////////////////////////////////////

	void	DrawDebug	( void );
	void	Update		( tgFloat DeltaTime, CEntity* Parent ) override;

	//////////////////////////////////////////////////////////////////////////

	SmellList GetSmellList() { return m_SmellList; }

	//////////////////////////////////////////////////////////////////////////

private:

	tgFloat		m_SmellIndex;
	tgFloat		m_Timer;
	tgFloat		m_Cooldown;

	tgBool		m_SmellDetected;

	SmellList	m_SmellList;
};

