#pragma once
#include <tgSystem.h>

#include "SNode.h"
#include "ECS/CEntity.h"

#include <tgMemoryDisable.h>
#include <vector>
#include <tgMemoryEnable.h>
#include <tgCTriangle3D.h>
#include <tgCWorld.h>

typedef std::vector< SNode* > SNodeList;

class CNavMesh
{
public:

	 CNavMesh( tgCString WorldName );
	~CNavMesh( void );

	//////////////////////////////////////////////////////////////////////////

	SNodeList	GetNodeList	( void )	{ return m_Nodes; }

	//////////////////////////////////////////////////////////////////////////

private:

	tgCWorld*			m_pWorldMap;

	const tgCMesh::SVertex*		m_pVertex_array;

	const tgUInt32*			m_pIndex_array;

	int				m_TriangleAmount;

	std::vector < tgCTriangle3D >	m_Triangles;

	std::vector < SNode* >		m_Nodes;

};

