

// Find all empty neighbours
vector<int> neighbours = vertex(my_bike.vertex).neighbours; //necesario
vector<pair<int, int> > avaible_neighbours; //pair <id del vertice, puntuación>

for (int i = 0; i < (int)neighbours.size(); i++) {
  int id = neighbours[i];
  if (vertex(id).wall == -1) {
      int conta = 0;
      vector<int> subneighbours = vertex(id).neighbours; //guardamos los vecinos del sub-vertice
      for(int j = 0; j < (int)subneighbours.size(); j++){
        int identi = subneighbours[j];
        if (vertex(identi).wall != -1) conta -= -1;
        else if (vertex(identi).bonus != None) conta += 1;
        else if (vertex(identi).bike != -1) cont -= 1;
        else if (vertex(identi).id == vertex(id).id) conta += 0;
        else if(vertex(identi).bike == -1 and vertex(identi).wall == -1) conta += 1;
      }
      empty_neighbours.push_back(make_pair (id, conta));
  }
}
