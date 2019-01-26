#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "global.h"
#include "gamebase.h"

class EditorState : public GameState
{
	const int TILESIZE = 32;

	// ctor
	using GameState::GameState;

	Font    font;
	Texture tileSet;

	Point pickedIdx  = Point(0, 0);
	bool  isPainting = false;
	bool  isPanning  = false;
	bool  showGrid   = false;

	Array<Array<Point, 20>, 40> level;
	Point camera;

public:

	virtual void Init() override
	{
		if( !font )
			font = TTF::Font( "../assets/fonts/RobotoSlab-Bold.ttf", 16 );

		if( !tileSet )
			tileSet = IMG::LoadTexture( renderer, "../assets/graphics/streets.png" );

		for( auto& row : level )
		{
			row.fill( Point( 8, 8 ) );
		}

		camera = Point( 0, 0 );
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
					showGrid = !showGrid;
				}
				break;
			}

			case Event::Type::MOUSEBUTTONDOWN:
			{
				auto me = evt.MouseButton();
				if (me.button == 1)
				{
					if (me.x < 320 && me.y < 320)
					{
						const Point p = Point(me.x, me.y) / TILESIZE;
						// pick
						pickedIdx = p;
					}
					else
					{
						const Point p = (Point(me.x, me.y) - camera) / TILESIZE;
						// paint
						level[p.x][p.y] = pickedIdx;
						isPainting = true;
					}
				}
				else if (me.button == 3)
				{
					isPanning = true;
				}
				break;
			}

			case Event::Type::MOUSEBUTTONUP:
			{
				auto me = evt.MouseButton();
				if (me.button == 1)
				{
					isPainting = false;
				}
				else if (me.button == 3)
				{
					isPanning = false;
				}
				break;
			}

			case Event::Type::MOUSEMOTION:
				if (isPainting)
				{
					auto me = evt.MouseMotion();
					const Point p = (Point(me.x, me.y) - camera) / TILESIZE;
					level[p.x][p.y] = pickedIdx;
				}
				else if (isPanning)
				{
					auto me = evt.MouseMotion();
					camera += Point(me.xrel, me.yrel);
				}

				break;
			}
		}
	}

	virtual void Update(const int frame, const float deltaT) override
	{
		// soon TM
	}

	virtual void Render(const int frame, const float deltaT) override
	{
		renderer.ClearColor(90, 90, 90);

		// draw level
		for (uint x = 0; x < level.size(); ++x)
		{
			for (uint y = 0; y < level[x].size(); ++y)
			{
				tileSet.DrawSprite(
					Point(x * TILESIZE, y * TILESIZE) + camera,
					Point(9, 9),
					level[x][y],
					0.5f);
			}
		}

		renderer.SetDrawColor(255, 0, 0);
		renderer.DrawRect(Rect(camera.x, camera.y, TILESIZE*level.size(), TILESIZE*level[0].size()));

		// draw grid on F1
		// TODO: super slow, can be solved way more efficiently
		if (showGrid)
		{
			renderer.SetDrawColor(0, 0, 0, 63); // grid color
			renderer.SetDrawBlendMode(SDL::BlendMode::BLEND);
			for (uint x = 0; x < level.size(); ++x)
			{
				for (uint y = 0; y < level[x].size(); ++y)
				{
					renderer.DrawRect(Rect(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE) + camera);
				}
			}
		}

		//renderer.SetDrawColor(rand() % 256, rand() % 256, rand() % 256);
		//renderer.FillRect(Rect(rand() % 384, rand() % 384, 128, 128));

		renderer.SetDrawColor(0, 0, 0);
		renderer.FillRect(Rect(0, 0, 320, 320));

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
		tileSet.Draw(Point(0, 0), 0.5f);
		renderer.DrawRect(Rect(pickedIdx.x * TILESIZE, pickedIdx.y * TILESIZE, TILESIZE, TILESIZE));

		// cheapo benchmarking
		//for (uint x = 0; x < 100; ++x)
		{
			std::ostringstream oss;
			const char* text = "Mapeditor:\n\nLinksklick: Oben links im Bild selektiert das zu zeichnende Tile.\n"
							   "Linksklick: Im Rest des Bildschirms platziert die Tile (nur im roten Rahmen).\n"
							   "Rechte Maustaste halten: Karte läßt sich mit der Maus bewegen.\n\n";
			oss << text
				<< deltaT * 1000.0f
				<< "ms";
			{
				Color   c( 255, 255, 255 );
				//SDL::C::TTF_SetFontHinting(font, ((t & 0x600) >> 9));
				//SDL::C::TTF_SetFontOutline(font, 1);
				Surface surf = font.RenderUTF8_Blended_Wrapped( oss.str(), c, 640 );
				Texture t2( renderer, surf );
				t2.SetColorMod( Color( 0, 0, 0 ) );
				//SDL::C::SDL_SetTextureBlendMode(t2, SDL::C::SDL_BLENDMODE_BLEND);
				const Point p( 360, 20 );
				for( const Point& pd : { Point( -1, -1 ), Point( 1, -1 ), Point( -1, 1 ), Point( 1, 1 ) } )
					t2.Draw( p + pd );
				t2.SetColorMod( Color( 255, 255, 255 ) );
				t2.Draw( p );
			}
		}

		renderer.Present();
	}
};

#endif
