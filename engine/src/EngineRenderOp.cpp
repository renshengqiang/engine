#include "EngineRenderOp.h"
#include "EngineSimpleMeshEffect.h"
//#include "EngineParticleEffect.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef GLenum DataType;
typedef short DataSize;

//------------------------------------------------------------------------------
//                                                          INDEX_OBJ
//------------------------------------------------------------------------------
class IndexObj 
{
public:
	GLsizei n;				// 索引缓冲区对象中的总共索引个数
	DataType dataType;		//索引的数据类型
	GLsizei offset;			//
	GLuint object;			//索引缓冲区对象的硬件索引id

	IndexObj():
		object(0)
	{
	}
};

INDEX_OBJ* CreateIndexObject(int n, const unsigned int *p)
{
	IndexObj *obj = (struct IndexObj*)malloc(sizeof(struct IndexObj));	
	if(obj!=NULL)
	{
		glGenBuffers(1, &(obj->object));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->object);
		obj->n = n;
		obj->dataType = GL_UNSIGNED_INT;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n*sizeof(unsigned int), p, GL_STATIC_DRAW);
	}
	return obj;
}

void DestroyIndexObject(INDEX_OBJ *indexObject)
{
	if(indexObject && indexObject->object!=INVALID_OBJECT_LOCATION)
	{
		glDeleteBuffers(1, &(indexObject->object));
	}
	else
	{
		fprintf(stderr, "DestroyIndexObject : Not a valid INDEX_OBJ\n");
	}
}

//------------------------------------------------------------------------------
//                                                          VERTEX_OBJ
//------------------------------------------------------------------------------
/*
	该类表示VertexObj中每种数据的维数(个数)和在缓冲区的偏移量
	*Num:表示该数据的维数，为0表示没有
	*Offset:表示该数据在结构中的字节偏移量
*/
class ElementInfo
{
public:
	DataSize coordNum;
	DataSize coordOffset;
	
	DataSize colorNum;
	DataSize colorOffset;
	
	DataSize color2Num;
	DataSize color2Offset;

	DataSize normalNum;				//0 or 3
	DataSize normalOffset;

	DataSize textureCoordNum;
	DataSize textureCoordOffset;

	DataSize textureCoord2Num;
	DataSize textureCoord2Offset;

	DataSize boneSize;					//0 or 1
	DataSize boneOffset;
};
class MeshData
{
public:
	void *meshData;					//具体的顶点数据
	int pushedNum; 					//已经写入的顶点数量(用于单个顶点逐渐push case)
};
class VertexObj
{
public:
	GLuint object;						//缓冲区对象的id
	GLuint replaceObject;				//用于替换的缓冲区对象id
	GLsizei numVertex;					//顶点的数量
	DataSize stride;					//顶点的跨距(字节表示)
	DataType dataType;					//GL_FLOAT OR GL_DOUBLE
	MeshData meshData;				//顶点的数据
	ElementInfo elementInfo;			//顶点数据的描述结构
};

void _GetGLErrorInfo()
{
	int errorId = glGetError();
	switch (errorId)
	{
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			printf ("GL Invalid Enum\n");
			return;
		case GL_INVALID_VALUE:
			printf ("GL Invalid Value\n");
			return;
		case GL_INVALID_OPERATION:
			printf ("GL Invalid Operation\n");
			return;
		case GL_OUT_OF_MEMORY:
			printf ("GL Out Of Memory\n");
			return;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf ("GL Invalid FrameBuffer Operation\n");
			return;
		case  GL_STACK_OVERFLOW:
			printf ("GL Stack Overflow\n");
			return;
		case GL_STACK_UNDERFLOW:
			printf ("GL Stack Underflow\n");
			return;
		case GL_TABLE_TOO_LARGE:
			printf ("GL Table Too Large\n");
			return;
		default:
			printf("GL Undefined Error\n");
			return;
	};
}

