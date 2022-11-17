//////// STUDENTS DO NOT NEED TO READ BELOW THIS LINE ////////  

#include "Info.hh"

bool Info::ok() const {

  if (int(grid.size()) != board_rows()) {
    cerr << "error: mismatch in number of rows" << endl;
    return false;
  }

  if (int(grid[0].size()) != board_cols()) {
    cerr << "error: mismatch in number of columns" << endl;
    return false;
  }

  if (not (rnd >= 0 and rnd <= num_rounds())) {
    cerr << "error: wrong number of rounds" << endl;
    return false;
  }
  
  for (int pl = 0; pl < num_players(); ++pl) {
    double st = stats[pl];
    if (st != -1 and not (0 <= st and st <= 1)) {
      cerr << "error: status should be -1 or within [0, 1]" << endl;
      return false;
    }
  }

  if (int(player2alive_units.size()) != num_players()) {
    cerr << "error: size of player2alive_units should be number of players" << endl;
    return false;
  }

  if (int(player2dead_units.size()) != num_players()) {
    cerr << "error: size of player2dead_units should be number of players" << endl;
    return false;
  }

  uint expected_units = num_players() * num_ini_units_per_clan() + num_ini_zombies();
  if (units.size() != expected_units) {
    cerr << "error: units has a wrong number of units" << endl;
    return false;
  }
  
  vector<int> tmp_nb_cells(num_players(), 0);
  uint total_ids_in_cells = 0;
  
  for (int i = 0; i < board_rows(); ++i) {
    for (int j = 0; j < board_cols(); ++j) {
      const Cell& c = grid[i][j];

      if (c.type == Waste) {
	if (c.owner != -1) {
	  cerr << "error: waste cells cannot have owner" << endl;
	  return false;
	}
	if (c.id != -1) {
	  cerr << "error: waste cells cannot have a unit on them" << endl;
	  return false;
	}
	if (c.food) {
	  cerr << "error: waste cells cannot have food" << endl;
	  return false;
	}
      }
      else if (c.type == Street) {
	if (c.food and c.id != -1) {
	  cerr << "error: street cells cannot have food and a unit on them" << endl;
	  return false;
	}
	if (c.id != -1) {
	  auto it = units.find(c.id);
          if (it == units.end()) {
            cerr << "error: could not find unit identifier" << endl;
            return false;
          }
	  
	  const Unit& u = it->second;
          if (u.pos != Pos(i, j)) {
            cerr << "error: mismatch in the position of unit " << c.id << endl;
            return false;
          }
	  if (u.id != c.id) {
	    cerr << "error: mismatch in the identifier of unit at pos " << Pos(i,j) << endl;
	    return false;
	  }

	  ++total_ids_in_cells;
	  if (u.type == Zombie){
	    if (c.owner != -1) {
	      cerr << "error: a cell with a zombie on it cannot have owner" << endl;
	      cerr << "This happens with cell " << i << " " << j << " which owns id " << c.id << endl;
	      return false;
	    }
	  }
	  else if (u.player != c.owner) { // It has alive/dead unit
	    cerr << "error: cell contains a unit but is not owned by her player" << endl;
	    return false;
	  }

	  if (u.type == Zombie and zombies_.count(c.id) == 0) {
	    cerr << "error: zombie at position " << Pos(i,j) << " is not in zombies_" << endl;
	    return false;
	  }

	  if (u.type == Alive and player2alive_units[u.player].count(u.id) == 0) {
	    cerr << "error: alive unit at posision " << Pos(i,j) << " is not in player2alive_units" << endl;
	    return false;
	  }

	  if (u.type == Dead and player2dead_units[u.player].count(u.id) == 0) {
	    cerr << "error: dead unit at posision " << Pos(i,j) << " is not in player2dead_units" << endl;
	    return false;
	  }

	}
	
	if (c.owner != -1) {
	  if (c.owner < 0 or c.owner >= num_players()) {
	    cerr << "error: owner of a cell should be a number between 0 and " << num_players() - 1 << endl;
	    return false;
	  }
	  ++tmp_nb_cells[c.owner];	  
	}
      }
      else {
        cerr << "error: cells should be either waster or streets" << endl;
        return false;	
      }
    }
  }

  if (total_ids_in_cells != expected_units) {
    cerr << "error: wrong number of units in the grid" << endl;
    return false;
  }

  if (nb_cells != tmp_nb_cells) {
    cerr << "error: nb_cells structure does not match the contents of the grid" << endl;
    return false;
  }
  
  uint tmp_units = 0;
  for (auto& x : player2alive_units) tmp_units += x.size();
  for (auto& x : player2dead_units)  tmp_units += x.size();
  tmp_units += zombies_.size();

  if (tmp_units != expected_units) {
    cerr << "error: wrong number of units in the player2alive/dead + zombies" << endl;
    return false;
  }

  for (const auto& p : units) {
    const Unit& u = p.second;
    if (u.type != Zombie and not player_ok(u.player)) {
      cerr << "error: wrong player identifier" << endl;
      return false;
    }

    if (u.id < 0 or u.id >= int(expected_units)){
      cerr << "error: wrong identifier for unit" << endl;
      return false;
    }
    
    if (u.rounds_for_zombie == 0 or
	u.rounds_for_zombie < -1 or
	(u.rounds_for_zombie > 0 and u.type == Zombie)) {
      cerr << "error: wrong round_for_zombie in unit " << u.id << endl;
      return false;
    }
  }
  return true;
}
