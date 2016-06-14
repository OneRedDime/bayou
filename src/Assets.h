/***************************************************************************//**
@defgroup assets_group Assets
Assets defines a collection of functions that 
interact with hidden tables of art assets. The Assets module defines
functions Bayou programmers should use exclusively when accessing assets. The 
functions in Assets abstract away Allegro functions such as al_load_bitmap. 
You should NEVER initialize raw Allegro pointers such as ALLEGRO_BITMAP* when 
programming your game, and all assets should be loaded during game load time.
Include the file Assets.h to use the functions documented in this section.

The submodule Asset Tables Group details how assets are stored at a lower level.
@{
*******************************************************************************/
#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <memory>
#include <string>

/***************************************************************************//**
@typedef shared_bitmap
Shared pointer of type ALLEGRO_BITMAP
@typedef shared_font
Shared pointer of type ALLEGRO_FONT
@typedef shared_sample
Shared pointer of type ALLEGRO_SAMPLE
*******************************************************************************/
typedef std::shared_ptr<ALLEGRO_BITMAP> shared_bitmap;
typedef std::shared_ptr<ALLEGRO_FONT> shared_font;
typedef std::shared_ptr<ALLEGRO_SAMPLE> shared_sample;
/**@}*/

/*******************************************************************************
@fn shared_bitmap make_shared_bitmap(ALLEGRO_BITMAP *bmp)
Returns a shared_bitmap created from the passed raw bitmap pointer.
@fn shared_font make_shared_font(ALLEGRO_FONT *font)
Returns a shared_font created from the passed raw font pointer.
@fn shared_sample make_shared_sample(ALLEGRO_SAMPLE *sample)
Returns a shared_sample created from the passed raw sample pointer.
*******************************************************************************/
shared_bitmap make_shared_bitmap(ALLEGRO_BITMAP *bmp);
shared_font make_shared_font(ALLEGRO_FONT *font);
shared_sample make_shared_sample(ALLEGRO_SAMPLE *sample);

/*******************************************************************************
@fn shared_bitmap find_bitmap(std::string name)
Returns a shared bitmap from one of the bitmap tables whose name matches the
string argument. Returns NULL if not found.
@fn shared_font find_font(std::string name)
Searches for a font in the urgent_font table whose name matches the string
argument. This font will have a pre-determined size. Returns NULL if not 
found.
@fn shared_font find_font(std::string name, int size)
Searches for a font in the facile_font table whose name matches the string
argument. This font will be loaded into memory from disk at the specified
size. Returns NULL if not found.
@fn shared_sample find_sample(std::string name)
Searches for a sample in the sample tables whose name matches the string
argument. Returns NULL if not found.
*******************************************************************************/
shared_bitmap find_bitmap(std::string name);
shared_font find_font(std::string name);
shared_font find_font(std::string name, int size);
shared_sample find_sample(std::string name);

/*******************************************************************************
@fn void load_bitmaps()
All bitmaps you plan on using in your game should be loaded in this function.
@fn void load_fonts()
All fonts you plan on using in your game should be loaded in this function.
@fn void load_samples()
All samples you plan on using in your game should be loaded in this function.
@fn void unload_assets()
Properly destroys all assets in the tables. Should be called after game loop
ends. Game will crash at exit if this isn't called.
*******************************************************************************/
void load_bitmaps();
void load_fonts();
void load_samples();
void unload_assets();