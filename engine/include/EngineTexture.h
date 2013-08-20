#ifndef _ENGINE_TEXTURE_H
#define _ENGINE_TEXTURE_H
#include <GL/glew.h>
#include <string>

namespace Engine
{
class Texture
{
public:
	Texture(const std::string& FileName);

	bool load();
	void activate();
	GLenum getTextureTarget() const { return m_textureTarget;}
	GLenum getTextureUnit() const { return m_textureUnit;}
	GLuint getTextureObj() const{return m_textureObj;}

private:
	std::string m_fileName;
	GLuint m_textureObj;
	GLenum m_textureTarget;
	GLenum m_textureUnit;
	bool m_loaded;
};
}
#endif
