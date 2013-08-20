#ifndef _ENGINE_PARTICLE_SYSTEM_EFFECT_H
#define _ENGINE_PARTICLE_SYSTEM_EFFECT_H

#include <EngineEffect.h>
#include <string>

namespace Engine
{
class Matrix4f;

class ParticleEffect : public Effect
{
public:
	ParticleEffect(const char *vertexShaderFile, const char *fragmentShaderFile);
	~ParticleEffect();

	bool init(void);
	
	void enableVertexArray(void);
	void disableVertexArray(void);
	
	void setWVP(const Matrix4f& WVP);
	void setTextureUnit(int textureUnit);
	void setTime(float time);
	
	void setPositionAttribPointer(int coordNum, int stride, int offset);
	void setVelocityAttribPointer(int coordNum, int stride, int offset);
	void setTextureCoordAttribPointer(int coordNum, int stride, int offset);	
	
private:
	std::string m_vertexShaderFile, m_fragmentShaderFile;
	GLuint m_positionLocation;
	GLuint m_velocityLocation;
	GLuint m_textureCoordLocation;
	GLuint m_timeLocation;
	GLuint m_WVPLocation;
	GLuint m_sampleLocation;
};
}
#endif
