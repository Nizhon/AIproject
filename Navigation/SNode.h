#pragma once
#include	<tgCTriangle3D.h>
#include	<tgMemoryDisable.h>
#include	<vector>
#include	<tgMemoryEnable.h>
struct SNode
{
	bool		Checked;
	int			F_Cost;
	int			H_Cost;
	int			G_Cost;

	SNode*		ParentNode;

	std::vector< SNode* > Neighbour;

	tgCTriangle3D Triangle;

	tgCV3D MiddlePoint;

#include <tgMemoryDisable.h>
	SNode( const SNode& ) = delete;
#include <tgMemoryEnable.h>

	SNode(tgCTriangle3D triangle)
		: Triangle(triangle)
	{
		MiddlePoint = (Triangle.GetVertex(0) + Triangle.GetVertex(1) + Triangle.GetVertex(2)) / 3;
	}
};

