struct Coord
{
  int x;
  int y;
  Coord operator+(const Coord &b)
  {
    Coord ret;
    ret.x = this->x+b.x;
    ret.y = this->y+b.y;
    return ret;
  }
  Coord operator-(const Coord &b)
  {
    Coord ret;
    ret.x = this->x-b.x;
    ret.y = this->y-b.y;
    return ret;
  }
  bool operator==(const Coord &b)
  {
    return(b.x == this->x && b.y == this->y);
  }
};

struct BoardUnit
{
  bool occupy;
  int type; // For the color
};

// A wrap class for a game board closed in left right and bottom
class GameBoard
{
private:
  BoardUnit** board;
  SDL_Renderer renderer;
public:
  GameBoard(int boardWidth, int boardHeight, SDL_Renderer _renderer);
  bool collision(Coord p, int ori);
  bool inBoard(Coord p);
  void clearBoard();
}
