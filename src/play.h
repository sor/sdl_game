#pragma once
#ifndef PLAY_H
#define PLAY_H

#include "global.h"
#include "gamebase.h"

class PlayState : public GameState
{
	const int TILESIZE = 32;

	// ctor
	using GameState::GameState;

	//virtual bool IsFPSLimited() const override { return false; }

	Font	font;
	Texture	spriteSheet;

	float v_x = 0.0f, v_y = 0.0f;
	float p_x = 100.0f, p_y = 100.0f;

	bool	isPainting = false;
	bool	isPanning  = false;
	bool	isDrag     = true;
	bool	isJumping  = false;
	bool	isGravity  = true;
	bool	isCollision = true;

	Array<Array<char, 30>, 20> level = {
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x...........................x",
		"x.................xxxxxxxxxxx",
		"x.........xxxxx...x",
		"x.........x.x.x...x",
		"x.........xxxxxxxxx",
		"x.................x",
		"x.................x",
		"x..xxxxxxxxxxx....x",
		"x..xxxxxxxxxxx....x",
		"x..xxxxxxxxxxx....x",
		"xxxxxxxxxxxxxxxxxxx",
	};
	Point camera;

public:

	virtual void Init() override
	{
		if( !font )
			font = TTF::Font( "../assets/fonts/RobotoSlab-Bold.ttf", 16 );

		if( !spriteSheet )
			spriteSheet = IMG::LoadTexture( renderer, "../assets/graphics/merc.png" );

		camera = Point( 50, 50 );
	}

	virtual void Uninit() override
	{
		// for example, do not unload, just keep it loaded
	}

	virtual void Events(const int frame, const float deltaT) override
	{
		Event::Pump();
		Event evt;
		while (Event::Poll(evt))
		{
			if (game.HandleEvent(evt))
				continue;

			switch (evt.type())
			{
			case Event::Type::KEYDOWN:
			{
				auto        kd = evt.Keyboard();
				SDL::Keysym what_key = kd.keysym;
				if (what_key.scancode() == SDL::Scancode::ESCAPE)
				{
					game.SetNextState(0); // back to intro
				}
				else if (what_key.scancode() == SDL::Scancode::F1 && kd.repeat == 0)
				{
					isGravity = !isGravity;
				}
				else if (what_key.scancode() == SDL::Scancode::F2 && kd.repeat == 0)
				{
					isDrag = !isDrag;
				}
				else if (what_key.scancode() == SDL::Scancode::F3 && kd.repeat == 0)
				{
					isCollision = !isCollision;
				}
				else if (what_key.scancode() == SDL::Scancode::SPACE && kd.repeat == 0)
				{
					isJumping = true;
				}
				break;
			}

			case Event::Type::MOUSEBUTTONDOWN:
			{
				auto me = evt.MouseButton();
				if (me.button == 1)
				{
				}
				else if (me.button == 3)
				{
				}
				break;
			}

			case Event::Type::MOUSEBUTTONUP:
			{
				auto me = evt.MouseButton();
				if (me.button == 1)
				{
				}
				else if (me.button == 3)
				{
				}
				break;
			}

			case Event::Type::MOUSEMOTION:
				if (isPainting)
				{
				}
				else if (isPanning)
				{
				}

				break;
			}
		}
	}

	virtual void Update(const int frame, const float deltaT) override
	{
		const SDL::Uint8* key_array = SDL::GetKeyboardState( nullptr );

		if( key_array[SDL::Scancode::DOWN ] ) v_y += 3000.0f * deltaT;
		if( key_array[SDL::Scancode::UP   ] ) v_y -= 3000.0f * deltaT;
		if( key_array[SDL::Scancode::LEFT ] ) v_x -= 2000.0f * deltaT;
		if( key_array[SDL::Scancode::RIGHT] ) v_x += 2000.0f * deltaT;

		if( isGravity )
		{
			v_y += 2000.0f * deltaT;
		}

		if( isJumping )
		{
			v_y       = -1100.0f;
			isJumping = false;
		}

		if( isDrag )
		{
			const float drag_factor = powf( 0.15, deltaT );
			v_x *= drag_factor;
			v_y *= drag_factor;
			if( v_x > 0 )
				v_x -= 200 * deltaT;
			else
				v_x += 200 * deltaT;
		}

		float dest_x = p_x + v_x * deltaT;
		float dest_y = p_y + v_y * deltaT;
		if( isCollision )
		{
			int i_x = dest_x / TILESIZE;
			int i_y = dest_y / TILESIZE;

			//int i_x = round(dest_x / TILESIZE);
			//int i_y = round(dest_y / TILESIZE);

			if( level[i_y][i_x] == 'x'
			 || level[i_y][i_x + 1] == 'x'
			 || level[i_y + 1][i_x] == 'x'
			 || level[i_y + 1][i_x + 1] == 'x'
			 )
			{
				v_x = 0;
				v_y = 0;
			}
			else
			{
				p_x = dest_x;
				p_y = dest_y;
			}
		}
		else
		{
			p_x = dest_x;
			p_y = dest_y;
		}
	}

