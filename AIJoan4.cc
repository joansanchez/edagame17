#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Joan4


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



    bool vertokey(int id, set<int> mynextpos, set <int> dangervertex){
      set<int>::iterator it;
      it = mynextpos.find(id);
      set<int>:: iterator it2;
      it2 = dangervertex.find(id);
      if(vertex(id).bike == -1 and vertex(id).wall == -1 and no_colision(id, mynextpos) and it == mynextpos.end() and it2 == dangervertex.end()) return true;
      else return false;
    }



    int bfs (int id, const set<int>& dangervertex, set<int> mynextpos) {

      vector <bool> visitats(nb_vertices(), false);
      queue <pair <int, int> > Q; //vertex actual, pare
      Q.push(make_pair(id, -1));
      int token = vertex(id).neighbours.size();
      while(not Q.empty()){
        int v = Q.front().first;
        int p = Q.front().second;
        cerr << "VERTEX " << v << " PADRE " << p << endl;
        Q.pop();
        int siz = Q.size();
        if(not visitats[v]){
          visitats[v] = true;
          vector <int> veins = vertex(v).neighbours;
          for (int w : veins){
            if (vertokey(w, mynextpos, dangervertex) and not visitats[w]){
              if (p != -1) Q.push(make_pair(w, p));
              else Q.push(make_pair(w, w));
            }
          }
        }
        cerr << "SIZ: " << siz << " Q.SIZE(): " << Q.size() << " TOKEN: " << token << endl;
        if(siz == Q.size() and Q.size() <= token)return p;
      }
      return -1;
    }


    void play () {
        vector<int> my_bikes = bikes(me()); //vector de mis motos
        set<int> dangervertex; //se guardan las posiciones vecinas a las bikes enemigas //cambiar por vect
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
            Movement movement(my_bike.id);
            int move = bfs(my_bike.vertex, dangervertex, mynextpos);
            cerr << "MOVE RETURN " << move << endl;
            if (move != -1) {
              movement.next_vertex = move;
              movement.use_bonus = true;
              cerr << "I FEM MOVIMENT AL VERTEX" << movement.next_vertex << endl;
              mynextpos.insert(movement.next_vertex);
              command(movement);
            }
        }
      }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
