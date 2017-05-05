#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Joan


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


    /**
     * Play method.
     *
     * This method will be invoked once per each round.
     * You have to read the board and place your actions
     * for this round.
     */
    bool no_colision(int vert, const vector<int>& ocupada){
      int n = ocupada.size();
      for (int a = 0; a < n; ++a){
        if (vert == ocupada[a])return false;
      }
      return true;
    }

    void evitarcolisions(int vert, vector<int>& ocupada){
      int n = ocupada.size();
      int m = 0;
      while (m < n and ocupada[m] != -1) ++m;
      ocupada[m] = vert;
    }

    void play () {
        vector<int> ocupada (nb_bikes (), -1); //para guardar los vertices a los que van mis bicis y evitar choques
        vector<int> my_bikes = bikes(me());
        for (int l = 0; l < (int)my_bikes.size(); ++l) {

    			const Bike& my_bike = bike(my_bikes[l]);

    			// Do something only if this bike is alive
    			if (!my_bike.alive) {
    				continue;
    			}

    			//Only turbo bikes can move at odd rounds
    			if (round() % 2 && my_bike.turbo_duration <= 0) {
    				continue;
    			}

          // Find all suitable neighbours
          vector<int> neighbours = vertex(my_bike.vertex).neighbours; //necesario
          vector<pair<int, int> > avaible_neighbours; //pair <id del vertice, puntuación>

          for (int i = 0; i < (int)neighbours.size(); i++) {
            int id = neighbours[i];
            if (vertex(id).wall == -1 and no_colision(vertex(id).id, ocupada) == true) {
                int conta = 0;
                vector<int> subneighbours = vertex(id).neighbours; //guardamos los vecinos del sub-vertice
                for(int j = 0; j < (int)subneighbours.size(); j++){
                  int identi = subneighbours[j];
                  if (vertex(identi).wall != -1) conta -= -1;
                  else if (vertex(identi).bonus != None) conta += 1;
                  else if (vertex(identi).bike != -1) conta -= 1;
                  else if (vertex(identi).id == vertex(id).id) conta += 0;
                  else if(vertex(identi).bike == -1 and vertex(identi).wall == -1) conta += 1;
                }
                avaible_neighbours.push_back(make_pair (id, conta));
            }
          }


    			// Create an empty movement
    			Movement movement(my_bike.id);
    			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
    			if (!avaible_neighbours.empty()) {
            //vector <int> nextvert;
            pair <int, int> max = make_pair (avaible_neighbours[0].first, avaible_neighbours[0].second);
            for (int i = 1; i < (int)avaible_neighbours.size(); i++){
              if (avaible_neighbours[i].second > max.second) {
                max.second = avaible_neighbours[i].second;
                max.first = avaible_neighbours[i].first;
              }
            }
    				movement.next_vertex = max.first;
    			} else {
    				movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
    			}

          evitarcolisions(movement.next_vertex, ocupada);

    			// Use bonus randomly when we have one
    			if (my_bike.bonus != None && rand()%5 > 3) {
    				movement.use_bonus = true;
    			}

    			// Command the movement
                command(movement);

        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
