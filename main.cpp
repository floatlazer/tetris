#include <SDL.h>
#include <iostream>
#include "tetromino.h"
#include <stdlib.h>
#define SQUARE_WIDTH 20
#define SQUARE_BORD 1
#define BOARD_WIDTH 12 // 12 colomns
#define BOARD_HEIGHT 24 // 24 rows
#define WINDOW_WIDTH SQUARE_WIDTH*BOARD_WIDTH
#define WINDOW_HEIGHT SQUARE_WIDTH*BOARD_HEIGHT

enum gameState { ON_START, RUNNING, ON_PAUSE, PAUSE, ON_FINISH, FINISH };

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Rect square = {0,0,SQUARE_WIDTH-2*SQUARE_BORD,SQUARE_WIDTH-2*SQUARE_BORD}; // x,y,w,h
SDL_Event event; // Events

BoardUnit** board; // The map array

unsigned long drop_millis; // Control drop rate
unsigned long present_millis; // Control present rate
unsigned long event_millis; // Control event pulling rate
unsigned long lock_start_millis; // Control of lock after touching the bottom
unsigned long fast_drop_millis; // Control of fast drop
unsigned long drop_interval = 500;
unsigned long present_interval = 33; // 30 times per second
unsigned long event_interval = 100; // 10 times per second
unsigned long lock_duration = 1000;
unsigned long fast_drop_interval = 15;
Tetromino tetromino; // The tetromino object
// Control variables
bool lock_start = false;
bool locked = false;
bool fast_drop = false;
gameState state = ON_START;
bool init()
{
  //Init board
  board = new BoardUnit*[BOARD_HEIGHT];
  for(int i = 0; i<BOARD_HEIGHT;i++)
  {
    board[i] = new BoardUnit[BOARD_WIDTH];
    for(int j = 0; j<BOARD_WIDTH;j++)
    {
      board[i][j].occupy = false;
      board[i][j].type = -1;
    }
  }

  // Init SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout<<"SDL could not initialize ! SDL_Error: " << SDL_GetError()<<std::endl;
    return false;
  }
  // Init Window
  gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if(gWindow == NULL)
  {
    std::cout<<"Window could not initialize ! SDL_Error: "<<SDL_GetError()<<std::endl;
    return false;
  }
  // Init renderer
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if(gRenderer == NULL)
  {
    std::cout<<"Renderer could not be created ! SDL_Error: "<<SDL_GetError()<<std::endl;
    return false;
  }

  // Init tetromino
  tetromino.setBoard(board, BOARD_WIDTH, BOARD_HEIGHT);

  return true;
}

void close()
{
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;
  SDL_Quit();
}

void clearBoard()
{
  for(int i = 0; i<BOARD_HEIGHT;i++)
  {
    for(int j = 0; j<BOARD_WIDTH;j++)
    {
      board[i][j].occupy = false;
      board[i][j].type = -1;
    }
  }
}

void renderSquare(int x, int y)
{
  square.x = x*SQUARE_WIDTH+SQUARE_BORD;
  square.y = y*SQUARE_WIDTH+SQUARE_BORD;
  SDL_RenderFillRect(gRenderer, &square);
}

// Render the whole board
void renderBoard()
{
  for(int i=0;i<BOARD_HEIGHT;i++)
  {
    for(int j=0;j<BOARD_WIDTH;j++)
    {
      if(board[i][j].occupy)
      {
        switch (board[i][j].type) {
          case 0: // I
          SDL_SetRenderDrawColor(gRenderer,45,255,254,0xFF);
          break;
          case 1: // O
          SDL_SetRenderDrawColor(gRenderer,255,253,56,0xFF);
          break;
          case 2: // T
          SDL_SetRenderDrawColor(gRenderer,152,37,251,0xFF);
          break;
          case 3: // L
          SDL_SetRenderDrawColor(gRenderer,253,169,41,0xFF);
          break;
          case 4: // J
          SDL_SetRenderDrawColor(gRenderer,11,36,251,0xFF);
          break;
          case 5: // Z
          SDL_SetRenderDrawColor(gRenderer,252,13,27,0xFF);
          break;
          case 6: // S
          SDL_SetRenderDrawColor(gRenderer,41,253,47,0xFF);
          break;
        }
        renderSquare(j,i); // Coord system is different
      }
    }
  }
}
void initTetromino()
{
  tetromino.init();
  tetromino.setType(rand()%7);
  tetromino.setPosition(tetromino.getInitPosition().x, tetromino.getInitPosition().y);
  tetromino.setOrientation(rand()%4);
}

void eliminateLine()
{
  for(int i = BOARD_HEIGHT-1; i>=0; i--)
  {
    bool full_line  = true;
    for(int j = 0; j<BOARD_WIDTH; j++)
    {
      if(!board[i][j].occupy)
      {
        full_line  = false;
      }
    }
    if(full_line) // Drop down
    {
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Eliminate line %d",i);
      BoardUnit* aux = board[i];
      for(int w = i; w>0; w--)
      {
        board[w] = board[w-1];
      }
      board[0] = aux;
      for(int k = 0; k<BOARD_WIDTH; k++)
      {
        board[0][k].occupy = false;
        board[0][k].type = -1;
      }
      i++;
    }
  }
}

