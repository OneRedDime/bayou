#pragma once
#include "Assets.h"

/***************************************************************************//**
This class serves to help out with managing animations with Allegro. The 
typical way to animate things using Allegro is to create a bitmap, then 
maintain an elaborate/clunky system of counters to keep track of what region 
of the bitmap to draw. This class abstracts out that mess by providing built
methods to assist with \link Animation::update updating \endlink and \link
Animation::render rendering\endlink the animation. There's also a few control 
methods to \link Animation::setCurrentFrame skip around\endlink or
\link Animation::setLoop prevent it from looping\endlink.
*******************************************************************************/
class Animation
{
    public:
/***************************************************************************//**
@param reel The bitmap which will be used for the entire animation.
@param duration The duration (in game cycles) the animation will spend on each 
    frame.
@param num_frames Total number of separate images in animation.
*******************************************************************************/
        Animation(shared_bitmap reel, int duration, int num_frames = 1);
        ~Animation();

/***************************************************************************//**
@fn int getTotalDuration() const
Returns the total length of the animation in game cycles.
@fn int getNumFrames() const
Returns the number of separate images in the animation
@fn int getCurrentFrame() const
Returns the number of the frame the animation is currently on. Frame counting
starts at 0.
@fn int getTimePassed() const
Returns the total amount of time the animation has been running. Resets to 0
after each loop.
*******************************************************************************/
        int getTotalDuration() const { return total_duration; }
        int getNumFrames() const { return total_frames; }
        int getCurrentFrame() const { return current_frame; }
        int getTimePassed() const { return frame_timer + frame_duration * current_frame; }

/***************************************************************************//**
@fn void setLoop(bool l)
Sets the loop flag. The animation will stop on its last frame if set to false.
@fn void setCurrentFrame(int f)
Skips the animation to a certain frame. f must be greater than or equal to 0,
and less than the number of frames in the animimation, otherwise this function
does nothing.
*******************************************************************************/
        void setCurrentFrame(int f);
        void setLoop(bool l) { loop = l; }

/***************************************************************************//**
@fn bool isOnFirstFrame()
Returns true if the current frame is 0.
@fn bool isOnFrame(int f)
Returns true if the current frame is f.
@fn bool isOnLastFrame()
Returns true if the current frame is the last one in the animation.
@fn bool isOver()
Returns true if the animation isn't looping, and is on the last frame.
*******************************************************************************/
        bool isOnFirstFrame(){ return !current_frame; }
        bool isOnFrame(int f){ return current_frame == f; }
        bool isOnLastFrame(){ return current_frame == total_frames - 1; }
        bool isOver(){ return !loop && isOnLastFrame(); }

/***************************************************************************//**
@fn void update()
Should be called once per animation per game cycle.
@fn void render(float x, float y, float x_scale = 1, float y_scale = 1) const
Draws the current animation frame to the screen.
@param x X coordinate of animation on screen.
@param y Y coordinate of animation on screen.
@param x_scale Scale which the animation will be drawn along the x-axis.
@param y_scale Scale which the animation will be drawn along the y-axis.
*******************************************************************************/
        void update();
        void render(float x, float y, float x_scale = 1, float y_scale = 1) const;

    private:
        int frame_timer, frame_duration, total_duration,
            current_frame, total_frames,
            frame_width, frame_height;
        bool loop;
        shared_bitmap frames;
};

