#include "tetromino.h"
#include <iostream>
void Tetromino::putOnBoard()
{
  if(onBoard)
  {
    // Remove previous tetromino on board
    for(int i = 0; i<SIZE; i++)
    {
      Coord prev_squarePosition = prev_position+shapes[type][prev_orientation][i];
      if(inBoard(prev_squarePosition))
      {
        board[prev_squarePosition.x][prev_squarePosition.y].occupy = false;
        board[prev_squarePosition.x][prev_squarePosition.y].type = -1;
      }
    }
  }
  // Add new tetromino
  for(int i = 0; i<SIZE; i++)
  {
    Coord squarePosition = position+shapes[type][orientation][i];
    if(inBoard(squarePosition))
    {
      board[squarePosition.x][squarePosition.y].occupy = true;
      board[squarePosition.x][squarePosition.y].type = type;
    }
  }
  // Update prev_orientation and prev_position
  prev_position = position;
  prev_orientation = orientation;
  onBoard = true; // Tetromino is showed
}

bool Tetromino::inBoard(Coord p)
{
  return p.x>=0 && p.x<boardHeight && p.y>=0 && p.y<boardWidth;
}
bool Tetromino::rotate()
{
  int ori;
  if(orientation<3)
  {
    ori = orientation + 1;
  }
  else
  {
    ori = 0;
  }
  if(type == 1 || !collision(position, ori)) // No problem for O
  {
    orientation = ori;
    return true;
  }
  else if(!collision(position+(Coord){0,1}, ori)) // Wall kick
  {
    position = position + (Coord){0,1};
    orientation = ori;
    return true;
  }
  else if(!collision(position+(Coord){0,-1}, ori))
  {
    position = position + (Coord){0,-1};
    orientation = ori;
    return true;
  }
  else if(type == 0 && !collision(position+(Coord){0,2}, ori))
  {
    position = position + (Coord){0,2};
    orientation = ori;
    return true;
  }
  else if(type == 0 && !collision(position+(Coord){0,-2}, ori))
  {
    position = position + (Coord){0,-2};
    orientation = ori;
    return true;
  }
  else if(!collision(position+(Coord){1,0}, ori)) // floor kick
  {
    position = position + (Coord){1,0};
    orientation = ori;
    return true;
  }
  else if(!collision(position+(Coord){-1,0}, ori))
  {
    position = position + (Coord){-1,0};
    orientation = ori;
    return true;
  }
  else if(type == 0 && !collision(position+(Coord){2,0}, ori))
  {
    position = position + (Coord){2,0};
    orientation = ori;
    return true;
  }
  else if(type == 0 && !collision(position+(Coord){-2,0}, ori))
  {
    position = position + (Coord){-2,0};
    orientation = ori;
    return true;
  }
  return false;
}

bool Tetromino::move(int x,int y)
{
  Coord p = position+(Coord){x,y};
  if(!collision(p, orientation))
  {
    position = p;
    return true;
  }
  else
  {
    return false;
  }
}

