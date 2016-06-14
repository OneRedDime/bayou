/***************************************************************************//**
@defgroup asset_tables_group Asset Tables
@ingroup assets_group
Details in how art assets are stored and accessed at a lower level detailed in
this section. Since this has a significant impact on performance, it is
suggested a developer using Bayou read so they have confidence their assets are
stored and accessed effeciently.

All assets (bitmaps, fonts, samples) are stored in unordered_maps. These maps
are accessed using the ::find_bitmap, find_font(string name),
find_font(string name, int size), and ::find_sample functions. Each map's 
key is a string representing the name of an asset. Each value may differ.

Each asset has 3 tables associated with it:
- urgent (name, shared_bitmap)
- facile (name, path on disk)
- weak (name, weak_bitmap)

While bitmaps are the asset of choice for the example above, each asset type has
its own set of maps. Ie. There are 3 urgent maps; one for ::shared_bitmap,
::shared_font, and ::shared_sample. The differences between the maps will be
explained now.
- Urgent tables\n
  Each key value pair is a string->shared_asset. Assets belonging to
  an urgent map are loaded into memory when the game starts, and removed when
  the game ends. Asset lookup is fastest from the urgent table, as the asset
  is already loaded into memory. Store assets here when their use is time
  critical, they are widely used in the game, and who's constant presence
  in memory won't have a noticible effect on overall game footprint. For
  example, if a common enemy fires many bullets at once during a fast paced and
  exciting battle, store bullet.png here.
- Facile tables\n
  Each index is a pair of strings; one for the name of the asset and another
  representing the path to the asset on disk. This path is relative to the
  game's executable. When you lookup an asset from here, it must be loaded
  into memory from disk, so the initial load is slow. Second lookup is
  almost as fast as lookup from the urgent table. More on that in the weak
  table section. Store assets here when they are too large or sparsely used to
  warrant constant presence in memory. Ie: background images or boss music.
- Weak tables\n
  Each index here is a string-weak pointer pair. You will never explicitly load
  an asset here. An entry in this table is made when an asset is accessed from
  a facile table. While the appropriately typed shared pointer is returned to
  the caller, a weak pointer to that shared pointer is made and loaded here
  under the same name. If that same asset is accessed again before all
  references to it are removed, then the weak pointer from before will be locked
  and a shared pointer will be returned. This prevents an asset from being
  loaded multiple times into memory, and improves load times of assets currently
  in use.

You are responsible for loading the urgent tables and facile tables by modifying
the functions ::load_bitmaps, ::load_fonts, and ::load_samples. Documentation 
in those functions details how to do this properly.
*******************************************************************************/
#include "Assets.h"
#include <unordered_map>
using namespace std;

void insert_urgent_bitmap(string name, string path);
void insert_facile_bitmap(string name, string path);

void insert_urgent_font(string name, string path, int size);
void insert_facile_font(string name, string path);

void insert_urgent_sample(string name, string path);
void insert_facile_sample(string name, string path);

/*******************************************************************************
Asset paths
*******************************************************************************/
const string ASSETS_PATH = "./assets/";
const string BITMAPS_PATH = ASSETS_PATH + "bitmaps/";
const string FONTS_PATH = ASSETS_PATH + "fonts/";
const string SAMPLES_PATH = ASSETS_PATH + "samples/";

/*******************************************************************************
*** Load your new assets here
****************************************************************************//**
@fn void load_bitmaps()
@ingroup assets_group
This function should be called before the game begins.\n
Modify this function to load your bitmaps into the bitmap tables. All bitmaps
you plan on using in your game should be loaded into either ::urgent_bitmaps or
::facile_bitmaps here.
\verbatim
// Loading a bitmap into the urgent table. The first argument is the name of the
// bitmap, the second is the path to it on disk relative to your game's 
// binary.
insert_urgent_bitmap("main_menu", BITMAPS_PATH + "main_menu.png");

// Loading a bitmap into the facile table. The first argument is the name of the
// bitmap, the second is the path to it on disk.
insert_facile_bitmap("meteor", BITMAPS_PATH + "meteor_sheet.png");
\endverbatim
*******************************************************************************/
void load_bitmaps()
{
    insert_urgent_bitmap("main_menu", BITMAPS_PATH + "main_menu.png"); 
    insert_urgent_bitmap("elf_walk", BITMAPS_PATH + "elf_walk_right.png");
}

