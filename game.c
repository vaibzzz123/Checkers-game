#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct cposn {
  char col;
  int row;
  int player; //1 for p1, 2 for p2, 0 for invalid (spot got killed)
};

struct game {
//  char *board[8][8];
  struct cposn *coords; //array of posns
  int p1p; //# of pieces p1 has
  int p2p; //# of pieces p2 has
//  int won_game; //1 if p1 won, 2 if p2 won, 0 if no one has won
};

int game_won(struct game *g) {
  if(g->p1p == 0) {
    return 2;
  }
  else if(g->p2p == 0) {
    return 1;
  }
  else {
    return 0;
  }
}

int coord_search(char col, int row,struct game *g) {
  int len = g->p1p + g->p2p;
  struct cposn *coords = g->coords;
  for(int i = 0; i < len; ++i) {
    if(((coords[i]).row == row) && ((coords[i]).col == col)) {
      if((coords[i]).player == 1) {
        return 1;
      }
      else {
        return 2;
      }
    }
  }
  return 0;
}

void print_board(struct game *g) {
  printf("    A B C D E F G H\n");
  printf("   ----------------\n");
  for(int currow = 8; currow > 0; --currow) {
    printf("%d |", currow);
    for(int curcol = 'A'; curcol <= 'H'; ++curcol) {
      int search = coord_search(curcol,currow,g);
      if(search == 0) {
        printf("  ");
      }
      else if(search == 1) {
        printf(" *");
      }
      else { //search == 2
        printf(" +");
      }
    }
    printf("|\n");
  }
  printf("   ----------------\n");
}

bool valid_move(int oldrow, char oldcol, int newrow, char newcol,struct game *g) {
  int player = coord_search(oldcol,oldrow,g);
  int search = coord_search(newcol,newrow,g);
  if(player == 0) {
    return false;
  }
  if((player == 1) && (search == 1)) { //trying to move to a position already occupied
    return false;
  }
  if((player == 2) && (search == 2)) { //trying to move to a position already occupied
    return false;
  }
  int validcol = (('A' <= newcol) && (newcol <= 'H'))
  || (('a' <= newcol) && (newcol <= 'h'));
  int colcheck = validcol && ((newcol == oldcol - 1) || (newcol == oldcol + 1));
  int validrow = ((1 <= newrow) && (newrow <= 8));
  int rowcheck = newcol && ((newrow == oldrow - 1) || (newrow == oldrow + 1));
  return rowcheck && colcheck;
}

void remove_piece(int row, char col, struct game *g) {
  //swapping algorithm, swaps last item in array with the item to be removed
  int pos = 0;
  int len = (g->p1p + g->p2p);
  for(;pos < len; ++pos) {
    if((g->coords[pos].row == row) && (g->coords[pos].col == col)) {
      break;
    }
  }
  int temprow = g->coords[pos].row;
  char tempcol = g->coords[pos].col;
  int tempplayer = g->coords[pos].player;
  g->coords[pos].row = g->coords[len - 1].row;
  g->coords[pos].col = g->coords[len - 1].col;
  g->coords[pos].player = g->coords[len - 1].player;
  g->coords[len - 1].row = temprow;
  g->coords[len - 1].col = tempcol;
  g->coords[len - 1].player = tempplayer;
  g->coords = realloc(g->coords,(len-1)*sizeof(struct cposn));
}

void move(int oldrow, char oldcol, int newrow, char newcol, struct game *g) {
  if(!valid_move(oldrow,oldcol,newrow,newcol,g)) {
    return;
  }
  int player = coord_search(oldcol,oldrow,g);
  int search = coord_search(newcol,newrow,g);
  int len = g->p1p + g->p2p;
  struct cposn *coords = g->coords;
  if((player == 1 && search == 0) || (player == 2 && search == 0)) { //not removing opponent piece
    for(int i = 0; i < len; ++i) {
      if(((coords[i]).row == oldrow) && ((coords[i]).col == oldcol)) {
        g->coords[i].row = newrow;
        g->coords[i].col = newcol;
        return;
      }
    }
  }
  else if(player == 1 && search == 2) { //removing opponent pieces as p1
    remove_piece(newrow,newcol,g);
    g->p2p -= 1;
    for(int i = 0; i < len - 1 ; ++i) {
      if(((coords[i]).row == oldrow) && ((coords[i]).col == oldcol)) {
        g->coords[i].row = newrow;
        g->coords[i].col = newcol;
        return;
      }
    }
  }
  else { //(player == 2 && search == 1)
    remove_piece(newrow,newcol,g);
    g->p1p -= 1;
    for(int i = 0; i < len - 1 ; ++i) {
      if(((coords[i]).row == oldrow) && ((coords[i]).col == oldcol)) {
        g->coords[i].row = newrow;
        g->coords[i].col = newcol;
        return;
      }
    }
  }
  return;
}

