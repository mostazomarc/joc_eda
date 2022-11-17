#include "Settings.hh"


Settings Settings::read_settings (istream& is) {
  Settings r;
  string s, v;

  // Version, compared part by part.
  istringstream vs(version());
  while (!vs.eof()) {
    is >> s;
    vs >> v;
    assert(s == v);
  };
  
  is >> s >> r.NUM_PLAYERS;
  _my_assert(s == "NUM_PLAYERS", "Expected 'NUM_PLAYERS' while parsing.");
  _my_assert(r.NUM_PLAYERS == 4, "NUM_PLAYERS should be 4.");

  is >> s >> r.NUM_ROUNDS;
  _my_assert(s == "NUM_ROUNDS", "Expected 'NUM_ROUNDS' while parsing.");
  _my_assert(r.NUM_ROUNDS >= 1,   "NUM_ROUNDS should be >=1.");

  is >> s >> r.BOARD_ROWS;
  _my_assert(s == "BOARD_ROWS", "Expected 'BOARD_ROWS' while parsing.");
  _my_assert(r.BOARD_ROWS == 60, "BOARD_COLS should be 60.");
  
  is >> s >> r.BOARD_COLS;
  _my_assert(s == "BOARD_COLS", "Expected 'BOARD_COLS' while parsing.");
  _my_assert(r.BOARD_COLS == 60, "BOARD_COLS should be 60.");

  is >> s >> r.NUM_INI_UNITS_PER_CLAN;
  _my_assert(s == "NUM_INI_UNITS_PER_CLAN", "Expected 'NUM_INI_UNITS_PER_CLAN' while parsing.");

  is >> s >> r.NUM_INI_ZOMBIES;
  _my_assert(s == "NUM_INI_ZOMBIES", "Expected 'NUM_INI_ZOMBIES' while parsing.");

  is >> s >> r.NUM_INI_FOOD;
  _my_assert(s == "NUM_INI_FOOD", "Expected 'NUM_INI_FOOD' while parsing.");
  _my_assert(r.NUM_INI_FOOD >=1, "NUM_INI_FOOD should be >=1.");

  is >> s >> r.CLAN_INI_STRENGTH;
  _my_assert(s == "CLAN_INI_STRENGTH", "Expected 'CLAN_INI_STRENGTH' while parsing.");
  _my_assert(r.CLAN_INI_STRENGTH >=1, "CLAN_INI_STRENGTH should be >=1.");

  is >> s >> r.POINTS_FOR_KILLING_PERSON;
  _my_assert(s == "POINTS_FOR_KILLING_PERSON", "Expected 'POINTS_FOR_KILLING_PERSON' while parsing.");
  _my_assert(r.POINTS_FOR_KILLING_PERSON >=1, "POINTS_FOR_KILLING_PERSON should be >=1.");

  is >> s >> r.POINTS_FOR_KILLING_ZOMBIE;
  _my_assert(s == "POINTS_FOR_KILLING_ZOMBIE", "Expected 'POINTS_FOR_KILLING_ZOMBIE' while parsing.");
  _my_assert(r.POINTS_FOR_KILLING_ZOMBIE < r.POINTS_FOR_KILLING_PERSON, "POINTS_FOR_KILLING_ZOMBIE should be < POINTS_FOR_KILLING_.");

  is >> s >> r.POINTS_PER_OWNED_CELL;
  _my_assert(s == "POINTS_PER_OWNED_CELL", "Expected 'POINTS_PER_OWNED_CELL' while parsing.");
  _my_assert(r.POINTS_PER_OWNED_CELL >= 1, "POINTS_PER_OWNED_CELL should be >= 1");

  is >> s >> r.FOOD_STRENGTH;
  _my_assert(s == "FOOD_STRENGTH", "Expected 'FOOD_STRENGTH' while parsing.");
  _my_assert(r.FOOD_STRENGTH >= 1, "FOOD_STRENGTH should be >= 1");

  is >> s >> r.ROUNDS_BEFORE_BECOMING_ZOMBIE;
  _my_assert(s == "ROUNDS_BEFORE_BECOMING_ZOMBIE", "Expected 'ROUNDS_BEFORE_BECOMING_ZOMBIE' while parsing.");
  _my_assert(r.ROUNDS_BEFORE_BECOMING_ZOMBIE >= 1, "ROUNDS_BEFORE_BECOMING_ZOMBIE should be >= 1");

  return r;
}
