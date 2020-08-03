#include <iostream>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <map>
#include <bits/stdc++.h> //pair (i think)
#include <tuple>

#include "tile.h"
#include "player.h"

using namespace std;

vector<vector<int>> createboard(int lines, int columns, int mines) {

    vector<int> nones (mines, -1);
    vector<int> nzeros (lines*columns - mines, 0);

    vector<int> vecmines;
    move(nones.begin(), nones.end(), back_inserter(vecmines));
	move(nzeros.begin(), nzeros.end(), std::back_inserter(vecmines));

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); //seed used for the randomization
    shuffle (vecmines.begin(), vecmines.end(), std::default_random_engine(seed)); //randomization

    vector<vector<int>> board (lines, vector<int> (columns));

    for(int i=0; i<lines; i++)
        for(int j=0; j<columns; j++)
            board[i][j] = vecmines[i*lines + j];
    //Essa primeira parte criou o campo. A segunda faz a solução.
    int viz[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

    for(int i = 0; i < lines; i++){
        for(int j = 0; j < columns; j++){
            if(board[i][j] != -1){
                int vizinhos = 0;
                for(auto& x : viz)
                        if((0<=i+x[0])&&(i+x[0]<lines)&&(0<=j+x[1])&&(j+x[1]<columns)){
                            if(board[i+x[0]][j+x[1]] == -1){vizinhos += 1;}
                        }
                board[i][j] = vizinhos;

            }

        }
    }

    return board;
}


tuple<bool, bool, double> reallyplay_it(int lines, int columns, int mines){

    bool defeat = false;
    bool bad_luck = false;
    double prob;

    TPlayer myplayer(createboard(lines, columns, mines), lines, columns);

    myplayer._myBoard[{0,0}]->_isClosed = false;

    if(myplayer._myBoard[{0,0}]->_number != 0){
        defeat = true;
        bad_luck = true;
        prob = 0;
        return make_tuple(defeat, bad_luck, prob);
    }

    //myplayer.printboard();

    while(defeat == false){
        if(myplayer.KnownTiles() == lines*columns){
            //cout << "youve won" ;
            defeat = false;
            bad_luck = false;
            prob = 0;
            return make_tuple(defeat, bad_luck, prob);
        }

        //cout << endl;

        myplayer.MarkKnown();

        myplayer.FirstPlay();

        if(myplayer.PlayableTiles() == 0){
            defeat = true;
            bad_luck = false;
            return make_tuple(defeat, bad_luck, 1);
        }

        bool canmakeagain = false; //checks if we can make firstplayagain

        for(auto x : myplayer._myBoard){
            if((x.second->_isKnownAround == false)&&(x.second->_isClosed ==  false)&&((x.second->_number == myplayer.AreFlagged(x.first))||(x.second->_number == myplayer.AreClosed(x.first)))){
                canmakeagain = true;
            }
        }

        if(canmakeagain == false){
            bool isitgood;
            vector<int> myguess;
            double prob1;

            tie(isitgood, myguess, prob1) = myplayer.GuessPlay();

            if(isitgood == false){
                return make_tuple(true, false, 1);
            }

            if(myplayer._myBoard[myguess]->_number < 0){
                defeat = true;
                bad_luck = false;
                //cout << "lost";
                return make_tuple(defeat, bad_luck, prob1);
            }

            myplayer._myBoard[myguess]->_isClosed = false;
        }

    //myplayer.printboard();

    }
}

void beautifulplay(int lines, int columns, int mines){

    bool good = false;
    vector<vector<int>> board;

    while(good == false){
        board = createboard(lines, columns, mines);
        if(board[0][0] == 0){
            good = true;
        }
    }

    if(good == false){cout << "too many bombs"; return;}

    TPlayer myplayer(board, lines, columns);

    bool defeat = false;

    myplayer._myBoard[{0,0}]->_isClosed = false;

    myplayer.PrintBoard();

    while(defeat == false){
        if(myplayer.KnownTiles() == lines*columns){
            cout << "youve won" ;
            return;
        }

        cout << endl;

        myplayer.MarkKnown();

        myplayer.FirstPlay();

        bool canmakeagain = false; //checks if we can make firstplayagain

        for(auto x : myplayer._myBoard){
            if(((x.second)->_isKnownAround == false)&&((x.second)->_isClosed ==  false)&&(((x.second)->_number == myplayer.AreFlagged(x.first))||((x.second)->_number == myplayer.AreClosed(x.first)))){
                canmakeagain = true;
            }
        }

        int i;
        cout << "press 1" << endl;
        cin >> i;

        if(canmakeagain == false){
            bool isitgood;
            vector<int> myguess;
            double prob1;

            std::tie(isitgood, myguess, prob1) = myplayer.GuessPlay();

            if(isitgood == false){
                cout << "you've lost" << endl;
                cout << "just no good place to guess :)";
                cout << endl;
                return;
            }

            if(isitgood == true){
                cout << "my guess = " << myguess[0] + 1 << myguess[1] + 1 << endl;
                cout << "prob of losing = " << prob1;
                cout << endl;
                if(myplayer._myBoard[myguess]->_number < 0){
                    cout << "lost";
                    return;
                    }

                myplayer._myBoard[myguess]->_isClosed = false;
            }
        }

        if(myplayer.PlayableTiles() == 0){
            cout << "youve lost";
            return;
        }

    myplayer.PrintBoard();
    }
    return;
}

int main()
{
    int lines = 16;
    int columns = 30;
    vector<int> mines = {2, 6, 10, 14, 18, 24, 32, 44, 99};

    vector<int> valid_played = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> valid_won = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for(int i = 0; i < mines.size(); i++){
        while(valid_played[i] < 100){

            bool _defeat;
            bool _badluck;
            double _prob;

            std::tie(_defeat, _badluck, _prob) = reallyplay_it(lines, columns, mines[i]);

            if(_badluck == false){
                valid_played[i] += 1;
                if(_defeat == false){valid_won[i] += 1;}
            }
        }
    }

    for(int i = 0; i < mines.size(); i++){
        double miness = mines[i];
        double density = miness/(lines*columns);

        cout << "Tabuleiro " << lines << "x" << columns << ", com " << mines[i] << " minas (densidade de " << density << "):" << endl;
        cout << "Eu joguei " << valid_played[i] << " vezes e em " << valid_won[i] << " dessas eu ganhei ";
        cout << "sem precisar recorrer à sorte";
        cout << endl << endl;
    }
    /////////////////////////


    return 0;

}
