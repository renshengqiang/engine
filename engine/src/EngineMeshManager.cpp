#include "EngineMeshManager.h"
#include "EngineSubMesh.h"
#include <stdio.h>
#include <tr1/functional>
using namespace std::tr1::placeholders;

namespace Engine
{
template<> MeshManager* Util::Singleton<MeshManager>::sp_instance  = 0;

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPtr MeshManager::createMesh(const std::string& name)
{
	MeshPtr meshPtr;
	MeshWeakPtr &wkPtr = m_meshPtrMap[name];
	meshPtr = wkPtr.lock();
	if(!meshPtr)
	{
		//绑定当meshPtr中的对象delete时在这里执行removeMesh函数
		//remove函数的第一个参数是this, 第二个参数是Mesh*
		Mesh *pMesh = new Mesh(name);
		if(pMesh->load() == true)
		{
			meshPtr.reset(pMesh, std::tr1::bind(&MeshManager::removeMesh, this, _1));
			m_toFinalizeMeshVec.push_back(pMesh);
			wkPtr = meshPtr;
		}
	}
	/**	返回的MeshPtr需要进行判断，判断是否为空
	 	如果是默认的MeshPtr则是空
	*/
	return meshPtr;
}

MeshPtr MeshManager::createPlane(const std::string &name, const Plane &plane, float width, float height, 
								int xsegments, int ysegments, bool normals, 
								float xTile, float yTile, const Vector3f &upVector)
{
	MeshPtr meshPtr;
	MeshWeakPtr &wkPtr = m_meshPtrMap[name];
	meshPtr = wkPtr.lock();
	if(meshPtr)
	{
		return meshPtr;
	}
	
	// store parameters
        MeshBuildParams params;
        params.plane = plane;
        params.width = width;
        params.height = height;
        params.xsegments = xsegments;
        params.ysegments = ysegments;
        params.normals = normals;
        params.xTile = xTile;
        params.yTile = yTile;
        params.upVector = upVector;
		
	Mesh *pMesh = new Mesh(name);
	loadManualPlane(pMesh, params);

	meshPtr.reset(pMesh, std::tr1::bind(&MeshManager::removeMesh, this, _1));
	m_toFinalizeMeshVec.push_back(pMesh);
	wkPtr = meshPtr;
	return meshPtr;
}

void MeshManager::removeMesh(Mesh *pMesh)
{
	if(pMesh)
	{
		m_meshPtrMap.erase(pMesh->getName());
		delete pMesh;
	}
}

void MeshManager::loadManualPlane(Mesh *pMesh, MeshBuildParams& params)
{
	SubMesh *pSubMesh = new SubMesh();
	
	// Work out the transform required
	// Default orientation of plane is normal along +z, distance 0
	Matrix4f xlate, xform, rot;
	Matrix3f rot3;
	xlate.initIdentity();
	rot.initIdentity();
	// Determine axes
	Vector3f zAxis, yAxis, xAxis;
	zAxis = params.plane.m_normal;
	zAxis.normalise();
	yAxis = params.upVector;
	yAxis.normalise();
	xAxis = yAxis.crossProduct(zAxis);
	
	if (xAxis.length() == 0)
	{
		//upVector must be wrong
		fprintf(stderr, "ERROR MeshManager::CreatePlane : The upVector you supplied is parallel to the plane normal, so is not valid.");
	}
	
	//从朝向观察者的方向来看，是正确的
	rot3.fromAxes(xAxis, yAxis, zAxis);
	rot = rot3;

	// Set up standard transform from origin 此处为什么是负的?
	xlate.initTranslationTransform(params.plane.m_normal * -params.plane.m_distance);

	// concatenate 最后的偏移矩阵
	xform = xlate * rot;

	// Generate vertex data
	float xSpace = params.width / params.xsegments;
	float ySpace = params.height / params.ysegments;
	float halfWidth = params.width / 2;
	float halfHeight = params.height / 2;
	float xTex = (1.0f * params.xTile) / params.xsegments;
	float yTex = (1.0f * params.yTile) / params.ysegments;
	Vector3f vec;
	Vector3f min = Vector3f::ZERO, max = Vector3f::UNIT_SCALE;
	bool firstTime = true;

	for (int y = 0; y < params.ysegments + 1; ++y)
	{
		for (int x = 0; x < params.xsegments + 1; ++x)
		{
			// Work out centered on origin
			vec.x = (x * xSpace) - halfWidth;
			vec.y = (y * ySpace) - halfHeight;
			vec.z = 0.0f;
			// Transform by orientation and distance
			vec = xform.transformAffine(vec);
			// Assign to geometry
			pSubMesh->addCoord(vec);

			// Build bounds as we go
			if (firstTime)
			{
				min = vec;
				max = vec;
				firstTime = false;
			}
			else
			{
				min.makeFloor(vec);
				max.makeCeil(vec);
			}

			if (params.normals)
			{
				/*计算法线，以后加上光照再加入
				// Default normal is along unit Z
				vec = Vector3f::UNIT_Z;
				// Rotate
				vec = rot.transformAffine(vec);
				*/
			}
			pSubMesh->addTextureCoord(Vector2f(x*xTex, 1-y*yTex));
		} // x
	} // y
	tesselate2DMesh(pSubMesh, params.xsegments + 1, params.ysegments + 1);
	pMesh->addSubMesh(pSubMesh);
	pMesh->setBoundingBox(min, max);
}

void MeshManager::tesselate2DMesh(SubMesh* pSub, unsigned short meshWidth, unsigned short meshHeight)
{
	// The mesh is built, just make a list of indexes to spit out the triangles
	unsigned short vInc, uInc, v, u;
	unsigned short vCount, uCount;
	
	// Make tris in a zigzag pattern (compatible with strips)
	vInc = 1;
	v = 0;
	u = 0;
	uInc = 1; // Start with moving +u

	unsigned short v1, v2, v3;		

	vCount = meshHeight - 1;
	while (vCount--)
	{
		uCount = meshWidth - 1;
		while (uCount--)
		{
			// First Tri in cell
			//*v2
			//*
			//*    *
			//v1   v3
			v1 = ((v + vInc) * meshWidth) + u;
			v2 = (v * meshWidth) + u;
			v3 = ((v + vInc) * meshWidth) + (u + uInc);
			// Output indexes
			pSub->addIndex(v2, v1, v3);
			
			// Second Tri in cell
			//*v2 *v3
			//       *
			//       *v1
			v1 = ((v + vInc) * meshWidth) + (u + uInc);
			v2 = (v * meshWidth) + u;
			v3 = (v * meshWidth) + (u + uInc);
			// Output indexes
			pSub->addIndex(v2, v1, v3);

			// Next column
			u += uInc;
		}
		// Next row
		v += vInc;
		u = 0;
	}
}

/// used by rendering thread
bool MeshManager::_finalize()
{
	for(ToFinalizeMeshIter it = m_toFinalizeMeshVec.begin();
			it != m_toFinalizeMeshVec.end(); ++it)
	{
		if( (*it)->_finalize() == false)
		{
			return false;
		}
	}
	m_toFinalizeMeshVec.clear();
	return true;	
}

}
