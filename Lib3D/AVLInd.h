// AVL.h: Template of AVL class.
// By Gaspard Breton
// January, 2000
//
//////////////////////////////////////////////////////////////////////
//
// AVL = Adelson-Velskii & Landis (russian mathematicians), 1960
//
// AVL trees are well balanced trees that ensure the depth is ALWAYS log2(n).
// Operations like insertion or deletion use rotations for rebalancing the tree.
//
// Implementation :
// Each class having a sortable value can be handled by an AVL.
// The AVLStuct_t must be added in the class and the register method called
// before use.
// Data structures don't have to be changed, and moreover, the same
// element can be handled by several AVL trees.
// The CAVL class is a template that needs the class it's working on and the type
// of value for declaration. For example : CAVL<CElement,float>.
// This implementation doesn't do any dynamic memory allocation and only works 
// with indices in an array (because of dynamic reallocations).
// That means that each sorted value must belong to a UNIQUE element.
// This implementation, because it works with indices, supports insertion of 
// identical values.
// Although methods seem to be big, they execute very quickly because they are, 
// in fact, made of many cases.
//
// Methods :
//	Insert (insertion)
//	Delete (deletion)
//  GetFirst et GetNext (for loops)
//  GetMin an GetMax (minimum and maximum)
//
// EXAMPLE BELOW ///////////////////////////////////////////////////////
// 
// #include "AVL.h"
// 
// class CElement
// {
// public:
// 	int SomeStuffHere;
// 	float Val;
// 	int SomeStuffThere;
// 	AVLStruct_t AVLStruct; 
// 	float DoneWithStuff;
// };
// 
// int main(int argc, char* argv[])
// {
// 	CElement ElementArr[5];
//	int Tmp;
// 	CAVL<CElement,float> MyAVL;
//  register int i;
//
// 	MyAVL.Register(&ElementArr[0],&ElementArr[0].Val,&ElementArr[0].AVLStruct);
//
// 	ElementArr[0].Val=42.0f;
// 	ElementArr[1].Val=20.0f;
// 	ElementArr[2].Val=17.0f;
// 	ElementArr[3].Val=34.0f;
// 	ElementArr[4].Val=23.0f;
//
//	for (i=0;i<sizeof(ElementArr)/sizeof(ElementArr[0]);i++
//		MyAVL.Insert(Array,i);
//
//	for (Tmp=MyAVL.GetFirst(Array);AVLNULL!=Tmp;Tmp=MyAVL.GetNext(Array))
// 		printf("Min %6.2f\n",ElementArr[Tmp].Val);
//
// 	MyAVL.Delete(Array,3);
//
// 	while (AVLNULL!=(Tmp=MyAVL.GetMin(Array)))
// 	{
// 		printf("Min %6.2f\n",ElementArr[Tmp].Val);
// 		MyAVL.Delete(Array,Tmp);
// 	}
// }
//
////////////////////////////////////////////////////////////////////////

//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVL_H__5DFE6574_AECE_11D3_8576_00600834A808__INCLUDED_)
#define AFX_AVL_H__5DFE6574_AECE_11D3_8576_00600834A808__INCLUDED_

#include "windows.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#define AVLNULL	-1

//////////////////////////////////////////////////////////////////////

#define AVLUNEXPLORED	0
#define AVLOUT	 		1
#define AVLDONE			2

//////////////////////////////////////////////////////////////////////

#define AVLLEFT		0
#define AVLRIGHT	1

//////////////////////////////////////////////////////////////////////

#define GETVAL(Ind) (*((Val_t *)((char *)&Array[Ind]+m_ValOfs)))
#define GETAVLS(Ind) ((AVLStruct_t *)((char *)&Array[Ind]+m_AVLStructOfs))

//////////////////////////////////////////////////////////////////////

typedef struct AVLStruct_t
{
	struct 
	{
		char Balance:4;
		char Flag:4;
	};
	int Parent;
	int LeftChild;
	int RightChild;
} AVLStruct_t;

//////////////////////////////////////////////////////////////////////

