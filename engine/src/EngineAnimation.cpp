#include "EngineAnimation.h"
#include "EngineNode.h"
#include "EngineAnimationTrack.h"
#include <stdio.h>

namespace Engine
{

NodeAnimationTrack *Animation::createNodeTrack(unsigned short handle, const std::string &name)
{
	if(m_nodeTrackList.find(handle) != m_nodeTrackList.end())
	{
		fprintf(stderr, "ERROR Animation::CreateNodeTrack : Node track with the specified hangle %d already exists\n",handle);
		return NULL;
	}
	NodeAnimationTrack *ret = new NodeAnimationTrack(this, name);
	m_nodeTrackList.insert(std::make_pair(handle, ret));
	ret->createNodeKeyFrame(0);//默认添加第一帧
	return ret;
}

NodeAnimationTrack *Animation::createNodeTrack(unsigned short handle, Node *node)
{
	NodeAnimationTrack *ret = createNodeTrack(handle, node->getName());
	ret->setAssociatedNode(node);
	return ret;
}

void Animation::destoryNodeTrack(unsigned short handle)
{
	NodeTrackIterator iter = m_nodeTrackList.find(handle);
	if(iter != m_nodeTrackList.end())
	{
		delete iter->second;
		m_nodeTrackList.erase(iter);
	}
}

void Animation::destroyAllNodeTracks(void)
{
	for(NodeTrackIterator iter = m_nodeTrackList.begin(); iter != m_nodeTrackList.end(); ++iter)
	{
		delete iter->second;
	}
	m_nodeTrackList.clear();
}

void Animation::apply(float timePos, float weight, float scale)
{
	for(NodeTrackIterator iter = m_nodeTrackList.begin(); iter!= m_nodeTrackList.end(); ++iter)
	{
		iter->second->apply(timePos, weight, scale);
	}
	return;
}

void Animation::applyToNode(Node *node, float timePos, float weight, float scale)
{
	for(NodeTrackIterator iter = m_nodeTrackList.begin(); iter!= m_nodeTrackList.end(); ++iter)
	{
		iter->second->applyToNode(node, timePos, weight, scale);
	}
	return;
}

}