//--------------------------------------------------------------------------
/**
	通过分析elementMask掩码填充elementInfo结构
	返回表示elementMask形式表示的顶点需要多少字节的数据
*/
int  _ResolveElement(struct ElementInfo *elementInfo, int elementMask)
{
	int dataOffset = 0, stride ;
	
	elementInfo->coordOffset = 0;
	switch(elementMask&COORD_MASK)
	{
		case COORD_3:
			dataOffset += 3;
			elementInfo->coordNum = 3;
			break;
		case COORD_4:
			dataOffset += 4;
			elementInfo->coordNum= 4;
			break;
		default:
			elementInfo->coordNum = 0;
			break;
	}
	
	elementInfo->colorOffset = dataOffset * sizeof(float);
	switch(elementMask&COLOR_MASK)
	{
		case COLOR_3:
			dataOffset+=3;
			elementInfo->colorNum = 3;
			break;
		case COLOR_4:
			dataOffset+=4;
			elementInfo->colorNum = 4;
			break;
		default:
			elementInfo->colorNum = 0;
			break;
	}
	
	elementInfo->color2Offset = dataOffset * sizeof(float);
	switch(elementMask&COLOR2_MASK)
	{
		case COLOR2_3:
			dataOffset+=3;
			elementInfo->color2Num = 3;
			break;
		case COLOR2_4:
			dataOffset+=4;
			elementInfo->color2Num = 4;
			break;
		default:
			elementInfo->color2Num = 0;
			break;
	}

	elementInfo->normalOffset = dataOffset * sizeof(float);
	switch(elementMask&NORMAL_MASK)
	{
		case NORMAL_3:
			dataOffset+=3;
			elementInfo->normalNum = 3;
			break;
		default:
			elementInfo->normalNum = 0;
			break;
	}

	elementInfo->textureCoordOffset = dataOffset*sizeof(float);
	switch(elementMask&TEXTURE_MASK)
	{
		case TEXTURE_2:
			dataOffset+=2;
			elementInfo->textureCoordNum = 2;
			break;
		case TEXTURE_3:
			dataOffset+=3;
			elementInfo->textureCoordNum = 3;
			break;
		default:
			elementInfo->textureCoordNum = 0;
			break;
	}
	
	elementInfo->textureCoord2Offset = dataOffset*sizeof(float);
	switch(elementMask&TEXTURE2_MASK)
	{
		case TEXTURE2_2:
			dataOffset+=2;
			elementInfo->textureCoord2Num = 2;
			break;
		case TEXTURE2_3:
			dataOffset+=3;
			elementInfo->textureCoord2Num = 3;
			break;
		default:
			elementInfo->textureCoord2Num = 0;
			break;
	}
	elementInfo->boneOffset = dataOffset*sizeof(float);
	if((elementMask&BONE_MASK) == BONE_4)
	{
		elementInfo->boneSize  = 1;
		stride = (dataOffset+4) * sizeof(float) + 4*sizeof(float);//boneid是float类型的
	}
	else
	{
		elementInfo->boneSize  = 0;
		stride = dataOffset*sizeof(float);
	}
	return stride;
}
//--------------------------------------------------------------------------
/**
	根据给定的顶点数创建一个VertexObject
	返回NULL表示内存错误
*/
VERTEX_OBJ* CreateVertexObject(int elements,  int n)
{
	int stride;
	struct VertexObj *obj = (struct VertexObj *)malloc(sizeof(struct VertexObj));
	
	if(obj!=NULL)
	{
		obj->numVertex = n;
		obj->dataType = GL_FLOAT;
		stride = _ResolveElement(&(obj->elementInfo), elements);
		obj->stride = stride;
		obj->replaceObject = INVALID_OBJECT_LOCATION;
		obj->object = INVALID_OBJECT_LOCATION;
		obj->meshData.meshData = malloc(stride * n);
		obj->meshData.pushedNum = -1;
		
#ifndef _BUGING_
		printf("num %d stride %d\n", obj->numVertex, obj->stride);
#endif
		if(obj->meshData.meshData== NULL)
		{
			fprintf(stderr, "CreateVertexObject : memery allocation error\n");
			free(obj);
			return NULL;
		}
		return obj;
	}
	else
	{
		fprintf(stderr, "CreateVertexObject : memory allocation error\n");
		return NULL;
	}
}
//-----------------------------------------------------------------------------
/**
	@remarks : 
		check if vobj is a valid VertexObject
	@return value:
		1 for success
		0 for failure
*/
int _CheckVertexObject(VERTEX_OBJ *vobj)
{
	if(vobj == NULL || vobj->meshData.meshData == NULL)
	{
		return 0;
	}
	return 1;
}
//-----------------------------------------------------------------------------
/*
	根据给定的VertexObject查找制定的element在顶点中的位移
*/
void _GetElementSizeOffset(VERTEX_OBJ *vobj, int element, int *offset, int *size)
{
	switch(element)
	{
		case COORD_3:
			*offset = vobj->elementInfo.coordOffset;
			*size = 3 * sizeof(float);
			break;
		case COORD_4:
			*offset = vobj->elementInfo.coordOffset;
			*size = 4 * sizeof(float);
			break;
		case COLOR_3:
			*offset = vobj->elementInfo.colorOffset;
			*size = 3 * sizeof(float);
			break;
		case COLOR_4:
			*offset = vobj->elementInfo.colorOffset;
			*size = 4 * sizeof(float);
			break;
		case COLOR2_3:
			*offset = vobj->elementInfo.color2Offset;
			*size = 3 * sizeof(float);
			break;
		case COLOR2_4:
			*offset = vobj->elementInfo.color2Offset;
			*size = 4 * sizeof(float);
			break;
		case NORMAL_3:
			*offset = vobj->elementInfo.normalOffset;
			*size = 3 * sizeof(float);
			break;
		case TEXTURE_2:
			*offset = vobj->elementInfo.textureCoordOffset;
			*size = 2 * sizeof(float);
			break;
		case TEXTURE_3:
			*offset = vobj->elementInfo.textureCoordOffset;
			*size = 3 * sizeof(float);
			break;
		case TEXTURE2_2:
			*offset = vobj->elementInfo.textureCoord2Offset;
			*size = 2 * sizeof(float);
			break;
		case TEXTURE2_3:
			*offset = vobj->elementInfo.textureCoord2Offset;
			*size = 3 * sizeof(float);
			break;
		case BONE_4:
			*offset = vobj->elementInfo.boneOffset;
			*size = 4*(sizeof(float) + sizeof(float));
			break;
	}
	return;
}
//---------------------------------------------------------------------------------------
void VertexObjectPushElementAll(VERTEX_OBJ *vobj, int element, float *p)
{
	void *destPos,*srcPos;
	int offset, size, i;
	if(_CheckVertexObject(vobj)==0)
	{
		fprintf(stderr, "VertexObjectPushElement : Not a valid VERTEX_OBJ\n");
		return;
	}
	_GetElementSizeOffset(vobj, element, &offset, &size);
	for(i=0; i<vobj->numVertex; ++i)
	{
		destPos = (char *)vobj->meshData.meshData + vobj->stride * i + offset;
		srcPos = (char *)p + size * i;
		memcpy(destPos, srcPos, size);
	}
	return;
}
//---------------------------------------------------------------------------------------
void VertexObjectEnd(VERTEX_OBJ *vobj)
{
	if(_CheckVertexObject(vobj)==0)
	{
		fprintf(stderr, "VertexObjectPushElement : Not a valid VERTEX_OBJ\n");
		return;
	}
	glGenBuffers(1, &(vobj->object));
	glBindBuffer(GL_ARRAY_BUFFER, vobj->object);	
	//glBufferData(GL_ARRAY_BUFFER, vobj->numVertex*vobj->stride,  vobj->meshData.meshData, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vobj->numVertex*vobj->stride,  vobj->meshData.meshData, GL_STATIC_DRAW);
	free(vobj->meshData.meshData);
	vobj->meshData.meshData = NULL;
	vobj->meshData.pushedNum = 0;
	return;
}
//-----------------------------------------------------------------------------
void DestroyVertexObject(VERTEX_OBJ *vertexObject)
{
	if(vertexObject )
	{
		if(vertexObject->object != INVALID_OBJECT_LOCATION)
			glDeleteBuffers(1, &(vertexObject->object));
		if(vertexObject->object != INVALID_OBJECT_LOCATION)
			glDeleteBuffers(1, &(vertexObject->replaceObject));
	}
	else
	{
		fprintf(stderr, "DestroyVertexObject : Not a valid VERTEX_OBJ\n");
	}
	return;
}


