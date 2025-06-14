//Aiden D'Antuono

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "player.h"
#include "ghost.h"
#include "Arrow.h"
#include <cstdlib>

int main(void)
{

	const int WIDTH = 800;
	const int HEIGHT = 400;
	const int NUM_ArrowS = 5;
	const int NUM_ghostS = 10;
	enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};
	bool keys[5] = {false, false, false, false, false};


	//primitive variable
	bool done = false;
	bool redraw = true;
	const int FPS = 60;


	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	if (!al_init_font_addon()) {
		return -1;
	}

	if (!al_init_ttf_addon()) {
		return -1;
	}

	ALLEGRO_FONT *font = al_load_font("PressStart2P.ttf", 12, 0);
	if (font == NULL) {
		return(-1);
	}
	ALLEGRO_FONT *bigFont = al_load_font("PressStart2P.ttf", 36, 0);
	if (bigFont == NULL) {
		return(-1);
	}

	al_install_keyboard();
	al_init_image_addon();

	//object variables
	player myPlayer(HEIGHT);
	Arrow Arrows[NUM_ArrowS];
	ghost ghosts[NUM_ghostS];

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	srand(time(NULL));

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if(keys[UP])
				myPlayer.MoveUp();
			if(keys[DOWN])
				myPlayer.MoveDown(HEIGHT);
			if(keys[LEFT])
				myPlayer.MoveLeft();
			if(keys[RIGHT])
				myPlayer.MoveRight();

			for(int i=0;i<NUM_ArrowS;i++)
				Arrows[i].UpdateArrow(WIDTH);
			for(int i=0;i<NUM_ghostS;i++)
				ghosts[i].Startghost(WIDTH,HEIGHT);
			for(int i=0;i<NUM_ghostS;i++)
				ghosts[i].Updateghost();
			for(int i=0;i<NUM_ArrowS;i++)
				Arrows[i].CollideArrow(ghosts, NUM_ghostS, myPlayer);
			for(int i=0;i<NUM_ghostS;i++)
				ghosts[i].Collideghost(myPlayer);
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				for(int i=0;i<NUM_ArrowS;i++)
					Arrows[i].FireArrow(myPlayer);
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false; 

			myPlayer.DrawPlayer();
			for(int i=0;i<NUM_ArrowS;i++)
				Arrows[i].DrawArrow();
			for(int i=0;i<NUM_ghostS;i++)
				ghosts[i].Drawghost();

			al_draw_textf(font, al_map_rgb(255, 255, 255), 8, HEIGHT - 18, ALLEGRO_ALIGN_LEFT, "Lives: %i", myPlayer.getLives());
			al_draw_textf(font, al_map_rgb(255, 255, 255), 150, HEIGHT - 18, ALLEGRO_ALIGN_LEFT, "Score: %i", myPlayer.getScore());

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}

		if (myPlayer.getLives() < 1) {

			//Display game over screen
			al_draw_textf(bigFont, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 36, ALLEGRO_ALIGN_CENTER, "GAME OVER");
			al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 12, ALLEGRO_ALIGN_CENTER, "Final Score: %i", myPlayer.getScore());
			al_flip_display();

			//Wait until 5 seconds have passed then stop the game
			time_t startTime = time(NULL);
			time_t currentTime = time(NULL);
			
			while ((currentTime - startTime) < 5.0) {
				currentTime = time(NULL);
			}

			done = true;
		}

	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object
	system("Pause");
	return 0;
}





