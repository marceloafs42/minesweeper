#include "player.h"
#include "tile.h"
#include "binary.h"

TPlayer::TPlayer(std::vector<std::vector<int>> board, int lines, int columns){

    for(int i = 0; i < lines; i++){
        for(int j = 0; j < columns; j++){
            _myBoard[{i,j}] = new TTile(i,j, board[i][j]);
        }
    }

    _lines = lines;
    _columns = columns;
    _realBoard = board;
}

void TPlayer::PrintBoard(){
    for(int i = 0; i < _lines; i++){
        for(int j = 0; j < _columns; j++){
            if((_myBoard[{i,j}]->_isClosed == false)&&(_myBoard[{i,j}]->_number >= 0)){
                std::cout << _myBoard[{i,j}]->_number << "\t";
            }
            else{
                if(_myBoard[{i,j}]->_isFlagged){
                    std::cout << "f" << "\t";
                }
                else{
                    std::cout << "*" << "\t";
                }
            }
        }
        std::cout << std::endl;
    }

}

int TPlayer::KnownTiles(){
    int result {0};

    for(auto x : _myBoard){
            if(((x.second)->_isClosed == false)||((x.second)->_isFlagged == true)){
                result += 1;
            }
    }

    return result;
}

int TPlayer::PlayableTiles(){
    int result {0};

    for(auto x : _myBoard){
        if(((x.second)->_isClosed == false)&&((x.second)->_isKnownAround == false)){
            result += 1;
        }
    }

    return result;
}

int TPlayer::AreClosed(std::vector<int> coords){
    int result = 0;

    std::vector<std::vector<int>> y = _myBoard[coords]->Neighbors(_lines, _columns);

    for(auto x : y){
        if(_myBoard[x]->_isClosed){ result += 1;}
    }
    return result;
}

int TPlayer::AreFlagged(std::vector<int> coords){
    int result = 0;

    std::vector<std::vector<int>> y = _myBoard[coords]->Neighbors(_lines, _columns);

    for(auto x : y){
        if(_myBoard[x]->_isFlagged){ result += 1;}
    }
    return result;
}

void TPlayer::MarkKnown(){
    for(auto x: _myBoard){
        if(((x.second)->_isClosed == false)&&(AreClosed(x.first) == AreFlagged(x.first))){ //should we use closed == flagged or number == closed?
            (x.second)->_isKnownAround = true;
        }
    }
}

void TPlayer::OpenAround(std::vector<int> coords){

    _myBoard[coords]->_isClosed = false;

    std::vector<std::vector<int>> y = _myBoard[coords]->Neighbors(_lines, _columns);

    for(auto x : y){
        if(_myBoard[x]->_number < 0){_myBoard[x]->_isFlagged = true;}
        else{_myBoard[x]->_isClosed = false;}
    }
}

void TPlayer::FirstPlay(){
    for(auto x : _myBoard){
        if((x.second)->_isKnownAround == false){
            if(((x.second)->_isClosed ==  false)&&(((x.second)->_number == AreFlagged(x.first))||((x.second)->_number == AreClosed(x.first)))){
                OpenAround(x.first);
            }
        }
    }
}

std::vector<std::vector<int>> TPlayer::Frontier(){
    std::vector<std::vector<int>> frontier;

    for(auto x : _myBoard){

        if(((x.second)->_isKnownAround == false)&&((x.second)->_isClosed == false)){
            std::vector<std::vector<int>> y = (x.second)->Neighbors(_lines, _columns);
            for(auto z : y){
                if ((_myBoard[z]->_isClosed == true)&&(_myBoard[z]->_isFlagged == false)){
                    if(frontier.empty() == true){ frontier.push_back(z); }
                    if((std::count(frontier.begin(), frontier.end(), z)) == 0){frontier.push_back(z);}
                }
            }
        }
    }

    return frontier;
}

std::vector<std::vector<std::vector<int>>> TPlayer::ConnectedFrontier(){
    auto frontier = Frontier();
    std::vector<std::vector<std::vector<int>>> connectedfrontier;

    // We take one element of the frontier
    for(auto x : frontier){
        bool isalready = false;

        if(connectedfrontier.empty() == false){
            // We have to check if x is already in any of the connected components
            for(auto components : connectedfrontier){
                if(std::count(components.begin(), components.end(), x) != 0){ isalready = true; }
            }
        }

        if(isalready == false){ // We now create its connected component
            std::vector<std::vector<int>> newcomponent = {x};
            int depth = 0;

            while(depth < frontier.size()){
                for(auto y : frontier){
                    for( auto xx : newcomponent ){
                        if((std::abs(xx[0] - y[0]) <= 1)&&(std::abs(xx[1] - y[1]) <= 1)){
                            if(std::count(newcomponent.begin(), newcomponent.end(), y) == 0){ newcomponent.push_back(y); }
                            break;
                        }
                    }
                }
                depth += 1;
            }
            connectedfrontier.push_back(newcomponent);
        }
    }

    return connectedfrontier;
}

