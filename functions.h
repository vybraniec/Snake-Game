#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#define WINDOW_WIDTH     630
#define WINDOW_HEIGHT    480
#define UPPER_BAR        60
#define SNAKE_WIDTH      30
#define SNAKE_HEIGHT     30
#define MAX_LENGTH     ((WINDOW_WIDTH)*(WINDOW_HEIGHT-UPPER_BAR))/(SNAKE_WIDTH*SNAKE_HEIGHT) //amount of squares on the window
#define SNAKE_START_X   300
#define SNAKE_START_Y   210


extern SDL_Color white;
extern int game_on;


void initialize_game(); // initializes all necessary libraries, textures, loads images etc.
void draw_menu();
void load_score();
void load_high_score();
void start_game(); // the main game loop
void check_input();
void draw_snake(int r, int g, int b);
void spawn_snake(); // called at the beginning of every game
void move_snake();
void change_direction(SDL_KeyCode new_direction); // changes the direction of the snake according to user's input
void check_collision(); // checks if the snake collided with the walls or its body
void spawn_food();
void draw_food();
void game_over(); // shows the game over screen
void close_game(); // destroys all textures and quits SDL

#endif // FUNCTIONS_H_INCLUDED
