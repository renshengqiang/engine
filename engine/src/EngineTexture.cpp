#include "EngineTexture.h"
#include <ImageMagick/Magick++.h>
#include <assert.h>
#include <stdio.h>

namespace Engine
{
Texture::Texture(const std::string &name):
	m_fileName(name),
	m_textureTarget(GL_TEXTURE_2D),
	m_textureUnit(GL_TEXTURE0),
	m_loaded(false)
{
}
bool Texture::load()
{
	try 
	{
		Magick::Image *pImage;
		Magick::Blob *pBlob;

		//�Ӵ����ж�ȡ����ͼ��
		pImage = new Magick::Image(m_fileName);
		pBlob = new Magick::Blob();
		pImage->write(pBlob, "RGBA");
		fprintf(stdout, "Loading texture: %s\n", m_fileName.c_str());

		//�����������������������
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);
		glTexImage2D(m_textureTarget, 0, GL_RGB, pImage->columns(), pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, pBlob->data());
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//ɾ���м�����
		delete pBlob;
		delete pImage;

		m_loaded =true;
	}
	catch (Magick::Error& error)
	{
		fprintf(stderr, "Error loading texture: %s, %s\n", m_fileName.c_str(), error.what());
		return false;
	}
	return true;
}
void Texture::activate()
{
	assert(m_loaded == true);

	glActiveTexture(m_textureUnit);
	glBindTexture(m_textureTarget, m_textureObj);	
}
}
