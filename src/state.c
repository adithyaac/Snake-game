#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
  game_state_t *default_state = (game_state_t *)malloc(sizeof(game_state_t));
  default_state->num_rows = 18;
  default_state->board = (char **)malloc(sizeof(char *) * default_state->num_rows);
  default_state->num_snakes = 1;
  default_state->snakes = malloc(sizeof(snake_t));

  (default_state->snakes)->head_col = 4;
  (default_state->snakes)->head_row = 2;
  (default_state->snakes)->live = 1;
  (default_state->snakes)->tail_col = 2;
  (default_state->snakes)->tail_row = 2;

  for (int i = 0; i < 18; i++)
  {
    default_state->board[i] = (char *)malloc(sizeof(char) * 21);
  }
  for (int i = 0; i < 18; i++)
  {
    for (int j = 0; j < 20; j++)
    {
      if ((i == 0) || (i == 17))
      {
        default_state->board[i][j] = '#';
      }
      else if ((j == 0) || (j == 19))
      {
        default_state->board[i][j] = '#';
      }
      else
      {
        default_state->board[i][j] = ' ';
      }
    }
  }
  for (int i = 0; i < 18; i++)
  {
    default_state->board[i][20] = '\0';
  }
  default_state->board[2][9] = '*';
  default_state->board[2][4] = 'D';
  default_state->board[2][3] = '>';
  default_state->board[2][2] = 'd';
  // for (int i = 0; i < 18; i++)
  // {
  //   for (int j = 0; j < 20; j++)
  //   {
  //     printf("%c", default_state->board[i][j]);
  //   }
  //   printf("\n");
  // }
  return default_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  // TODO: Implement this function.
  for (int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  // TODO: Implement this function.
  for (int i = 0; i < state->num_rows; i++)
  {

    fprintf(fp, "%s\n", (state->board)[i]);
  }

  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  // TODO: Implement this function.
  if (c != 'w')
  {
    if (c != 'a')
    {
      if (c != 's')
      {
        if (c != 'd')
        {
          return false;
        }
      }
    }
  }
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  // TODO: Implement this function.
  if (c != 'W')
  {
    if (c != 'A')
    {
      if (c != 'S')
      {
        if (c != 'D')
        {
          return false;
        }
      }
    }
  }
  return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  // TODO: Implement this function.
  char checker[] = "wasd^<v>WASDx";
  for (int i = 0; i < strlen(checker); i++)
  {
    if (c == checker[i])
    {
      return true;
    }
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  // TODO: Implement this function.
  char body[] = "^<v>";
  char tail[] = "wasd";
  for (int i = 0; i < 4; i++)
  {
    if (c == body[i])
    {
      c = tail[i];
      return c;
    }
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
  // TODO: Implement this function.
  char body[] = "WASD";
  char tail[] = "^<v>";
  for (int i = 0; i < 4; i++)
  {
    if (c == body[i])
    {
      c = tail[i];
      return c;
    }
  }
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  if (c == 'v' || c == 's' || c == 'S')
  {
    return cur_row + 1;
  }
  else if (c == '^' || c == 'w' || c == 'W')
  {
    return cur_row - 1;
  }
  // TODO: Implement this function.
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D')
  {
    return cur_col + 1;
  }
  else if (c == '<' || c == 'a' || c == 'A')
  {
    return cur_col - 1;
  }

  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  int X = state->snakes[snum].head_row;
  int Y = state->snakes[snum].head_col;
  char c = state->board[X][Y];
  int y = get_next_col(Y, c);
  int x = get_next_row(X, c);
  return state->board[x][y];
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  int cur_col = state->snakes[snum].head_col;
  int cur_row = state->snakes[snum].head_row;
  char c = get_board_at(state, cur_row, cur_col);

  int new_col = get_next_col(cur_col, c);
  int new_row = get_next_row(cur_row, c);
  state->board[new_row][new_col] = c;
  state->board[cur_row][cur_col] = head_to_body(c);
  state->snakes[snum].head_col = new_col;
  state->snakes[snum].head_row = new_row;
  // for (int i = 0; i < 18; i++)
  // {
  //   for (int j = 0; j < 20; j++)
  //   {
  //     printf("%c", state->board[i][j]);
  //   }
  //   printf("\n");
  // }

  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  int cur_col = state->snakes[snum].tail_col;
  int cur_row = state->snakes[snum].tail_row;
  char c = get_board_at(state, cur_row, cur_col);

  int new_col = get_next_col(state->snakes[snum].tail_col, c);
  int new_row = get_next_row(state->snakes[snum].tail_row, c);
  state->board[cur_row][cur_col] = ' ';
  state->board[new_row][new_col] = body_to_tail(get_board_at(state, new_row, new_col));
  state->snakes[snum].tail_col = new_col;
  state->snakes[snum].tail_row = new_row;
  // for (int i = 0; i < 18; i++)
  // {
  //   for (int j = 0; j < 20; j++)
  //   {
  //     printf("%c", state->board[i][j]);
  //   }
  //   printf("\n");
  // }

  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.
  // printf("%d\n", state->num_snakes);

  for (int i = 0; i < state->num_snakes; i++)
  {
    //  printf("check4.5helpadi\n");
    //  int x = state->snakes[i].live;
    //  printf("%d", x);
    if (state->snakes[i].live == 0)
    {
      continue;
    }

    int cur_col = state->snakes[i].head_col;
    int cur_row = state->snakes[i].head_row;
    char c = get_board_at(state, cur_row, cur_col);

    int new_col = get_next_col(state->snakes[i].head_col, c);
    int new_row = get_next_row(state->snakes[i].head_row, c);
    if (get_board_at(state, new_row, new_col) == '#')
    {
      set_board_at(state, cur_row, cur_col, 'x');
      state->snakes[i].live = 0;
    }
    else if (get_board_at(state, new_row, new_col) == '*')
    {
      update_head(state, i);
      int x = add_food(state);
      // printf("%d", x);
    }
    else if (is_snake(get_board_at(state, new_row, new_col)) == 1)
    {
      set_board_at(state, cur_row, cur_col, 'x');
      state->snakes[i].live = 0;
    }
    else
    {
      //  printf("check4.5\n");
      update_head(state, i);
      update_tail(state, i);
    }
  }
  // for (int i = 0; i < state->num_rows; i++)
  // {
  //   for (int j = 0; state->board[i][j] != '\0'; j++)
  //   {
  //     printf("%c", state->board[i][j]);
  //   }
  //   printf("\n");
  // }
  return;
}

/* Task 5 */
game_state_t *load_board(FILE *fp)
{
  // TODO: Implement this function.
  game_state_t *state = (game_state_t *)malloc(sizeof(game_state_t));
  int num_rows = 0;
  int num_cols = 0;
  state->num_snakes = 0;
  state->snakes = NULL;
  state->board = NULL;
  while (!feof(fp))
  {
    // printf("%d\n", num_rows);
    state->board = realloc(state->board, ((num_rows + 1) * sizeof(char *)));
    char ch = fgetc(fp);
    if (feof(fp))
    {
      break;
    }
    int col = 0;
    state->board[num_rows] = NULL;
    while (ch != '\n')
    {
      col++;
      state->board[num_rows] = realloc(state->board[num_rows], (col * sizeof(char)));
      state->board[num_rows][col - 1] = ch;
      ch = fgetc(fp);
    }
    state->board[num_rows] = realloc(state->board[num_rows], ((col + 1) * sizeof(char)));
    state->board[num_rows][col] = '\0';
    //  char array[3002];
    // fgets(array, 3002, fp);
    // int col = strlen(array);
    // //  printf("%d", col);
    // state->board[num_rows] = (char *)malloc((col) * sizeof(char));
    // strcpy(state->board[num_rows], array);
    // state->board[num_rows][col - 1] = '\0';
    //  printf("%s\n", state->board[num_rows]);
    num_rows++;
  }
  state->num_rows = num_rows;
  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  int col = state->snakes[snum].tail_col;
  int row = state->snakes[snum].tail_row;
  char ch = get_board_at(state, row, col);
  while (!is_head(ch))
  {
    col = get_next_col(col, ch);
    row = get_next_row(row, ch);
    ch = get_board_at(state, row, col);
  }
  state->snakes[snum].head_col = col;
  state->snakes[snum].head_row = row;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.
  state->num_snakes = 0;
  state->snakes = malloc(sizeof(snake_t));

  for (int i = 0; i < state->num_rows; i++)
  {
    for (int j = 0; state->board[i][j] != '\0'; j++)
    {
      char ch = state->board[i][j];
      if (is_tail(ch))
      {
        state->num_snakes++;
        state->snakes = realloc(state->snakes, (state->num_snakes) * (sizeof(snake_t)));
        state->snakes[state->num_snakes - 1].tail_row = i;
        state->snakes[state->num_snakes - 1].tail_col = j;
        find_head(state, state->num_snakes - 1);
        state->snakes[state->num_snakes - 1].live = 1;
        // if(state->snakes[state->num_snakes])
      }
    }
  }
  return state;
}
