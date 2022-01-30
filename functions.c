#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "functions.h"

SDL_Window* window;
SDL_Renderer* renderer;

// textures for the menu, food, map and flashing text
SDL_Texture *main_menu;
SDL_Texture *food_texture;
SDL_Texture *play_texture;
SDL_Texture *map_texture;
SDL_Texture *game_over_texture;
SDL_Texture *try_again_texture;
SDL_Texture *score_text_texture;
SDL_Texture *high_score_text_texture;
SDL_Texture *trophy;
SDL_Texture *score_moon;

// variables for sound effects
Mix_Music *bg_music;
Mix_Chunk *chomp;
Mix_Chunk *game_over_sound;

TTF_Font *score_font;
SDL_Color white = {222, 222, 222, 255};

// rectangles to store snake, food and flashing text
SDL_Rect snake[MAX_LENGTH];
SDL_Rect past_snake[MAX_LENGTH];
SDL_Rect food;
SDL_Rect play;
SDL_Rect try_again_button;
SDL_Rect score_rect;
SDL_Rect high_score_rect;
SDL_Rect trophy_rect;
SDL_Rect score_moon_rect;

// variables for the current state of the game
int game_on = 1; // = the program is running (always 1 unless the user closes the program)
int playing = 0; // = the game is running (only when the snake is moving)
int you_lost = 0; // = the user lost (only when the game over screen is displayed)
int score = 0;
int high_score = 0;
char score_string[15];
char high_score_string[15];

// variables to store information about the direction of the snake
enum direction{up, down, left, right}current_direction;
SDL_KeyCode skipped; // stores the direction in which the snake has to move the next time it's allowed, prevents rapid movement
int direction_change_allowed;
int direction_x; // according to the X axis
int direction_y; // according to the Y axis

void initialize_game(){

    // initializing SDL
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == -1){
            printf("SDL2 did not initialize");
        }

        if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1){
            printf("SDL_mixer did not initialize");
        }
        if(TTF_Init() == -1){
            printf("SDL_ttf did not initialize");
        }


    // creating a window
        window = SDL_CreateWindow("SNAKE - MoonEater ver.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // setting a renderer
        Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        renderer = SDL_CreateRenderer(window, -1, render_flags);

    // loading images
        main_menu = IMG_LoadTexture(renderer, "IMAGES/main_menu_arcade.png");
        map_texture = IMG_LoadTexture(renderer, "IMAGES/purple_map5.png");
        game_over_texture = IMG_LoadTexture(renderer, "IMAGES/game_over.png");
        play_texture = IMG_LoadTexture(renderer, "IMAGES/menu_enter.png");
        try_again_texture = IMG_LoadTexture(renderer, "IMAGES/try_again.png");
        food_texture = IMG_LoadTexture(renderer,"IMAGES/moon2.png");
        trophy = IMG_LoadTexture(renderer, "IMAGES/trophy.png");
        score_moon = food_texture;

    // loading background music and sound effects
        bg_music = Mix_LoadMUS("SOUNDS/soundtrack.ogg");
        chomp = Mix_LoadWAV("SOUNDS/chomp.wav" );
        game_over_sound = Mix_LoadWAV("SOUNDS/game-over2.wav");

    // loading font
        score_font = TTF_OpenFont( "ARCADE_N.ttf", 28 );
        load_score();
        load_high_score();


    // setting the size and positioning of the flashing text
        play.x = 187;
        play.y = 320;
        play.w = 256;
        play.h = 32;
        try_again_button.x = 159;
        try_again_button.y = 320;
        try_again_button.w = 312;
        try_again_button.h = 18;

        trophy_rect.x = 360;
        trophy_rect.y = 0;
        trophy_rect.w = 57;
        trophy_rect.h = 57;

        score_moon_rect.x = 123;
        score_moon_rect.y = 5;
        score_moon_rect.w = 57;
        score_moon_rect.h = 57;


    // setting the width and height of the food rectangle
        food.w = SNAKE_WIDTH;
        food.h = SNAKE_HEIGHT;

}

void check_input(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        // checking if the user clicked on the close button
            if(e.type == SDL_QUIT){
                game_on = 0;
                close_game();
            }
        // checking if the user is on the menu or game over screen
            if(playing == 0){
                // checking if the user pressed enter
                    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
                        playing = 1;
                        you_lost = 0;
                    }
            }
            else{
                // checking if the user pressed the arrow button
                    if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                        change_direction(e.key.keysym.sym);
                    }
            }
    }
}

