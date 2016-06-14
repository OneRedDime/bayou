#include "Animation.h"

Animation::Animation(shared_bitmap reel, int duration, int num_frames)
{
    frames = reel;
    total_duration = duration*num_frames;
    total_frames = num_frames;
    frame_duration = duration;
    frame_timer = 0;
    frame_width = al_get_bitmap_width(frames.get()) / num_frames;
    frame_height = al_get_bitmap_height(frames.get());
    current_frame = 0;
    loop = true;
}

Animation::~Animation()
{
}

void Animation::setCurrentFrame(int f)
{
    if (f >= 0 && f < total_frames)
        current_frame = f;
}

/***************************************************************************//**
This function is responsible for updating the animation. This entails checking
how long we've been on the current frame. If we've been here for longer than the
duration given in ::Animation, then the animation will switch to the next frame.
*******************************************************************************/
void Animation::update()
{
    frame_timer++;
    if (!loop && isOnLastFrame() && frame_timer >= frame_duration)
    {
        frame_timer = 0;
    }
    else if (frame_timer >= frame_duration)
    {
        frame_timer = 0;
        current_frame = (current_frame + 1) % total_frames;
    }
}

void Animation::render(float x, float y, float x_scale, float y_scale) const
{
    al_draw_scaled_bitmap(frames.get(), getCurrentFrame() * frame_width, 0, frame_width, frame_height, x, y, x_scale * frame_width, y_scale * frame_height, 0);
}