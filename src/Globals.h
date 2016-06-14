/***************************************************************************//**
@file Globals.h
Globals contains variables and macros that could/should be used by the whole
game. Bayou includes only a few values by default. There are also some Allegro
color macros, but those won't be documented here.
*******************************************************************************/
#pragma once
#include <allegro5/allegro.h>

/***************************************************************************//**
@var WIDTH
Resolution of your games window's width.
@var HEIGHT
Resolution of your games window's height.
@var FPS
Frames per second. Locked at 60 by default.
*******************************************************************************/
const int WIDTH = 1024;
const int HEIGHT = 576;
const int FPS = 60;

// Colors
#define BLACK        al_map_rgb(0,0,0)
#define WHITE        al_map_rgb(255,255,255)
#define GREY        al_map_rgb(127,127,127)
#define LIGHT_GREY    al_map_rgb(200,200,200)
#define RED            al_map_rgb(255,0,0)
#define GREEN        al_map_rgb(0,255,0)
#define BLUE        al_map_rgb(0,0,255)
#define BLUE_GREEN    al_map_rgb(0,255,255)
#define TRANS_BLUE    al_map_rgba(0,0,255,50)
#define YELLOW        al_map_rgb(255,255,0)
#define TRANS_RED    al_map_rgba(255,0,0,50)
