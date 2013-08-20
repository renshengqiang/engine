#ifndef _ENGINE_SIMPLE_MESH_EFFECT_H
#define _ENGINE_SIMPLE_MESH_EFFECT_H

#include <EngineEffect.h>
#include <string>

#define MAX_BONE_NUM 100

namespace Engine
{
class Matrix4f;

class SimpleMeshEffect : public Effect
{
public:
	SimpleMeshEffect(const char* vertexShaderFile, const char* fragmentShaderFile);
	~SimpleMeshEffect();

	bool init(void);
	
	void enableVertexArray(void);
	void disableVertexArray(void);
	
	void setWVP(const Matrix4f& WVP);
	void setTextureUnit(int textureUnit);
	void setHasBones(bool hasBone);
	void setBoneTransMatrix(const Matrix4f& trans, unsigned boneId);

	void setPositionAttribPointer(int coordNum, int stride, int offset);
	void setTextureCoordAttribPointer(int coordNum, int stride, int offset);
	void setBoneIdAttribPointer(int coordNum, int stride, int offset);
	void setBoneWeightAttribPointer(int coordNum, int stride, int offset);	
	
private:
	std::string m_vertexShaderFile, m_fragmentShaderFile;
	GLuint m_positionLocation;
	GLuint m_textureCoordLocation;
	GLuint m_boneIdLocation;
	GLuint m_boneWeightLocation;
	GLuint m_hasBonesLocation;
	GLuint m_WVPLocation;
	GLuint m_boneTransMatrixLocation[MAX_BONE_NUM];
	GLuint m_sampleLocation;
};
}

#endif
