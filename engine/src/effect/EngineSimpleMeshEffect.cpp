#include "EngineSimpleMeshEffect.h"
#include "EngineMatrix.h"
#include <string.h>//memset
#include <stdio.h>

namespace Engine
{
SimpleMeshEffect::SimpleMeshEffect(const char *vertexShaderFile, const char *fragmentShaderFile):
	m_vertexShaderFile(vertexShaderFile),
	m_fragmentShaderFile(fragmentShaderFile)
{
}
	
SimpleMeshEffect::~SimpleMeshEffect()
{
}

bool SimpleMeshEffect::init()
{
	if(!Effect::init()) return false;

	if(!addShader(VERTEX_SHADER, m_vertexShaderFile.c_str())) return false;

	if(!addShader(FRAGMENT_SHADER, m_fragmentShaderFile.c_str())) return false;

	if(!finalize()) return false;

	m_positionLocation = getAttribLocation("vertexPosition");
	m_textureCoordLocation = getAttribLocation("vertexTexCoord");
	m_boneIdLocation = getAttribLocation("boneIds");
	m_boneWeightLocation = getAttribLocation("weights");
	
	m_hasBonesLocation = getUniformLocation("hasBones");
	m_WVPLocation = getUniformLocation("PVMMatrix");
	m_sampleLocation = getUniformLocation("sampler");
	
	if(m_positionLocation == INVALID_ATTRIB_LOCATION ||
		m_textureCoordLocation == INVALID_ATTRIB_LOCATION ||
		m_boneIdLocation == INVALID_ATTRIB_LOCATION ||
		m_boneWeightLocation == INVALID_ATTRIB_LOCATION ||
		m_hasBonesLocation == INVALID_ATTRIB_LOCATION || 
		m_WVPLocation == INVALID_ATTRIB_LOCATION ||
		m_sampleLocation == INVALID_ATTRIB_LOCATION)
		return false;

	for(unsigned int i=0; i < MAX_BONE_NUM; ++i){
		char name[128];
		memset(name, 0, sizeof(name));
		snprintf(name, sizeof(name), "boneTransform[%d]", i);
		m_boneTransMatrixLocation[i] = getUniformLocation(name);
		if(m_boneTransMatrixLocation[i] == INVALID_ATTRIB_LOCATION)
		{
			return false;
		}
	}
	return true;
}

void SimpleMeshEffect::enableVertexArray()
{
	glEnableVertexAttribArray(m_positionLocation);
	glEnableVertexAttribArray(m_textureCoordLocation);
	glEnableVertexAttribArray(m_boneIdLocation);
	glEnableVertexAttribArray(m_boneWeightLocation);
}

void SimpleMeshEffect::disableVertexArray()
{
	glDisableVertexAttribArray(m_positionLocation);
	glDisableVertexAttribArray(m_textureCoordLocation);
	glDisableVertexAttribArray(m_boneIdLocation);
	glDisableVertexAttribArray(m_boneWeightLocation);
}

void SimpleMeshEffect::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, &(WVP.m[0][0]));
}

void SimpleMeshEffect::setTextureUnit(int textureUnit)
{
	glUniform1i(m_sampleLocation, textureUnit);
}

void SimpleMeshEffect::setHasBones(bool hasBone)
{
	glUniform1i(m_hasBonesLocation, hasBone);
}

void SimpleMeshEffect::setBoneTransMatrix(const Matrix4f& trans, unsigned boneId)
{
	glUniformMatrix4fv(m_boneTransMatrixLocation[boneId], 1, GL_TRUE, &(trans.m[0][0]));
}

void SimpleMeshEffect::setPositionAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_positionLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}

void SimpleMeshEffect::setTextureCoordAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_textureCoordLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}

void SimpleMeshEffect::setBoneIdAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_boneIdLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}

void SimpleMeshEffect::setBoneWeightAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_boneWeightLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}
}