/***************************************************************************//**
@fn void load_fonts()
@ingroup assets_group
This function should be called before the game begins. All fonts you plan on
using in your game should be loaded into either ::urgent_fonts or ::facile_fonts
here.\n
This function is very similar to ::load_bitmaps, but differs slightly. Read
below for details.
\verbatim
// When a font is loaded into the urgent table, it must have a set size. A good
// naming convention to use when loading fonts into the urgent table is to name
// the font its filename (minus the extension), followed by its size.
insert_urgent_font("times48", FONTS_PATH + "times.ttf", 48);

// Fonts loaded into the facile table do not need a size, as that will be
// determined later when they are looked up using find_font(string name,
//  int size) insert_facile_font("times", FONTS_PATH + "times.ttf");
\endverbatim
*******************************************************************************/
void load_fonts()
{
    insert_urgent_font("times48", FONTS_PATH + "times.ttf", 48);
    insert_facile_font("times", FONTS_PATH + "times.ttf");
}

/***************************************************************************//**
@fn void load_samples()
@ingroup assets_group
This function should be called before the game begins. All samples you plan on
using in your game should be loaded into either ::urgent_samples or
::facile_samples here.\n
Modify this function to load samples into their tables. This function is almost
identical to ::load_bitmaps.

\verbatim
// Loading a sample into the urgent table. The first argument is the name of the
// sample, the second is the path to it on disk relative to your game's
// binary.
insert_urgent_sample("pop", SAMPLES_PATH + "pop.wav");

// Loading a sample into the facile table. The first argument is the name of the
// sample, the second is the path to it on disk.
insert_facile_sample("theme_music", SAMPLES_PATH + "theme_music.wav");
\endverbatim
*******************************************************************************/
void load_samples()
{
    insert_urgent_sample("pop", SAMPLES_PATH + "pop.wav");
}

/* Private typedefs only to be used here */
typedef weak_ptr<ALLEGRO_BITMAP> weak_bitmap;
typedef weak_ptr<ALLEGRO_FONT> weak_font;
typedef weak_ptr<ALLEGRO_SAMPLE> weak_sample;

/***************************************************************************//**
@var unordered_map <string, shared_bitmap > urgent_bitmaps
@ingroup asset_tables_group
Contains shared_bitmap indexed by string names. These bitmaps will be loaded in
memory until the game exits. Bitmaps are located here using ::find_bitmap.
@var unordered_map <string, weak_bitmap> weak_bitmaps
@ingroup asset_tables_group
Provides temporary storage for bitmaps loaded via the paths in facile_bitmaps.
If that bitmap is accessed again before all references are out of scope, this
table will provide a quick lookup, so it need not be reloaded from disk.
@var unordered_map <string, string> facile_bitmaps
@ingroup asset_tables_group
Contains paths to bitmaps on disk. If ::find_bitmap locates a bitmap here, then
that bitmap is loaded into memory as a ::shared_bitmap and returned.
*******************************************************************************/
unordered_map <string, shared_bitmap> urgent_bitmaps;
unordered_map <string, weak_bitmap> weak_bitmaps;
unordered_map <string, string> facile_bitmaps;