void default_vals(struct game *g) {

  //p1

  g->coords[0].col = 'B';
  g->coords[0].row = 8;
  g->coords[0].player = 1;

  g->coords[1].col = 'D';
  g->coords[1].row = 8;
  g->coords[1].player = 1;

  g->coords[2].col = 'F';
  g->coords[2].row = 8;
  g->coords[2].player = 1;

  g->coords[3].col = 'H';
  g->coords[3].row = 8;
  g->coords[3].player = 1;


  g->coords[4].col = 'A';
  g->coords[4].row = 7;
  g->coords[4].player = 1;

  g->coords[5].col = 'C';
  g->coords[5].row = 7;
  g->coords[5].player = 1;

  g->coords[6].col = 'E';
  g->coords[6].row = 7;
  g->coords[6].player = 1;

  g->coords[7].col = 'G';
  g->coords[7].row = 7;
  g->coords[7].player = 1;


  g->coords[8].col = 'B';
  g->coords[8].row = 6;
  g->coords[8].player = 1;

  g->coords[9].col = 'D';
  g->coords[9].row = 6;
  g->coords[9].player = 1;

  g->coords[10].col = 'F';
  g->coords[10].row = 6;
  g->coords[10].player = 1;

  g->coords[11].col = 'H';
  g->coords[11].row = 6;
  g->coords[11].player = 1;


  //p2


  g->coords[12].col = 'A';
  g->coords[12].row = 3;
  g->coords[12].player = 2;

  g->coords[13].col = 'C';
  g->coords[13].row = 3;
  g->coords[13].player = 2;

  g->coords[14].col = 'E';
  g->coords[14].row = 3;
  g->coords[14].player = 2;

  g->coords[15].col = 'G';
  g ->coords[15].row = 3;
  g->coords[15].player = 2;


  g->coords[16].col = 'B';
  g->coords[16].row = 2;
  g->coords[16].player = 2;

  g->coords[17].col = 'D';
  g->coords[17].row = 2;
  g->coords[17].player = 2;

  g->coords[18].col = 'F';
  g->coords[18].row = 2;
  g->coords[18].player = 2;

  g->coords[19].col = 'H';
  g->coords[19].row = 2;
  g->coords[19].player = 2;


  g->coords[20].col = 'A';
  g->coords[20].row = 1;
  g->coords[20].player = 2;

  g->coords[21].col = 'C';
  g->coords[21].row = 1;
  g->coords[21].player = 2;

  g->coords[22].col = 'E';
  g->coords[22].row = 1;
  g->coords[22].player = 2;

  g->coords[23].col = 'G';
  g->coords[23].row = 1;
  g->coords[23].player = 2;

}

void startgame(void) {
  struct game *g = malloc(sizeof(struct game));
  g->coords = malloc(24*sizeof(struct cposn));
  g->p1p = 12;
  g->p2p = 12;

  default_vals(g);

  print_board(g);

  int first = 0;
  int r = rand();
  if((r % 2) == 0) {
    first = 1;//player 1
  }
  if((r % 2) == 1) {
    first = 2;
  }
  int curturn = first;

  printf("Player going first is player %d!\n",first);
  printf("Commands\nm:move piece on board\nq:quit game\ns:view scoreboard\np:print the board\n");
  char c;

  while(scanf(" %c",&c) != 0) {
    if(game_won(g) != 0) {
      printf("Congratulations player %d, you have won! Play again (y/n)?",game_won(g));
      /*      char replay;
      scanf("%c",&replay);
      if(replay == 'y')  */
    }
    if(c == 'p') {
      print_board(g);
    }
    if(c == 'q') {
      printf("Thanks for playing!\n");
      return;
    }
    if(c == 'm') {
      printf("Please type in which piece you want to move, followed by where you want to move it ex)A3 B4\n");
      int oldrow = 0;
      char oldcol = 0;
      int newrow = 0;
      char newcol = 0;
      scanf(" %c",&oldcol);
      scanf(" %d",&oldrow);
      scanf(" %c",&newcol);
      scanf(" %d",&newrow);
      //printf("%c%d %c%d\n",oldcol,oldrow,newcol,newrow);
      if(valid_move(oldrow,oldcol,newrow,newcol,g)) {
        printf("Successful move!\n");
        move(oldrow,oldcol,newrow,newcol,g);
        print_board(g);
      }
      else if(!valid_move(oldrow,oldcol,newrow,newcol,g)) {
        printf("Invalid move, try again!\n");
      }
    }
    if(c == 's') {
      printf("Scoreboard\nPlayer 1: %d\nPlayer 2: %d\n",12-g->p2p,12-g->p1p);
    }
    if(c != 's' || c!= 'q' || c != 'm' || c != 'p') {
      //printf("%c\n",c);
      printf("Invalid Command!\n");
    }

  }


  /* if(valid_move(3,'A',4,'B',g)) {
    move(3,'A',4,'B',g);
  }
  print_board(g); */

  free(g->coords);
  free(g);
}

/* int main(void) {

  struct game *g = malloc(sizeof(struct game));
  g->coords = malloc(24*sizeof(struct cposn));
  g->p1p = 12;
  g->p2p = 12;

  default_vals(g);

  print_board(g);

  printf("%d\n",valid_move(3,'A',4,'B',g));
  move(3,'A',4,'B',g);
  print_board(g);


  free(g->coords);
  free(g);

} */
