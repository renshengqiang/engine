#ifndef _ENGINE_RAY_SCENE_QUERY_H
#define _ENGINE_RAY_SCENE_QUERY_H
#include <EngineRay.h>
#include <vector>

namespace Engine
{
class MoveableObject;
/** 该类保存光线查询的结果类型 */
struct RaySceneQueryResultEntry
{
	/// Distance along the ray
	float distance;
	
	/// The movable, or NULL if this is not a movable result
	MoveableObject *pMoveable;
	
	/// Comparison operator for sorting
	bool operator < (const RaySceneQueryResultEntry& rhs) const
	{
		return this->distance < rhs.distance;
	}

};
typedef std::vector<RaySceneQueryResultEntry> RaySceneQueryResult;

class SceneManager;
class RaySceneQuery
{
protected:
	SceneManager *mp_sceneManager;
	Ray m_ray;
	bool m_sortByDistance;
	unsigned short m_maxResults;
	RaySceneQueryResult m_result;

public:
	RaySceneQuery(SceneManager* mgr);
	virtual ~RaySceneQuery();
	
	/**设置光线查询的光线  */
	virtual void setRay(const Ray& ray);
	
	/** 获取光线查询的光线 */
	virtual const Ray& getRay(void) const;
	
	/**  设置光线查询的结果是否按照distance进行排序
		设置后在结果中，最近的SceneNode会在数组的最前面
		同时注意，这个查询的结果只是物体的包围盒交叉的结果
		是否真的交叉需要做更近一步的检查
		maxresults表示最多可保存多少个结果，为0表示不限
	*/
	virtual void setSortByDistance(bool sort, unsigned short maxresults = 0);
	
	/* 是否经过距离排序*/
	virtual bool getSortByDistance(void) const;
	
	/* 获取最大的结果数目 */
	virtual unsigned short getMaxResults(void) const;
	
	/* 执行光线查询，结果保存在m_result中*/
	virtual RaySceneQueryResult& execute(void);

	/* 获取查询的结果*/
	virtual RaySceneQueryResult& getLastResults(void);
	
	/* 清空查询的结果，执行execute时会自动执行*/
	virtual void clearResults(void);

	/* 当查询到交叉元素时执行的回调函数 */
	void queryResult(MoveableObject *pObject, float distance);
};
}

#endif