	virtual void Render(const int frame, const float deltaT) override
	{
		renderer.ClearColor(90, 90, 90);

		renderer.SetDrawColor(255, 128, 0);
		// draw level
		for (uint y = 0; y < level.size(); ++y)
		{
			for (uint x = 0; x < level[y].size(); ++x)
			{
				auto tile = level[y][x];
				if(tile == 'x'){
					renderer.FillRect( Rect(
						x * TILESIZE + camera.x,
						y * TILESIZE + camera.y,
						TILESIZE,
						TILESIZE));
				}
			}
		}

		renderer.SetDrawColor(255, 0, 0);
		renderer.DrawRect(Rect(camera.x, camera.y, TILESIZE*(level[0].size()-1), TILESIZE*level.size()));

		constexpr const Color frameColors[] = {
			Color(255, 0, 0),
			Color(0, 255, 0),
			Color(0, 0, 255),
			Color(255, 255, 255),
		};

		//u32 t = SDL::C::SDL_GetTicks();
		//auto blink = (t & 0x300) >> 8;
		auto blink = (frame & 0x30) >> 4;
		renderer.SetDrawColor(frameColors[blink]);

		// Player
		renderer.FillRect(Rect(p_x + camera.x, p_y + camera.y, TILESIZE, TILESIZE));


		/////////////////// Now for something completely different

		if(isGravity)
		{
			// Merc
			const int   scale      = 4;
			const int   tileSize   = 8;
			const int   scaledTile = scale * tileSize;
			const Point pos        = Point( 800, 500 );
			const Point tileCount  = Point( 512 / tileSize, 512 / tileSize );
			const Point feetPos    = pos + Point( -4, 24 - 8 ) * scale;
			const Point bodyPos    = pos + Point( 0, -8 ) * scale;

			spriteSheet.DrawSprite( feetPos,
									tileCount,
									Rect( 3 * blink + 3, 4, 3, 2 ),
									scale );

			spriteSheet.DrawSprite( bodyPos,
									tileCount,
									Rect( 3 * blink, 0, 3, 4 ),
									scale );

			renderer.SetDrawColor( frameColors[blink] );
			renderer.DrawRect( Rect( 800, 500, 16 * scale, 32 * scale ) );

			renderer.SetDrawColor( frameColors[(blink + 1) % 4] );
			renderer.FillRect( Rect( 800, 500 + 32 * scale, 16 * scale, scale ) );

			// outlines of the sprite tiles
			if( !isDrag )
			{
				renderer.SetDrawColor( frameColors[(blink + 2) % 4] );
				renderer.DrawRect( Rect( feetPos.x, feetPos.y, scaledTile*3, scaledTile*2 ) );

				renderer.SetDrawColor( frameColors[(blink + 3) % 4] );
				renderer.DrawRect( Rect( bodyPos.x, bodyPos.y, scaledTile*3, scaledTile*4 ) );
			}
		}

		// cheapo benchmarking
		//for (uint x = 0; x < 100; ++x)
		{
			std::ostringstream oss;
			const char* text = "Cheats:\n\nF1: Gravitiy.\nF2: Drag.\nF3: Collision.\n\n";
			oss << text
				<< deltaT * 1000.0f
				<< "ms";
			{
				Color c(255, 255, 255);
				//SDL::C::TTF_SetFontHinting(font, ((t & 0x600) >> 9));
				//SDL::C::TTF_SetFontOutline(font, 1);
				Surface surf = font.RenderUTF8_Blended_Wrapped(oss.str(), c, 640);
				Texture t2(renderer, surf);
				t2.SetColorMod(Color(0, 0, 0));
				//SDL::C::SDL_SetTextureBlendMode(t2, SDL::C::SDL_BLENDMODE_BLEND);
				const Point p(360, 20);
				for (const Point& pd : { Point(-1, -1), Point(1, -1), Point(-1, 1), Point(1, 1) })
					t2.Draw(p + pd);
				t2.SetColorMod(Color(255, 255, 255));
				t2.Draw(p);
			}
		}

		renderer.Present();
	}
};

#endif
