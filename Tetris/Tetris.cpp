// precompiled header file
#include "pch.h"

#pragma region generalDirectives
// SDL libs
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL extension libs 
#pragma comment(lib, "SDL2_image.lib") // Library to load image files
#pragma comment(lib, "SDL2_ttf.lib") // Library to use fonts

// SDL and OpenGL Includes
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <SDL_image.h> // png loading
#include <SDL_ttf.h> // Font
#pragma endregion generalDirectives

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

#include "structs.h"

#pragma region windowInformation
const float g_WindowWidth{ 1280.0f };
const float g_WindowHeight{ 720.0f };
const std::string g_WindowTitle{ "Project name - Name, firstname - 1DAExx" };
bool g_IsVSyncOn{ true };
#pragma endregion windowInformation

#pragma region textureDeclarations
struct Texture
{
	GLuint id;
	float width;
	float height;
};

bool TextureFromFile(const std::string& path, Texture & texture);
bool TextureFromString(const std::string & text, TTF_Font *pFont, const Color4f & textColor, Texture & texture);
bool TextureFromString(const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture);
void TextureFromSurface(const SDL_Surface *pSurface, Texture & textureData);
void DrawTexture(const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & sourceRect = {});
void DrawTexture(const Texture & texture, const Rectf & destinationRect, const Rectf & sourceRect = {});
void DeleteTexture(Texture & texture);
#pragma endregion textureDeclarations

#pragma region coreDeclarations
// Functions
void Initialize( );
void Run( );
void Cleanup( );
void QuitOnSDLError( );
void QuitOnOpenGlError( );
void QuitOnImageError();
void QuitOnTtfError();

// Variables
SDL_Window* g_pWindow{ nullptr }; // The window we'll be rendering to
SDL_GLContext g_pContext; // OpenGL context
Uint32 g_MilliSeconds{};
const Uint32 g_MaxElapsedTime{ 100 };
#pragma endregion coreDeclarations

#pragma region gameDeclarations
// Functions
void Update( float elapsedSec );
void Draw( );
void ClearBackground( );
void InitGameResources();
void FreeGameResources();
void ProcessKeyDownEvent(const SDL_KeyboardEvent  & e);
void ProcessKeyUpEvent(const SDL_KeyboardEvent  & e);
void ProcessMouseMotionEvent(const SDL_MouseMotionEvent & e);
void ProcessMouseDownEvent(const SDL_MouseButtonEvent & e);
void ProcessMouseUpEvent(const SDL_MouseButtonEvent & e);
// Variables
#pragma endregion gameDeclarations


int main( int argc, char* args[] )
{
	// seed the pseudo random number generator
	srand(unsigned int(time(nullptr)));	
	
	// Initialize SDL and OpenGL
	Initialize( );

	// Event loop
	Run( );

	// Clean up SDL and OpenGL
	Cleanup( );

	return 0;
}

#pragma region gameImplementations
void InitGameResources()
{

}

void FreeGameResources()
{

}

void ProcessKeyDownEvent(const SDL_KeyboardEvent  & e)
{

}

void ProcessKeyUpEvent(const SDL_KeyboardEvent  & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		//std::cout << "Left arrow key released\n";
		break;
	case SDLK_RIGHT:
		//std::cout << "`Right arrow key released\n";
		break;
	case SDLK_1:
	case SDLK_KP_1:
		//std::cout << "Key 1 released\n";
		break;
	}
}

void ProcessMouseMotionEvent(const SDL_MouseMotionEvent & e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}

void ProcessMouseDownEvent(const SDL_MouseButtonEvent & e)
{

}

void ProcessMouseUpEvent(const SDL_MouseButtonEvent & e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
	{
		//std::cout << "Left mouse button released\n";
		//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
		break;
	}
	case SDL_BUTTON_RIGHT:
		//std::cout << "Right mouse button released\n";
		break;
	case SDL_BUTTON_MIDDLE:
		//std::cout << "Middle mouse button released\n";
		break;
	}
}

void Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Draw( )
{
	ClearBackground( );
}

void ClearBackground( )
{
	glClearColor( 185.0f / 255.0f, 211.0f / 255.0f, 238.0f / 255.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
#pragma endregion gameImplementations

#pragma region coreImplementations
void Initialize( )
{
	//Initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		QuitOnSDLError( );
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	//Create window
	g_pWindow = SDL_CreateWindow(
		g_WindowTitle.c_str( ),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int( g_WindowWidth ),
		int( g_WindowHeight ),
		SDL_WINDOW_OPENGL );

	if ( g_pWindow == nullptr )
	{
		QuitOnSDLError( );
	}

	// Create an opengl context and attach it to the window 
	g_pContext = SDL_GL_CreateContext( g_pWindow );
	if ( g_pContext == nullptr )
	{
		QuitOnSDLError( );
	}

	if ( g_IsVSyncOn )
	{
		// Synchronize buffer swap with the monitor's vertical refresh
		if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
			QuitOnSDLError( );
		}
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	// Initialize Projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	// Set the clipping (viewing) area's left, right, bottom and top
	gluOrtho2D( 0, g_WindowWidth, 0, g_WindowHeight );

	// The viewport is the rectangular region of the window where the image is drawn.
	// Set it to the entire client area of the created window
	glViewport( 0, 0, int( g_WindowWidth ), int( g_WindowHeight ) );

	//Initialize Modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// Enable color blending and use alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		QuitOnImageError();
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		QuitOnTtfError();
	}

}
void Run( )
{
	//Main loop flag
	bool quit{ false };

	// Set start time
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	InitGameResources();
	
	//The event loop
	SDL_Event e{};
	while ( !quit )
	{
		// Poll next event from queue
		while ( SDL_PollEvent( &e ) != 0 )
		{
			// Handle the polled event
			switch ( e.type )
			{
			case SDL_QUIT:
				//std::cout << "\nSDL_QUIT\n";
				quit = true;
				break;
			case SDL_KEYDOWN:
				ProcessKeyDownEvent(e.key);
				break;
			case SDL_KEYUP:
				ProcessKeyUpEvent(e.key);
				break;
			case SDL_MOUSEMOTION:
				ProcessMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				ProcessMouseDownEvent(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				ProcessMouseUpEvent(e.button);
				break;
			default:
				//std::cout << "\nSome other event\n";
				break;
			}
		}

		if ( !quit )
		{

			// Calculate elapsed time
			// Get current time
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
			// Calculate elapsed time
			float elapsedSeconds = std::chrono::duration<float>(t2 - t1).count();
			// Update current time
			t1 = t2;
			// Prevent jumps in time caused by break points
			if (elapsedSeconds > g_MaxElapsedTime)
			{
				elapsedSeconds = g_MaxElapsedTime;
			}

			// Call update function, using time in seconds (!)
			Update(elapsedSeconds);

			// Draw in the back buffer
			Draw( );

			// Update screen: swap back and front buffer
			SDL_GL_SwapWindow( g_pWindow );
		}
	}
	FreeGameResources( );
}

void Cleanup( )
{
	SDL_GL_DeleteContext( g_pContext );

	SDL_DestroyWindow( g_pWindow );
	g_pWindow = nullptr;

	SDL_Quit( );
}

void QuitOnSDLError( )
{
	std::cout << "Problem during SDL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}

void QuitOnOpenGlError( )
{
	std::cout << "Problem during OpenGL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}

void QuitOnImageError()
{
	std::cout << "Problem during SDL_image initialization: ";
	std::cout << IMG_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}

void QuitOnTtfError()
{
	std::cout << "Problem during SDL_ttf initialization: ";
	std::cout << TTF_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}
#pragma endregion coreImplementations

#pragma region textureImplementations

bool TextureFromFile(const std::string& path, Texture & texture)
{
	//Load file for use as an image in a new surface.
	SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
		return false;
	}

	TextureFromSurface(pLoadedSurface, texture);

	//Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

bool TextureFromString(const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture)
{
	// Create font
	TTF_Font *pFont{};
	pFont = TTF_OpenFont(fontPath.c_str(), ptSize);
	if (pFont == nullptr)
	{
		std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
		std::cin.get();
		return false;
	}

	// Create texture using this fontand close font afterwards
	bool textureOk = TextureFromString(text, pFont, textColor, texture);
	TTF_CloseFont(pFont);

	return textureOk;
}

bool TextureFromString(const std::string & text, TTF_Font *pFont, const Color4f & color, Texture & texture)
{
	//Render text surface
	SDL_Color textColor{};
	textColor.r = Uint8(color.r * 255);
	textColor.g = Uint8(color.g * 255);
	textColor.b = Uint8(color.b * 255);
	textColor.a = Uint8(color.a * 255);

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
	//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
		return false;
	}

	// copy to video memory
	TextureFromSurface(pLoadedSurface, texture);

	//Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

void TextureFromSurface(const SDL_Surface *pSurface, Texture & texture)
{
	//Get image dimensions
	texture.width = float(pSurface->w);
	texture.height = float(pSurface->h);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 3:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::cerr << "TextureFromSurface error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";;
		texture.width = 0;
		texture.height = 0;
		return;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &texture.id);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, texture.id);

	// check for errors.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		std::cerr << "TextureFromSurface, error binding textures, Error id = " << e << '\n';
		texture.width = 0;
		texture.height = 0;
		return;
	}

	//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
	//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//         GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//      surface->w:    The width of the texture
	//      surface->h:    The height of the texture
	//               0:    The border.  Don't worry about this if you're just starting.
	//     pixelFormat:    The format that the *data* is in--NOT the texture! 
	//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	// surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void DeleteTexture(Texture & texture)
{
	glDeleteTextures(1, &texture.id);
}

void DrawTexture(const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & sourceRect)
{
	Rectf destinationRect{ bottomLeftVertex.x, bottomLeftVertex.y, sourceRect.width, sourceRect.height };
	DrawTexture(texture, destinationRect, sourceRect);
}

void DrawTexture(const Texture & texture, const Rectf & destinationRect, const Rectf & sourceRect)
{
	// Determine texture coordinates, default values = draw complete texture
	float textLeft{};
	float textRight{1.0f};
	float textTop{};
	float textBottom{1.0f};
	if (sourceRect.width > 0.0f && sourceRect.height > 0.0f) // Clip specified, convert them to the range [0.0, 1.0]
	{
		textLeft = sourceRect.left / texture.width;
		textRight = (sourceRect.left + sourceRect.width) / texture.width;
		textTop = (sourceRect.bottom - sourceRect.height) / texture.height;
		textBottom = sourceRect.bottom / texture.height;
	}

	// Determine vertex coordinates
	float vertexLeft{ destinationRect.left };
	float vertexBottom{ destinationRect.bottom };
	float vertexRight{};
	float vertexTop{};
	if (!(destinationRect.width > 0.0f && destinationRect.height > 0.0f)) // If no size specified use size of texture
	{
		vertexRight = vertexLeft + texture.width;
		vertexTop = vertexBottom + texture.height;
	}
	else
	{
		vertexRight = vertexLeft + destinationRect.width;
		vertexTop = vertexBottom + destinationRect.height;

	}

	// Tell opengl which texture we will use
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw
	glEnable(GL_TEXTURE_2D);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(textLeft, textBottom);
			glVertex2f(vertexLeft, vertexBottom);

			glTexCoord2f(textLeft, textTop);
			glVertex2f(vertexLeft, vertexTop);

			glTexCoord2f(textRight, textTop);
			glVertex2f(vertexRight, vertexTop);

			glTexCoord2f(textRight, textBottom);
			glVertex2f(vertexRight, vertexBottom);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

}
#pragma endregion textureImplementations