#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(void) {

printf("Welcome to Vaib's 2D Chess game!\nSelect one of the commands below:\n");

printf("\np: play a two player game\n");
printf("q: quit the current game\n");

char c;

  while(scanf(" %c",&c) != 0) {

    if(c == 'p') {
  printf ("game is starting up...\n\n");
  break;
    }

  else if(c == 'q') {
  printf("Thanks for playing!\n");
  return 0;
    }

  else {
  printf("Invalid Command! Try again.\n");
    }

  }
  startgame();
}
