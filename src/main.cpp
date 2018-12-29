#include <iostream>

#include <cstdlib>
#include <unistd.h>
#include <locale.h>
#include <time.h>

#include <ncurses.h>
#include <wchar.h>

//////////////////////////////////////////////////////////////////////////////////
//
//                          ASCII Pipe Screensaver v 1.0.0
//
//////////////////////////////////////////////////////////////////////////////////

struct Location
{
	int x, y;
	Location(int y = 0, int x = 0): y(y), x(x) {}
};

enum Direction {
	up, left, down, right
};

//////////////////////////////////////////////////////////////////////////////////

struct Pipe {
	Location loc;
	Direction dir_prev;
	Direction dir;
};

//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	initscr();									// Initialize the ncurses window
    noecho();									// Dissable echoing
    cbreak();									// Dissable line buffering
    curs_set(0);								// Hide the cursor
    srand(time(NULL));							// Set the randome seed based on current time
    start_color();								// Start the color functionalty

    init_pair(0, COLOR_YELLOW, COLOR_BLACK);	// Create color pairs (text, background) and assign an ID
    init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

	int rows, cols;								// Get the size of the terminal windowz
	getmaxyx(stdscr, rows, cols);

	int numPipes = 3;
	Pipe pipes[numPipes];						// Initialize the set of pipes
	for(int i = 0; i < numPipes; i++) {
		pipes[i].loc.y = int(rows/2);
		pipes[i].loc.x = int(cols/(numPipes+1) * (i+1));
	}

	while(true)									// Loop until ctrl^C is pressed...
	{	
		for(int i = 0; i < numPipes; i++) {

			attron(COLOR_PAIR(i));

			switch(pipes[i].dir) {				// Move the pipe forward
				case up:
					pipes[i].loc.y = (pipes[i].loc.y - 1 >= 0) ? pipes[i].loc.y - 1 : rows;
					break;
				case down:
					pipes[i].loc.y = (pipes[i].loc.y + 1 < rows) ? pipes[i].loc.y + 1 : 0;
					break;
				case left:
					pipes[i].loc.x = (pipes[i].loc.x - 1 >= 0) ? pipes[i].loc.x - 1 : cols;
					break;
				case right: 
					pipes[i].loc.x = (pipes[i].loc.x + 1 < cols) ? pipes[i].loc.x + 1 : 0;
					break;
			}

			pipes[i].dir_prev = pipes[i].dir;	
			if(rand() % 3 == 0) {				// Randomly turn left or right	
				if(rand() % 2 == 0) {
					pipes[i].dir = Direction((int(pipes[i].dir) + 1) % 4);
				} else {
					pipes[i].dir = Direction((int(pipes[i].dir) - 1) % 4);
				}
			}

			move(pipes[i].loc.y, pipes[i].loc.x);

			switch(pipes[i].dir_prev) {			// Draw the characters
				case up:
					switch(pipes[i].dir) {
						case up:
							addch(ACS_VLINE | A_BOLD);
							break;
						case down:
							addch(ACS_VLINE | A_BOLD);
							break;
						case left:
							addch(ACS_URCORNER | A_BOLD);
							break;
						case right:
							addch(ACS_ULCORNER | A_BOLD);
							break;
					}
					break;
				case down:
					switch(pipes[i].dir) {
						case up:
							addch(ACS_VLINE | A_BOLD);
							break;
						case down:
							addch(ACS_VLINE | A_BOLD);
							break;
						case left:
							addch(ACS_LRCORNER | A_BOLD);
							break;
						case right:
							addch(ACS_LLCORNER | A_BOLD);
							break;
					}
					break;
				case left:
					switch(pipes[i].dir) {
						case up:
							addch(ACS_LLCORNER | A_BOLD);
							break;
						case down:
							addch(ACS_ULCORNER | A_BOLD);
							break;
						case left:
							addch(ACS_HLINE | A_BOLD);
							break;
						case right:
							addch(ACS_HLINE | A_BOLD);
							break;
					}
					break;
				case right: 
					switch(pipes[i].dir) {
						case up:
							addch(ACS_LRCORNER | A_BOLD);
							break;
						case down:
							addch(ACS_URCORNER | A_BOLD);
							break;
						case left:
							addch(ACS_HLINE | A_BOLD);
							break;
						case right:
							addch(ACS_HLINE | A_BOLD);
							break;
					}
					break;
			}
			
			attroff(COLOR_PAIR(i));
		}
		refresh();								// Render the characters to the screen
		usleep(100000);							// Pause
	}

	endwin();									// End ncurses mode
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