//------------------------------------------------------------------------------
//                                                          PIXEL_OBJ
//------------------------------------------------------------------------------

class PixelObj
{
public:
	GLuint object;						//缓冲区对象的id
	GLenum textureTarget;				//纹理目标: GL_TEXTURE_2D, GL_TEXTURE_3D...
	GLenum textureUnit;				//纹理单元:GL_TEXTURE0, GL_TEXTURE1...
};

/// 根据纹理图像文件创建纹理缓冲区对象
PIXEL_OBJ *CreatePixelObject(const char *file)
{
	struct PixelObj *obj  = (struct PixelObj *)malloc(sizeof(struct PixelObj));
	if(obj!=NULL)
	{
		//Texture *texture = new Texture(textureTarget, file);
		//texture->Load();
		//obj->object = texture->GetTextureObj();
		obj->textureUnit = GL_TEXTURE0;
		obj->textureTarget = GL_TEXTURE_2D;
	}
	return obj;
}
/// 根据已有的纹理缓冲区对象创建缓冲区对象数据结构
PIXEL_OBJ *CreatePixelObject2(const Engine::Texture *pTexture)
{
	struct PixelObj *pixelObject = (struct PixelObj *)malloc(sizeof(struct PixelObj));
	if(pixelObject!=NULL)
	{
		pixelObject->object = pTexture->getTextureObj();
		pixelObject->textureUnit = pTexture->getTextureUnit();
		pixelObject->textureTarget = pTexture->getTextureTarget();
	}
	return pixelObject;
}

