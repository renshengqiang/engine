#include "EngineRenderWindow.h"
#include "EngineRenderOp.h"
#include <SDL/SDL.h>
#include <stdio.h>

namespace Engine
{
bool RenderWindow::initWindow(void)
{
	/* Information about the current video settings. */    
	const SDL_VideoInfo* info = NULL;
	
	/* Color depth in bits of our window. */    
	int bpp = 0;
	/* Flags we will pass into SDL_SetVideoMode. */
	int flags = 0;
	
	/* First, initialize SDL's video subsystem. */    
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
	{  
		/* Failed, exit. */        
		fprintf( stderr, "Video initialization failed: %s\n",SDL_GetError());    
		destroy();
		return false;
	}
	
	/* Let's get some video information. */    
	info = SDL_GetVideoInfo( );    
	if( !info ) 
	{
		/* This should probably never happen. */        
		fprintf( stderr, "Video query failed: %s\n",SDL_GetError());        
		destroy();
		return false;
	}
	bpp = info->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
	
	flags = SDL_OPENGL;
	/*Set the video mode     */    
	if( SDL_SetVideoMode(m_width, m_height, bpp, flags ) == 0 ) 
	{
		fprintf( stderr, "Video mode set failed: %s\n",SDL_GetError());        
		destroy();
		return false;
	}

	SDL_WM_SetCaption(m_title.c_str(), 0);
	//initial glew
	if(InitGlew()==false)
	{
		destroy();
		return false;
	}

	//initial rendering state
	if(InitRenderState() == false)
	{
		destroy();
		return false;
	}
	
	//show the OpenGL library version info , this is window-system specified
	GetGLInfo();
	
	return true;
}
//-----------------------------------------------------------------------
void RenderWindow::destroy()
{
	SDL_Quit();
}
//-----------------------------------------------------------------------
void RenderWindow::swapBuffer()
{
	SDL_GL_SwapBuffers();
	return;
}
}
