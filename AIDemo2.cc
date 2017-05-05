#include "Player.hh"

#include <cmath>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Demo2


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

    void play () {
        set<int> dangervertex; //se guardan las posiciones vecinas a las bikes enemigas
        llenardvertex(dangervertex);
        set<int>::iterator it;
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

			// Find all empty neighbours
			vector<int> neighbours = vertex(my_bike.vertex).neighbours;
			vector<int> empty_neighbours;
			for (int i = 0; i < (int)neighbours.size(); i++) {
				int id = neighbours[i];
        it = dangervertex.find(id);
				if (vertex(id).wall == -1 and it == dangervertex.end()) {
					empty_neighbours.push_back(id);
				}
			}

			// Create an empty movement
			Movement movement(my_bike.id);

			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
			if (!empty_neighbours.empty()) {
				movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
			} else {
				movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
			}

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
