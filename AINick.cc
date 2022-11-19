#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Nick



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

  //Returns Dir to the nearest avialable food
Dir bfs_food (Pos p)
{
    int i = p.i;
    int j = p.j;
    int n = 60;
    int m = 60;

    queue<Pos> Q; //Posicions per mirar
    vector < vector<int> > dist (n, vector<int> (m, -1)); //distancia per cada posició
    vector < vector<Pos> > previs (n, vector<Pos> (m));  //previ de cada posició visitada

    Q.push(p); //apuntem primera posició per mirar
    dist[p.i][p.j] = 0; //distancia primera posició es 0

    bool food = false;
    Pos posfood = p;
    while (not Q.empty() and not food) //mentre n'hi hagin posicions per mirar
    {
      Pos act = Q.front(); //agafem primera posició i la treiem de la cua
      Q.pop();

      //si es pot accedir al carrer anirem pel camí si no, no farem res
      if (cell(act.i,act.j).type == Street and cell(act.i,act.j).id == -1) {
        if (cell(act.i,act.j).food == true) {
          food = true;//si es menjar i no hi ha ningú return pos
          posfood = act;
          cerr << "found food at " << act.i << ',' << act.j << endl;
        }
        else if (dist[act.i][act.j] != -1) {
          if(pos_ok(act+Down)) {
            Q.push(act+Down); //afegir pos a la cua
            dist[act.i - 1][act.j] = dist[act.i][act.j] + 1; //actualitzar distancia
            previs[act.i-1][act.j] = act;
            }
          if(pos_ok(act+Up)) {
            Q.push(act+Up); 
            dist[act.i + 1][act.j] = dist[act.i][act.j] + 1;
            previs[act.i+1][act.j] = act;
            }
          if(pos_ok(act+Left)) {
            Q.push(act+Left); 
            dist[act.i][act.j - 1] = dist[act.i][act.j] + 1;
            previs[act.i][act.j - 1] =  act;
            }
          if(pos_ok(act+Right)) {
            Q.push(act+Right); 
            dist[act.i][act.j + 1] = dist[act.i][act.j] + 1;
            previs[act.i][act.j + 1] = act;
            }
        }
      }
    }
    //Si no hem trobat food retornem una dirreció imposible 'DR'
    if (not food) return DR;
    //Obtenim primera posició del camí fet
    Pos act  = posfood;
    while (previs[act.i][act.j] != p)
    {
      act = previs[act.i][act.j];
    }

    if (act.i > p.i) return Up;
    if (act.i < p.i) return Down;
    if (act.j > p.j) return Right;
    if (act.j < p.j) return Left;

    return DR;
}

  /**
   * Types and attributes for your player can be defined here.
   */


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

    for(int id: alive)
    {
      Dir dir = bfs_food(unit(id).pos);
      cerr << "unit " << id << "will go " << dir << endl;
      move(id,dir);
    }


    
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
