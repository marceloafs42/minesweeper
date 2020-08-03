#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <vector>
#include <map>

class TTile{
    public:

    int _number; //if _number == -1, the tile contains a bomb
    std::vector<int> _coords;
    bool _isKnownAround;
    bool _isClosed; //flagged tiles are considered closed
    bool _isFlagged;

    TTile(int i, int j, int _number);

    void Open();
    std::vector<std::vector<int>> Neighbors(int _lines, int _columns); //gives coordinates of neighbors, arguments are dimensions of board
    //int closed(std::map<std::vector<int>, Tile*> board, int i, int j); //we will move this out to Player
    //int flagged();
};

#endif // TILE_H_INCLUDED
