#ifndef _ENGINE_OCTREE_SCENE_MANAGER_H
#define _ENGINE_OCTREE_SCENE_MANAGER_H
#include <EngineSceneManager.h>
#include <EngineVector.h>
#include <EngineAxisAlignedBox.h>
#include <vector>

namespace Engine
{
class Octree;
class OctreeNode;
class RaySceneQuery;
class Camera;
class OctreeCamera;
class Ray;

/** 八叉树定制版本的SceneManager，原来的SceneManager只有场景图管理，这里加上了空间管理*/
class  OctreeSceneManager : public SceneManager
{
public:
	// Standard Constructor.  Initializes the octree to -10000,-10000,-10000 to 10000,10000,10000 with a depth of 8. 
	OctreeSceneManager();
	// Standard Constructor 
	OctreeSceneManager(AxisAlignedBox &box, int max_depth );
	// Standard destructor
	~OctreeSceneManager();

	virtual RaySceneQuery* createRayQuery(const Ray &ray);
	//Initializes the manager to the given box and depth.
	void init( AxisAlignedBox &box, int d );

	 // Creates a specialized OctreeCamera    
	virtual Camera *createCamera(const Vector3f &pos= Vector3f(0,0,0), const Vector3f &target=Vector3f(0,0,-1), const Vector3f &up=Vector3f(0,1,0));

	// Does nothing more 
	virtual void _updateSceneGraph();
	
	// Recurses through the octree determining which nodes are visible. 
	virtual void _findVisibleObjects(Camera *cam, RenderQueue &renderQueue);

	/** Walks through the octree, adding any visible objects to the render queue.
	@remarks
	If any octant in the octree if completely within the view frustum,
	all subchildren are automatically added with no visibility tests.
	*/
	void walkOctree( OctreeCamera *, RenderQueue &, Octree *,  bool);

	//Checks the given OctreeNode, and determines if it needs to be moved to a different octant.
	void _updateOctreeNode( OctreeNode * );
	
	//Removes the given octree node 
	void _removeOctreeNode( OctreeNode * );
	
	//Adds the Octree Node, starting at the given octree, and recursing at max to the specified depth.
	void _addOctreeNode( OctreeNode *, Octree *octree, int depth = 0 );

	/** Recurses the octree, adding any nodes intersecting with the ray into the given list.
	It ignores the exclude scene node.
	*/
	void findNodesIn( const Ray &ray, std::vector<SceneNode*> &list, SceneNode *exclude=0 );

protected:
	/// The root octree
	Octree *mp_octree;

	/// Max depth for the tree
	int m_maxDepth;
};
}
#endif
