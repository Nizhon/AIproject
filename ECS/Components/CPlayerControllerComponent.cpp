#include "CPlayerControllerComponent.h"

#include <tgCCollision.h>
#include <tgCDebugManager.h>
#include <tgCLine3D.h>

#include "CApplication.h"
#include "Camera/CCamera.h"
#include "ECS/CEntity.h"
#include "Managers/CWorldManager.h"

CPlayerControllerComponent::CPlayerControllerComponent()
: m_Mouse( 0 )
, m_MoveDirection( 0 )
, m_Gravity( 0, 4, 0 )
, m_MouseSensitivity( 0.1f )
, m_MoveX( 0.0f )
, m_MoveZ( 0.0f )
, m_Sprint( 1.0f )
, m_Sneak( 1.0f )
, m_SpeedX( 0.0f )
, m_SpeedZ( 0.0f )
, m_InvertX( -1 )
{
	tgInput::AddListener( this );
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
	tgInput::RemoveListener( this );
}

void CPlayerControllerComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
	if( Parent && Parent != nullptr )
	{
		tgCDebugManager& rDebug		= tgCDebugManager::GetInstance();
		CApplication& rApplication	= CApplication::GetInstance();
		CCamera* p3DCamera			= rApplication.Get3DCamera();
		tgCMatrix& rCamera			= p3DCamera->GetCamera()->GetTransform().GetMatrixLocal();
		tgCTransform* pPlayer		= Parent->GetEntityTransform();

		pPlayer->GetMatrixLocal().Pos	= Parent->GetEntityPos();

		rCamera.RotateX( m_Mouse.x, tgCMatrix::COMBINE_REPLACE );
		rCamera.RotateY( m_Mouse.y, tgCMatrix::COMBINE_POST_MULTIPLY );

		m_Mouse.x		= tgMathClamp( 90.0f, m_Mouse.x, 5.0f );

		rCamera.Pos		= rCamera.Up * 0.75 + pPlayer->GetMatrixLocal().Pos - ( rCamera.At * 5 );
		rCamera.Pos		= pPlayer->GetMatrixLocal().Pos;
		rCamera.Pos.y	= pPlayer->GetMatrixLocal().Pos.y + 0.8;

		tgCV3D CameraAt = rCamera.At;
		CameraAt.y		= 0;

		// Rotation for the droid
		pPlayer->GetMatrixLocal().RotateY( m_Mouse.y, tgCMatrix::COMBINE_REPLACE );

		// Movement Left and Right
		pPlayer->GetMatrixLocal().Pos = Parent->GetEntityPos() + rCamera.Left * (( m_MoveX * DeltaTime ) * (m_Sprint * m_Sneak ) );
		Parent->SetEntityPos( pPlayer->GetMatrixLocal().Pos );

		// Movement Forward and Backward
		pPlayer->GetMatrixLocal().Pos = Parent->GetEntityPos() + CameraAt * (( m_MoveZ * DeltaTime ) * ( m_Sprint * m_Sneak ) );
		Parent->SetEntityPos( pPlayer->GetMatrixLocal().Pos );

		m_SpeedX = m_MoveX * m_Sprint * m_Sneak;
		m_SpeedZ = m_MoveZ * m_Sprint * m_Sneak;

		p3DCamera->Update();
	}
}

void CPlayerControllerComponent::InputEvent( const tgInput::EType Type, const tgInput::SEvent* pEvent )
{
	//////////////////////////////////////////////////////////////////////////

	( void )Type;	// Silence warning
	( void )pEvent;	// Silence warning

	//////////////////////////////////////////////////////////////////////////

	// Check for key down and ignore key repeats
	if ( ( Type == tgInput::TYPE_KEY_DOWN) && (pEvent->Keyboard.Repeat == 0 ) )
	{
		if ( pEvent->Keyboard.VKey == 'A' ) { m_MoveX = 2.5f; }
		if ( pEvent->Keyboard.VKey == 'D' ) {	m_MoveX = -2.5; }
		if ( pEvent->Keyboard.VKey == 'W' ) {	m_MoveZ = 2.5f; }
		if ( pEvent->Keyboard.VKey == 'S' ) {	m_MoveZ = -2.5f; }
		if ( pEvent->Keyboard.VKey == VK_SHIFT ) m_Sprint = 1.75f;
		if ( pEvent->Keyboard.VKey == VK_CONTROL ) m_Sneak = 0.4f;
	}

	if ( Type == tgInput::TYPE_KEY_UP )
	{
		if ( pEvent->Keyboard.VKey == 'A' ) { m_MoveX = 0.0f; }
		if ( pEvent->Keyboard.VKey == 'D' ) {	m_MoveX = 0.0f; }
		if ( pEvent->Keyboard.VKey == 'W' ) {	m_MoveZ = 0.0f; }
		if ( pEvent->Keyboard.VKey == 'S' ) {	m_MoveZ = 0.0f; }
		if ( pEvent->Keyboard.VKey == VK_SHIFT ) m_Sprint = 1.0f;
		if ( pEvent->Keyboard.VKey == VK_CONTROL ) m_Sneak = 1.0f;
		if ( pEvent->Keyboard.VKey == MOUSEEVENTF_LEFTUP );
	}

	if ( Type == tgInput::TYPE_MOUSE_MOVE_RELATIVE )
	{
		m_Mouse.x += -pEvent->CurrPos.y * m_InvertX * m_MouseSensitivity;
		m_Mouse.y += -pEvent->CurrPos.x * m_MouseSensitivity;
	}
}
