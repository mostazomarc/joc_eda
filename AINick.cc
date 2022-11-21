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

  // Returns Dir to the nearest avialable adjacent space or somewhere to move otherwise
  Dir space_adj(Pos p)
  {
    Pos act = p;
    if (pos_ok(act + Down) and (act.i - 1 >= 0) and cell(act + Down).owner != me() and cell(act + Down).type == Street)
      return Down;
    if (pos_ok(act + Up) and (act.i + 1 <= 59) and cell(act + Up).owner != me() and cell(act + Up).type == Street)
      return Up;
    if (pos_ok(act + Left) and (act.j - 1 >= 0) and cell(act + Left).owner != me() and cell(act + Left).type == Street)
      return Left;
    if (pos_ok(act + Right) and (act.j + 1 <= 59) and cell(act + Right).owner != me() and cell(act + Right).type == Street)
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

  // auxiliars del bfs
  bool accesible(Pos pos)
  {
    if (cell(pos).type == Street)
      return true;
    return false;
  }

  bool menjar(Pos pos)
  {
    if (cell(pos).food)
      return true;
    return false;
  }

  bool proximpas(const Pos pos, const Dir sdir, const vector<vector<int>> &dist)
  {
    if (sdir == Down)
      return pos_ok(pos + Down) and (pos.i + 1 <= 59) and dist[pos.i + 1][pos.j] == -1 and accesible(pos + Down);
    if (sdir == Up)
      return pos_ok(pos + Up) and (pos.i - 1 >= 0) and dist[pos.i - 1][pos.j] == -1 and accesible(pos + Up);
    if (sdir == Left)
      return pos_ok(pos + Left) and (pos.j - 1 >= 0) and dist[pos.i][pos.j - 1] == -1 and accesible(pos + Left);
    if (sdir == Right)
      return pos_ok(pos + Right) and (pos.j + 1 <= 59) and dist[pos.i][pos.j + 1] == -1 and accesible(pos + Right);
    
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
    if (dist[posfood.i][posfood.j] > 10)
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

    if (posfood.i > p.i and accesible(act+Down))
      return Down;
    if (posfood.i < p.i and accesible(act+Up))
      return Up;
    if (posfood.j > p.j and accesible(act+Right))
      return Right;
    if (posfood.j < p.j and accesible(act+Left))
      return Left;

    return DR;
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
      if (dir != DR and strength(me()) > 90)
      {
        cerr << "som forts " << strength(me()) << endl;
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