//---------------------------Control flow start -----------------------------
void runGame()
{
  //----------------- fast drop ----------------------
  if(fast_drop)
  {
    if(SDL_GetTicks()-fast_drop_millis > fast_drop_interval)
    {
      fast_drop_millis  = SDL_GetTicks();
      if(!tetromino.move(1,0))
      {
        fast_drop = false;
      }
    }
  }
  // ----------------- Normal drop ---------------------
  else if(SDL_GetTicks() - drop_millis > drop_interval)
  {
    drop_millis = SDL_GetTicks();
    tetromino.move(1,0); //drop
  }
  // ----------------- Present the board ---------------
  if(SDL_GetTicks() - present_millis > present_interval)
  {
    present_millis = SDL_GetTicks();
    tetromino.putOnBoard();
    SDL_SetRenderDrawColor(gRenderer,174,174,174,255);
    SDL_RenderClear(gRenderer);
    renderBoard();
    SDL_RenderPresent(gRenderer);

    // Lock control
    if(tetromino.touch_bottom())
    {
      if(!lock_start) // Start lock timer
      {
        lock_start = true;
        lock_start_millis = SDL_GetTicks();
      }
      else if(SDL_GetTicks() - lock_start_millis > lock_duration) // Locked
      {
        locked = true;
      }
    }
    else // Not touch bottom
    {
      lock_start = false;
    }

    // Lock action
    if(!fast_drop && locked)
    {
      // Check block out
      if(tetromino.blockOut())
      {
        // Terminate the game;
        state = ON_FINISH;
        return;
      }

      lock_start = false;
      locked = false;

      // Line elimination
      eliminateLine();

      // New tetromino
      initTetromino();
      lock_start = false;
      }
      // Lock action end
  }
}

void onStartGame()
{
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"On start game");
  clearBoard();
  // Reset control variables
  bool lock_start = false;
  bool locked = false;
  bool fast_drop = false;
  // Init Timer
  drop_millis = SDL_GetTicks(); // Control drop rate
  present_millis = SDL_GetTicks(); // Control present rate
  event_millis = SDL_GetTicks(); // Control event pulling rate
  lock_start_millis = SDL_GetTicks(); // Control of lock after touching the bottom
  fast_drop_millis = SDL_GetTicks(); // Control of fast drop
  // Init tetromino
  initTetromino();
  // Change state
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Start game");
  state = RUNNING;
}

void onPauseGame()
{
  // Change state
  state = PAUSE;
}

void pauseGame()
{
  if(event.key.keysym.sym == SDLK_r)
  {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Resume game");
    state = RUNNING;
  }
}

void onFinishGame()
{
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"On finish game");
  // Change state
  state = FINISH;
}
void finishGame()
{

}
//---------------------------Control flow end -----------------------------

int main()
{
  // Initialize SDL
  init();
  SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
  SDL_RenderClear(gRenderer);
  SDL_RenderPresent(gRenderer);
  bool quit = false;
  // main loop
  while(!quit)
  {
    // ----------------- Events handler --------------------
    if(SDL_GetTicks() - event_millis > event_interval)
    {
      event_millis = SDL_GetTicks();
      // Pull all events
      while(SDL_PollEvent(&event)!=0)
      {
        if(event.type == SDL_QUIT)
        {
          quit = true;
          break;
        }
        if(!fast_drop && event.type == SDL_KEYDOWN ) // ignore key event when doing fast drop
        {
          switch( event.key.keysym.sym )
          {
            case SDLK_SPACE:
              if(tetromino.rotate())
              {
                lock_start = false; // infinity
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Rotate");
              }
              break;
            case SDLK_RIGHT:
              if(tetromino.move(0,1))
              {
                lock_start = false; // infinity
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Move right");
              }
              break;
            case SDLK_LEFT:
              if(tetromino.move(0,-1))
              {
                lock_start = false; // infinity
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Move left");
              }
              break;
            case SDLK_DOWN:
              fast_drop = true; // fast drop
              locked = true; // No lock delay
              SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Fast drop");
              break;
            case SDLK_n: // new game
              SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"New game");
              state = ON_START;
              break;
            case SDLK_p: // new game
              SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,"Pause game");
              state = ON_PAUSE;
              break;
           }
        }
      }
    }

    // Control flow
    switch (state) {
      case ON_START:
        onStartGame();
        break;
      case RUNNING:
        runGame();
        break;
      case ON_PAUSE:
        onPauseGame();
        break;
      case PAUSE:
        pauseGame();
        break;
      case ON_FINISH:
        onFinishGame();
        break;
      case FINISH:
        finishGame();
        break;
    }

  }
  // Clean up
  close();
  return 0;
}
