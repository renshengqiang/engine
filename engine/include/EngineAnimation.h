#ifndef _ENGINE_ANIMATION_H
#define _ENGINE_ANIMATION_H
#include <string>
#include <map>

namespace Engine
{
class NodeAnimationTrack;
class Node;

/*
	Animation类存在的作用:对多个Animation Track进行统一管理
	这些Track共享的信息有:
	1. 动画的时间长度
	2. 动画的当前位置
	3. 动画的插值方式
*/
class Animation
{
public:
	/** The types of animation interpolation available. */
	enum InterpolationMode
	{
		/** Values are interpolated along straight lines. */
		IM_LINEAR,
		/** Values are interpolated along a spline, resulting in smoother changes in direction. */
		IM_SPLINE
	};

	/** The types of rotational interpolation available. */
	enum RotationInterpolationMode
	{
		/** Values are interpolated linearly. This is faster but does not 
			necessarily give a completely accurate result.
		*/
		RIM_LINEAR,
		/** Values are interpolated spherically. This is more accurate but
			has a higher cost.
		*/
		RIM_SPHERICAL
	};
	typedef std::map<unsigned short, NodeAnimationTrack*> NodeTrackList;
	typedef NodeTrackList::iterator NodeTrackIterator;
	typedef NodeTrackList::const_iterator NodeTrackConstIterator;
	
	Animation(const std::string &name, float length):
		m_name(name), m_length(length),
		m_interpolationMode(IM_LINEAR),
		m_rotateInterpolationMode(RIM_LINEAR)
	{}
	virtual ~Animation()
	{
		destroyAllNodeTracks();
	}
	
	const std::string &getName(void) const
	{
		return m_name;
	}
	
	float getLength(void) const
	{
		return m_length;
	}
	
	void setLength(float length)
	{
		m_length = length;
	}
	
	void setInterpolationMode(InterpolationMode im)
	{
		m_interpolationMode = im;
	}
	
	InterpolationMode getInterpolationMode(void)
	{
		return m_interpolationMode;
	}
	
	void setRotationInterpolationMode(RotationInterpolationMode rim)
	{
		m_rotateInterpolationMode = rim;
	}
	
	RotationInterpolationMode getRotationInterpolationMode(void)
	{
		return m_rotateInterpolationMode;
	}

	unsigned short getNumNodeTracks(void) const
	{
		return m_nodeTrackList.size();
	}
	
	bool hasNodeTrack(unsigned short handle) const
	{
		return m_nodeTrackList.find(handle)!=m_nodeTrackList.end();
	}
	
	NodeAnimationTrack *getNodeTrack(unsigned short handle) const
	{
		NodeTrackConstIterator iter = m_nodeTrackList.find(handle);
		if(iter != m_nodeTrackList.end()) return iter->second;
		return NULL;
	}
	
	NodeTrackIterator _nodeTrackBegin(void) { return m_nodeTrackList.begin(); }
	const NodeTrackIterator _nodeTrackEnd(void) { return m_nodeTrackList.end(); }

	/*
		创建一个可驱动一个SceneNode的NodeAnimationTrack
		@param handle是用来检索该track在Animation中的位置；
		该Animation中的所有NodeAnimationTrack的handle必须是独立的
	*/
	NodeAnimationTrack *createNodeTrack(unsigned short handle, const std::string &name="");
	NodeAnimationTrack *createNodeTrack(unsigned short handle, Node *node);
	void destoryNodeTrack(unsigned short handle);
	void destroyAllNodeTracks(void);
	void apply(float timePos, float weight = 1.0, float scale=1.0);
	void applyToNode(Node *node, float timePos, float weight = 1.0, float scale = 1.0);
	
private:
	std::string m_name;
	float m_length;
	enum InterpolationMode m_interpolationMode;
	enum RotationInterpolationMode m_rotateInterpolationMode;

	NodeTrackList m_nodeTrackList;
};

}

#endif
