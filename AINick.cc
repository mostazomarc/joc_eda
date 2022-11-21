#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nick

/* Estrategia:
      - Si moltes unitats -- Buscar menjar
      - Si poques unitats -- Matar zombis i buscar menjar*/

struct PLAYER_NAME : public Player
{

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player *factory()
  {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up, Down, Left, Right};
  vector<int> alive;

   // auxiliars

  //retorna ture si la posició es accesible (no te Waste)
  bool accesible(Pos pos)
  {
    if (pos_ok(pos) and cell(pos).type == Street)
      return true;
    return false;
  }

  //retorna true si a la posició n'hi ha menjar
  bool menjar(Pos pos)
  {
    if (cell(pos).food)
      return true;
    return false;
  }

  //retorna si la posició es conquerible
  bool conq(Pos p)
  {
    return pos_ok(p) and cell(p).owner != me() and accesible(p);
  }

  // Returns Dir to the nearest avialable adjacent space or somewhere to move otherwise
  Dir space_adj(Pos p)
  {
    Pos act = p;
    if (conq(act+Down))
      return Down;
    if (conq(act+Up))
      return Up;
    if (conq(act+Left))
      return Left;
    if (conq(act+Right))
      return Right;

    cerr << "no adjacent spaces at pos" << p.i << ',' << p.j << endl;

    if (pos_ok(act + Up) and cell(act + Up).type == Street)
      return Up;
    if (pos_ok(act + Down) and cell(act + Down).type == Street)
      return Down;
    if (pos_ok(act + Right) and cell(act + Right).type == Street)
      return Right;
    if (pos_ok(act + Left) and cell(act + Left).type == Street)
      return Left;
    return Right;
  }

  //retorna si el proxim pas del bfs es una posició accesible i no visitada
  bool proximpas(const Pos pos, const Dir sdir, const vector<vector<int>> &dist)
  {
    if (sdir == Down)
      return (pos.i + 1 <= 59) and dist[pos.i + 1][pos.j] == -1 and accesible(pos + Down);
    if (sdir == Up)
      return (pos.i - 1 >= 0) and dist[pos.i - 1][pos.j] == -1 and accesible(pos + Up);
    if (sdir == Left)
      return (pos.j - 1 >= 0) and dist[pos.i][pos.j - 1] == -1 and accesible(pos + Left);
    if (sdir == Right)
      return (pos.j + 1 <= 59) and dist[pos.i][pos.j + 1] == -1 and accesible(pos + Right);
    
    else return false;
  }

  // Returns Dir to the nearest avialable food
  Dir bfs_food(int id, Pos p)
  {
    int i = p.i;
    int j = p.j;
    int n = 60;
    int m = 60;

    Pos posnull(-1, -1);

    queue<Pos> Q;                                           // Posicions per mirar (no s'afegirà una pos a la cua si no es accesible)
    vector<vector<int>> dist(n, vector<int>(m, -1));        // distancia per cada posició
    vector<vector<Pos>> previs(n, vector<Pos>(m, posnull)); // previ de cada posició visitada (s'haurà de borrar)
    stack<Pos> camí;

    Q.push(p);          // apuntem primera posició per mirar 
    dist[p.i][p.j] = 0; // distancia primera posició es 0

    bool food = false;
    Pos posfood = p;
    while (not Q.empty() and not food) // and dist[act.i][act.j] <= 7) //mentre n'hi hagin posicions per mirar
    {
      Pos act = Q.front(); // agafem primera posició i la treiem de la cua
      Q.pop();
      camí.push(act); // guardem la posició com camí fet

      // cerr << "pos act a mirar " << act.i << ',' << act.j << endl;

      if (menjar(act))
      {
        food = true; // si es menjar food = true perque hem trobat menjar
        posfood = act;
        cerr << id << " found food at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else
      {
        if (proximpas(act, Down, dist))
        {
          Q.push(act + Down);                              // afegir pos a la cua
          dist[act.i + 1][act.j] = dist[act.i][act.j] + 1; // actualitzar distancia
          previs[act.i + 1][act.j] = act;
        }
        if (proximpas(act, Up, dist))
        {
          Q.push(act + Up);
          dist[act.i - 1][act.j] = dist[act.i][act.j] + 1;
          previs[act.i - 1][act.j] = act;
        }
        if (proximpas(act, Left, dist))
        {
          Q.push(act + Left);
          dist[act.i][act.j - 1] = dist[act.i][act.j] + 1;
          previs[act.i][act.j - 1] = act;
        }
        if (proximpas(act, Right, dist))
        {
          Q.push(act + Right);
          dist[act.i][act.j + 1] = dist[act.i][act.j] + 1;
          previs[act.i][act.j + 1] = act;
        }
      }
    }

    // Si no hem trobat food retornem una dirreció imposible 'DR'
    if (not food)
    {
      cerr << id << " food not found" << endl;
      return DR;
    }
    if (dist[posfood.i][posfood.j] > 12)
    {
      cerr << id << " food massa lluny" << endl;
      return DR;
    }

    // Obtenim primera posició del camí fet
    Pos act = posfood;
    cerr << id << " getting food at " << act.i << ',' << act.j << "from " << p.i << ',' << p.j << endl;
    while (previs[act.i][act.j] != p)
    {
      act = previs[act.i][act.j];
    }
    cerr << id << " anire de p:" << p.i << ',' << p.j << " a nou: " << act.i << ' ' << act.j << endl;

    if (act.i > p.i and accesible(act+Down))
      return Down;
    if (act.i < p.i and accesible(act+Up))
      return Up;
    if (act.j > p.j and accesible(act+Right))
      return Right;
    if (act.j < p.j and accesible(act+Left))
      return Left;

    return DR;
  }

  void lluita(int id)
  {
    Pos pos = unit(alive[id]).pos;
    int jo = me();

    //get id of adjacent players
    int idup = cell(pos+Up).id;
    int iddown = cell(pos+Down).id;
    int idright = cell(pos+Right).id;
    int idleft = cell(pos+Left).id;

    if (idup != -1) {
      Unit unitup = unit(idup);
      if (strength(unitup.player) < strength(jo)) move (id,Up);
    }
    if (iddown != -1) {
      Unit unitdown = unit(iddown);
      if (strength(unitdown.player) < strength(jo)) move (id,Down);
    }
    if (idright != -1) {
      Unit unitright = unit(idright);
      if (strength(unitright.player) < strength(jo)) move (id,Right);
    }
    if (idleft != -1) {
      Unit unitleft = unit(idleft);
      if (strength(unitleft.player) < strength(jo)) move (id,Left);
    }
  }



  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {

    alive = alive_units(me());
    int força = strength(me());

    // Write debugging info about my units
    cerr << "At round " << round() << " player " << me() << " has " << alive.size() << " alive units: ";
    for (auto id : alive)
    {
      cerr << id << " at pos " << unit(id).pos << "; ";
    }
    cerr << endl;

    for (int id = 0; id < alive.size(); ++id)
    {
      Pos unitpos = unit(alive[id]).pos;
      cerr << "start BFS of " << alive[id] << " at pos " << unitpos.i << ',' << unitpos.j << endl;
      Dir dir = bfs_food(id, unitpos);
      lluita(id);
      if (dir != DR)
      {
        cerr << "unit " << id << " will go " << dir << endl;
        move(alive[id], dir);
      }
      else
      {
        cerr << "conquistant..." << endl;
        dir = space_adj(unitpos);
        cerr << "unit " << id << " conquistara cap a " << dir << endl;
        move(alive[id], dir);
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
