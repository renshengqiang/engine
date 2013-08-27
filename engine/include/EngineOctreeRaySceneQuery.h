#ifndef _ENGINE_OCTREE_RAY_SCENE_QUERY_H
#define _ENGINE_OCTREE_RAY_SCENE_QUERY_H
#include <EngineRaySceneQuery.h>

namespace Engine
{
/** Octree implementation of RaySceneQuery. */
class OctreeRaySceneQuery : public RaySceneQuery
{
public:
	OctreeRaySceneQuery(SceneManager *creator):
		RaySceneQuery(creator)
	{}
		
	~OctreeRaySceneQuery(){}

	/** See RayScenQuery. */
	virtual RaySceneQueryResult& execute();
};
}

#endif
