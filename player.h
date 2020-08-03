#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include <cmath> //abs

#include "tile.h"
#include "binary.h"

class TPlayer{
    public:

    std::map<std::vector<int>, TTile*> _myBoard; //when initializing, put myboard[{i,j}] = new Tiles(args)

    int _lines;
    int _columns;
    std::vector<std::vector<int>> _realBoard;

    TPlayer(std::vector<std::vector<int>> board, int lines, int columns);

    void PrintBoard();

    int KnownTiles();
    int PlayableTiles(); //returns zero if there in no closed tiles that we have info about
    int AreClosed(std::vector<int> coords); //how many closed tiles are around the tile in position coords
    int AreFlagged(std::vector<int> coords); //how many flagged tiles are around the tile in position coords

    void MarkKnown(); //goes through the tiles and mark those which are known around
    void OpenAround(std::vector<int> coords);
    void FirstPlay();
    std::tuple<bool, std::vector<int>, double> GuessPlay(); //return the guess and the probability that we've won

    std::vector<std::vector<int>> Frontier();
    std::vector<std::vector<std::vector<int>>> ConnectedFrontier();
    bool IsConsistent(std::vector<std::vector<int>> frontier, std::vector<int> comb);
    std::tuple<bool, std::vector<int>, double> DoGuess(std::vector<std::vector<int>> frontier, double cutoff);
};

#endif // PLAYER_H_INCLUDED