bool Tetromino::collision(Coord pos, int ori)
{
  // Left, right side and the bottom check
  for(int i = 0; i<SIZE; i++)
  {
    Coord p = pos+shapes[type][ori][i];
    if(p.y<0 || p.y>=boardWidth || p.x>=boardHeight)
    {
      return true;
    }
  }
  // Collision check with other points on board
  for(int i = 0; i<SIZE; i++)
  {
    Coord p = pos+shapes[type][ori][i];
    if(inBoard(p))
    {
      if(board[p.x][p.y].occupy)
      {
        bool is_prev = false;
        for(int j = 0; j<SIZE; j++) // prev position is safe
        {
          Coord p_prev = prev_position+shapes[type][prev_orientation][j];
          if(p == p_prev)
          {
            is_prev = true;
            break;
          }
        }
        if(!is_prev)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void Tetromino::setType(int _type)
{
  type = _type;
  if(type == 0) // I
  {
    initPosition = (Coord){-4,boardWidth/2-2};
  }
  else if(type == 1) // O
  {
    initPosition = (Coord){-2,boardWidth/2-1};
  }
  else
  {
    initPosition = (Coord){-3,boardWidth/2-1};
  }
}

void Tetromino::init()
{
  position = (Coord){0,0};
  orientation = 0;
  prev_position = position;
  prev_orientation = orientation;
  type = 0;
  onBoard = false; // Tetromino is not showed
}

bool Tetromino::touch_bottom()
{
    Coord p = position+(Coord){1,0}; // Try to move down
    return collision(p, orientation);
}

bool Tetromino::blockOut()
{
  for(int i = 0; i<SIZE; i++)
  {
    Coord squarePosition = position+shapes[type][orientation][i];
    if(squarePosition.x < 0)
    {
      return true;
    }
  }
  return false;
}

Tetromino::Tetromino()
{
  init();
  // I
  shapes[0][0][0] = (Coord){0,1};
  shapes[0][0][1] = (Coord){1,1};
  shapes[0][0][2] = (Coord){2,1};
  shapes[0][0][3] = (Coord){3,1};

  shapes[0][1][0] = (Coord){2,0};
  shapes[0][1][1] = (Coord){2,1};
  shapes[0][1][2] = (Coord){2,2};
  shapes[0][1][3] = (Coord){2,3};

  shapes[0][2][0] = (Coord){0,2};
  shapes[0][2][1] = (Coord){1,2};
  shapes[0][2][2] = (Coord){2,2};
  shapes[0][2][3] = (Coord){3,2};

  shapes[0][3][0] = (Coord){1,0};
  shapes[0][3][1] = (Coord){1,1};
  shapes[0][3][2] = (Coord){1,2};
  shapes[0][3][3] = (Coord){1,3};

  // O
  shapes[1][0][0] = (Coord){0,0};
  shapes[1][0][1] = (Coord){0,1};
  shapes[1][0][2] = (Coord){1,1};
  shapes[1][0][3] = (Coord){1,0};

  shapes[1][1][0] = (Coord){0,0};
  shapes[1][1][1] = (Coord){0,1};
  shapes[1][1][2] = (Coord){1,1};
  shapes[1][1][3] = (Coord){1,0};

  shapes[1][2][0] = (Coord){0,0};
  shapes[1][2][1] = (Coord){0,1};
  shapes[1][2][2] = (Coord){1,1};
  shapes[1][2][3] = (Coord){1,0};

  shapes[1][3][0] = (Coord){0,0};
  shapes[1][3][1] = (Coord){0,1};
  shapes[1][3][2] = (Coord){1,1};
  shapes[1][3][3] = (Coord){1,0};

  // T
  shapes[2][0][0] = (Coord){1,0};
  shapes[2][0][1] = (Coord){0,1};
  shapes[2][0][2] = (Coord){1,1};
  shapes[2][0][3] = (Coord){2,1};

  shapes[2][1][0] = (Coord){2,1};
  shapes[2][1][1] = (Coord){1,0};
  shapes[2][1][2] = (Coord){1,1};
  shapes[2][1][3] = (Coord){1,2};

  shapes[2][2][0] = (Coord){1,2};
  shapes[2][2][1] = (Coord){2,1};
  shapes[2][2][2] = (Coord){1,1};
  shapes[2][2][3] = (Coord){0,1};

  shapes[2][3][0] = (Coord){0,1};
  shapes[2][3][1] = (Coord){1,2};
  shapes[2][3][2] = (Coord){1,1};
  shapes[2][3][3] = (Coord){1,0};

  // L
  shapes[3][0][0] = (Coord){2,0};
  shapes[3][0][1] = (Coord){0,1};
  shapes[3][0][2] = (Coord){1,1};
  shapes[3][0][3] = (Coord){2,1};

  shapes[3][1][0] = (Coord){2,2};
  shapes[3][1][1] = (Coord){1,0};
  shapes[3][1][2] = (Coord){1,1};
  shapes[3][1][3] = (Coord){1,2};

  shapes[3][2][0] = (Coord){0,2};
  shapes[3][2][1] = (Coord){2,1};
  shapes[3][2][2] = (Coord){1,1};
  shapes[3][2][3] = (Coord){0,1};

  shapes[3][3][0] = (Coord){0,0};
  shapes[3][3][1] = (Coord){1,2};
  shapes[3][3][2] = (Coord){1,1};
  shapes[3][3][3] = (Coord){1,0};

  // J
  shapes[4][0][0] = (Coord){0,0};
  shapes[4][0][1] = (Coord){2,1};
  shapes[4][0][2] = (Coord){1,1};
  shapes[4][0][3] = (Coord){0,1};

  shapes[4][1][0] = (Coord){2,0};
  shapes[4][1][1] = (Coord){1,2};
  shapes[4][1][2] = (Coord){1,1};
  shapes[4][1][3] = (Coord){1,0};

  shapes[4][2][0] = (Coord){2,2};
  shapes[4][2][1] = (Coord){0,1};
  shapes[4][2][2] = (Coord){1,1};
  shapes[4][2][3] = (Coord){2,1};

  shapes[4][3][0] = (Coord){0,2};
  shapes[4][3][1] = (Coord){1,0};
  shapes[4][3][2] = (Coord){1,1};
  shapes[4][3][3] = (Coord){1,2};

  // Z
  shapes[5][0][0] = (Coord){0,0};
  shapes[5][0][1] = (Coord){1,0};
  shapes[5][0][2] = (Coord){1,1};
  shapes[5][0][3] = (Coord){2,1};

  shapes[5][1][0] = (Coord){2,0};
  shapes[5][1][1] = (Coord){2,1};
  shapes[5][1][2] = (Coord){1,1};
  shapes[5][1][3] = (Coord){1,2};

  shapes[5][2][0] = (Coord){2,2};
  shapes[5][2][1] = (Coord){1,2};
  shapes[5][2][2] = (Coord){1,1};
  shapes[5][2][3] = (Coord){0,1};

  shapes[5][3][0] = (Coord){0,2};
  shapes[5][3][1] = (Coord){0,1};
  shapes[5][3][2] = (Coord){1,1};
  shapes[5][3][3] = (Coord){1,0};

  // S
  shapes[6][0][0] = (Coord){2,0};
  shapes[6][0][1] = (Coord){1,0};
  shapes[6][0][2] = (Coord){1,1};
  shapes[6][0][3] = (Coord){0,1};

  shapes[6][1][0] = (Coord){1,0};
  shapes[6][1][1] = (Coord){1,1};
  shapes[6][1][2] = (Coord){2,1};
  shapes[6][1][3] = (Coord){2,2};

  shapes[6][2][0] = (Coord){2,1};
  shapes[6][2][1] = (Coord){1,1};
  shapes[6][2][2] = (Coord){1,2};
  shapes[6][2][3] = (Coord){0,2};

  shapes[6][3][0] = (Coord){1,2};
  shapes[6][3][1] = (Coord){1,1};
  shapes[6][3][2] = (Coord){0,1};
  shapes[6][3][3] = (Coord){0,0};
}
