#define SIZE 4
#define TYPES 7
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
  int type;
};

class Tetromino // Tetromino wrap class
{
protected:
  Coord position;
  Coord prev_position;
  Coord shapes[TYPES][SIZE][SIZE];
  int type; // 0-6 I O T L J Z S
  bool onBoard; // If tetromino is presented on board
  Coord initPosition;
  BoardUnit** board;
  int boardWidth;
  int boardHeight;
  int orientation; // possible values: 0 1 2 3
  int prev_orientation;
  bool collision(Coord p, int ori);
  bool inBoard(Coord p);
public:
  Tetromino();
  void setBoard(BoardUnit** _board ,int _boardWidth, int _boardHeight){board = _board; boardWidth = _boardWidth; boardHeight = _boardHeight;}; // Game board
  void putOnBoard();
  bool rotate(); // Go to next orientation
  bool move(int x, int y);
  void setOrientation(int _orientation){orientation = _orientation;}; // Set orientation 0 1 2 3
  void setPosition(int x, int y){position.x = x; position.y = y;};
  void setType(int _type);
  void init();
  bool touch_bottom();
  bool blockOut();
  Coord getInitPosition(){return initPosition;};
};
