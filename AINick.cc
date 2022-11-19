#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nick

// Returns whether (i, j) is a position inside the board. 
bool pos_ok(int i , int j );

//Returns the nearest avialable food
int bfs ( int r, int c)
{

}


/* Estrategia:
      - Si moltes unitats -- Buscar menjar
      - Si poques unitats -- Matar zombis i buscar menjar*/

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  struct Pos { 
    int i, j;
  };

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

    vector<int> alive = alive_units(me());
    int força = strength (me());

    // Write debugging info about my units
    cerr << "At round " << round() << " player " << me() << " has " << alive.size() << " alive units: ";
    for (auto id : alive) {
      cerr << id << " at pos " << unit(id).pos << "; ";
    }
    cerr << endl;



    
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
