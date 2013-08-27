#ifndef _ENGINE_OCTREE_H
#define _ENGINE_OCTREE_H
#include <EngineAxisAlignedBox.h>
#include <list>

namespace Engine
{
class OctreeNode;

/** 
这里采用的松散八叉树的实现，松散因子是0.5
松散因子是0.5意味着每个八叉树节点得到的包围盒的长宽高均是实际长宽高的两倍大小
所以实际的体积是实际体积的八倍大小

采用松散八叉树的优点在于当沿着父节点向下查询时不需要分割物体

若不采用松散因子，一个物体可能分布在一个八叉树节点的两个子节点中间，为了管理该物体的空间，必须切分该物体，而切分的复杂度太高，不太现实

当采用0.5的松散因子后，则这两个子节点中至少有一个子节点的空间范围能包括该物体，因此无需分割物体
如果两个子节点都不能包含该物体，则说明该物体只能包含在父节点下，不能让子节点管理
*/

class Octree
{
public:
	Octree( Octree * p );
	~Octree();

	/** Adds an Octree scene node to this octree level.
	@remarks
	This is called by the OctreeSceneManager after
	it has determined the correct Octree to insert the node into.
	*/
	void _addNode( OctreeNode * );

	/** Removes an Octree scene node to this octree level.
	*/
	void _removeNode( OctreeNode * );

	/** Returns the number of scene nodes attached to this octree
	*/
	int numNodes()
	{
		return m_numNodes;
	};

	/** The bounding box of the octree
	@remarks
	This is used for octant index determination and rendering, but not culling
	*/
	AxisAlignedBox m_box;

	/** Vector containing the dimensions of this octree / 2
	*/
	Vector3f m_halfSize;

	/** 3D array of children of this octree.
	@remarks
	Children are dynamically created as needed when nodes are inserted in the Octree.
	If, later, all the nodes are removed from the child, it is still kept around.
	*/
	Octree * m_children[ 2 ][ 2 ][ 2 ];

	/** Determines if this octree is twice as big as the given box.
	@remarks
	This method is used by the OctreeSceneManager to determine if the given
	box will fit into a child of this octree.
	*/
	bool _isTwiceSize( const AxisAlignedBox &box ) const;

	/**  Returns the appropriate indexes for the child of this octree into which the box will fit.
	@remarks
	This is used by the OctreeSceneManager to determine which child to traverse next when
	finding the appropriate octree to insert the box.  Since it is a loose octree, only the
	center of the box is checked to determine the octant.
	*/
	void _getChildIndexes( const AxisAlignedBox &, int *x, int *y, int *z ) const;

	/** Creates the AxisAlignedBox used for culling this octree.
	@remarks
	Since it's a loose octree, the culling bounds can be different than the actual bounds of the octree.
	*/
	void _getCullBounds( AxisAlignedBox * ) const;


	typedef std::list< OctreeNode * > NodeList;
	/** Public list of SceneNodes attached to this particular octree
	*/
	NodeList mNodes;

protected:

	/** Increments the overall node count of this octree and all its parents
	*/
	inline void _ref()
	{
		m_numNodes++;

		if ( mp_parent != 0 ) mp_parent -> _ref();
	};

	/** Decrements the overall node count of this octree and all its parents
	*/
	inline void _unref()
	{
		m_numNodes--;

		if ( mp_parent != 0 ) mp_parent -> _unref();
	};

	///number of SceneNodes in this octree and all its children.
	int m_numNodes;

	///parent octree
	Octree * mp_parent;

};

}

#endif