/***************************************************************************//**
@var unordered_map <string, shared_font > urgent_fonts
@ingroup asset_tables_group
Similar to ::urgent_bitmaps in that the fonts here persist until the game ends.
Fonts located here may only be accessed via find_font(string name).
@var unordered_map <string, weak_font> weak_fonts
@ingroup asset_tables_group
Serves an almost identical purpose as ::weak_bitmaps, except only
find_font(string name, int size) should access these fonts.
@var unordered_map <string, string> facile_fonts
@ingroup asset_tables_group
Serves an almost identical purpose as ::facile_bitmaps, except only
find_font(string name, int size) will access these fonts.
*******************************************************************************/
unordered_map <string, shared_font> urgent_fonts;
unordered_map <string, weak_font> weak_fonts;
unordered_map <string, string> facile_fonts;

/***************************************************************************//**
@var unordered_map <string, shared_sample> urgent_samples
@ingroup asset_tables_group
Almost identical to ::urgent_bitmaps, but is instead accessed via ::find_sample.
@var unordered_map <string, weak_sample> weak_samples
@ingroup asset_tables_group
Almost identical to ::weak_bitmaps, but is instead accessed via ::find_sample.
@var unordered_map <string, string> facile_samples
@ingroup asset_tables_group
Almost identical to ::facile_bitmaps, but is instead accessed via ::find_sample.
*******************************************************************************/
unordered_map <string, shared_sample> urgent_samples;
unordered_map <string, weak_sample> weak_samples;
unordered_map <string, string> facile_samples;

/***************************************************************************//**
@fn shared_bitmap make_shared_bitmap(ALLEGRO_BITMAP *bmp)
@ingroup assets_group
This function returns a ::shared_bitmap created from a raw bitmap argument. The
returned shared_bitmap will assume ownership.\n
A shared_bitmap created using this function will automatically have
al_destroy_bitmap set as its destructor, so it's advised all shared_bitmaps be
created using this function.
@fn shared_font make_shared_font(ALLEGRO_FONT *font)
@ingroup assets_group
This function returns a ::shared_font created from a raw font argument.
The returned shared_font will assume ownership.\n
A shared_font created using this function will automatically have
al_destroy_font set as its destructor, so it's advised all shared_fonts be
created using this function.
@fn shared_sample make_shared_sample(ALLEGRO_SAMPLE *sample)
@ingroup assets_group
This function will take a raw sample pointer, and create a shared_sample from
it. The returned shared_sample will assume ownership.\n
A shared_sample created using this function will automatically have
al_destroy_sample set as its destructor, so it's advised all shared_samples be
created using this function.
*******************************************************************************/
shared_bitmap make_shared_bitmap(ALLEGRO_BITMAP *bmp)
{
    return shared_bitmap(bmp, al_destroy_bitmap);
}

shared_font make_shared_font(ALLEGRO_FONT *font)
{
    return shared_font(font, al_destroy_font);
}

shared_sample make_shared_sample(ALLEGRO_SAMPLE *sample)
{
    return shared_sample(sample, al_destroy_sample);
}

/***************************************************************************//**
@ingroup assets_group
This function will search the various bitmap tables for an entry matching name.
NULL will be returned if unsuccessful.

::urgent_bitmaps will be searched first.

::weak_bitmaps will be searched second. This search will only succeed if a bitmap 
was previously looked up in facile_bitmaps, and there is still an existing
reference to that bitmap.

::facile_bitmaps will be searched last. If an entry is found here, then a new
shared_bitmap will be loaded into memory from disk, and an entry will be made
into weak_bitmaps to save time on future look ups.
*******************************************************************************/
shared_bitmap find_bitmap(string name) 
{
    auto bitmap1 = urgent_bitmaps.find(name);
    if (bitmap1 != urgent_bitmaps.end())
    {
        return bitmap1->second;
    }

    auto bitmap2 = weak_bitmaps.find(name);
      if (bitmap2 != weak_bitmaps.end())
      {
          if (auto wp = bitmap2->second.lock())
          {
              return wp;
          }
          else
          {
              weak_bitmaps.erase(bitmap2);
          }
      }

      auto path = facile_bitmaps.find(name);
      if (path != facile_bitmaps.end())
      {
        shared_bitmap bitmap = make_shared_bitmap(al_load_bitmap(path->second.c_str()));
        weak_bitmaps[name] = weak_bitmap (bitmap);
        return bitmap;
      }

    return NULL;
}