void start_game(){

    // resuming the music if it's not playing
        if(Mix_PlayingMusic() == 0){
            Mix_PlayMusic(bg_music, -1);
        }

    // showing the map on the screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map_texture, NULL, NULL);
        load_score();
        SDL_RenderCopy(renderer, score_text_texture, NULL, &score_rect);
        SDL_RenderPresent(renderer);

    // spawning sprites
        spawn_snake();
        spawn_food();

        load_high_score();

    // entering the game loop
        while(playing){

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, map_texture, NULL, NULL);
            SDL_RenderCopy(renderer, score_text_texture, NULL, &score_rect);
            SDL_RenderCopy(renderer, high_score_text_texture, NULL, &high_score_rect);
            SDL_RenderCopy(renderer, trophy, NULL, &trophy_rect);
            SDL_RenderCopy(renderer, score_moon, NULL, &score_moon_rect);

            check_input();
            move_snake();
            draw_food();
            draw_snake(198,160,212);

            SDL_RenderPresent(renderer);

            SDL_Delay(200);

        }
}

void load_score(){
        sprintf(score_string, ":%d", score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(score_font,score_string, white);
        score_text_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        score_rect.x = 180;
        score_rect.y = 5;
        score_rect.w = 75;
        score_rect.h = 57;
        SDL_FreeSurface(scoreSurface);
}

void load_high_score(){
        sprintf(high_score_string, ":%d", high_score);
        SDL_Surface* highscoreSurface = TTF_RenderText_Solid(score_font,high_score_string, white);
        high_score_text_texture = SDL_CreateTextureFromSurface(renderer, highscoreSurface);
        high_score_rect.x = 417;
        high_score_rect.y = 5;
        high_score_rect.w = 75;
        high_score_rect.h = 57;
        SDL_FreeSurface(highscoreSurface);
}

void draw_menu(){

    // playing background music
        Mix_PlayMusic(bg_music, -1);

    // showing the menu on the screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, main_menu, NULL, NULL);
        SDL_RenderPresent(renderer);

    // entering the menu loop
        while(playing == 0){

            // checking if the user started the game
                check_input();
            // making the text flash by showing it on an odd number of seconds and hiding on an even number
                time_t seconds = time(NULL);
                if(seconds%2 == 0){
                    SDL_RenderCopy(renderer, main_menu, NULL, NULL);
                    SDL_RenderPresent(renderer);
                }
                else{
                    SDL_RenderCopy(renderer, play_texture, NULL, &play);
                    SDL_RenderPresent(renderer);
                }
        }
}

void draw_snake(int r, int g, int b){

    // drawing the snake head
        SDL_SetRenderDrawColor(renderer ,r,g,b, 255);
        SDL_RenderFillRect(renderer, &snake[0]);
    // drawing the rest of the body
        for(int i = 1;i < (int)(sizeof(snake)/sizeof(snake[0]));i++){
            if(snake[i].w == 0){
                break;
            }
            // drawing the snake body cell
                SDL_SetRenderDrawColor(renderer ,r,g,b, 255);
                SDL_RenderFillRect(renderer, &snake[i]);

            // creating a border around each snake body cell
                SDL_SetRenderDrawColor(renderer, 58, 47, 86, 255);
                SDL_RenderDrawRect(renderer, &snake[i]);
        }
}

void spawn_snake(){

    // setting the initial direction of the snake to the right
        current_direction = right;
        direction_x = SNAKE_WIDTH;
        direction_y = 0;

    // clearing the snake
        for (int i = 0; i < (int)(sizeof(snake)/sizeof(snake[0])); i++) {
            snake[i].x = 0;
            snake[i].y = 0;
            snake[i].w = 0;
            snake[i].h = 0;
        }

    // setting the size and positioning of the head of the snake
        snake[0].x = SNAKE_START_X;
        snake[0].y = SNAKE_START_Y;
        snake[0].w = SNAKE_WIDTH;
        snake[0].h = SNAKE_HEIGHT;

    // setting the size and positioning of the rest of the body
        for (int i = 1; i < 5; i++) {
            snake[i] = snake[0];
            snake[i].x = snake[0].x - (SNAKE_WIDTH * i);
        }

}

void move_snake(){

    // moving snake only if the user hasn't lost
        if(you_lost){
            return;
        }

    // replacing all cells with the previous cell
        for (int i = (int)(sizeof(snake)/sizeof(snake[0]))-1; i > 0; i--) {
            past_snake[i] = snake[i];
            snake[i] = snake[i-1];
        }

    // moving the head one chunk forward
        past_snake[0] = snake[0];
        snake[0].x = snake[1].x + direction_x;
        snake[0].y = snake[1].y + direction_y;
        snake[0].h = SNAKE_HEIGHT;
        snake[0].w = SNAKE_WIDTH;

    // playing sound effect and spawning new food if the coordinates of the head are now equal to the coordinates of the food
        if (food.x == snake[0].x && food.y == snake[0].y) {
            Mix_PlayChannel( -1, chomp, 0 );
            spawn_food();
            score++;
            load_score();
        }
    // removing the tail if otherwise
        else {
            for (int i = 5; i < (int)(sizeof(snake)/sizeof(snake[0])); i++) {
                if (snake[i].w == 0) {
                    past_snake[i-1] = snake[i-1];
                    snake[i-1].x = 0;
                    snake[i-1].y = 0;
                    snake[i-1].w = 0;
                    snake[i-1].h = 0;
                    break;
                }
            }
        }
        direction_change_allowed = 1;
    // changing the direction if the user pressed the button too fast
        if((int)skipped != -1){
            change_direction(skipped);
            skipped = -1;
        }
        check_collision();

}

void change_direction(SDL_KeyCode new_direction){

    // checking if the snake is allowed to change direction
        if(!direction_change_allowed){
            skipped = new_direction;
            return;
        }

    // changing the direction of the snake
        if (new_direction == SDLK_UP && current_direction != down) {
            direction_x = 0;
            direction_y = -SNAKE_HEIGHT;
            current_direction = up;
        }
        if (new_direction == SDLK_DOWN && current_direction != up) {
            direction_x = 0;
            direction_y = SNAKE_HEIGHT;
            current_direction = down;
        }
        if (new_direction == SDLK_LEFT && current_direction != right) {
            direction_x = -SNAKE_WIDTH;
            direction_y = 0;
            current_direction = left;
        }
        if (new_direction == SDLK_RIGHT && current_direction != left) {
            direction_x = SNAKE_WIDTH;
            direction_y = 0;
            current_direction = right;
        }
        direction_change_allowed = 0;
}

void check_collision(){

        for (int i = 1; i < (int)(sizeof(snake)/sizeof(snake[0])); i++) {
            if (snake[i].w == 0) {
                break;
            }
        // checking if the coordinates of the head are equal to the coordinates of the cell 'i' or the wall
            if (snake[0].x < 0 || snake[0].x > WINDOW_WIDTH - SNAKE_WIDTH ||
                snake[0].y < UPPER_BAR || snake[0].y > WINDOW_HEIGHT - SNAKE_HEIGHT ||
                (snake[0].x == snake[i].x && snake[0].y == snake[i].y)) {
                for(int i = 0;i<(int)(sizeof(snake)/sizeof(snake[0]));i++){
                    snake[i] = past_snake[i];
                }
                if(score>high_score){
                    high_score = score;
                }
                score = 0;
                playing = 0;
                you_lost = 1;
                return;
            }
        }
}

void spawn_food(){

    //generating a number from 0,30,60,90,...,WINDOW_WIDTH
        food.x = rand() % (((WINDOW_WIDTH)/SNAKE_WIDTH))*SNAKE_WIDTH;

    //generating a number from 60,90,...,WINDOW_HEIGHT
        food.y = rand() % (((WINDOW_HEIGHT - UPPER_BAR)/SNAKE_HEIGHT))*SNAKE_HEIGHT+UPPER_BAR;

    // respawning the food if it spawned on the snake
        for (int i = 0; i < (int)(sizeof(snake)/sizeof(snake[0])); i++) {
            if (snake[i].w == 0) {
              break;
            }
            if (snake[i].x == food.x && snake[i].y == food.y) {
              spawn_food();
              break;
            }
        }
}

void draw_food() {

        SDL_RenderCopy(renderer, food_texture, NULL, &food);
}

void game_over(){

    // pausing the music and playing a sound effect
        Mix_HaltMusic();
        Mix_PlayChannel( -1, game_over_sound, 0 );

        for(int i = 0;i<5;i++){
            if(i%2 == 0){
                draw_snake(0,0,0);
            }
            else{
                draw_snake(198,160,212);
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(300);

        }

    // showing the game over screen
        SDL_RenderCopy(renderer, game_over_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    // entering the game over loop
        while(you_lost == 1){

        // checking if the user restarted the game
            check_input();

        // making the text flash by showing it on an odd number of seconds and hiding on an even number
            time_t i = time(NULL);
            if(i%2 == 0){
                SDL_RenderCopy(renderer, game_over_texture, NULL, NULL);
                SDL_RenderPresent(renderer);
            }
            else{
                SDL_RenderCopy(renderer, try_again_texture, NULL, &try_again_button);
                SDL_RenderPresent(renderer);
            }

        }
}

void close_game(){

    SDL_DestroyTexture(map_texture);
    SDL_DestroyTexture(main_menu);
    SDL_DestroyTexture(food_texture);
    SDL_DestroyTexture(play_texture);
    SDL_DestroyTexture(game_over_texture);
    SDL_DestroyTexture(try_again_texture);
    SDL_DestroyTexture(score_text_texture);
    SDL_DestroyTexture(high_score_text_texture);
    TTF_CloseFont(score_font);
    Mix_FreeChunk(chomp);
    Mix_FreeChunk(game_over_sound);
    Mix_FreeMusic(bg_music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    return;
}
