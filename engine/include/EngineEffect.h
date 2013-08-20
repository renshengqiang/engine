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
	
	//创建一个shader program
	virtual bool init(void);
	//使能一个shader program
	void enable(void);
	//disable一个shader program
	static void disable(void);
	
protected:
	//添加shader程序到effect中来
	bool addShader(SHADER_TYPE shaderType, const char* fileName);
	//finalize shader program
	bool finalize(void);

	//获取一个uniform类型变量的id
	GLint getUniformLocation(const char* pUniformName);
	//获取一个attribute类型变量的id
	GLint getAttribLocation(const char* pAttribName);
	
private:
	GLuint m_shaderProgram;
	ShaderObjList m_shaderObjList;
};
}

#endif