/***************************************************************************//**
@ingroup assets_group
This find_font will only search ::urgent_fonts for the location at name. NULL will 
be returned if the requested font was not found.\n
*******************************************************************************/
shared_font find_font(string name) 
{
    auto font1 = urgent_fonts.find(name);
    if (font1 != urgent_fonts.end())
    {
        return font1->second;
    }

    return NULL;
}

/***************************************************************************//**
@ingroup assets_group
This function will search facile_fonts and weak_fonts for the requested font.
NULL will be returned if unsuccessful.

::weak_fonts is searched first...however we're not looking for the name passed
into the function; rather we're looking for something like name+size.

ie. If name = "times" and size = 48, then we search weak_fonts for "times48".

If we don't find it in weak_fonts, or all external references are no longer
valid, we continue our search in ::facile_fonts.

We will search for just the name in facile_fonts. If we find it, then we will
load the font into memory at the specified size using make_shared_font. We will
then create a weak_font out of this shared_font, and load the font into
weak_fonts using the naming convention mentioned above. The shared pointer is
then returned.
*******************************************************************************/
shared_font find_font(string name, int size) 
{
    string fname(name + to_string(size));

    auto font2 = weak_fonts.find(fname);
      if (font2 != weak_fonts.end())
      {
          if (auto wp = font2->second.lock())
          { 
              return wp;
          }
          else
          {
              weak_fonts.erase(font2);
          }
      }

    auto font3 = facile_fonts.find(name);
    if (font3 != facile_fonts.end())
    {
        shared_font font = make_shared_font(al_load_font(font3->second.c_str(), size, 0));
        weak_fonts[fname] = weak_font(font);
        return font;
    }

    return NULL;
}

/***************************************************************************//**
@ingroup assets_group
This function is almost identical to ::find_bitmap, though its return value is a
shared_sample instead of shared_bitmap.
*******************************************************************************/
shared_sample find_sample(string name) 
{
    auto sample1 = urgent_samples.find(name);
    if (sample1 != urgent_samples.end())
    {
        return sample1->second;
    }

    auto sample2 = weak_samples.find(name);
      if (sample2 != weak_samples.end())
      {
          if (auto wp = sample2->second.lock())
          {
              return wp;
          }
          else
          {
              weak_samples.erase(sample2);
          }
      }

      auto path = facile_samples.find(name);
      if (path != facile_samples.end())
      {
        shared_sample sample3 = make_shared_sample(al_load_sample(path->second.c_str()));
        weak_samples[name] = weak_sample(sample3);
        return sample3;
      }

    return NULL;
}

/***************************************************************************//**
@ingroup assets_group
Goes through each urgent and facile table to erase each entry. Should be called
after game loop ends. The game will crash on exit if this isn't called.
*******************************************************************************/
void unload_assets()
{
    for (auto it = urgent_bitmaps.begin(); it != urgent_bitmaps.end();)
    {
        it = urgent_bitmaps.erase(it);
    }

    for (auto it = facile_bitmaps.begin(); it != facile_bitmaps.end();)
    {
        it = facile_bitmaps.erase(it);
    }

    for (auto it = urgent_fonts.begin(); it != urgent_fonts.end();)
    {
        it = urgent_fonts.erase(it);
    }

    for (auto it = facile_fonts.begin(); it != facile_fonts.end();)
    {
        it = facile_fonts.erase(it);
    }

    for (auto it = urgent_samples.begin(); it != urgent_samples.end();)
    {
        it = urgent_samples.erase(it);
    }

    for (auto it = facile_samples.begin(); it != facile_samples.end();)
    {
        it = facile_samples.erase(it);
    }
}

