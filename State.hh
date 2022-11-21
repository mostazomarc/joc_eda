#ifndef State_hh
#define State_hh


#include "Structs.hh"
#include "Settings.hh"

/**
 * Contains a class to store the current state of a game.
 */


/**
 * Stores the game state.
 */
class State {

public:

  /**
   * Returns the current round.
   */
  int round () const;

  /**
   * Returns a copy of the cell at (i, j).
   */
  Cell cell (int i, int j) const;

  /**
   * Returns a copy of the cell at p.
   */
  Cell cell (Pos p) const;

  /**
   * Returns the a copy of the unit with identifier id.
   */
  Unit unit (int id) const;

  /**
   * Returns the ids of the alive units of a player
   */
  vector<int> alive_units (int pl) const;

  /**
   * Returns the ids of the dead units of a player
   */
  vector<int> dead_units (int pl) const;

  /**
   * Returns the ids of the zombies
   */
  vector<int> zombies () const;
  
  /**
   * Returns the current strength of a player ( strength_points/alive_units )
   */
  int strength (int pl) const;
  
  /**
   * Returns the current score of a player.
   */
  int score (int pl) const;

  /**
   * Returns the percentage of cpu time used so far, in the
   * range [0.0 - 1.0] or a value lesser than 0 if the player is dead.
   */
   // NOTE: only returns a sensible value in server executions.
   // In local executions the returned value is meaningless.
  double status (int pl) const;


  //////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

private:

  friend class Info;
  friend class Board;
  friend class Game;
  friend class SecGame;
  friend class Player;

  vector< vector<Cell> >   grid;
  
  vector<int>              scr; // score of each player
  vector<int>              scr_accumulated; // score referring to units and zombies killed
  vector<int>              nb_cells; // current number of cells conquered by player
  vector<int>              overall_strength; // strength of player (to be distributed among units)

  vector<double>           stats; // -1 -> dead, 0..1 -> % of cpu time limit
  int                      rnd;
  
  map<int, Unit>           units;
  vector< set<int> >       player2alive_units;
  vector< set<int> >       player2dead_units;
  set<int>                 zombies_;

  /**
   * Returns whether id is a valid citizen identifier.
   */
  inline bool unit_ok (int id) const {
    return units.count(id);
  }
};

inline int State::round () const {
  return rnd;
}

inline Cell State::cell (int i, int j) const {
  if (i >= 0 and i < (int)grid.size() and j >= 0 and j < (int)grid[i].size())
    return grid[i][j];
  else {
    cerr << "warning: cell requested for position " << Pos(i, j) << endl;
    return Cell();
  }
}

inline Cell State::cell (Pos p) const {
  return cell(p.i, p.j);
}

inline Unit State::unit (int id) const {
  auto it = units.find(id);
  if (it != units.end()) {
    return it->second;
  }
  else {
    cerr << "warning: unit requested for identifier " << id << endl;
    return Unit();
  }
}

inline vector<int> State::alive_units (int pl) const {
  if (pl >= 0 and pl < (int) player2alive_units.size())
    return vector<int>(player2alive_units[pl].begin(), player2alive_units[pl].end());
  else {
    cerr << "warning: alive units requested for player " << pl << endl;
    return vector<int>();
  }
}

inline vector<int> State::dead_units (int pl) const {
  if (pl >= 0 and pl < (int) player2dead_units.size())
    return vector<int>(player2dead_units[pl].begin(), player2dead_units[pl].end());
  else {
    cerr << "warning: dead units requested for player " << pl << endl;
    return vector<int>();
  }
}

inline vector<int> State::zombies () const {
  return vector<int>(zombies_.begin(), zombies_.end());
}

inline int State::strength (int pl) const {
  if (pl >= 0 and pl < (int) scr.size()) {
    if (player2alive_units[pl].size() == 0) return 0;
    else return overall_strength[pl]/player2alive_units[pl].size();
  }
  else {
    cerr << "warning: strength requested for player " << pl << endl;
    return -1;
  }
}


inline int State::score (int pl) const {
  if (pl >= 0 and pl < (int) scr.size())
    return scr[pl];
  else {
    cerr << "warning: score requested for player " << pl << endl;
    return -1;
  }
}


inline double State::status (int pl) const {
  if (pl >= 0 and pl < (int)stats.size())
    return stats[pl];
  else {
    cerr << "warning: status requested for player " << pl << endl;
    return -2;
  }
}

#endif
