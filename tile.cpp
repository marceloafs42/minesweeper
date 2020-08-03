#include "tile.h"

TTile::TTile(int i, int j, int number){
    _coords.push_back(i);
    _coords.push_back(j);

    _isClosed = true; _isFlagged = false; _isKnownAround = false; _number = number;
}

void TTile::Open(){
    _isClosed = false;
}

std::vector<std::vector<int>> TTile::Neighbors(int i, int j){

    std::vector<std::vector<int>> poss = {{_coords[0] - 1, _coords[1] - 1}, {_coords[0] - 1, _coords[1]},{_coords[0] - 1, _coords[1] + 1},
                                          {_coords[0], _coords[1] - 1}, {_coords[0], _coords[1] + 1},
                                          {_coords[0] + 1, _coords[1] - 1}, {_coords[0] + 1, _coords[1]}, {_coords[0] + 1, _coords[1] + 1}};

    std::vector<std::vector<int>> result;

    for(auto x : poss){
        if((x[0] >= 0)&&(x[0] < i)&&(x[1] >= 0)&&(x[1] < j)){result.push_back(x);}
    }

    return result;
}

//int Tile::closed(std::map<std::vector<int>, Tile*> board, int i, int j){

 //   std::vector<std::vector<int>> myneighbors = neighbors(i, j);

   // int result;
    //for(auto x : myneighbors){
      //  if(board[x]->isclosed == true){result += 1;}
    //}
    //return result;
//}
