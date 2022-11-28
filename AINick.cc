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
  const vector<Dir> mesdirs = {Up, Down, Left, Right, DR, RU, UL, LD};
  vector<int> alive;
  const int maxunitats = 15;
  const int mindistmenjar = 12;
  const int mindistenemics = 8;
  const int distcerca = 4;
  const vector<string> busc = {"menjar", "enemic", "conq"};

  // auxiliars

  // retorna ture si la posició es accesible (no te Waste ni cap unitat morta)
  bool accesible(Pos pos)
  {
    if (not pos_ok(pos))
      return false;
    int unitid = cell(pos).id;
    if (unitid != -1)
    {
      Unit u = unit(unitid);
      if (u.type == Dead)
        return false;
    }
    if (pos_ok(pos) and cell(pos).type == Street)
      return true;
    return false;
  }

  // retorna true si a la posició n'hi ha menjar
  bool menjar(Pos pos)
  {
    if (cell(pos).food)
      return true;
    return false;
  }

  // retorna true si a la posició p n'hi ha un zombie
  bool zombie(int unitid)
  {

    if (unitid == -1)
      return false;
    Unit u = unit(unitid);
    if (u.player == -1)
      return true;
    return false;
  }

  // retorna true si a la posició n'hi ha un enemic o un zombie
  bool enemic(Pos p)
  {
    int unitid = cell(p).id;
    if (unitid == -1)
      return false;
    Unit u = unit(unitid);
    if (u.player != me() and u.player != -1)
      return true;
    return false;
  }

  // retorna true si l'enemic de la posició es més fluix que jo o si n'hi ha error
  bool ganador(int id)
  {
    if (id != -1)
    {
      Unit u = unit(id);
      if (strength(u.player) < strength(me()) and u.player != -1)
        return true;
      else
        return false;
    }
    else
      return false;
  }

  // retorna si la posició es conquerible
  bool conq(Pos p)
  {
    return pos_ok(p) and cell(p).owner != me() and accesible(p);
  }

  // Returns Dir to the nearest conquer avialable space
  Dir space_adj(int id, Pos p)
  {
    Pos act = p;
    // mirem si ens podem moure cap a una posició adjacent per conquerir
    if (conq(act + Down))
      return Down;
    if (conq(act + Up))
      return Up;
    if (conq(act + Left))
      return Left;
    if (conq(act + Right))
      return Right;

    cerr << "no adjacent spaces at pos" << p.i << ',' << p.j << endl;

    // buscar direcció cap al space més proxim
    Dir dir = bfs_space(id, act);
    if (dir != DR)
      return dir;

    // si no hem trobat cap space moures cap a on sigui
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

  // retorna si el proxim pas del bfs es una posició accesible i no visitada
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

    return false;
  }

  // retorna si el proxim pas del bfs es una posició accesible, no visitada i que no hi ha cap unitat en aquesta
  bool proximpas_space(const int id, const Pos pos, const Dir sdir, const vector<vector<int>> &dist)
  {
    if (sdir == Down)
      if (cell(pos + Down).id == -1)
      {
        return (pos.i + 1 <= 59) and dist[pos.i + 1][pos.j] == -1 and accesible(pos + Down);
      }
    if (sdir == Up)
      if (cell(pos + Up).id == -1)
      {
        return (pos.i - 1 >= 0) and dist[pos.i - 1][pos.j] == -1 and accesible(pos + Up);
      }
    if (sdir == Left)
      if (cell(pos + Left).id == -1)
      {
        return (pos.j - 1 >= 0) and dist[pos.i][pos.j - 1] == -1 and accesible(pos + Left);
      }
    if (sdir == Right)
      if (cell(pos + Right).id == -1)
      {
        return (pos.j + 1 <= 59) and dist[pos.i][pos.j + 1] == -1 and accesible(pos + Right);
      }

    return false;
  }

  // Returns path to the food
  vector<Pos> bfs(const int id, const Pos p, vector<vector<int>> &dist, const string busca)
  {

    vector<Pos> camíbuit(1);
    camíbuit[0] = p;

    queue<vector<Pos>> Q; // Cua de posibles camins

    Q.push(camíbuit);   // apuntem primera posició com a primer pas del camí
    dist[p.i][p.j] = 0; // distancia primera posició es 0

    bool found = false;
    vector<Pos> camifinal = {p};
    while (not Q.empty() and not found) // mentre n'hi hagin posicions per mirar i no s'hagi trobat menjar
    {
      vector<Pos> camí = Q.front(); // agafem el primer camí i el treiem de la cua
      Q.pop();
      Pos act = camí[camí.size() - 1]; // agafem la ultima pos de el camí

      // cerr << "pos act a mirar " << act.i << ',' << act.j << endl;
      int unitid = cell(act).id;

      if (menjar(act) and ((dist[act.i][act.j] <= distcerca) or ((dist[act.i][act.j] <= mindistmenjar or alive.size() > maxunitats) and busca == "menjar")))
      {
        found = true; // si es menjar food = true perque hem trobat menjar
        camifinal = camí;
        cerr << id << " found food at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (zombie(unitid) and ((dist[act.i][act.j] <= distcerca) or (dist[act.i][act.j] <= mindistenemics and busca == "enemic") or (dist[act.i][act.j] <= mindistenemics and busca == "lastenem")))
      {
        found = true;
        camifinal = camí;
        cerr << id << " found zombie at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (conq(act) and busca == "conq")
      {
        found = true;
        camifinal = camí;
        cerr << id << " found espai per conquerir at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (enemic(act) and ((dist[act.i][act.j] <= distcerca) or (dist[act.i][act.j] <= mindistenemics and busca == "enemic")  or (dist[act.i][act.j] <= mindistenemics and busca == "lastenem")))
      {
        found = true;
        camifinal = camí;
        cerr << id << " found enemic at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else
      {
        for (int i = 0; i < dirs.size(); ++i)
        {
          if (proximpas(act, dirs[i], dist))
          {
            vector<Pos> nou_camí = camí;
            Pos newpos = act + dirs[i];
            nou_camí.push_back(newpos);
            Q.push(nou_camí);                                  // afegir nou_camí (camí + nova posicio) a la cua de camins
            dist[newpos.i][newpos.j] = dist[act.i][act.j] + 1; // actualitzar distancia
          }
        }
      }
    }

    if (found)
      return camifinal;
    else
      return camíbuit;
  }

  Dir dir_menjar(int id, Pos p, const string busca)
  {

    int n = 60;
    int m = 60;

    vector<vector<int>> dist(n, vector<int>(m, -1));
    vector<Pos> camí = bfs(id, p, dist, busca);

    // Si no hem trobat food retornem una dirreció imposible 'DR'
    if (camí.size() < 2)
    {
      cerr << id << " ERROR: nothing found" << endl;
      return DR;
    }
    Pos objectiu = camí[camí.size() - 1];

    // Obtenim primera posició del camí fet
    Pos adjacent = camí[1];
    cerr << id << " getting " << busca << " at " << objectiu.i << ',' << objectiu.j << "from " << p.i << ',' << p.j << endl; // WRONG

    cerr << id << " anire de p:" << p.i << ',' << p.j << " a nou: " << adjacent.i << ' ' << adjacent.j << endl;

    if (adjacent == (p + Down) and accesible(adjacent))
      return Down;
    if (adjacent == (p + Up) and accesible(adjacent))
      return Up;
    if (adjacent == (p + Right) and accesible(adjacent))
      return Right;
    if (adjacent == (p + Left) and accesible(adjacent))
      return Left;

    cout << "ERROR DIRECCIÓ A POSICIÓ SEGUENT NO TROBADA" << endl;
    return DR;
  }


  // retorna true si el player de la unitat no soc jo
  bool not_me(int unitid)
  {
    Unit u = unit(unitid);
    if (u.player != me())
      return true;
    return false;
  }


  // Returns Dir to the nearest avialable food
  Dir bfs_space(int id, Pos p)
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

    bool space = false;
    Pos posspace = p;
    while (not Q.empty() and not space) // mentre n'hi hagin posicions per mirar
    {
      Pos act = Q.front(); // agafem primera posició i la treiem de la cua
      Q.pop();
      camí.push(act); // guardem la posició com camí fet

      // cerr << "pos act a mirar " << act.i << ',' << act.j << endl;

      if (conq(act))
      {
        space = true; // si es menjar food = true perque hem trobat menjar
        posspace = act;
        cerr << id << " conquerable space at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else
      {
        if (proximpas_space(id, act, Down, dist))
        {
          Q.push(act + Down);                              // afegir pos a la cua
          dist[act.i + 1][act.j] = dist[act.i][act.j] + 1; // actualitzar distancia
          previs[act.i + 1][act.j] = act;
        }
        if (proximpas_space(id, act, Up, dist))
        {
          Q.push(act + Up);
          dist[act.i - 1][act.j] = dist[act.i][act.j] + 1;
          previs[act.i - 1][act.j] = act;
        }
        if (proximpas_space(id, act, Left, dist))
        {
          Q.push(act + Left);
          dist[act.i][act.j - 1] = dist[act.i][act.j] + 1;
          previs[act.i][act.j - 1] = act;
        }
        if (proximpas_space(id, act, Right, dist))
        {
          Q.push(act + Right);
          dist[act.i][act.j + 1] = dist[act.i][act.j] + 1;
          previs[act.i][act.j + 1] = act;
        }
      }
    }

    // Si no hem trobat space retornem una dirreció imposible 'DR'
    if (not space)
    {
      cerr << id << " SPACE not found" << endl;
      return DR;
    }

    // Obtenim primera posició del camí fet
    Pos act = posspace;
    cerr << id << " CONQUERING SPACE at " << act.i << ',' << act.j << "from " << p.i << ',' << p.j << endl;
    while (previs[act.i][act.j] != p)
    {
      act = previs[act.i][act.j];
    }
    cerr << id << " anire de p:" << p.i << ',' << p.j << " a nou: " << act.i << ' ' << act.j << endl;

    if (act.i > p.i and accesible(act + Down))
      return Down;
    if (act.i < p.i and accesible(act + Up))
      return Up;
    if (act.j > p.j and accesible(act + Right))
      return Right;
    if (act.j < p.j and accesible(act + Left))
      return Left;

    return DR;
  }

  //fuig cap a la dirreció contraria de enem
  void fuig (int id, Dir enem) 
  {
    Pos pos = unit(id).pos;
    if (enem == Up) {
      if (accesible(pos+Down)) move(id,Down);
      else if (accesible(pos+Left)) move(id,Left);
      else if (accesible(pos+Right)) move(id,Right);
    } else if (enem == Down) {
      if (accesible(pos+Up)) move(id,Up);
      else if (accesible(pos+Left)) move(id,Left);
      else if (accesible(pos+Right)) move(id,Right);
    } else if (enem == Right) {
      if (accesible(pos+Left)) move(id,Left);
      else if (accesible(pos+Up)) move(id,Up);
      else if (accesible(pos+Down)) move(id,Down);
    } else if (enem == Left) {
      if (accesible(pos+Right)) move(id,Right);
      else if (accesible(pos+Up)) move(id,Up);
      else if (accesible(pos+Down)) move(id,Down);
    }
  }

  //retorna true si ha hagut de fer alguna acció en base a un enemic adjacent
  bool batalla(int id)
  {
    Pos pos = unit(id).pos;
      for (Dir dir : dirs) 
      {
        Pos newpos = pos + dir;
        if (pos_ok(newpos) and accesible(newpos))
        {
          int enemid = cell(newpos).id;
          if (enemid != -1)
          {
            if (zombie(enemid)) {
              cerr << id << " ATACANT ZOMBIE" << endl;
              move(id,dir);
              return true;
            } else if (ganador(enemid) or round() > (num_rounds()-num_rounds()/4)) {
              cerr << id << " ATACANT ENEMIC" << endl;
              move(id,dir);
              return true;
            } else if (not ganador(enemid) and not_me(enemid)) {
              cerr << id << " FUIG!" << endl;
              fuig(id,dir);
              return true;
            }
          }
        }
      }
      return false;
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {

    alive = alive_units(me());  // actualitzem el vector de ids de les meves unitats
    int força = strength(me()); // obtenim la nostra força

    // Write debugging info about my units
    cerr << "At round " << round() << " player " << me() << " has " << alive.size() << " alive units" << endl;

    for (int id : alive)
    {
      Pos unitpos = unit(id).pos;
      if (not batalla(id)) {
      cerr << "start BFS of " << id << " at pos " << unitpos.i << ',' << unitpos.j << endl;
      int i = 0;
      Dir dir = DR;
      while (dir == DR and i < busc.size() and round() < (num_rounds()-num_rounds()/4))
      {
        dir = dir_menjar(id, unitpos, busc[i]); // buscar direcció al menjar més proper
        ++i;
      }
      if (round() > (num_rounds()-num_rounds()/4)) dir = dir_menjar(id,unitpos,"lastenem");
      
      if (dir != DR)
      {
        cerr << "unit " << id << " will go " << dir << endl;
        move(id, dir); // si s'ha trobat menjar moure's cap a ell
      }

      else
      {
        cerr << "conquistant..." << endl;
        dir = space_adj(id, unitpos); // direcció al space conquerible més proper
        cerr << "unit " << id << " conquistara cap a " << dir << endl;
        move(id, dir); // ens movem cap allà
      }
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
