#include "EngineMesh.h"
#include "EngineSubMesh.h"
#include "EngineTexture.h"
#include "EngineMatrix.h"
#include "EngineSimpleMeshEffect.h"
#include <stdio.h>
#include <Importer.hpp>      // C++ importer interface
#include <scene.h>       // Output data structure
#include <postprocess.h> // Post processing flags


//功能函数:根据文件名获取其路径并返回
std::string _getDir(const std::string filename)
{
	std::string::size_type slashIndex = filename.find_last_of("/");
	std::string dir;

	if(slashIndex == std::string::npos)
		dir = ".";
	else if(slashIndex == 0)
		dir = "/";
	else
		dir = filename.substr(0, slashIndex);
	return dir;
}

namespace Engine
{
Mesh::Mesh(const std::string &fileName):
	m_name(fileName),
	m_finalized(false)
{
	m_AABB = AxisAlignedBox(AxisAlignedBox::EXTENT_FINITE);
}

Mesh::~Mesh()
{
}

bool Mesh::load()
{
	Assimp::Importer importer;
	const aiScene *pScene = importer.ReadFile(m_name.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |aiProcess_FlipUVs);
	assert(pScene);
	fprintf(stdout, "Paring Mesh: '%s'\n", m_name.c_str());
	
	Matrix4f globalInverseTransform;

	//FIXME: DONT USE THIS ASSIGNMENT
	globalInverseTransform = pScene->mRootNode->mTransformation;
	globalInverseTransform.inverse();

	m_subMeshes.resize(pScene->mNumMeshes);
	m_texturNameVec.resize(pScene->mNumMaterials);
	
	//load materials
	std::string dir = _getDir(m_name);
	for(unsigned i=0; i<pScene->mNumMaterials; ++i)
	{
		const aiMaterial *pMaterial = pScene->mMaterials[i];
		m_texturNameVec[i] = "";
		if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string fullPath = dir + "/" + path.data;
				m_texturNameVec[i] = fullPath;
				fprintf(stdout, "material name:%s\n", fullPath.c_str());
			}
		}
		if(m_texturNameVec[i] == "")
		{
			m_texturNameVec[i] = dir + "/white.png";
		}
	}

	//load submeshes
	fprintf(stdout, "%d subMeshes\n", pScene->mNumMeshes);
	for(unsigned i=0; i<pScene->mNumMeshes; ++i)
	{
		const aiVector3D zeroCoord(0.0f, 0.0f, 0.0f);
		const aiMesh *pSubMesh = pScene->mMeshes[i];
		m_subMeshes[i] = new SubMesh();
		fprintf(stdout, "submesh %d (%d vertices, %d faces)\n", i+1, pSubMesh->mNumVertices, pSubMesh->mNumFaces);

		for(unsigned j=0; j<pSubMesh->mNumVertices; ++j)
		{
			const aiVector3D &coord = pSubMesh->mVertices[j];
			const aiVector3D &texCoord = pSubMesh->HasTextureCoords(0) ? pSubMesh->mTextureCoords[0][j]:zeroCoord;
			m_subMeshes[i]->addCoord(Vector3f(coord.x, coord.y, coord.z));
			m_subMeshes[i]->addTextureCoord(Vector2f(texCoord.x, texCoord.y));
			m_AABB.merge(Vector3f(coord.x, coord.y, coord.z));
		}
		
		for(unsigned j=0; j<pSubMesh->mNumFaces; ++j)
		{
			const aiFace &face = pSubMesh->mFaces[j];
			assert(face.mNumIndices == 3);
			m_subMeshes[i]->addIndex(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}
		m_subMeshes[i]->setTextureName(m_texturNameVec[pSubMesh->mMaterialIndex].c_str());
	}

	Vector3f min = m_AABB.getMinimum();
	Vector3f max = m_AABB.getMaximum();
	printf("AABB min(%f %f %f), max(%f %f %f)\n", min.x, min.y, min.z, max.x, max.y, max.z);

	return true;
}

void Mesh::render(SimpleMeshEffect &effect)
{
	for(unsigned int i=0; i < m_subMeshes.size(); ++i)
	{
		if(m_finalized == false)
			m_subMeshes[i]->finalize();
		m_subMeshes[i]->render(effect);
	}
	m_finalized = true;
}
}
