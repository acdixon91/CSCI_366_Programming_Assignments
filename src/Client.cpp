//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"
#include <regex>

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){

    this->player = player;
    this->board_size = board_size;

    ofstream startupBoardFile("player_1.action_board.json");

    {
        cereal::JSONOutputArchive archive( startupBoardFile );
        vector<vector<int>> board =  {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        archive(CEREAL_NVP(board));
    }

    startupBoardFile.close();
}


void Client::fire(unsigned int inX, unsigned int inY) {

    string file;
    if (this->player == 1){
        file = "player_1.shot.json";
    } else if (this->player == 2){
        file = "player_2.shot.json";
    }
    ofstream fireFile(file);

    {
        cereal::JSONOutputArchive archive(fireFile);
        int x = inX;
        int y = inY;
        archive(CEREAL_NVP(x), CEREAL_NVP(y));
    }
}


bool Client::result_available() {

    string file;
    if (this->player == 1){
        file = "player_1.result.json";
    } else if (this->player == 2){
        file = "player_2.result.json";
    }

    ifstream fireFile(file);

    if(fireFile.good()){
        return true;
    } else return false;
}


int Client::get_result() {
    string file;
    string resultString;
    int result;
    if (this->player == 1){
        file = "player_1.result.json";
    } else if (this->player == 2){
        file = "player_2.result.json";
    }

    ifstream fireFile(file);
    // Calling getline twice so that it gets to the second line
    getline(fireFile, resultString);
    getline(fireFile,resultString);
//    printf("result string: %s \n", resultString.c_str());

    string input[2];
    try {
//        cout << "made it to regex" <<"\n";
        regex re("(-\\d)|(\\d)");
        sregex_iterator next(resultString.begin(), resultString.end(), re);
        sregex_iterator end;
        int i = 0;
        while (next != end) {
            smatch match = *next;
//            cout << match.str() << " result of regex\n";
            input[i] = match.str();
            i++;
            next++;

        }
        result = stoi(input[0]);
    } catch (regex_error& e){
        // Syntax error in the regular expression
    }

    // Remove result for player
    if (this->player == 1){
        remove("player_1.result.json");
    } else if (this->player == 2){
        remove("player_2.result.json");
    }

    // Process result number and return
    if (result == 1){
        return HIT;
    }else if (result == -1){
        return MISS;
    }else if (result == 0){
        return OUT_OF_BOUNDS;
    } else {throw 20;}

}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    ifstream is("player_1.action_board.json");
    vector<vector<int>> board;

    {
        cereal::JSONInputArchive archive(is);
        archive(board);
    }
    is.close();
    remove("player_1.action_board.json");

//    cout << board.at(y).at(x) << " - Location on board 0,0 ";
    board.at(y).at(x) = result;


    ofstream of("player_1.action_board.json");
    {
        cereal::JSONOutputArchive archive(of);
        archive(CEREAL_NVP(board));
    }

}


string Client::render_action_board(){

}