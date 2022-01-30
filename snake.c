#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "functions.h"


int main (int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    initialize_game();
    draw_menu();
    while(game_on){
        start_game();
        game_over();
    }
    return 0;
}

