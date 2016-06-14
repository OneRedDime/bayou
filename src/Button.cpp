#include "Button.h"
#include "Mouse.h"
#include <cstdio>

Button::~Button()
{
}

bool Button::getCursorHover() const
{
    return get_mouse_x() > x && get_mouse_x() < x + length
        && get_mouse_y() > y && get_mouse_y() < y + height;
}

void Button::update()
{
    // Put this in pre active state if the mouse is hovering over this 
    pre_active = getCursorHover();
    
    // If mouse is clicked on button, then activate
    if (getCursorHover() && check_mouse_button(LEFT_CLICK_UP))
    {
        activate();
    }
}

void Button::render() const
{
    if (!pre_active && image)
    {
        al_draw_bitmap(image.get(), x, y, 0);
    }
    else if (pre_active && active_image)
    {
        al_draw_bitmap(active_image.get(), x, y, 0);
    }

    if (!pre_active)
        al_draw_text(font.get(), color, x, y, ALLEGRO_ALIGN_LEFT, text.c_str());
    else
        al_draw_text(font.get(), active_color, x, y, ALLEGRO_ALIGN_LEFT, text.c_str());
}
