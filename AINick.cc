#include <list>
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

  // retorna ture si la posició es accesible (no te Waste)
  bool accesible(Pos pos)
  {
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
  vector<Pos> bfs_food(const int id, const Pos p, vector<vector<int>>& dist)
  {

    vector<Pos> camíbuit (1);
    camíbuit[0] = p;

    queue<vector<Pos>> Q;                                     // Cua de posibles camins
    

    Q.push(camíbuit);          // apuntem primera posició com a primer pas del camí
    dist[p.i][p.j] = 0; // distancia primera posició es 0

    bool food = false;
    vector<Pos> camifinal = {p};
    while (not Q.empty() and not food) //mentre n'hi hagin posicions per mirar i no s'hagi trobat menjar
    {
      vector<Pos> camí = Q.front(); // agafem el primer camí i el treiem de la cua
      Q.pop();
      Pos act = camí[camí.size()-1]; // agafem la ultima pos de el camí

      // cerr << "pos act a mirar " << act.i << ',' << act.j << endl;

      if (menjar(act))
      {
        food = true; // si es menjar food = true perque hem trobat menjar
        camifinal = camí;
        cerr << id << " found food at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else
      {
        if (proximpas(act, Down, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act+Down);
          Q.push(nou_camí);                              // afegir nou_camí (camí + nova posicio) a la cua de camins
          dist[act.i + 1][act.j] = dist[act.i][act.j] + 1; // actualitzar distancia
        }
        if (proximpas(act, Up, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act+Up);
          Q.push(nou_camí); 
          dist[act.i - 1][act.j] = dist[act.i][act.j] + 1;
        }
        if (proximpas(act, Left, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act+Left);
          Q.push(nou_camí); 
          dist[act.i][act.j - 1] = dist[act.i][act.j] + 1;
        }
        if (proximpas(act, Right, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act+Right);
          Q.push(nou_camí); 
          dist[act.i][act.j + 1] = dist[act.i][act.j] + 1;
        }
      }
    }

    return camifinal;
  }

  Dir dir_menjar(int id, Pos p) {

    int n = 60;
    int m = 60;

    vector<vector<int>> dist(n, vector<int>(m, -1));
    vector<Pos> camí = bfs_food(id, p, dist);
    

    // Si no hem trobat food retornem una dirreció imposible 'DR'
    if (camí.size() < 2)
    {
      cerr << id << " ERROR: food not found" << endl;
      return DR;
    }
    Pos food = camí[camí.size()-1];
    if (dist[food.i][food.j] > 12)
    {
      cerr << id << " food massa lluny" << endl;
      return DR;
    }

    // Obtenim primera posició del camí fet
    Pos adjacent = camí[1];
    cerr << id << " getting food at " << adjacent.i << ',' << adjacent.j << "from " << p.i << ',' << p.j << endl; //WRONG
    

    cerr << id << " anire de p:" << p.i << ',' << p.j << " a nou: " << adjacent.i << ' ' << adjacent.j << endl;

    if (adjacent == (p+Down) and accesible(adjacent + Down))
      return Down;
    if (adjacent == (p+Up) and accesible(adjacent + Up))
      return Up;
    if (adjacent == (p+Right) and accesible(adjacent + Right))
      return Right;
    if (adjacent == (p+Left) and accesible(adjacent + Left))
      return Left;

    cout << "ERROR DIRECCIÓ A POSICIÓ SEGUENT NO TROBADA" << endl;
    return DR;
  }

  //la unitat amb id 'id' es mourà a la direcció contraria de direnem per fugir
  void fugir(int id, Dir direnem)
  {
    Pos pos = unit(alive[id]).pos;
    if (direnem == Up)
    {
      if (accesible(pos + Down) and cell(pos + Down).id == -1)
        move(alive[id], Down);
      else if (accesible(pos + Left) and cell(pos + Left).id == -1)
        move(alive[id], Left);
      else if (accesible(pos + Right) and cell(pos + Right).id == -1)
        move(alive[id], Right);
      else
        move(alive[id], Up);
    }
    else if (direnem == Down)
    {
      if (accesible(pos + Up) and cell(pos + Up).id == -1)
        move(alive[id], Up);
      else if (accesible(pos + Left) and cell(pos + Left).id == -1)
        move(alive[id], Left);
      else if (accesible(pos + Right) and cell(pos + Right).id == -1)
        move(alive[id], Right);
      else
        move(alive[id], Down);
    }
    else if (direnem == Right)
    {
      if (accesible(pos + Left) and cell(pos + Left).id == -1)
        move(alive[id], Left);
      else if (accesible(pos + Up) and cell(pos + Up).id == -1)
        move(alive[id], Up);
      else if (accesible(pos + Down) and cell(pos + Down).id == -1)
        move(alive[id], Down);
      else
        move(alive[id], Right);
    }
    else if (direnem == Left)
    {
      if (accesible(pos + Right) and cell(pos + Right).id == -1)
        move(alive[id], Right);
      else if (accesible(pos + Up) and cell(pos + Up).id == -1)
        move(alive[id], Up);
      else if (accesible(pos + Down) and cell(pos + Down).id == -1)
        move(alive[id], Down);
      else
        move(alive[id], Left);
    }
    else if (direnem == DR)
    {
      if (accesible(pos + Up) and cell(pos + Up).id == -1)
        move(alive[id], Down);
      else if (accesible(pos + Left) and cell(pos + Left).id == -1)
        move(alive[id], Left);
    }
    else if (direnem == RU)
    {
      if (accesible(pos + Down) and cell(pos + Down).id == -1)
        move(alive[id], Down);
      else if (accesible(pos + Left) and cell(pos + Left).id == -1)
        move(alive[id], Left);
    }
    else if (direnem == UL)
    {
      if (accesible(pos + Down) and cell(pos + Down).id == -1)
        move(alive[id], Down);
      else if (accesible(pos + Right) and cell(pos + Right).id == -1)
        move(alive[id], Right);
    }
    else if (direnem == LD)
    {
      if (accesible(pos + Up) and cell(pos + Up).id == -1)
        move(alive[id], Up);
      else if (accesible(pos + Right) and cell(pos + Right).id == -1)
        move(alive[id], Right);
    }
  }

  //retorna true si a la posició p n'hi ha un zombie
  bool zombie (Pos p)
  {
    int unitid = cell(p).id;
    Unit u = unit(unitid);
    if(u.type == Zombie) return true;
    return false;
  }


  // comprova les posicions adjacents per enemics i lluita o fuig segons les posibilitats de guanyar
  void lluita(int id)
  {
    Pos pos = unit(alive[id]).pos;
    int jo = me();

    // get id of adjacent players
    int idup = cell(pos + Up).id;
    int iddown = cell(pos + Down).id;
    int idright = cell(pos + Right).id;
    int idleft = cell(pos + Left).id;


    // si soc més fort que la unitat adjacen o aquesta es un zombie atacar
    if (idup != -1)
    {
      Unit unitup = unit(idup);
      if (strength(unitup.player) < strength(jo) or zombie(pos+Up))
        move(alive[id], Up);
    }
    else if (iddown != -1)
    {
      Unit unitdown = unit(iddown);
      if (strength(unitdown.player) < strength(jo) or zombie(pos+Down))
        move(alive[id], Down);
    }
    else if (idright != -1)
    {
      Unit unitright = unit(idright);
      if (strength(unitright.player) < strength(jo) or zombie(pos+Right))
        move(id, Right);
    }
    else if (idleft != -1)
    {
      Unit unitleft = unit(idleft);
      if (strength(unitleft.player) < strength(jo) or zombie(pos+Left))
        move(alive[id], Left);
    }

    //fugir dels zombies en diagonal
    int iddr = cell(pos + DR).id;
    int idru = cell(pos + RU).id;
    int idul = cell(pos + UL).id;
    int idld = cell(pos + LD).id;
    
    if(iddr != -1 and zombie(pos+DR)) fugir(id,DR);
    if(idru != -1 and zombie(pos+RU)) fugir(id,RU);
    if(idul != -1 and zombie(pos+UL)) fugir(id,UL);
    if(idld != -1 and zombie(pos+LD)) fugir(id,LD);



    // si soc més fluix o igual de fort que la unitat adjacent fujir
    else if (idup != -1)
    {
      Unit unitup = unit(idup);
      if (strength(unitup.player) >= strength(jo))
        fugir(id, Up);
    }
    else if (iddown != -1)
    {
      Unit unitdown = unit(iddown);
      if (strength(unitdown.player) >= strength(jo))
        fugir(id, Down);
    }
    else if (idright != -1)
    {
      Unit unitright = unit(idright);
      if (strength(unitright.player) >= strength(jo))
        fugir(id, Right);
    }
    else if (idleft != -1)
    {
      Unit unitleft = unit(idleft);
      if (strength(unitleft.player) >= strength(jo))
        fugir(id, Left);
    }
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

  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {

    alive = alive_units(me()); //actualitzem el vector de ids de les meves unitats
    int força = strength(me()); //obtenim la nostra força

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
      Dir dir = dir_menjar(id, unitpos); //buscar direcció al menjar més proper
      lluita(id); //lluitar si fa falta
      if (dir != DR) 
      {
        cerr << "unit " << id << " will go " << dir << endl;
        move(alive[id], dir); //si s'ha trobat menjar moure's cap a ell
      }
      else
      {
        cerr << "conquistant..." << endl;
        dir = space_adj(id, unitpos); //direcció al space conquerible més proper
        cerr << "unit " << id << " conquistara cap a " << dir << endl;
        move(alive[id], dir); //ens movem cap allà
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
