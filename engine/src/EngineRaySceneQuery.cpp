#include "EngineRaySceneQuery.h"
#include "EngineSceneNode.h"
#include "EngineMoveableObject.h"
#include "EngineSceneManager.h"
#include <queue>

namespace Engine
{
RaySceneQuery::RaySceneQuery(SceneManager* mgr)
{
	mp_sceneManager = mgr;
	m_sortByDistance = false;
	m_maxResults = 0;
}

RaySceneQuery::~RaySceneQuery()
{
}

void RaySceneQuery::setRay(const Ray& ray)
{
	m_ray = ray;
}

const Ray& RaySceneQuery::getRay(void) const
{
	return m_ray;
}

void RaySceneQuery::setSortByDistance(bool sort, unsigned short maxresults)
{
	m_sortByDistance = sort;
	m_maxResults = maxresults;
}

bool RaySceneQuery::getSortByDistance(void) const
{
	return m_sortByDistance;
}

unsigned short RaySceneQuery::getMaxResults(void) const
{
	return m_maxResults;
}

RaySceneQueryResult& RaySceneQuery::execute(void)
{
	// Clear without freeing the vector buffer
	m_result.clear();

	// Call callback version with self as listener
	std::queue<SceneNode*> IterQueue;
	int no_intersection = 0;

	IterQueue.push(mp_sceneManager->getRootNode());
	while(!IterQueue.empty())
	{
		SceneNode *pNode = IterQueue.front();
		IterQueue.pop();

		MoveableObject *pMoveable = pNode->getAttachedMoveableObject();
		if(pMoveable)
		{
			AxisAlignedBox box = pMoveable->getBoundingBox();
			box.transformAffine(pNode->_getFullTransform());
			
			// Do ray/box test
			std::pair<bool, float> result = m_ray.intersects(box);

			if (result.first)
			{
				queryResult(pNode->getAttachedMoveableObject(), result.second);
			}
			else
			{
				no_intersection++;
			}
		}
		
		int n  = pNode->numChildren();
		for(int i=0; i<n; ++i)
			IterQueue.push((SceneNode*)(pNode->getChild(i)));		
	}

	if (m_sortByDistance)
	{
		if (m_maxResults != 0 && m_maxResults < m_result.size())
		{
			// Partially sort the N smallest elements, discard others
			std::partial_sort(m_result.begin(), m_result.begin()+m_maxResults, m_result.end());
			m_result.resize(m_maxResults);
		}
		else
		{
			// Sort entire result array
			std::sort(m_result.begin(), m_result.end());
		}
	}

	return m_result;
}

RaySceneQueryResult& RaySceneQuery::getLastResults(void)
{
	return m_result;
}

void RaySceneQuery::clearResults(void)
{
	m_result.clear();
}

void RaySceneQuery::queryResult(MoveableObject *pObject, float distance)
{
	// Add to internal list
	RaySceneQueryResultEntry dets;
	dets.distance = distance;
	dets.pMoveable = pObject;
	//dets.worldFragment = NULL;
	m_result.push_back(dets);
}
}
