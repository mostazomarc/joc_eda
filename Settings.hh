#ifndef Settings_hh
#define Settings_hh


#include "Structs.hh"


/** \file
 * Contains a class to store all the game settings that do not change
 * during a game, except the names of the players.
 */


/**
 * Stores most of the game settings.
 */
class Settings {

public:
    /**
   * Returns a string with the game name and version.
   */
  static string version ();

  /**
   * Returns the number of players in the game.
   */
  int num_players () const;

  /**
   * Returns the number of rounds a match lasts.
   */
  int num_rounds () const;

  /**
   * Returns the number of rows of the board.
   */
  int board_rows () const;

  /**
   * Returns the number of columns of the board.
   */
  int board_cols () const;

  /**
   * Returns the initial number of units per clan
   */
  int num_ini_units_per_clan () const;

  /**
   * Returns the initial number of zombies on the board
   */
  int num_ini_zombies () const;

  /**
   * Returns the initial number of food items on the board
   */
  int num_ini_food () const;

  /**
   * Returns the initial strength of each clan
   */
  int clan_ini_strength () const;

  /**
   * Returns the points obtained after killing a person
   */
  int points_for_killing_person () const;

  /**
   * Returns the points obtained after killing a zombie
   */
  int points_for_killing_zombie () const;

  /**
   * Returns the points obtained for each owned cell at the end of a round
   */
  int points_per_owned_cell () const;

  /**
   * Returns the units of strenght obtained by eating an item of food
   */
  int food_strength () const;

  /**
   * Returns the number of rounds before a bitten/dead person becomes a zombie
   */
  int rounds_before_becoming_zombie () const;

  /**
   * Returns whether pl is a valid player identifier.
   */
  bool player_ok (int pl) const;

  /**
   * Returns whether (i, j) is a position inside the board.
   */
  bool pos_ok (int i, int j) const;

  /**
   * Returns whether p is a position inside the board.
   */
  bool pos_ok (Pos p) const;

private:
  
  friend class Info;
  friend class Board;
  friend class Game;
  friend class SecGame;
  friend class Player;

  int NUM_PLAYERS;
  int NUM_ROUNDS;
  int BOARD_ROWS;
  int BOARD_COLS;
  int NUM_INI_UNITS_PER_CLAN;
  int NUM_INI_ZOMBIES;
  int NUM_INI_FOOD;
  int CLAN_INI_STRENGTH;
  int POINTS_FOR_KILLING_PERSON;
  int POINTS_FOR_KILLING_ZOMBIE;
  int POINTS_PER_OWNED_CELL;
  int FOOD_STRENGTH;
  int ROUNDS_BEFORE_BECOMING_ZOMBIE;
  
  /**
   * Reads the settings from a stream.
   */
  static Settings read_settings (istream& is);

  
  bool ok () const;
};


inline string Settings::version () {
  return "TheWalkingDead 1.0";
}

inline int Settings::num_players                   () const { return NUM_PLAYERS                    ;}
inline int Settings::num_rounds                    () const { return NUM_ROUNDS                     ;}
inline int Settings::board_rows                    () const { return BOARD_ROWS                     ;}
inline int Settings::board_cols                    () const { return BOARD_COLS                     ;}
inline int Settings::num_ini_units_per_clan        () const { return NUM_INI_UNITS_PER_CLAN         ;}
inline int Settings::num_ini_zombies               () const { return NUM_INI_ZOMBIES                ;}
inline int Settings::num_ini_food                  () const { return NUM_INI_FOOD                   ;}
inline int Settings::clan_ini_strength             () const { return CLAN_INI_STRENGTH              ;}
inline int Settings::points_for_killing_person     () const { return POINTS_FOR_KILLING_PERSON      ;}
inline int Settings::points_for_killing_zombie     () const { return POINTS_FOR_KILLING_ZOMBIE      ;}
inline int Settings::points_per_owned_cell         () const { return POINTS_PER_OWNED_CELL          ;}
inline int Settings::food_strength                 () const { return FOOD_STRENGTH                  ;}
inline int Settings::rounds_before_becoming_zombie () const { return ROUNDS_BEFORE_BECOMING_ZOMBIE  ;}


inline bool Settings::player_ok (int pl) const {
  return pl >= 0 and pl < num_players();
}

inline bool Settings::pos_ok (int i, int j) const {
  return i >= 0 and i < board_rows() and j >= 0 and j < board_cols();
}

inline bool Settings::pos_ok (Pos p) const {
  return pos_ok(p.i, p.j);
}


inline bool Settings::ok() const {
  if (num_players() != 4) return false;
  return true;
}


#endif
