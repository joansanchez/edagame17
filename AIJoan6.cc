#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Joan6


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

    int dfs_recursi(int id, vector <bool>& vis, int dist){
      if (not vis[id] and vertex(id).wall == -1 and vertex(id).bike == -1){
        int max = dist;
        vis[id] = true;
        vector<int> veins = vertex(id).neighbours;
        for(int v : veins){
          int aux = dfs_recursi(v, vis, dist + 1);
          if (aux > max) max = aux;
        }
        return max;
      }
      return dist;
    }


    int dfs_rec(int id){
      int max = 0;
      int dir = -1;
      vector<int> neighbours = vertex(id).neighbours;
      for (int u = 0; u < neighbours.size(); ++ u){
        vector <bool> vis (nb_vertices(), false);
        vis[id] = true;
        int aux = dfs_recursi(neighbours[u], vis, 0);
        if (aux > max){
          max = aux;
          dir = neighbours[u];
        }
      }
      return dir;
    }

    void play () {

        vector<int> my_bikes = bikes(me());
        for (int i = 0; i < (int)my_bikes.size(); ++i) {
  			const Bike& my_bike = bike(my_bikes[i]);

  			// Do something only if this bike is alive
  			if (!my_bike.alive) {
  				continue;
  			}

  			//Only turbo bikes can move at odd rounds
  			if (round() % 2 && my_bike.turbo_duration <= 0) {
  				continue;
  			}

        int mo = dfs_rec(my_bike.vertex);
  			// Find all empty neighbours
  			/*vector<int> neighbours = vertex(my_bike.vertex).neighbours;
  			vector<int> empty_neighbours;
  			for (int i = 0; i < (int)neighbours.size(); i++) {
  				int id = neighbours[i];
  				if (vertex(id).wall == -1) {
  					empty_neighbours.push_back(id);
  				}
  			}*/

  			// Create an empty movement
  			Movement movement(my_bike.id);

  			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
  		/*	if (!empty_neighbours.empty()) {
  				movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
  			} else {
  				movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
  			}*/
        movement.next_vertex = mo;
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
