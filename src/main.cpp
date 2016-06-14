// Compilation command
/*
g++ -std=c++0x *.cpp -o mygame $(pkg-config --libs allegro-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0)
*/

#include "Globals.h"
#include "Assets.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Manager.h"
#include "TitleMenu.h"
#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main()
{
    //shell vars
    bool render = false;

    //allegro vars
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    //allegro init functions
    printf ("Initializing allegro\n");
    if (!al_init())
    {
        al_show_native_message_box(NULL, NULL, NULL, "failed", NULL, 0);
        return -1;
    }

    printf("Creating display\n");
    display = al_create_display(WIDTH, HEIGHT);
    if (!display)
    {
        al_show_native_message_box(NULL, NULL, NULL, "failed", NULL, 0);
        return -1;
    }

    printf("Installing addons\n");
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    //project inits
    srand(time(NULL));

    printf("Initializing timer\n");
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    printf("Registering event sources\n");
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    printf("Init mouse and keyboard\n");
    init_keyboard();
    init_mouse();

    printf("Loading assets\n");
    load_bitmaps();
    load_fonts();
    load_samples();

    printf ("Creating manager\n");
    push_state(new TitleMenu());
    
    printf("Beginning game\n");
    while (!is_game_over())
    {
        //declare an event
        ALLEGRO_EVENT event;

        //monitor event sources
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            end_game();
        }
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;

            update_mouse();
            update_keyboard();

            handle_key();
            update_game();
        }

        // Render screen
        if (render && al_event_queue_is_empty(event_queue))
        {
            render = false;
            render_game();
            al_flip_display();
        }
    }

    unload_assets();
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}
