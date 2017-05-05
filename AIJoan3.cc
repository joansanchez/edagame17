#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Joan3


struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }


    /**
     * Attributes for your player can be defined here.
     */
    //vector<int> my_awesome_vector_of_integers;


    int dfs_rec (int u, set<int>& vis, int ronda, bool& bonus) {
      if (ronda >= 100) return ronda;
      if (vertex(u).bonus != -1) bonus = true;
      set<int>::iterator it = vis.find(u);
      int dista = 0;
      if (it == vis.end()) {
        vis.insert(u);
        vector<int> vecinosaux = vertex(u).neighbours;
        for (int i = 0; i < (int)vecinosaux.size(); ++i) {
          if(vertex(vecinosaux[i]).wall == -1 and vertex(vecinosaux[i]).bike == -1){
            int aux = dfs_rec(vecinosaux[i], vis, ronda + 1, bonus);
            if (aux > dista) dista = aux;
          }
        }
      }
      return dista;
    }


    pair<int, bool> obtener_puntuacion_vertice (int vertexid) {
      vector<int> vecinos = vertex(vertexid).neighbours;
      int n = vecinos.size();
      int maxdis = -1;
      bool bonus = false;

      for (int u = 0; u < n; ++u){
        set<int> vis;
        int auxdist = dfs_rec(vecinos[u], vis, 0, bonus);
        if (auxdist > maxdis) maxdis = auxdist;
      }
      return make_pair(maxdis, bonus);
    }


    //En un set se guardan las posiciones vecinas a las bikes enemigas
    void llenardvertex(set<int> dangervertex){
      for (int bikeplayer = 0; bikeplayer < (nb_bikes () * nb_players ()); ++ bikeplayer){
        const Bike& aux_bike = bike(bikeplayer);
        if (aux_bike.player != me()){
          vector<int> auxneighbours = vertex(aux_bike.vertex).neighbours;
          for(int auxj = 0; auxj <  (int)auxneighbours.size(); ++auxj){
            dangervertex.insert(auxneighbours[auxj]);
          }
        }
      }
    }

    bool no_colision(int ver, set<int> mynextpos){
      set<int>::iterator it;
      it = mynextpos.find(ver);
      if(it != mynextpos.end()) return false;
      else return true;
    }

    void play () {
        vector<int> my_bikes = bikes(me()); //vector de mis motos
        set<int> dangervertex; //se guardan las posiciones vecinas a las bikes enemigas
        llenardvertex(dangervertex);
        set<int>::iterator it2;
        set<int> mynextpos; //se guardan las posiciones a las que envío mis motos


        for (int bikenumb = 0; bikenumb < (int)my_bikes.size(); ++bikenumb) {
            const Bike& my_bike = bike(my_bikes[bikenumb]); //copiamos la moto para trabajar rápido

            if (!my_bike.alive) { //ahorro de cpu
      				continue;
      			}

            //Only turbo bikes can move at odd rounds
      			if (round() % 2 && my_bike.turbo_duration <= 0) {
      				continue;
      			}

            //buscamos el vértice más conveniente//
            vector<int> neighbours = vertex(my_bike.vertex).neighbours; //cargamos los vecinos del vértice actual
            vector<pair<int, bool> > puntuaciones(neighbours.size()); //guardaremos la puntuación de cada vértice par después decidir
            //Obtenemos la puntuación de los vértices vecinos
            for (int nvei = 0; nvei < (int)neighbours.size(); nvei++) {
              int id = neighbours[nvei];
              it2 = dangervertex.find(id);
              if (vertex(id).wall == -1){
                if(no_colision(vertex(id).id, mynextpos) == true and vertex(id).bike == -1 and it2 == dangervertex.end()){
                  puntuaciones[nvei] = obtener_puntuacion_vertice(id);
                }
              }
              else puntuaciones[nvei] = make_pair(-1, false);
              cerr << "PUNTUACIO MAX:" << puntuaciones[nvei].first << " CON INDICE " << id << endl;
              //MEJORA: tener en cuenta los bonus
            }

            //miramos cuál es la dirección más conveniente mirando el max del vector puntuaciones
            //MEJORA: buscar que vecinos tienen la misma o parecida puntuación y lanzar
            //una sonda (en modo dfs) para ver cuánto podría avanzar sin quedarse encerrada
            //por ahora cogemos el primer máximo
            //MEJORA2: más fácil. Random entre las puntuaciones más altas
            int max = -1;
            int imax = -1; // indice del vector neighbours dónde se guarda el id del next_vertex
            //bool tebonus = false;
            for (int i = 0; i < (int)puntuaciones.size(); ++i){
              if (puntuaciones[i].first > max){
                max = puntuaciones[i].first;
                imax = i;
                //if (puntuaciones[i].second) tebonus = true;
              }
            }
            vector <int> choose;
            for(int t = 0; t < (int)puntuaciones.size(); ++t){
              if (puntuaciones[t].first == max) choose.push_back(neighbours[t]);
            }


            //Creamos un movimiento
            Movement movement(my_bike.id);

            if (imax != -1){

              movement.next_vertex = neighbours[imax];
              //choose[ rand() % (int)choose.size() ];
              movement.use_bonus = false;
              cerr << "I FEM MOVIMENT AL VERTEX" << movement.next_vertex << endl;
            }
            /*else if(my_bike.ghost_duration != 0){
              movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
              movement.use_bonus = true;
            }
            else{
              movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
              movement.use_bonus = false;
            }*/
            mynextpos.insert(movement.next_vertex);
            command(movement);
        }
      }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
