#ifndef _ENGINE_OCTREE_CAMERA_H
#define _ENGINE_OCTREE_CAMERA_H
#include <EngineCamera.h>

namespace Engine
{
class AxisAlignedBox;

/** Specialized viewpoint from which an Octree can be rendered.
@remarks
This class contains several specializations of the Ogre::Camera class. It
implements the getRenderOperation method in order to return displayable geometry
for debugging purposes. It also implements a visibility function that is more granular
than the default.
*/

class	OctreeCamera : public Camera
{
public:

	/** Visibility types */
	enum Visibility
	{
		NONE,
		PARTIAL,
		FULL
	};

	/* Standard constructor */
	OctreeCamera(const Vector3f &pos, const Vector3f &target, const Vector3f &up):
		Camera(pos, target, up)
	{}

	/** Returns the visiblity of the box
	*/
	OctreeCamera::Visibility getVisibility( const AxisAlignedBox &bound );

};

}

#endif
