#ifndef _ENGINE_RAY_SCENE_QUERY_H
#define _ENGINE_RAY_SCENE_QUERY_H
#include <EngineRay.h>
#include <vector>

namespace Engine
{
class MoveableObject;
/** ���ౣ����߲�ѯ�Ľ������ */
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
	
	/**���ù��߲�ѯ�Ĺ���  */
	virtual void setRay(const Ray& ray);
	
	/** ��ȡ���߲�ѯ�Ĺ��� */
	virtual const Ray& getRay(void) const;
	
	/**  ���ù��߲�ѯ�Ľ���Ƿ���distance��������
		���ú��ڽ���У������SceneNode�����������ǰ��
		ͬʱע�⣬�����ѯ�Ľ��ֻ������İ�Χ�н���Ľ��
		�Ƿ���Ľ�����Ҫ������һ���ļ��
		maxresults��ʾ���ɱ�����ٸ������Ϊ0��ʾ����
	*/
	virtual void setSortByDistance(bool sort, unsigned short maxresults = 0);
	
	/* �Ƿ񾭹���������*/
	virtual bool getSortByDistance(void) const;
	
	/* ��ȡ���Ľ����Ŀ */
	virtual unsigned short getMaxResults(void) const;
	
	/* ִ�й��߲�ѯ�����������m_result��*/
	virtual RaySceneQueryResult& execute(void);

	/* ��ȡ��ѯ�Ľ��*/
	virtual RaySceneQueryResult& getLastResults(void);
	
	/* ��ղ�ѯ�Ľ����ִ��executeʱ���Զ�ִ��*/
	virtual void clearResults(void);

	/* ����ѯ������Ԫ��ʱִ�еĻص����� */
	void queryResult(MoveableObject *pObject, float distance);
};
}

#endif
