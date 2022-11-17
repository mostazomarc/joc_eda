#ifndef Structs_hh
#define Structs_hh


#include "Utils.hh"


/**
 * Contains the Dir enumeration,
 * the CommandType enumeration,
 * the Pos struct,
 * the CellType enumeration, the Cell struct,
 * the UnitType enumeration, the Unit struct,
 * and some useful little functions.
 */


/**
 * Enum to encode directions. Remember that alive units cannot move diagonally
 */
enum Dir {
  Down, DR, Right, RU, Up, UL, Left, LD
};

inline ostream& operator << (ostream& out, Dir d) {
  switch (d) {
  case Down:   out << "Down";  break;
  case DR:     out << "DR";    break;
  case Right:  out << "Right"; break;
  case RU:     out << "RU";    break;
  case Up:     out << "Up";    break;
  case UL:     out << "UL";    break;
  case Left:   out << "Left";  break;
  case LD:     out << "LD";    break;
  default:     out << int(d);  break;
  }
  return out;
}


/**
 * Simple struct to handle positions.
 */
struct Pos {

  int i, j;

  /**
   * Default constructor (0, 0).
   */
  Pos ();
  /**
   * Constructor with all defining fields.
   */
  Pos (int i, int j);

  /**
   * Print operator.
   */
  friend ostream& operator<< (ostream& os, const Pos& p);

  /**
   * Comparison operator.
   */
  friend bool operator== (const Pos& a, const Pos& b);

  /**
   * Comparison operator.
   */
  friend bool operator!= (const Pos& a, const Pos& b);

  /**
   * Comparison operator, mostly needed for sorting.
   */
  friend bool operator< (const Pos& a, const Pos& b);

  /**
   * Increment operator: moves a position according to a direction.
   */
  Pos& operator+= (Dir d);

  /**
   * Addition operator: Returns a position by adding a direction.
   */
  Pos operator+ (Dir d) const;

  /**
   * Increment operator: moves a position according to another position.
   */
  Pos& operator+= (Pos p);

  /**
   * Addition operator: Returns a position by adding another position.
   */
  Pos operator+ (Pos p) const;
};

/**
 * Defines kinds of cells.
 */
enum CellType {
  Street,
  Waste
};


inline ostream& operator << (ostream& out, CellType c) {
  switch (c) {
  case Street:   out << "Street";   break;
  case Waste:    out << "Waste"; break;
  default:       out << int(c);     break;    
  }
  return out;
}


/**
 * Describes a cell on the board, and its contents.
 */
struct Cell {

  CellType         type; // The kind of cell (street or waste).
  int             owner; // The player that owns it, otherwise -1.
  int                id; // The id of a unit if present, or -1 otherwise.
  bool             food; // Whether it contains food

  /**
   * Default constructor (Street, -1, -1, false).
   */
  Cell ();

  /**
   * Constructor with all defining fields.
   */
  Cell (CellType t, int o, int i, bool f);

  bool is_empty( ) const;
};


/**
 * Defines the type of the unit.
 */
enum UnitType {
  Alive,
  Dead,
  Zombie
};


inline ostream& operator << (ostream& out, UnitType c) {
  switch (c) {
  case  Alive:  out << "Alive";  break;
  case   Dead:  out << "Dead";  break;
  case Zombie:  out << "Zombie";  break;
  default:      out << int(c);    break;    
  }
  return out;
}


/**
 * Describes an unit on the board and its properties.
 */
struct Unit {

  UnitType type;         // The type of unit.
  int id;                // The unique id of this unit during the game.
  int player;            // The player that owns this unit (-1 if is a zombie)
  Pos pos;               // The position on the board.    
  int rounds_for_zombie; // Rounds before it becomes a zombie (-1 if is not being converted to zombie)
  
  /**
   * Default constructor (Alive, -1, -1, (0, 0), -1).
   */
  Unit ();

  /**
   * Constructor with all defining fields.
   */
  Unit (UnitType t, int i, int pl,  Pos p, int r);

};


//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////


/**
 * Enum to encode commands.
 */
enum CommandType {
  Move
};