//------------------------------------------------------------------------------
//                                                          Render_Op
//------------------------------------------------------------------------------
void DrawSimpleMesh(const INDEX_OBJ *indexObj, 
					const VERTEX_OBJ *vertexObj, 
					const PIXEL_OBJ *pixelObj, 
					Engine::SimpleMeshEffect &effect)
{
	effect.enableVertexArray();
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexObj->object);

	effect.setPositionAttribPointer(vertexObj->elementInfo.coordNum, vertexObj->stride, vertexObj->elementInfo.coordOffset);
	effect.setTextureCoordAttribPointer(vertexObj->elementInfo.textureCoordNum, vertexObj->stride, vertexObj->elementInfo.textureCoordOffset);
	effect.setBoneIdAttribPointer(4, vertexObj->stride, vertexObj->elementInfo.boneOffset);
	effect.setBoneWeightAttribPointer(4, vertexObj->stride, vertexObj->elementInfo.boneOffset + 4*sizeof(float));

	effect.setTextureUnit(pixelObj->textureUnit - GL_TEXTURE0);
	
	glActiveTexture(pixelObj->textureUnit);
	glBindTexture(pixelObj->textureTarget, pixelObj->object);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexObj->object);
	glDrawElements(GL_TRIANGLES, indexObj->n, indexObj->dataType, 0);
	
	effect.disableVertexArray();
}

//-----------------------------------------------------------------------
/*
void DrawParticle(const INDEX_OBJ *indexObj, const VERTEX_OBJ *vertexObj, const PIXEL_OBJ *pixelObj, ParticleEffect& effect)
{
	effect.enableVertexArray();
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexObj->object);

	effect.setPositionAttribPointer(vertexObj->elementInfo.coordNum, vertexObj->stride, vertexObj->elementInfo.coordOffset);
	effect.setTextureCoordAttribPointer(vertexObj->elementInfo.textureCoordNum, vertexObj->stride, vertexObj->elementInfo.textureCoordOffset);
	effect.setVelocityAttribPointer(vertexObj->elementInfo.colorNum, vertexObj->stride, vertexObj->elementInfo.colorOffset);

	effect.setTextureUnit(pixelObj->textureUnit - GL_TEXTURE0);
	glActiveTexture(pixelObj->textureUnit);
	glBindTexture(pixelObj->textureTarget, pixelObj->object);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexObj->object);
	glDrawElements(GL_TRIANGLES, indexObj->n, indexObj->dataType, 0);

	effect.disableVertexArray();
}
*/
//-----------------------------------------------------------------------
bool InitGlew(void)
{
	//init glew
	GLenum res = glewInit();
	if(res != GLEW_OK){
		fprintf(stderr, "InitGlew: glew init failure\n");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------
void GetGLInfo(void)
{
	GLint depth,units;
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&depth);
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
	printf("----------- OpenGL Version Info -----------\n"
	        "Opengl version: %s\n""Vendor version: %s\n"
	        "Shading language version: %s\n"
	        "Extentions info:%s\n"
	        "ModelView matrix stack depth:%d\n"
	        "Max texture units:%d\n"
	       "-----------------------------------------\n\n",
	       glGetString(GL_VERSION),
	       glGetString(GL_VENDOR), 
	       glGetString(GL_SHADING_LANGUAGE_VERSION),
	       glGetString(GL_EXTENSIONS),
	       depth,
	       units
	       );
}

//-----------------------------------------------------------------------
bool InitRenderState(void)
{	
	//-- Enable smooth shading
	glShadeModel(GL_SMOOTH);

	//-- Set the background black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//-- Enables Depth Test
	glEnable(GL_DEPTH_TEST);

	//-- The Type of Depth Test To Do
	glDepthFunc(GL_LEQUAL);

	//-- Really Nice Perspective Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

//-----------------------------------------------------------------------
void ClearBuffer(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
