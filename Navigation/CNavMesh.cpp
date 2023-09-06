#include "CNavMesh.h"

#include <tgCDebugManager.h>
#include <tgCString.h>
#include "ECS/CWorld.h"
#include "Managers/CWorldManager.h"

CNavMesh::CNavMesh( tgCString WorldName )
{
	CWorldManager& rWorld = CWorldManager::GetInstance();
	m_pWorldMap = rWorld.GetWorld( WorldName );
	
	// We get the amount of sectors the map hold
	int SectorAmount = m_pWorldMap->GetNumSectors();

	// We loop through the Sectors
	for ( size_t SectorIndex = 0; SectorIndex < SectorAmount; SectorIndex++ )
	{
		// We get the amount of meshes there are in every sector
		int MeshAmount = m_pWorldMap->GetSector( SectorIndex )->NumMeshes;

		// We loop through the meshs for each sector
		for ( size_t MeshIndex = 0; MeshIndex < MeshAmount; MeshIndex++ )
		{
			// We now check every mesh and get every position in the vertex array and index array.
			m_pVertex_array = m_pWorldMap->GetSector( SectorIndex )->pMeshArray[ MeshIndex ].GetVertexArray();
			m_pIndex_array	= m_pWorldMap->GetSector( SectorIndex )->pMeshArray[ MeshIndex ].GetIndexArray();

			// We now get the amount of triangles there are in every mesh
			m_TriangleAmount = m_pWorldMap->GetSector( SectorIndex )->pMeshArray[MeshIndex].GetNumTotalTriangles();

			// We loop through the amounts of triangles and get their points/edges, we then combine them into a triangle
			for ( size_t VertixIndex = 0; VertixIndex < m_TriangleAmount * 3; VertixIndex )
			{
				tgCV3D TrianglePointA = m_pVertex_array[ m_pIndex_array[ VertixIndex++ ] ].Position;
				tgCV3D TrianglePointB = m_pVertex_array[ m_pIndex_array[ VertixIndex++ ] ].Position;
				tgCV3D TrianglePointC = m_pVertex_array[ m_pIndex_array[ VertixIndex++ ] ].Position;

				tgCTriangle3D Triangle;

				Triangle.Set( TrianglePointA, TrianglePointB, TrianglePointC );

				// We create a SNode and add the Triangle to it.
				m_Nodes.emplace_back( new SNode( Triangle ) );
			}
		}
	}

	// Neighbour check
	for ( int i = 0; i < m_Nodes.size(); i++ )  
	{
		for ( int j = 0; j < m_Nodes.size(); j++ )
		{
			tgCTriangle3D NodeI = m_Nodes[i]->Triangle;
			tgCTriangle3D NodeJ = m_Nodes[j]->Triangle;

			if ( i == j )
				continue;

			int found = 0;

			for ( int k = 0; k < 3; k++ )
			{
				if ( NodeI.GetVertex( k ) == NodeJ.GetVertex( 0 ) || NodeI.GetVertex( k ) == NodeJ.GetVertex( 1 ) || NodeI.GetVertex( k ) == NodeJ.GetVertex( 2 ) )
					found++;
				
			}
			
			if ( found == 2 )
				m_Nodes[i]->Neighbour.push_back( m_Nodes[j] );
		}
	}
}

CNavMesh::~CNavMesh( void )
{
	std::vector< SNode* >::iterator Nodes = m_Nodes.begin();

	while (!m_Nodes.empty())
	{
		delete* Nodes;
		m_Nodes.erase(Nodes);
		Nodes = m_Nodes.begin();
	}

	m_Nodes.clear();
}