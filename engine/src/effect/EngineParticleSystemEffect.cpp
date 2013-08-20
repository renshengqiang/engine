#include "EngineParticleSystemEffect.h"
#include "EngineMatrix.h"

namespace Engine
{
ParticleEffect::ParticleEffect(const char *vertexShaderFile, const char *fragmentShaderFile):
	m_vertexShaderFile(vertexShaderFile),
	m_fragmentShaderFile(fragmentShaderFile)
{
}
	
ParticleEffect::~ParticleEffect()
{
}

bool ParticleEffect::init()
{
	if(!Effect::init()) return false;

	if(!addShader(VERTEX_SHADER, m_vertexShaderFile.c_str())) return false;

	if(!addShader(FRAGMENT_SHADER, m_fragmentShaderFile.c_str())) return false;

	if(!finalize()) return false;

	m_positionLocation = getAttribLocation("pos");
	m_textureCoordLocation = getAttribLocation("texCoord");
	m_velocityLocation = getAttribLocation("velocity");
	m_timeLocation = getUniformLocation("time");
	m_WVPLocation = getUniformLocation("wvpMatrix");
	m_sampleLocation = getUniformLocation("sampler");
	if(m_positionLocation == INVALID_ATTRIB_LOCATION ||
		m_textureCoordLocation == INVALID_ATTRIB_LOCATION ||
		m_velocityLocation== INVALID_ATTRIB_LOCATION ||
		m_timeLocation== INVALID_ATTRIB_LOCATION ||
		m_WVPLocation == INVALID_ATTRIB_LOCATION ||
		m_sampleLocation == INVALID_ATTRIB_LOCATION)
		return false;
	return true;
}

void ParticleEffect::enableVertexArray()
{
	glEnableVertexAttribArray(m_positionLocation);
	glEnableVertexAttribArray(m_textureCoordLocation);
	glEnableVertexAttribArray(m_velocityLocation);
}
void ParticleEffect::disableVertexArray()
{
	glDisableVertexAttribArray(m_positionLocation);
	glDisableVertexAttribArray(m_textureCoordLocation);
	glDisableVertexAttribArray(m_velocityLocation);
}
void ParticleEffect::setWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, &(WVP.m[0][0]));
}
void ParticleEffect::setTextureUnit(int textureUnit)
{
	glUniform1i(m_sampleLocation, textureUnit);
}
void ParticleEffect::setTime(float time)
{
	glUniform1f(m_timeLocation, time);
}
void ParticleEffect::setPositionAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_positionLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}
void ParticleEffect::setTextureCoordAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_textureCoordLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}
void ParticleEffect::setVelocityAttribPointer(int coordNum, int stride, int offset)
{
	glVertexAttribPointer(m_velocityLocation, coordNum, GL_FLOAT, GL_FALSE, stride, (const GLvoid *)offset);
}
}
