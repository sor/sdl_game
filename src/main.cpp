#include <iostream>
#include <stdio.h>

#include "global.h"

#include "gamebase.h"
#include "intro.h"
#include "editor.h"
#include "play.h"

class MyGame : public Game
{
public:
	MyGame()
		: Game( Rect( 1024, 720 ), false )
	{
		allStates[0] = new IntroState( *this, renderer );
		allStates[1] = new EditorState( *this, renderer );
		allStates[2] = new PlayState( *this, renderer );
		SetNextState( 0 );
	}

	virtual ~MyGame()
	{
		for( auto state : allStates )
			if( state != nullptr )
				delete state;
	}
};

extern "C"
int main( int argc, char* argv[] )
try
{
	MyGame game;
	return game.Run();
}
catch( SDL::Exception& e )
{
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
