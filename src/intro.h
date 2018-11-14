#pragma once
#ifndef INTRO_H
#define INTRO_H

#include "global.h"
#include "gamebase.h"

class IntroState : public GameState
{
	using GameState::GameState;

	TTF::Font font;
	SDL::Texture image;
	SDL::Texture blendedText;

public:

	virtual void Init() override
	{
		font = TTF::Font("../assets/fonts/RobotoSlab-Bold.ttf", 24);
		image = IMG::LoadTexture(renderer, "../assets/graphics/background.png");
	}

	virtual void Uninit() override
	{
		font = TTF::Font();
		image = SDL::Texture();
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
				if (what_key.scancode() == SDL::Scancode::F9)
					game.SetNextState(1); // continue to editor on F9
				else if (what_key.scancode() != SDL::Scancode::ESCAPE)
					game.SetNextState(2); // continue to game on everything else than ESC
				break;
			}
			case Event::Type::MOUSEBUTTONDOWN:
				game.SetNextState(1);
				break;
			}
		}
	}

	virtual void Update(const int frame, const float deltaT) override
	{

	}

	virtual void Render(const int frame, const float deltaT) override
	{
		//for (uint x = 0; x < 100; ++x)
		{
			image.Draw(Rect(0, 0, 1024, 768));

			// uncomment to use the cache
			//if (!blendedText)
			{
				const char* text = "                                          --== Super Mega Gamey Game 3000 ==--\n\n"
					"Dies ist ein Typoblindtext. An ihm kann man sehen, ob alle Buchstaben da sind und wie sie aussehen. Manchmal benutzt man Worte wie Hamburgefonts, Rafgenduks oder Handgloves, um Schriften zu testen. Manchmal Sätze, die alle Buchstaben des Alphabets enthalten - man nennt diese Sätze »Pangrams«. Sehr bekannt ist dieser: The quick brown fox jumps over the lazy old dog. Oft werden in Typoblindtexte auch fremdsprachige Satzteile eingebaut (AVAIL® and Wefox™ are testing aussi la Kerning), um die Wirkung in anderen Sprachen zu testen. In Lateinisch sieht zum Beispiel fast jede Schrift gut aus. Quod erat demonstrandum. Seit 1975 fehlen in den meisten Testtexten die Zahlen, weswegen nach TypoGb. 204 § ab dem Jahr 2034 Zahlen in 86 der Texte zur Pflicht werden. Nichteinhaltung wird mit bis zu 245 € oder 368 $ bestraft. Genauso wichtig in sind mittlerweile auch Âçcèñtë, die in neueren Schriften aber fast immer enthalten sind. Ein wichtiges aber schwierig zu integrierendes Feld sind OpenType-Funktionalitäten. Je nach Software und Voreinstellungen können eingebaute Kapitälchen, Kerning oder Ligaturen (sehr pfiffig) nicht richtig dargestellt werden."
					"\n\nPress any key to continue!";

				Color c(255, 255, 255);
				Surface surf = font.RenderUTF8_Blended_Wrapped(text, c, 960);
				blendedText = Texture(renderer, surf);
			}

			blendedText.SetColorMod(Color(0, 0, 0));
			const Point p(32, 50);
			for (const Point& pd : { Point(-1, -1), Point(1, -1), Point(-1, 1), Point(1, 1), Point(0, 2), Point(2, 0), Point(0, -2), Point(-2, 0) })
				blendedText.Draw(p + pd);
			blendedText.SetColorMod(Color(255, 255, 255));
			blendedText.Draw(p);
		}

		renderer.Present();
	}
};

#endif
