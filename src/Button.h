#pragma once
#include "Globals.h"
#include "Assets.h"
#include <functional>
#include <algorithm>

/***************************************************************************//**
Bayou has 3 classes to assist with game menus/HUDS. Buttons is one of those.
The other two are ::Menu and ::HudElement.\n
Buttons represent parts of one's menu that can be clicked on. Any button could
potentially perform any function. This will be detailed in the Button()
constructor.
*******************************************************************************/
class Button
{
    public:
/***************************************************************************//**
@param image This is the image the button will show normally. You may send in
NULL if you have no wish for your button to have an image.
@param active_image The button will display this image when the mouse is
hovering over it.
@param text This is the text the button will display. 
@param font This is the font the button will use when displaying text
@param color This color will be the color of the text under normal conditions.
@param active_color This will be the color of the text when the cursor is
hovering over the button.
@param x X coordinate of the buttons's upper left hand corner. The total length
of the button is decided by the maximum length between the button's image and
the length of the text.
@param y Y coordinate of button's upper left hand corner. The button's height is
decided by the maximum height of the button's image and the height of the text.
@param f Name of function this button will call when clicked on. This function
must be void, but can require any number of parameters.
@param args If f requires aruments, list them here. They will be locked for the
button's lifetime.

- Here is an example button creation for a supposed heart-shaped healing button.
    -# First write the healing function
       \verbatim
       void heal(Player *player, int health) {
            player->setHealth(player->getHealth() + health);
       }
       \endverbatim
    -# Now create the button
       \verbatim
       Player *hero = new Player();
       shared_bitmap h1 = find_bitmap("dark_red_heart");
       shared_bitmap h2 = find_bitmap("pink_heart");
       shared_font times = find_font("times48");
       ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
       Button healing_button (
          h1, h2, 
          "Heal", times, 
          blue, blue, 
          0, SCREEN_HEIGHT - al_get_image_height(h1.get()), 
          heal, hero, 50);
       \endverbatim
    -# Now activating the button will produce the same effect as calling
        \verbatim
        heal(hero, 50);
        \endverbatim
*******************************************************************************/
        template<typename F, typename ...Args>
        Button(
            shared_bitmap image, shared_bitmap active_image,
            std::string text, shared_font font,
            ALLEGRO_COLOR color, ALLEGRO_COLOR active_color,
            int x, int y,
            F f, Args... args) : on_click(std::bind(f, args...))
        {
            Button::image = image;
            Button::active_image = active_image;
            Button::text = text;
            Button::font = font;
            Button::color = color;
            Button::active_color = active_color;
            Button::x = x;
            Button::y = y;

            if (image)
            {
                height = std::max(al_get_font_line_height(font.get()),
                    al_get_bitmap_height(image.get()));
                length = std::max(al_get_text_width(font.get(), text.c_str()),
                    al_get_bitmap_width(image.get()));
            }
            else
            {
                height = al_get_font_line_height(font.get());
                length = al_get_text_width(font.get(), text.c_str());
            }
            
            setPreActive(false);
        }

/***************************************************************************//**
Buttons have no owned dynamic data besides ::shared_bitmap, so the destructor
only outputs a message when the button is destroyed.
*******************************************************************************/
        ~Button();

/***************************************************************************//**
@fn int getX() const
Returns the x coordinate of the upper left hand corner of the button.
@fn int getY() const
Returns the y coordinate of the upper left hand corner of the button.
@fn int getLength() const
Returns the buttons length. Length is decided as the maximum length between the
button's image and the text. A button's length is decided when the button is
created, so the button's image and active_image should be the same size.
@fn int getHeight() const
Very similar to Button::getLength, but instead returns height.
@fn bool getCursorHover() const
Returns true if the mouse cursor intersects the button.
*******************************************************************************/
        int getX() const { return x; }
        int getY() const { return y; }
        int getLength() const { return length; }
        int getHeight() const { return height; }
        bool getCursorHover() const;

/***************************************************************************//**
If set to true, then button will show its active color and active image.
Button::update primarily uses this to change the preactive state to true when
the mouse cursor is hovering over this.
*******************************************************************************/
        void setPreActive(bool p) { pre_active = p; }

/***************************************************************************//**
Calls the button's associated function provided in Button()
*******************************************************************************/
        void activate()    { on_click(); }

/***************************************************************************//**
@fn void update()
Should be called once per button per game cycle. Checks the mouse's location
for intersection with this button. The button will be set in the pre-active
state if this happens. The button will be activated if the left mouse button
is released while hovering over this button.
@fn void render() const
Should be called once per button per game cycle. This will draw the button to
the screen.
*******************************************************************************/
        void update();
        void render() const;

    private:
        shared_bitmap image, active_image;
        std::string text;
        shared_font font;
        ALLEGRO_COLOR color, active_color;
        int x, y;
        std::function<void()> on_click;

        int length, height;

        bool pre_active;
};
