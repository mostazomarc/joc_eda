//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#ifndef Board_hh
#define Board_hh


#include "Info.hh"
#include "Action.hh"
#include "Random.hh"


/**
 * Contains the Board class, with all the game information,
 * plus the names of the players and a random generator.
 */


/**
 * Manages a random generator and the information of the board.
 */
class Board : public Info, public Random_generator {

  friend class Game;
  friend class SecGame;

  vector<string> names;
  int            fresh_id;

  /**
   * Checks whether initial fixed board is ok
   */
  void check_is_good_initial_fixed_board() const;  
  
  /**
   * Reads the generator method, and generates or reads the grid.
   */
  void read_generator_and_grid (istream& is) {
    string generator;
    is >> generator;
    if (generator == "FIXED") {
      read_grid(is);
      check_is_good_initial_fixed_board();
    }
    else if (generator == "RANDOM") generate_random_board();
    else                            _my_assert(false,"unknown generator  " + generator);
  }

  /**
   * Prints some information of the unit.
   */
  inline static void print_unit (Unit& u, ostream& os) {
    os << UnitType2char(u.type)  << '\t'
       << u.id                      << '\t'
       << u.player                  << '\t'
       << u.pos.i                   << '\t'
       << u.pos.j                   << '\t'        
       << u.rounds_for_zombie       << '\t'
       << endl;
  }


  /**
   * Tries to apply a move. Returns true if it could.
   */
  bool execute(const Command&    m,
	       vector<int>&          food_to_regenerate,
	       vector<vector<int>>&  zombie_to_unit,
	       vector<vector<int>>&  alive_to_dead
	       );


  /*
   * Returns whether unit u1 wins u2 in an attack
   */
  bool first_unit_wins_attack(const Unit& u1, const Unit& u2);

  /* 
   * Perfom an attack from orig_u to dest_u and updates alive_to_dead accordingly
   */
  void perform_attack (Unit& orig_u, Unit& dest_u, vector<vector<int>>& alive_to_dead);  

  /*
   * Returns whether p is an appropriate position to regenerate an object.
     Should be empty and have no unit/food in the two surrounding squares
   */
  bool is_good_pos_to_regen ( const Pos& p) const;

  /*
   * Tries to get a random position which is good_pos_to_regen. If not, return an empty position
   * Note that there should always be empty positions in the board
   */  
  Pos get_random_pos_where_regenerate ( );
  
  /*
   * All units with rounds_for_zombie == 0 are converted into zombies.
   * player2alive, player2dead and zombies_ are updated accordingly
   */  
  void execute_conversion_to_zombie ( );

  /*
   * zombie_to_unit[p] contains all zombies that player p has killed in last round
   * All these units are regenerated as alive units of player p
   * player2alive and zombies_ are updated accordingly
   */  
  void execute_conversion_zombie_to_alive (vector<vector<int>>& zombie_to_unit);

  /*
   * alive_to_dead[p] contains all alive units of player p that have been killed in last round
   * All these units are converted to dead
   * player2alive and player2dead are updated accordingly
   */  
  void execute_conversion_alive_to_dead (vector<vector<int>>& alive_to_dead);

  /*
   * With probability 20% an alive unit from the clan with largest number of alive units becomes 
   * an alive unit of the clan with smallest number of alive units.
   * player2alive is updated accordingly
   */    
  void execute_random_conversion_live_unit ( );

  /*
   * Returns <p1,p2> where p1 is a player with minimum number of alive units and
   * p2 is a player with maximum number of alive units
   */      
  pair<int,int> selectLargestSmallestClan ( );

  /* 
   * Decrement round_for_zombie for all units such that round_for_zombie > 0
   */
  void decrement_rounds_for_becoming_zombie ( );

  /*
    Generate a new food item on the board. Used for regenerating consumed food items
   */
  void generate_food_item ( );

