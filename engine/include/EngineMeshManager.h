#ifndef _ENGINE_MESH_MANAGER_H
#define _ENGINE_MESH_MANAGER_H
#include <UtilSingleton.h>
#include <EngineMesh.h>
#include <EnginePlane.h>
#include <map>
#include <string>
#include <vector>

namespace Engine
{
//MeshManager����ͳһ����Mesh��Դ�����ڽ���Mesh��Դ�Ĺ���������ڹ���
class MeshManager: public Util::Singleton<MeshManager>
{
public:
	typedef std::map<std::string, MeshWeakPtr> MeshPtrMap;
	typedef MeshPtrMap::iterator MeshPtrMapIterator;
	
	typedef std::vector<Mesh *>	ToFinalizeMeshVec;
	typedef ToFinalizeMeshVec::iterator ToFinalizeMeshIter;

	MeshManager();
	~MeshManager();

	/*����һ��mesh�ļ���������һ��Mesh*/
	MeshPtr createMesh(const std::string& name);

	/// ����Ⱦѭ���е��ã��������Կ��г�ʼ����Դ
	bool _finalize();
	
 	/** Saved parameters used to (re)build a manual mesh built by this class */
	struct MeshBuildParams
	{
		Plane plane;
		float width;
		float height;
		float curvature;
		int xsegments;
		int ysegments;
		bool normals;
		unsigned short numTexCoordSets;
		float xTile;
		float yTile;
		Vector3f upVector;
		Quaternion orientation;
	};
	/** ����һ���ֶ���ƽ�����͵�Mesh
	Creates a basic plane, by default majoring on the x/y axes facing positive Z.
	@param
		name The name to give the resulting mesh
 	@param
		plane The orientation of the plane and distance from the origin
	@param
		width The width of the plane in world coordinates
	@param
		height The height of the plane in world coordinates
	@param
		xsegments The number of segments to the plane in the x direction
	@param
		ysegments The number of segments to the plane in the y direction
	@param
		normals If true, normals are created perpendicular to the plane
	@param
		uTile The number of times the texture should be repeated in the u direction
	@param
		vTile The number of times the texture should be repeated in the v direction
	@param
		upVector The 'Up' direction of the plane texture coordinates.
	*/
	MeshPtr createPlane(const std::string& name, const Plane& plane, float width, float height, 
						int xsegments=1, int ysegments=1, 
						bool normals=false, float xTile=1, float yTile=1, 
						const Vector3f& upVector=Vector3f::UNIT_Z);
protected:
	void removeMesh(Mesh *pMesh);

	/** �����û��ṩ�Ĳ���������һ��Plane Mesh*/
	void loadManualPlane(Mesh *pMesh, MeshBuildParams& params);
	
	/** ��2D Meshϸ�֣��õ���������	*/
	void tesselate2DMesh(SubMesh* pSub, unsigned short meshWidth, unsigned short meshHeight);
	
private:
	MeshPtrMap			m_meshPtrMap;
	ToFinalizeMeshVec 		m_toFinalizeMeshVec;
};
}

#endif