template <class Node_t,class Val_t>
class CAVL  
{
private:
	int m_Root;
	int m_CurNode;
	AVLStruct_t *m_CurNodeStruct;
	int m_ValOfs;
	int m_AVLStructOfs;

public:

CAVL()
{
}

~CAVL()
{
}

void Register(Node_t *Node,Val_t *Key,AVLStruct_t *AVLStruct)
{
	// compute address displacement to reach key in node
	m_ValOfs=(char *)Key-(char *)Node;

	// compute address displacement to reach struct in node
	m_AVLStructOfs=(char *)AVLStruct-(char *)Node;

	// tree is empty for the moment
	m_Root=AVLNULL;
}

void Purge()
{
	// empty tree
	m_Root=AVLNULL;
}

void Insert(Node_t *Array,int Node)
{
	// launch recursive insertion
	Insert(Array,&m_Root,AVLNULL,Node);
}

void Delete(Node_t *Array,int Node)
{
	int AuxNode;
	int *PopupNode;
	AVLStruct_t *NodeStruct=GETAVLS(Node);
	AVLStruct_t *AuxNodeStruct;
	AVLStruct_t *PopupNodeStruct;
	BOOL DepthDecrease;
	int Side;

	// abort if empty tree
	if (AVLNULL==m_Root)
		return;

	// if node has two children
	if (AVLNULL!=NodeStruct->LeftChild && AVLNULL!=NodeStruct->RightChild)
	{
		// find the replace node, the node having the very next value
		for (AuxNode=NodeStruct->RightChild;AVLNULL!=GETAVLS(AuxNode)->LeftChild;AuxNode=GETAVLS(AuxNode)->LeftChild)
			continue;

		// get replace node structure
		AuxNodeStruct=GETAVLS(AuxNode);

		// update balance
		AuxNodeStruct->Balance=NodeStruct->Balance;

		// update left child
		AuxNodeStruct->LeftChild=NodeStruct->LeftChild;
		GETAVLS(NodeStruct->LeftChild)->Parent=AuxNode;

		// if replace node is right child
		if (GETAVLS(AuxNodeStruct->Parent)->RightChild==AuxNode)
		{
			// get address of node for poping up and update parent
			AuxNodeStruct->Parent=NodeStruct->Parent;
			if (AVLNULL==NodeStruct->Parent)
			{
				PopupNode=&m_Root;
				m_Root=AuxNode;
			}
			else
			{
				if (GETAVLS(NodeStruct->Parent)->LeftChild==Node)
				{
					PopupNode=&GETAVLS(NodeStruct->Parent)->LeftChild;
					GETAVLS(NodeStruct->Parent)->LeftChild=AuxNode;
				}
				else
				{
					PopupNode=&GETAVLS(NodeStruct->Parent)->RightChild;
					GETAVLS(NodeStruct->Parent)->RightChild=AuxNode;
				}
			}

			// coming from right
			Side=AVLRIGHT;
		}

		// otherwise replace node is a left child lower in the tree
		else
		{
			// update child of replace node
			GETAVLS(AuxNodeStruct->Parent)->LeftChild=AuxNodeStruct->RightChild;
			if (AVLNULL!=AuxNodeStruct->RightChild)
				GETAVLS(AuxNodeStruct->RightChild)->Parent=AuxNodeStruct->Parent;

			// update right child
			AuxNodeStruct->RightChild=NodeStruct->RightChild;
			GETAVLS(NodeStruct->RightChild)->Parent=AuxNode;

			// get address of node for poping up
			if (AVLNULL==GETAVLS(AuxNodeStruct->Parent)->Parent)
				PopupNode=&m_Root;
			else
			{
				if (GETAVLS(GETAVLS(AuxNodeStruct->Parent)->Parent)->LeftChild==(AuxNodeStruct->Parent))
					PopupNode=&GETAVLS(GETAVLS(AuxNodeStruct->Parent)->Parent)->LeftChild;
				else
					PopupNode=&GETAVLS(GETAVLS(AuxNodeStruct->Parent)->Parent)->RightChild;
			}

			// update parent
			AuxNodeStruct->Parent=NodeStruct->Parent;
			if (AVLNULL==NodeStruct->Parent)
				m_Root=AuxNode;
			else
			{
				if (GETAVLS(NodeStruct->Parent)->LeftChild==Node)
					GETAVLS(NodeStruct->Parent)->LeftChild=AuxNode;
				else
					GETAVLS(NodeStruct->Parent)->RightChild=AuxNode;
			}

			// coming from left
			Side=AVLLEFT;
		}
	}

	// if node has zero or one child only
	else
	{
		// node has one left child
		if (AVLNULL!=NodeStruct->LeftChild)
		{
			// set root to left child, if node is root
			if (AVLNULL==NodeStruct->Parent)
			{
				m_Root=NodeStruct->LeftChild;
				GETAVLS(NodeStruct->LeftChild)->Parent=AVLNULL;
				PopupNode=NULL;
			}

			// otherwise not root
			else
			{
				// set parent child to left child
				if (GETAVLS(NodeStruct->Parent)->LeftChild==Node)
				{
					GETAVLS(NodeStruct->Parent)->LeftChild=NodeStruct->LeftChild;
					Side=AVLLEFT;
				}
				else
				{
					GETAVLS(NodeStruct->Parent)->RightChild=NodeStruct->LeftChild;
					Side=AVLRIGHT;
				}
				GETAVLS(NodeStruct->LeftChild)->Parent=NodeStruct->Parent;

				// get address of node for poping up
				if (AVLNULL==GETAVLS(NodeStruct->Parent)->Parent)
					PopupNode=&m_Root;
				else
				{
					if (GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild==NodeStruct->Parent)
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild;
					else
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->RightChild;
				}
			}
		}

		// node has one right child
		else if (AVLNULL!=NodeStruct->RightChild)
		{
			// set root to right child, if node is root
			if (AVLNULL==NodeStruct->Parent)
			{
				m_Root=NodeStruct->RightChild;
				GETAVLS(NodeStruct->RightChild)->Parent=AVLNULL;
				PopupNode=NULL;
			}

			// otherwise not root
			else
			{
				// set parent child to right child
				if (GETAVLS(NodeStruct->Parent)->LeftChild==Node)
				{
					GETAVLS(NodeStruct->Parent)->LeftChild=NodeStruct->RightChild;
					Side=AVLLEFT;
				}
				else
				{
					GETAVLS(NodeStruct->Parent)->RightChild=NodeStruct->RightChild;
					Side=AVLRIGHT;
				}
				GETAVLS(NodeStruct->RightChild)->Parent=NodeStruct->Parent;

				// get address of node for poping up
				if (AVLNULL==GETAVLS(NodeStruct->Parent)->Parent)
					PopupNode=&m_Root;
				else
				{
					if (GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild==NodeStruct->Parent)
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild;
					else
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->RightChild;
				}
			}
		}

		// node has no child
		else
		{
			// set root to AVLNULL, if node is root
			if (AVLNULL==NodeStruct->Parent)
			{
				m_Root=AVLNULL;
				PopupNode=NULL;
			}

			// otherwise not root
			else
			{
				// set parent child to AVLNULL
				if (GETAVLS(NodeStruct->Parent)->LeftChild==Node)
				{
					GETAVLS(NodeStruct->Parent)->LeftChild=AVLNULL;
					Side=AVLLEFT;
				}
				else
				{
					GETAVLS(NodeStruct->Parent)->RightChild=AVLNULL;
					Side=AVLRIGHT;
				}

				// get address of node for poping up
				if (AVLNULL==GETAVLS(NodeStruct->Parent)->Parent)
					PopupNode=&m_Root;
				else
				{
					if (GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild==NodeStruct->Parent)
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->LeftChild;
					else
						PopupNode=&GETAVLS(GETAVLS(NodeStruct->Parent)->Parent)->RightChild;
				}
			}
		}
	}

	// correct AVL balance
	DepthDecrease=TRUE;
	while (NULL!=PopupNode && TRUE==DepthDecrease)
	{
		// get popup node structure
		PopupNodeStruct=GETAVLS((*PopupNode));

		// if coming from left
		if (AVLLEFT==Side)
		{
			// correct balance
			switch (PopupNodeStruct->Balance)
			{
			case 1:
				PopupNodeStruct->Balance=0;
				break;
			case 0:
				PopupNodeStruct->Balance=-1;
				DepthDecrease=FALSE;
				break;
			case -1:
				if (1==GETAVLS(PopupNodeStruct->RightChild)->Balance)
					RightLeftRotation(Array,PopupNode,PopupNodeStruct->Parent);
				else
					DepthDecrease=LeftRotation(Array,PopupNode,PopupNodeStruct->Parent);
				PopupNodeStruct=GETAVLS((*PopupNode));
				break;
			}
		}

		// otherwise, coming from right
		else
		{
			// correct balance
			switch (PopupNodeStruct->Balance)
			{
			case -1:
				PopupNodeStruct->Balance=0;
				break;
			case 0:
				PopupNodeStruct->Balance=1;
				DepthDecrease=FALSE;
				break;
			case 1:
				if (-1==GETAVLS(PopupNodeStruct->LeftChild)->Balance)
					LeftRightRotation(Array,PopupNode,PopupNodeStruct->Parent);
				else
					DepthDecrease=RightRotation(Array,PopupNode,PopupNodeStruct->Parent);
				PopupNodeStruct=GETAVLS((*PopupNode));
				break;
			}
		}

		// stop recursion if node is root
		if (AVLNULL==PopupNodeStruct->Parent)
			PopupNode=NULL;

		// otherwise, not root
		else
		{
			// get side
			if ((*PopupNode)==GETAVLS(PopupNodeStruct->Parent)->LeftChild)
				Side=AVLLEFT;
			else
				Side=AVLRIGHT;

			// get address of node for poping up
			if (AVLNULL==GETAVLS(PopupNodeStruct->Parent)->Parent)
				PopupNode=&m_Root;
			else
			{
				if (GETAVLS(GETAVLS(PopupNodeStruct->Parent)->Parent)->LeftChild==PopupNodeStruct->Parent)
					PopupNode=&GETAVLS(GETAVLS(PopupNodeStruct->Parent)->Parent)->LeftChild;
				else
					PopupNode=&GETAVLS(GETAVLS(PopupNodeStruct->Parent)->Parent)->RightChild;
			}
		}
	}
}

int GetMin(Node_t *Array)
{
	int Node;

	// abort if empty tree
	if (AVLNULL==m_Root)
		return AVLNULL;

	// find the most at the left in the tree
	for (Node=m_Root;AVLNULL!=GETAVLS(Node)->LeftChild;Node=GETAVLS(Node)->LeftChild)
		continue;

	return Node;
}

int GetMax(Node_t *Array)
{
	int Node;

	// abort if empty tree
	if (AVLNULL==m_Root)
		return AVLNULL;

	// find the most at the right in the tree
	for (Node=m_Root;AVLNULL!=GETAVLS(Node)->RightChild;Node=GETAVLS(Node)->RightChild)
		continue;

	return Node;
}

int GetFirst(Node_t *Array)
{
	// abort if empty tree
	if (AVLNULL==m_Root)
		return AVLNULL;

	// start from root
	m_CurNode=m_Root;
	m_CurNodeStruct=GETAVLS(m_CurNode);
	m_CurNodeStruct->Flag=AVLUNEXPLORED;

	// get first element
	return GetNext(Array);
}

int GetNext(Node_t *Array)
{
	int Node=AVLNULL;

	// abort if empty tree
	if (AVLNULL==m_Root)
		return AVLNULL;

	// while a node is found
	while (AVLNULL==Node)
	{
		switch (m_CurNodeStruct->Flag)
		{
			// if current node is unexplored
		case AVLUNEXPLORED:

			// next time it will be outputed
			m_CurNodeStruct->Flag=AVLOUT;

			// recurse to left child if exists
			if (AVLNULL!=m_CurNodeStruct->LeftChild)
			{
				m_CurNode=m_CurNodeStruct->LeftChild;
				m_CurNodeStruct=GETAVLS(m_CurNode);
				m_CurNodeStruct->Flag=AVLUNEXPLORED;
			}
			break;

			// if current node need to be outputed
		case AVLOUT:

			// done with this node
			Node=m_CurNode;
			m_CurNodeStruct->Flag=AVLDONE;

			// recurse to right child if exists
			if (AVLNULL!=m_CurNodeStruct->RightChild)
			{
				m_CurNode=m_CurNodeStruct->RightChild;
				m_CurNodeStruct=GETAVLS(m_CurNode);
				m_CurNodeStruct->Flag=AVLUNEXPLORED;
			}
			break;

			// if done with current node
		case AVLDONE:

			// move back
			m_CurNode=GETAVLS(m_CurNode)->Parent;
			m_CurNodeStruct=GETAVLS(m_CurNode);

			// done for ever if parent is root
			if (AVLNULL==m_CurNode)
				return AVLNULL;
			break;
		}
	}

	return Node;
}

void Trace(Node_t *Array)
{
	Trace(Array,m_Root);
}

private:

BOOL Insert(Node_t *Array,int *Tree,int Parent,int Node)
{
	BOOL DepthIncrease=FALSE;
	AVLStruct_t *TreeStruct;
	AVLStruct_t *NodeStruct;

	// insert node if in a leaf
	if (AVLNULL==(*Tree))
	{
		// get node structure
		NodeStruct=GETAVLS(Node);

		// fill node
		NodeStruct->Balance=0;
		NodeStruct->Parent=Parent;
		NodeStruct->LeftChild=AVLNULL;
		NodeStruct->RightChild=AVLNULL;

		// and insert it
		(*Tree)=Node;
		DepthIncrease=TRUE;
	}

	// otherwise, walk tree
	else
	{
		// get tree structure
		TreeStruct=GETAVLS((*Tree));

		// go left if less
		if (GETVAL(Node)<GETVAL((*Tree)))
		{
			// if depth changed
			if (TRUE==Insert(Array,&TreeStruct->LeftChild,(*Tree),Node))
			{
				// correct balance
				switch (TreeStruct->Balance)
				{
				case -1:
					TreeStruct->Balance=0;
					break;
				case 0:
					TreeStruct->Balance=1;
					DepthIncrease=TRUE;
					break;
				case 1:
					if (-1==GETAVLS(TreeStruct->LeftChild)->Balance)
						LeftRightRotation(Array,Tree,Parent);
					else
						RightRotation(Array,Tree,Parent);
					break;
				}
			}
		}

		// go right if greater or equal
		else
		{
			// if depth changed
			if (TRUE==Insert(Array,&TreeStruct->RightChild,(*Tree),Node))
			{
				// correct balance
				switch (TreeStruct->Balance)
				{
				case 1:
					TreeStruct->Balance=0;
					break;
				case 0:
					TreeStruct->Balance=-1;
					DepthIncrease=TRUE;
					break;
				case -1:
					if (1==GETAVLS(TreeStruct->RightChild)->Balance)
						RightLeftRotation(Array,Tree,Parent);
					else
						LeftRotation(Array,Tree,Parent);
					break;
				}
			}
		}
	}

	return DepthIncrease;
}

BOOL LeftRotation(Node_t *Array,int *Tree,int Parent)
{
	BOOL Balanced;
	int AuxNode;
	AVLStruct_t *TreeStruct=GETAVLS((*Tree));
	AVLStruct_t *AuxStruct;

	// get right child
	AuxNode=TreeStruct->RightChild;
	AuxStruct=GETAVLS(AuxNode);

	// perform rotation for tree
	TreeStruct->RightChild=AuxStruct->LeftChild;
	if (AVLNULL!=TreeStruct->RightChild)
		GETAVLS(TreeStruct->RightChild)->Parent=(*Tree);
	TreeStruct->Parent=AuxNode;

	// perform rotation for left child
	AuxStruct->LeftChild=(*Tree);
	AuxStruct->Parent=Parent;

	// reset balance
	switch (AuxStruct->Balance)
	{
	case 0:
		TreeStruct->Balance=-1;
		AuxStruct->Balance=1;
		Balanced=FALSE;
		break;
	case -1:
		TreeStruct->Balance=0;
		AuxStruct->Balance=0;
		Balanced=TRUE;
		break;
	}

	// attach subtree
	(*Tree)=AuxNode;

	return Balanced;
}

BOOL RightRotation(Node_t *Array,int *Tree,int Parent)
{
	BOOL Balanced;
	int AuxNode;
	AVLStruct_t *TreeStruct=GETAVLS((*Tree));
	AVLStruct_t *AuxStruct;

	// get left child
	AuxNode=TreeStruct->LeftChild;
	AuxStruct=GETAVLS(AuxNode);

	// perform rotation for tree
	TreeStruct->LeftChild=AuxStruct->RightChild;
	if (AVLNULL!=TreeStruct->LeftChild)
		GETAVLS(TreeStruct->LeftChild)->Parent=(*Tree);
	TreeStruct->Parent=AuxNode;

	// perform rotation for right child
	AuxStruct->RightChild=(*Tree);
	AuxStruct->Parent=Parent;

	// reset balance
	switch (AuxStruct->Balance)
	{
	case 0:
		TreeStruct->Balance=1;
		AuxStruct->Balance=-1;
		Balanced=FALSE;
		break;
	case 1:
		TreeStruct->Balance=0;
		AuxStruct->Balance=0;
		Balanced=TRUE;
		break;
	}

	// attach subtree
	(*Tree)=AuxNode;

	return Balanced;
}

void LeftRightRotation(Node_t *Array,int *Tree,int Parent)
{
	int AuxNode1,AuxNode2;
	AVLStruct_t *TreeStruct=GETAVLS((*Tree));
	AVLStruct_t *AuxStruct1;
	AVLStruct_t *AuxStruct2;

	// get left child
	AuxNode1=TreeStruct->LeftChild;
	AuxStruct1=GETAVLS(AuxNode1);

	// get right left child
	AuxNode2=AuxStruct1->RightChild;
	AuxStruct2=GETAVLS(AuxNode2);

	// perform rotation for tree
	TreeStruct->LeftChild=AuxStruct2->RightChild;
	if (AVLNULL!=TreeStruct->LeftChild)
		GETAVLS(TreeStruct->LeftChild)->Parent=(*Tree);
	TreeStruct->Parent=AuxNode2;

	// perform rotation for left child
	AuxStruct1->RightChild=AuxStruct2->LeftChild;
	if (AVLNULL!=AuxStruct1->RightChild)
		GETAVLS(AuxStruct1->RightChild)->Parent=AuxNode1;
	AuxStruct1->Parent=AuxNode2;

	// perform rotation for right left child
	AuxStruct2->LeftChild=AuxNode1;
	AuxStruct2->RightChild=(*Tree);
	AuxStruct2->Parent=Parent;

	// reset balance
	switch (AuxStruct2->Balance)
	{
	case -1:
		AuxStruct1->Balance=1;
		TreeStruct->Balance=0;
		break;
	case 0:
		AuxStruct1->Balance=0;
		TreeStruct->Balance=0;
		break;
	case 1:
		AuxStruct1->Balance=0;
		TreeStruct->Balance=-1;
		break;
	}
	AuxStruct2->Balance=0;

	// attach subtree
	(*Tree)=AuxNode2;
}

void RightLeftRotation(Node_t *Array,int *Tree,int Parent)
{
	int AuxNode1,AuxNode2;
	AVLStruct_t *TreeStruct=GETAVLS((*Tree));
	AVLStruct_t *AuxStruct1;
	AVLStruct_t *AuxStruct2;

	// get right child
	AuxNode1=TreeStruct->RightChild;
	AuxStruct1=GETAVLS(AuxNode1);

	// get left right child
	AuxNode2=AuxStruct1->LeftChild;
	AuxStruct2=GETAVLS(AuxNode2);

	// perform rotation for tree
	TreeStruct->RightChild=AuxStruct2->LeftChild;
	if (AVLNULL!=TreeStruct->RightChild)
		GETAVLS(TreeStruct->RightChild)->Parent=(*Tree);
	TreeStruct->Parent=AuxNode2;

	// perform rotation for right child
	AuxStruct1->LeftChild=AuxStruct2->RightChild;
	if (AVLNULL!=AuxStruct1->LeftChild)
		GETAVLS(AuxStruct1->LeftChild)->Parent=(AuxNode1);
	AuxStruct1->Parent=AuxNode2;

	// perform rotation for left right child
	AuxStruct2->LeftChild=(*Tree);
	AuxStruct2->RightChild=AuxNode1;
	AuxStruct2->Parent=Parent;

	// reset balance
	switch (AuxStruct2->Balance)
	{
	case -1:
		AuxStruct1->Balance=0;
		TreeStruct->Balance=1;
		break;
	case 0:
		AuxStruct1->Balance=0;
		TreeStruct->Balance=0;
		break;
	case 1:
		AuxStruct1->Balance=-1;
		TreeStruct->Balance=0;
		break;
	}
	AuxStruct2->Balance=0;

	// attach subtree
	(*Tree)=AuxNode2;
}

void Trace(Node_t *Array,int Node)
{
	if (AVLNULL==Node)
		return;

	printf("Val : %6.2f\t Balance : %d\t",GETVAL(Node),GETAVLS(Node)->Balance);
	if (AVLNULL==GETAVLS(Node)->LeftChild)
		printf("Left : AVLNULL\t");
	else
		printf("Left : %6.2f\t",GETVAL(GETAVLS(Node)->LeftChild));
	if (AVLNULL==GETAVLS(Node)->RightChild)
		printf("Right : AVLNULL\n");
	else
		printf("Right : %6.2f\n",GETVAL(GETAVLS(Node)->RightChild));

	Trace(Array,GETAVLS(Node)->LeftChild);
	Trace(Array,GETAVLS(Node)->RightChild);
}

};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_AVL_H__5DFE6574_AECE_11D3_8576_00600834A808__INCLUDED_)
