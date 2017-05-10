#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME YungBeef


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
    void omplirvert (vector<int>& dang){
      for (int i = 0; i < (nb_bikes() * nb_players()-1); ++ i){
        if(bike(i).player != me()) dang[i] = bike(i).id;
      }
    }

    bool cleanvertice(int id, const vector<int>& dang){
      for (int i = 0; i < (int)dang.size(); ++i){
        if (is_neighbour(id, dang[i])) return false;
      }
      return true;
    }

    int dfs_recursi(int id, vector <bool>& vis, int dist, const vector<int>& dang){
      if (not vis[id] and vertex(id).wall == -1 and vertex(id).bike == -1 and cleanvertice(id, dang)){
        int max = dist;
        vis[id] = true;
        vector<int> veins = vertex(id).neighbours;
        for(int v : veins){
          int aux = dfs_recursi(v, vis, dist + 1, dang);
          if (aux > max) max = aux;
        }
        return max;
      }
      return dist;
    }


    int dfs_rec(int id, const set<int>& nextmove, const vector<int>& dang){
      int max = 0;
      int dir = -1;
      vector<int> neighbours = vertex(id).neighbours;
      for (int u = 0; u < (int)neighbours.size(); ++ u){
        vector <bool> vis (nb_vertices(), false);
        vis[id] = true;
        set<int>::iterator it;
        it = nextmove.find(neighbours[u]);
        if(it == nextmove.end()){
          int aux = dfs_recursi(neighbours[u], vis, 0, dang);
          if (aux > max){
            max = aux;
            dir = neighbours[u];
          }
        }
      }
      return dir;
    }

    void play () {
        set<int> nextmove;
        vector<int> my_bikes = bikes(me());
        for (int i = 0; i < (int)my_bikes.size(); ++i) {
    			const Bike& my_bike = bike(my_bikes[i]);
          vector<int> dang((nb_bikes() * nb_players())-1);
          omplirvert (dang);
    			// Do something only if this bike is alive
    			if (!my_bike.alive) {
    				continue;
    			}

    			//Only turbo bikes can move at odd rounds
    			if (round() % 2 && my_bike.turbo_duration <= 0) {
    				continue;
    			}

          int mo = dfs_rec(my_bike.vertex, nextmove, dang);


    			// Create an empty movement
    			Movement movement(my_bike.id);
          movement.next_vertex = mo;
    			// Use bonus randomly when we have one
    			if (my_bike.bonus != None && rand()%5 > 3) {
    				movement.use_bonus = true;
    			}
          nextmove.insert(mo);
    			// Command the movement
              command(movement);

        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
