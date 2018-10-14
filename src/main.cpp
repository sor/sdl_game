#include <iostream>
#include <stdio.h>

#include <SDL.hpp>
#include <SDL_ttf.hpp>
#include <SDL_image.hpp>

namespace TTF = SDL::TTF;
namespace IMG = SDL::IMG;

extern "C"
int main( int argc, char* argv[] )
try
{
	bool running = true;
	std::cout << "Hello, World!" << std::endl;

	SDL::Init     init;
	SDL::Window   window;
	SDL::Renderer renderer;

	TTF::Init ttf_init;
	TTF::Font font;

	IMG::Init img_init;
	SDL::Texture tex;

	init.initialize( SDL::Init::Flags::VIDEO );
	ttf_init.initialize();
	img_init.initialize( IMG::Init::Flags::PNG );

	// Create an application window with the following settings:
	window = SDL::Window(
		"An SDL2 window",
		SDL::Window::Pos::CENTERED, // initial x position
		SDL::Window::Pos::CENTERED, // initial y position
		800,                        // width, in pixels
		600,                        // height, in pixels
		SDL::Window::Flags::OPENGL |
		SDL::Window::Flags::SHOWN
	);

	// Check that the window was successfully created
	if( !window )
	{
		// In the case that the window could not be made...
		printf( "Could not create window: %s\n", SDL::Error() );
		return 1;
	}

	renderer = SDL::Renderer(
		window,
		-1,
		SDL::Renderer::Flags::ACCELERATED |
		SDL::Renderer::Flags::PRESENTVSYNC
	);

	renderer.SetDrawColor( SDL::Color( 255, 0, 0, 255 ) );

	font = TTF::Font( "../assets/fonts/RobotoSlab-Bold.ttf", 16 );

	tex = IMG::LoadTexture( renderer, "../assets/graphics/streets.png" );

	while( running )
	{
		SDL::Event::Pump();
		SDL::Event evt;
		while( SDL::Event::Poll( evt ) )
		{
			switch( evt.type() )
			{
				case SDL::Event::Type::KEYDOWN:
				{
					auto        kd       = evt.Keyboard();
					SDL::Keysym what_key = kd.keysym;

					if( (what_key.mod() & SDL::Keymod::ALT) != SDL::Keymod::NONE &&
						(what_key.scancode() == SDL::Scancode::F4)
						||
						what_key.scancode() == SDL::Scancode::ESCAPE )
					{
						SDL::Event( SDL::Event::Type::QUIT ).Push();
					}

					break;
				}

				case SDL::Event::Type::QUIT:
				{
					running = false;
					//exit( 0 );
				}
			}
		}

		renderer.ClearColor( 90, 90, 90 );
		renderer.SetDrawColor( rand() % 256, rand() % 256, rand() % 256 );
		renderer.FillRect( SDL::Rect( rand() % 384, rand() % 384, 128, 128 ) );

		tex.Draw( SDL::Point( 500, 50 ) );

		const char* text = "Hello World, Test Teﬆ üößðđŋ↓Æ§Ðß´ĸĦŊ‘©><\u0032";
		{
			SDL::Color   c( 0, 0, 0 );
			SDL::Surface surf = font.RenderUTF8_Blended( text, c );
			SDL::Texture t2( renderer, surf );
			t2.Draw( SDL::Point( 61, 49 ) );
		}
		{
			SDL::Color   c( 255, 255, 255 );
			SDL::Surface surf = font.RenderUTF8_Blended( text, c );
			SDL::Texture t2( renderer, surf );
			t2.Draw( SDL::Point( 60, 50 ) );
		}

		renderer.Present();
	}
	//SDL::Delay( 3000 ); // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	//SDL_DestroyWindow( window );

	// Clean up
	//SDL_Quit();
	return 0;
}
catch( SDL::Exception e )
{
	char cCurrentPath[FILENAME_MAX];
	_getcwd(cCurrentPath, sizeof(cCurrentPath));
	std::cout << "Current working directory: " << cCurrentPath << std::endl;

	std::cout
		<< "Aborting program: " << e.Text()
		<< " error code: " << e.Code() << std::endl;

	// put a breakpoint in this line, to prevent the shell from closing
	return e.Code();
}
catch( ... )
{
	std::cout << "Non-SDL Exception has occured!" << std::endl;
	// 
	// put a breakpoint in this line, to prevent the shell from closing
	return -999;
}