bool TPlayer::IsConsistent(std::vector<std::vector<int>> frontier, std::vector<int> comb){
    bool isconsistent = true;

    for(auto x : _myBoard){
        if(((x.second)->_isKnownAround == false)&&((x.second)->_isClosed == false)){

            int newflags = 0;
            std::vector<std::vector<int>> y = (x.second)->Neighbors(_lines, _columns); //we will scan through the tiles neighbors and check if any flag has been added

            for(auto z : y){
                if(std::count(frontier.begin(), frontier.end(), z)){
                    std::vector<std::vector<int>>::iterator it = std::find(frontier.begin(), frontier.end(), z);
                    int indexy = std::distance(frontier.begin(), it);
                    if(comb[indexy] == 1){
                        newflags += 1;
                    }
                }
            }

            bool mybool = ((x.second)->_number == AreFlagged(x.first) + newflags);
            isconsistent = (isconsistent)&&(mybool);
        }
    }
    return isconsistent;
}

std::tuple<bool, std::vector<int>, double> TPlayer::DoGuess(std::vector<std::vector<int>> frontier, double cutoff){

    int n = frontier.size();

    TBinary bin;
    std::vector<int> b;
    bin._length = n;
    bin.GenerateBinary(b, 0);

    auto combs = bin._finalVector; //combinations of 1s and 0s

    std::vector<std::vector<int>> consistentcombs;

    for(auto comb : combs){
        if(IsConsistent(frontier, comb)){ consistentcombs.push_back(comb); } //now we have all consistent configurations
    }

    //now we gotta build the probabilitieeeessss (of having bombs)
    int N = consistentcombs.size();
    std::vector<int> probs(n, 0);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < n; j++){
            if(consistentcombs[i][j] == 1){probs[j] += 1;}
        }//to get the actual probabilities we should divide by N, but we dont do that
    }

    //check if some of them has probability zero
    for(int i = 0; i < n; i++){
        if(probs[i] == 0){return std::make_tuple(true,frontier[i], 0.);}
    }

    std::vector<int>::iterator lowest = std::min_element(std::begin(probs), std::end(probs));
    int index = std::distance(std::begin(probs), lowest);

    double a = probs[index];
    double prob = a/N;

    if(prob < cutoff){ // We can decide for a satisfying cutoff
        return std::make_tuple(true, frontier[index], prob);
    }

    bool nah = false;
    std::vector<int> guess = {0,0};
    double prob1 = 1;

    return std::make_tuple(nah, guess, prob1);
}

std::tuple<bool, std::vector<int>, double> TPlayer::GuessPlay(){
    //We already ran the first algorithm, and now resort to this second one. In this algorithm we
    //map all the Neighbors which are unknown tiles (we suppose there are n of them). Then we construct the 2^n possible
    //combinations of bombs, and retain all of them which match the information of the Known Tiles.

    //first we get all the closed and unknown neighbors of all the tiles for which knonwaround == false,
    //and make a set with this

    //we count the number of elements of this set, n

    //now we create all the 2^n combinations of 1s and 0s possible

    //now we need to have a way to verify if a given combination is consistent. so we create a copy of our board, and
    //assign the bombs and clear numbers. Then we run trough our **known** tiles, and see if for each of them we still have
    //number(x) <= flagged(x). if all_of == true, then the combination is consistent

    //we get all of the consistent combinations, and make the probabilities

    //we guess

    auto connectedparts = ConnectedFrontier();

    for(auto frontier : connectedparts){
        // We will slice the frontier in components of 10
        int i = 0;

        if(frontier.size() < 10){
            auto myguess = DoGuess(frontier, 1/2);
            bool isitgood = std::get<0>(myguess);

            if(isitgood == true){
                return myguess;
            }
        }

        else{
            while( i <= frontier.size() - 10){
                std::vector<std::vector<int>> frontierslice = {};

                int j = 0;
                while(j < 10){
                    frontierslice.push_back(frontier[i + j]);
                    j = j + 1;
                }

                auto myguess = DoGuess(frontierslice, 1/4);
                bool isitgood = std::get<0>(myguess);

                if(isitgood == true){
                    return myguess;
                }
                i = i + 1;
            }
        }
    }

    bool nah = false;
    std::vector<int> coords = {0,0};
    double prob = 1;
    return std::make_tuple(nah, coords, prob);

}
