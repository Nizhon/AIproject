#include "CPathfindingComponent.h"

#include <tgMath.h>

#include "ECS/CWorld.h"

CPathfindingComponent::CPathfindingComponent()
: m_pStart( nullptr )
, m_pGoal( nullptr )
, m_Result()
{
}

CPathfindingComponent::~CPathfindingComponent()
{
}

void CPathfindingComponent::Update( tgFloat DeltaTime, CEntity* Parent )
{
}

Path* CPathfindingComponent::GetPath( SNode* Start, SNode* Goal )
{
	SNode* Current = Start;

	Current->G_Cost = tgMathAbs(Start->MiddlePoint.x - Current->MiddlePoint.x) + tgMathAbs(Start->MiddlePoint.z - Current->MiddlePoint.z);
	Current->H_Cost = tgMathAbs(Goal->MiddlePoint.x - Current->MiddlePoint.x) + tgMathAbs(Goal->MiddlePoint.z - Current->MiddlePoint.z);
	Current->F_Cost = Current->G_Cost + Current->H_Cost;
	Current->Checked = false;
	Current->ParentNode = nullptr;

	m_OpenList.clear();
	m_ClosedList.clear();

	// Adding the first element to the list
	m_OpenList.push_back( Current );

	while ( !m_OpenList.empty() )
	{
		Current->Checked = true;

		// We set pCurrent to the first value in the open list
		Current = m_OpenList[0];

		// current = node in OPEN with the lowest f_cost
		for ( int i = 0; i < m_OpenList.size(); i++ )
		{
			if ( Current->F_Cost > m_OpenList[i]->F_Cost )
			{
				SNode* Temp = Current;
				Current = m_OpenList[i];

				if( Temp->H_Cost <= Current->H_Cost  )
				{
					Current = Temp;
				}
			}
		}

		// add current to CLOSED
		m_ClosedList.push_back( Current );

		// remove current from OPEN
		for ( int i = 0; i < m_OpenList.size(); i++ )
		{
			if ( m_OpenList[i] == Current )
				m_OpenList.erase( m_OpenList.begin() + i );
		}

		// if current is the target node, return path has been found
		if ( Current->MiddlePoint == Goal->MiddlePoint )
		{
			while ( Current->ParentNode != nullptr )
			{
				m_Path.push_back( Current );
				Current = Current->ParentNode;
			}

			for ( int i = 0; i < m_Path.size(); i++ )
			{
				if ( m_Path[i] == Goal )
					m_Path.erase( m_Path.begin() + i );
			}

			// We recreate the m_path into a tgCV3D Vector that is more straight to the point.
			for ( int i = m_Path.size(); i > 0; i-- )
			{
				if( i - 3 > 0 )
				{
					tgCV3D WalkPoint = tgCV3D( ( m_Path[i]->MiddlePoint + m_Path[i-1]->MiddlePoint + m_Path[i-2]->MiddlePoint + m_Path[i-3]->MiddlePoint ) /4 );
					m_WalkPath.push_back( WalkPoint );
					i -= 3;
				}
				else if( i - 2 > 0 )
				{
					tgCV3D WalkPoint = tgCV3D( ( m_Path[i]->MiddlePoint + m_Path[i-1]->MiddlePoint ) /2 );

					m_WalkPath.push_back( WalkPoint );

					i--;
				}
				else
				{
					m_WalkPath.push_back( m_Path[i]->MiddlePoint );
				}
			}

			m_Result = PATH_FOUND;

			return nullptr;
		}

		for ( int i = 0; i < Current->Neighbour.size(); i++ )
		{
			Current->Neighbour[i]->G_Cost = 
				tgMathAbs( Current->MiddlePoint.x - Current->Neighbour[i]->MiddlePoint.x )
				+ tgMathAbs( Current->MiddlePoint.z - Current->Neighbour[i]->MiddlePoint.z )
				+ tgMathAbs( Current->G_Cost );

			Current->Neighbour[i]->H_Cost = 
				tgMathAbs( Goal->MiddlePoint.x - Current->Neighbour[i]->MiddlePoint.x )
				+ tgMathAbs( Goal->MiddlePoint.z - Current->Neighbour[i]->MiddlePoint.z );

			Current->Neighbour[i]->F_Cost = 
				Current->Neighbour[i]->G_Cost + Current->Neighbour[i]->H_Cost;

			
			if ( Current->Neighbour[i]->Checked == false )
			{
				Current->Neighbour[i]->ParentNode = Current;
				Current->Neighbour[i]->Checked = true;
				m_OpenList.push_back( Current->Neighbour[i] );
			}
		}
	}

	m_Result = PATH_NOT_FOUND;

	return nullptr;
}