inline ostream& operator << (ostream& out, CommandType c) {
  switch (c) {
  case Move:       out << "Move";      break;
  default:         out << int(c);      break;
  }
  return out;
}


inline bool dir_ok (Dir dir) {
  return dir >= Down and dir <= LD;
}

inline Pos::Pos (            ) : i(0), j(0) { }
inline Pos::Pos (int i, int j) : i(i), j(j) { }

inline ostream& operator<< (ostream& os, const Pos& p) {
  return os << "(" << p.i << ", " << p.j << ")";
}

inline bool operator== (const Pos& a, const Pos& b) {
  return a.i == b.i and a.j == b.j;
}

inline bool operator!= (const Pos& a, const Pos& b) {
  return not (a == b);
}

inline bool operator< (const Pos& a, const Pos& b) {
  if (a.i != b.i) return a.i < b.i;
  return a.j < b.j;
}

inline Pos& Pos::operator+= (Dir d) {
  switch (d) {
  case Down:   ++i;  break;
  case DR: ++i; ++j; break;
  case Right:  ++j;  break;
  case RU: --i; ++j; break;
  case Up:     --i;  break;
  case UL: --i; --j; break;
  case Left:   --j;  break;
  case LD: ++i; --j; break;
  default: ; // do nothing
  }
  return *this;
}

inline Pos Pos::operator+ (Dir d) const {
  Pos p = *this;
  p += d;
  return p;
}

inline Pos& Pos::operator+= (Pos p) {
    this->i += p.i;
    this->j += p.j;
    return *this;
  }

inline Pos Pos::operator+ (Pos p) const {
    Pos p2 = *this;
    p2 += p;
    return p2;
  }


inline Cell::Cell (                              ) :
  type(Street), owner(-1), id(-1), food(false) { }
inline Cell::Cell (CellType t, int o, int i, bool f) :
  type(t),      owner(o),     id(i), food(false)  { }

inline bool Cell::is_empty ( ) const {
  return
    type == Street and
    id == -1 and
    not food;
}

inline bool command_type_ok (CommandType c_type) {
  return c_type == Move;
}


/**
 * Conversion from command type to char.
 */
inline char CommandType2char (int t) {
  switch (t) {
  case Move:      return 'm';
  default:        return '_';
  }
}


/**
 * Conversion from char to command type.
 */
inline int char2CommandType (char c) {
  switch (c) {
  case 'm': return Move;
  }
  return -1; // Can't abort: if data were corrupted, master would fail.
}


/**
 * Conversion from direction to char.
 * u, d, l, r are obvious
 * diagonals are named after the position of letters in the keyboard:
 * qwe
 * asd
 * zxc
 */
inline char Dir2char (int d) {
  switch (d) {
  case Down:   return 'd';
  case DR:     return 'c';
  case Right:  return 'r';
  case RU:     return 'e';
  case Up:     return 'u';
  case UL:     return 'q';
  case Left:   return 'l';
  case LD:     return 'z';
  default:     return '_';
  }
}

/**
 * Conversion from char to direction.
 */
inline int char2Dir (char c) {
  switch (c) {
  case 'd': return Down;
  case 'c': return DR;
  case 'r': return Right;
  case 'e': return RU;
  case 'u': return Up;
  case 'q': return UL;
  case 'l': return Left;
  case 'z': return LD;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


/**
 * Conversion from unit type to char.
 */
inline char UnitType2char (int t) {
  switch (t) {
  case Alive:  return 'a';
  case Dead:   return 'd';
  case Zombie: return 'z';
  default:     return '_';
  }
}


/**
 * Conversion from char to unit type.
 */
inline int char2UnitType (char c) {
  switch (c) {
  case 'a': return Alive;
  case 'd': return Dead;
  case 'z': return Zombie;
  }
  return -1; // Can't abort: if data is corrupted, master will fail.
}


inline Unit::Unit ()
  : type(Alive), id(-1), player(-1), pos(0, 0), rounds_for_zombie(-1) { }

inline Unit::Unit (UnitType t, int i, int pl, Pos p, int r)
  : type(t), id(i),  player(pl), pos(p), rounds_for_zombie(r) { }


#endif

