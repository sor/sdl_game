#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <array>
#include <algorithm>

#include <SDL.hpp>
#include <SDL_ttf.hpp>
#include <SDL_image.hpp>

namespace TTF = SDL::TTF;
namespace IMG = SDL::IMG;

using uint = unsigned int;
using i32 = int32_t;
using u32 = uint32_t;

using SDL::Color;
using SDL::Event;
using SDL::Point;
using SDL::Rect;
using SDL::Surface;
using SDL::Texture;

using TTF::Font;

using std::min;
using std::max;

template<class T, size_t Size>
using Array = std::array<T, Size>;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

#endif