std::vector< tgCV3D > CPathfindingComponent::SearhForPlayer( CEntity* Parent )
{
	std::vector < SNode* > Nodes = Parent->GetWorld()->GetNodeList();
	int Max = Nodes.size();

	m_WalkPath.clear();
	m_Path.clear();

	int Start = 0;

	for ( int i = 0; i < 2; i++ )
	{
		int randNum = std::rand() % Max;

		if ( i == 0 )
		{
			m_pStart = Nodes[ randNum ];

			for ( int j = 0; j < Nodes.size(); j++ )
			{
				tgFloat Distance = ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - Nodes[j]->MiddlePoint ).Length();

				if ( Distance < ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - m_pStart->MiddlePoint ).Length() )
				{
					m_pStart = Nodes[j];
					Start = j;
				}
			}
			m_WalkPath.push_back( Nodes[ Start ]->MiddlePoint );
			m_StartGoal.push_back( Nodes[ Start ] );
		}
		else
		{
			m_pGoal = Nodes[ randNum ];
			m_StartGoal.push_back( Nodes[ randNum ] );
		}
	}

	for ( int i = 0; i < Nodes.size(); i++ )
	{
		for ( int j = 0; j < Nodes[i]->Neighbour.size(); j++ )
		{
			Nodes[i]->Neighbour[j]->Checked = false;
		}
	}

	if ( m_pStart != nullptr && m_pGoal != nullptr )
	{
		GetPath( m_pStart, m_pGoal );
	}

	return m_WalkPath;
}

std::vector< tgCV3D > CPathfindingComponent::FollowSound( CEntity* Parent )
{
	std::vector < SNode* > Nodes = Parent->GetWorld()->GetNodeList();

	m_WalkPath.clear();
	m_Path.clear();

	int StartGoal = 0;

	for ( int i = 0; i < 2; i++ )
	{
		if ( i == 0 )
		{
			m_pStart = Nodes[ 0 ];

			for ( int j = 0; j < Nodes.size(); j++ )
			{
				tgFloat Distance = ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - Nodes[j]->MiddlePoint ).Length();

				if ( Distance < ( Parent->GetEntityTransform()->GetMatrixWorld().Pos - m_pStart->MiddlePoint ).Length() )
				{
					m_pStart = Nodes[j];
					StartGoal = j;
				}
			}
			m_WalkPath.push_back( Nodes[ StartGoal ]->MiddlePoint );
			m_StartGoal.push_back( Nodes[ StartGoal ] );
		}
		else
		{
			m_pGoal = Nodes[ 0 ];

			for ( int j = 0; j < Nodes.size(); j++ )
			{
				tgFloat Distance = ( Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos() - Nodes[j]->MiddlePoint ).Length();

				if ( Distance < ( Parent->GetWorld()->GetEntity( "Player" )->GetEntityPos() - m_pGoal->MiddlePoint ).Length() )
				{
					m_pGoal = Nodes[j];
					StartGoal = j;
				}
			}
			m_StartGoal.push_back( Nodes[ StartGoal ] );
		}
	}

	for ( int i = 0; i < Nodes.size(); i++ )
	{
		for ( int j = 0; j < Nodes[i]->Neighbour.size(); j++ )
		{
			Nodes[i]->Neighbour[j]->Checked = false;
		}
	}

	if ( m_pStart != nullptr && m_pGoal != nullptr )
	{
		GetPath( m_pStart, m_pGoal );
	}

	return m_WalkPath;
}