  /* 
   * food_to_regenerate[p] is the number of food items consumed by player p in last round
   * Food items consumed by zombis are in position 4
   * We regenerate all food items and update the strength of clans accordingly
   */
  void regenerate_food_and_update_strength (vector<int>& food_to_regenerate);

  /*
   * Update nb_cells vector so that it contains the number of cell possessed by each player
   */
  void update_nb_cells ( );

  /* 
   * Update scr vector in order to contain the score of each player
   */
  void update_score ( );

  /* 
   * Returns whether cell(i,j) has a dead unit on it
   */
  bool cell_has_dead_unit (int i, int j);

  /* 
   * Returns whether cell(i,j) has a zombie on it
   */
  bool cell_has_zombie (int i, int j);

  /*
   * Move all zombies at the end of the round by moving them to the closest alive unit
   * Parameters food_to_regenerate, zombie_to_unit, alive_to_dead and commands_done are needed for calling "execute"
   */
  void move_zombies (vector<int>& food_to_regenerate, vector<vector<int>>& zombie_to_unit, vector<vector<int>>& alive_to_dead, vector<Command>& commands_done);
  
  /////////////////////// BEGIN BOARD GENERATION ///////////////////////


  /**
   * Generates a board.
   */
  void generate_random_board ( );
  
  /**
   * Randomnly returns an empty pos
   */
  Pos get_empty_pos ( );

  /**
   * s_id identifies the current street. Position should be not in border. Not already occupied by street. 
   * Num of adjacent cells of the current street is <= 2.
   * Num of adjacents cells of other streets is 0.
   */
  bool pos_ok_for_street(int s_id, const Pos& p);

  /**
   * s_id identifies the current street. Position should be not in border. Not already occupied by street. 
   * Num of adjacent cells of some street is 0.
   */
  bool pos_ok_for_initial_street(const Pos& p);
  
  /**
   * Randomnly returns a position that fulfills pos_ok_for_street(s_id)
   */
  Pos get_ok_pos_for_street (int s_id );

  /**
   * Randomnly returns a position that fulfills pos_ok_for_initial_street()
   */
  Pos get_ok_pos_for_initial_street ( );


  /**
   * Tries to randomly generates num_building_cells cells of type Building in num_street disconnected parts
   * Sometime it does not succeed and generates less.
   */
  void generate_all_waste (int num_building_cells, int num_streets);

  /**
  * Tries to generate a stree of length with identifier s_id
  */
  int generate_waste (int s_id, int length);

  /**
   * Returns the number of connected component grid has (taking into account only streets and buildings)
   */
  int num_connected_components( );

  /** 
   * Auxiliary DFS to count connected components
   */
  void explore_from(vector<vector<int>>& G, int i, int j, int n);

  /** 
   * Creates a unit alive unit for player in position p
   */  
  void create_new_unit (Pos& p, int player);

  /** 
   * Creates a zombie at position p
   */  
  void create_new_zombie (Pos& p);
  
  // Grid that stores the already generated streets. 0 means no street. Otherwise, contains street_ids
  vector<vector<int>> street_plan;


  /////////////////////// END BOARD GENERATION ///////////////////////  
  
public:

  /**
   * Construct a board by reading information from a stream.
   */
  Board (istream& is, int seed);

  /**
   * Returns the name of a player.
   */
  inline string name (int pl) const {
    _my_assert(player_ok(pl), "Player is not ok.");
    return names[pl];
  }

  /**
   * Prints the board settings to a stream.
   */
  void print_settings (ostream& os) const;

  /**
   * Prints the name players to a stream.
   */
  void print_names (ostream& os) const;

  /**
   * Prints the state of the board to a stream.
   */
  void print_state (ostream& os);

  /**
   * Prints the results and the names of the winning players.
   */
  void print_results () const;

  /**
   * Computes the next board aplying the given actions to the current board.
   * It also prints to os the actual actions performed.
   */
  void next (const vector<Action>& act, ostream& os);

};

#endif
