#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Joan2


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


    int obtener_puntuacion_vertice(int id, const set<int>& dangervertex){
      int conta = 0;
      set<int>::iterator it;
      vector<int> auxvei = vertex(id).neighbours;
        for (int itera = 0; itera < (int)auxvei.size(); itera++) {
          int idaux = auxvei[itera];
          if (vertex(idaux).wall != -1) conta -= 1;
          it = dangervertex.find(idaux);
          if (it != dangervertex.end()) conta -= 1; // not working at all
          if (vertex(idaux).bonus != None) conta += 1;

        }
        //return conta;
        return 1;
    } //bfs


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
            vector<int> puntuaciones(neighbours.size()); //guardaremos la puntuación de cada vértice par después decidir
            //Obtenemos la puntuación de los vértices vecinos
            for (int nvei = 0; nvei < (int)neighbours.size(); nvei++) {
              int id = neighbours[nvei];
              if (vertex(id).wall == -1 and no_colision(vertex(id).id, mynextpos)){
                puntuaciones[nvei] = obtener_puntuacion_vertice(id, dangervertex);
              }
              //MEJORA: tener en cuenta los bonus
            }

            //miramos cuál es la dirección más conveniente mirando el max del vector puntuaciones
            //MEJORA: buscar que vecinos tienen la misma o parecida puntuación y lanzar
            //una sonda (en modo dfs) para ver cuánto podría avanzar sin quedarse encerrada
            //por ahora cogemos el primer máximo
            //MEJORA2: más fácil. Random entre las puntuaciones más altas
            int max = -9999;
            int imax = -1; // indice del vector neighbours dónde se guarda el id del next_vertex
            for (int i = 0; i < (int)puntuaciones.size(); ++i){
              if (puntuaciones[i] > max){
                max = puntuaciones[i];
                imax = i;
              }
            }

            //Creamos un movimiento
            Movement movement(my_bike.id);

            if (imax != -1){
              movement.next_vertex = neighbours[imax];
              movement.use_bonus = false;
            }
            else if(my_bike.ghost_duration != 0){
              movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
              movement.use_bonus = true;
            }
            else{
              movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
              movement.use_bonus = false;
            }
            mynextpos.insert(movement.next_vertex);
            command(movement);
        }
      }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
