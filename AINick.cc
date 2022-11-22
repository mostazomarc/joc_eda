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
  const int maxunitats = 15;
  const int mindistmenjar = 12;
  const int mindistenemics = 9;

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
    if (not pos_ok(pos))
      return false;
    if (cell(pos).food)
      return true;
    return false;
  }

  // retorna true si a la posició p n'hi ha un zombie
  bool zombie(const Pos p)
  {
    if (not pos_ok(p))
      return false;
    int unitid = cell(p).id;
    if (unitid == -1)
      return false;
    Unit u = unit(unitid);
    if (u.type == Zombie)
      return true;
    return false;
  }

  bool enemic(Pos p)
  {
    if (not pos_ok(p))
      return false;
    int unitid = cell(p).id;
    if (unitid == -1)
      return false;
    Unit u = unit(unitid);
    if (u.player != me() and u.player != -1)
      return true;
    return false;
  }

  bool ganador(Pos p)
  {
    if (not pos_ok(p))
      return false;
    int id = cell(p).id;
    if (id != -1)
    {
      Unit u = unit(id);
      if (strength(u.player) < strength(me()))
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

    return DR;
    cerr << "no adjacent spaces at pos" << p.i << ',' << p.j << endl;
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

  // Returns path to the food
  vector<Pos> bfs(const int id, const Pos p, vector<vector<int>> &dist)
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

      if (menjar(act) and (dist[act.i][act.j] <= mindistmenjar or alive.size() > maxunitats))
      {
        found = true; // si es menjar food = true perque hem trobat menjar
        camifinal = camí;
        cerr << id << " found food at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (zombie(act) and dist[act.i][act.j] < mindistenemics and alive.size() < maxunitats)
      {
        found = true;
        camifinal = camí;
        cerr << id << " found zombie at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (conq(act) and dist[act.i][act.j] >= mindistmenjar and alive.size() < maxunitats)
      {
        found = true;
        camifinal = camí;
        cerr << id << " found espai per conquerir at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (enemic(act) and ganador(act) and dist[act.i][act.j] < mindistenemics and alive.size() < maxunitats)
      {
        found = true;
        camifinal = camí;
        cerr << id << " found enemic at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else if (conq(act) and dist[act.i][act.j] >= mindistmenjar and alive.size() < maxunitats)
      {
        found = true;
        camifinal = camí;
        cerr << id << " found espai per conquerir at " << act.i << ',' << act.j << " a distancia " << dist[act.i][act.j] << endl;
      }
      else
      {
        if (proximpas(act, Down, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act + Down);
          Q.push(nou_camí);                                // afegir nou_camí (camí + nova posicio) a la cua de camins
          dist[act.i + 1][act.j] = dist[act.i][act.j] + 1; // actualitzar distancia
        }
        if (proximpas(act, Up, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act + Up);
          Q.push(nou_camí);
          dist[act.i - 1][act.j] = dist[act.i][act.j] + 1;
        }
        if (proximpas(act, Left, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act + Left);
          Q.push(nou_camí);
          dist[act.i][act.j - 1] = dist[act.i][act.j] + 1;
        }
        if (proximpas(act, Right, dist))
        {
          vector<Pos> nou_camí = camí;
          nou_camí.push_back(act + Right);
          Q.push(nou_camí);
          dist[act.i][act.j + 1] = dist[act.i][act.j] + 1;
        }
      }
    }

    return camifinal;
  }

  Dir dir_menjar(int id, Pos p)
  {

    int n = 60;
    int m = 60;

    vector<vector<int>> dist(n, vector<int>(m, -1));
    vector<Pos> camí = bfs(id, p, dist);

    // Si no hem trobat food retornem una dirreció imposible 'DR'
    if (camí.size() < 2)
    {
      cerr << id << " ERROR: nothing found" << endl;
      return DR;
    }
    Pos objectiu = camí[camí.size() - 1];

    // Obtenim primera posició del camí fet
    Pos adjacent = camí[1];
    cerr << id << " getting food at " << objectiu.i << ',' << objectiu.j << "from " << p.i << ',' << p.j << endl; // WRONG

    cerr << id << " anire de p:" << p.i << ',' << p.j << " a nou: " << adjacent.i << ' ' << adjacent.j << endl;

    if (adjacent == (p + Down) and accesible(adjacent + Down))
       move(alive[id],Down);
    else if (adjacent == (p + Up) and accesible(adjacent + Up))
      move(alive[id],Up);
    else if (adjacent == (p + Right) and accesible(adjacent + Right))
       move(alive[id],Right);
    else if (adjacent == (p + Left) and accesible(adjacent + Left))
      move(alive[id],Left);

    cout << "ERROR DIRECCIÓ A POSICIÓ SEGUENT NO TROBADA" << endl;
    return DR;
  }

  // la unitat amb id 'id' es mourà a la direcció contraria de direnem per fugir
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

  bool en_perill(const Pos p)
  {
    Pos pos = p;

    int idup = -1;
    int iddown = -1;
    int idright = -1;
    int idleft = -1;
    int iddr = -1;
    int idru = -1;
    int idul = -1;
    int idld = -1;

    if (pos_ok(p + Up))
      idup = cell(pos + Up).id;
    if (pos_ok(p + Down))
      iddown = cell(pos + Down).id;
    if (pos_ok(p + Right))
      idright = cell(pos + Right).id;
    if (pos_ok(p + Left))
      idleft = cell(pos + Left).id;
    if (pos_ok(p + DR))
      iddr = cell(pos + DR).id;
    if (pos_ok(p + RU))
      idru = cell(pos + RU).id;
    if (pos_ok(p + UL))
      idul = cell(pos + UL).id;
    if (pos_ok(p + LD))
      idld = cell(pos + LD).id;

    if (idup != -1 or iddown != -1 or idright != -1 or idleft != -1 or iddr != -1 or idru != -1 or idul != -1 or idld != -1)
    {
      if (idup != me() or iddown != me() or idright != me() or idleft != me() or iddr != me() or idru != me() or idul != me() or idld != me())
        return true;
      else
        return false;
    }
    else
      return false;
  }

  // comprova les posicions adjacents per enemics i lluita o fuig segons les posibilitats de guanyar
  void lluita(const int id)
  {
    Pos pos = unit(alive[id]).pos;
    int jo = me();

    // get id of adjacent units

    int idup = -1;
    int iddown = -1;
    int idright = -1;
    int idleft = -1;
    int iddr = -1;
    int idru = -1;
    int idul = -1;
    int idld = -1;

    if (pos_ok(pos + Up))
      idup = cell(pos + Up).id;
    if (pos_ok(pos + Down))
      iddown = cell(pos + Down).id;
    if (pos_ok(pos + Right))
      idright = cell(pos + Right).id;
    if (pos_ok(pos + Left))
      idleft = cell(pos + Left).id;
    if (pos_ok(pos + DR))
      iddr = cell(pos + DR).id;
    if (pos_ok(pos + RU))
      idru = cell(pos + RU).id;
    if (pos_ok(pos + UL))
      idul = cell(pos + UL).id;
    if (pos_ok(pos + LD))
      idld = cell(pos + LD).id;

    // si soc més fort que la unitat adjacen o aquesta es un zombie atacar
    if (ganador(pos + Up) or zombie(pos + Up))
      move(alive[id], Up);
    else if (ganador(pos + Down) or zombie(pos + Down))
      move(alive[id], Down);
    else if (ganador(pos + Right) or zombie(pos + Right))
      move(id, Right);
    else if (ganador(pos + Left) or zombie(pos + Left))
      move(alive[id], Left);

    // fugir dels zombies en diagonal

    if (iddr != -1 and zombie(pos + DR))
      fugir(id, DR);
    if (idru != -1 and zombie(pos + RU))
      fugir(id, RU);
    if (idul != -1 and zombie(pos + UL))
      fugir(id, UL);
    if (idld != -1 and zombie(pos + LD))
      fugir(id, LD);

    // si soc més fluix o igual de fort que la unitat adjacent fujir
    else if (idup != -1)
    {
      Unit unitup = unit(idup);
      if (strength(unitup.player) >= strength(jo) and unitup.player != me())
        fugir(id, Up);
    }
    else if (iddown != -1)
    {
      Unit unitdown = unit(iddown);
      if (strength(unitdown.player) >= strength(jo) and unitdown.player != me())
        fugir(id, Down);
    }
    else if (idright != -1)
    {
      Unit unitright = unit(idright);
      if (strength(unitright.player) >= strength(jo) and unitright.player != me())
        fugir(id, Right);
    }
    else if (idleft != -1)
    {
      Unit unitleft = unit(idleft);
      if (strength(unitleft.player) >= strength(jo) and unitleft.player != me())
        fugir(id, Left);
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {

    alive = alive_units(me());  // actualitzem el vector de ids de les meves unitats
    int força = strength(me()); // obtenim la nostra força

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

      if (en_perill(unitpos))
        lluita(id); // lluitar si fa falta
      cerr << "start BFS of " << alive[id] << " at pos " << unitpos.i << ',' << unitpos.j << endl;
      Dir dir = dir_menjar(id, unitpos); // buscar direcció al menjar més proper
      if (dir != DR)
      {
        cerr << "unit " << id << " will go " << dir << endl;
        move(alive[id], dir); // si s'ha trobat menjar moure's cap a ell
      }
      else
      {
        cerr << "conquistant..." << endl;
        dir = space_adj(id, unitpos); // direcció al space conquerible més proper
        cerr << "unit " << id << " conquistara cap a " << dir << endl;
        move(alive[id], dir); // ens movem cap allà
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