/*******************************************************************************
@fn void insert_urgent_bitmap(string name, string path)
This function's job is to create a new shared_bitmap and load it into 
urgent_bitmaps. It accepts two strings as arguments; the name of the bitmap as
found in the table, and the path on disk to that bitmap. This path is relative
to the game's executable. It will load the bitmap found at the location
specified by path using ::make_shared_bitmap, and the resulting shared_bitmap 
will be loaded into urgent_bitmaps.\n
Load bitmaps here when they are small and/or used often throughout the game.
@fn void insert_facile_bitmap(string name, string path)\n
A good naming convention to consider when loading bitmaps may be to name the
bitmap as its filename minus the extension.
This function's job is to load a new entry into facile_bitmaps. The two arguments
it accepts are the name of an bitmap you may wish to load later, and the path to
that bitmap.\n
A good naming convention to consider when loading bimaps may be to name the
bitmap as its filename minus the extension.
*******************************************************************************/
void insert_urgent_bitmap(string name, string path)
{
    shared_bitmap bmp = make_shared_bitmap(al_load_bitmap(path.c_str()));
    if (!bmp)
    {
        printf("Could not find find bitmap %s\n", path.c_str());
    }
    urgent_bitmaps.insert(pair<string, shared_bitmap >(name, bmp));
}

void insert_facile_bitmap(string name, string path)
{
    shared_bitmap bmp = make_shared_bitmap(al_load_bitmap(path.c_str()));
    if (!bmp)
    {
        printf("Could not find find bitmap %s\n", path.c_str());
    }
    facile_bitmaps.insert(pair<string, string>(name, path));
}

/*******************************************************************************
@fn void insert_urgent_font(string name, string path, int size)
This function differs slightly from ::insert_urgent_bitmap and 
::insert_urgent_sample. A font requires a predetermined size in order to be
loaded into memory, so fonts loaded here are loaded at a set size. This size
cannot be changed once loaded.\n
A good naming convention to consider when loading fonts here would be to name
them as their filename (minus the extension) plus their size.\n
ie. Loading times font at size 48 would be inserted with the name 'times48'
@fn void insert_facile_font(string name, string path)\n
This function is very similar to insert_facile_bitmap and insert_facile_sample.
The only difference is this function's path should point to a font file.\n
A good naming convention to consider when loading fonts here would be to name
them as their filename minus the extension. The size will be figured out later
when it is looked up using find_font(string name).
*******************************************************************************/
void insert_urgent_font(string name, string path, int size)
{
    shared_font f = make_shared_font(al_load_font(path.c_str(), size, 0));
    if (!f)
    {
        printf("Could not find find font %s\n", path.c_str());
    }
    urgent_fonts.insert(pair<string, shared_font>(name, f));
}

void insert_facile_font(string name, string path)
{
    shared_font font = make_shared_font(al_load_font(path.c_str(), 8, 0));
    if (!font)
    {
        printf("Could not find find font %s\n", path.c_str());
    }
    facile_fonts.insert(pair<string, string>(name, path));
}

/*******************************************************************************
@fn void insert_urgent_sample(string name, string path)
This function is very similar to ::insert_urgent_bitmap.
The only difference is this function's path should point to an audio file.\n
A good naming convention to consider when loading fonts here would be to name
them as their filename minus the extension.
@fn void insert_facile_sample(string name, string path)\n
This function is very similar to ::insert_facile_bitmap. The only difference is
this function's path should point to an audio file.\n
A good naming convention to consider when loading fonts here would be to name
them as their filename minus the extension.
*******************************************************************************/
void insert_urgent_sample(string name, string path)
{
    shared_sample s = make_shared_sample(al_load_sample(path.c_str()));
    if (!s)
    {
        printf("Could not find find sample %s\n", path.c_str());
    }
    urgent_samples.insert(pair<string, shared_sample>(name, s));
}

void insert_facile_sample(string name, string path)
{
    shared_sample sample = make_shared_sample(al_load_sample(path.c_str()));
    if (!sample)
    {
        printf("Could not find find sample %s\n", path.c_str());
    }
    facile_samples.insert(pair<string, string>(name, path));
}
