#pragma once

#include "ECS/IComponent.h"
#include "tgCV3D.h"

#include <tgCInputListener.h>

class CPlayerControllerComponent : public IComponent, public tgCInputListener
{
public:

	// Constructor / Destructor
	 CPlayerControllerComponent();
	~CPlayerControllerComponent();

	//////////////////////////////////////////////////////////////////////////

	void Update( tgFloat DeltaTime, CEntity* Parent ) override;

	//////////////////////////////////////////////////////////////////////////

	void InputEvent( const tgInput::EType Type, const tgInput::SEvent* pEvent );

	//////////////////////////////////////////////////////////////////////////

	tgFloat GetSpeedX () { return m_SpeedX; }
	tgFloat GetSpeedY () { return m_SpeedZ; }

private:

	tgCV3D							m_Mouse;
	tgCV3D							m_MoveDirection;
	tgCV3D							m_Gravity;

	tgFloat							m_MouseSensitivity;
	tgFloat							m_MoveX;
	tgFloat							m_MoveZ;
	tgFloat							m_Sprint;
	tgFloat							m_Sneak;
	tgFloat							m_SpeedX;
	tgFloat							m_SpeedZ;

	tgSInt16						m_InvertX;
};

