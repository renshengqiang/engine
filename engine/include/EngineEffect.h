#ifndef _ENGINE_EFFECT_H
#define _ENGINE_EFFECT_H
#include <list>
#include <GL/glew.h>

#define INVALID_PROGRAM_LOCATION	0
#define INVALID_ATTRIB_LOCATION	0xFFFFFFFF
namespace Engine
{
class Effect
{
public:
	typedef std::list<GLuint> ShaderObjList;
	enum SHADER_TYPE{	VERTEX_SHADER, FRAGMENT_SHADER};
	
	Effect();
	~Effect();
	
	//����һ��shader program
	virtual bool init(void);
	//ʹ��һ��shader program
	void enable(void);
	//disableһ��shader program
	static void disable(void);
	
protected:
	//���shader����effect����
	bool addShader(SHADER_TYPE shaderType, const char* fileName);
	//finalize shader program
	bool finalize(void);

	//��ȡһ��uniform���ͱ�����id
	GLint getUniformLocation(const char* pUniformName);
	//��ȡһ��attribute���ͱ�����id
	GLint getAttribLocation(const char* pAttribName);
	
private:
	GLuint m_shaderProgram;
	ShaderObjList m_shaderObjList;
};
}

#endif
