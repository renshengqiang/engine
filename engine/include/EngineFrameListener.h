#ifndef _ENGINE_FRAME_LISTENER
#define _ENGINE_FRAME_LISTENER

namespace Engine
{
class FrameListener
{
public:
	virtual bool frameRenderingQueued(float timeSinceLastFrame){return true;}
	virtual ~FrameListener(){}
};
}

#endif
