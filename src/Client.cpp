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
#include <string>
#include <iostream>

Client::~Client() {
    if(player == 1){
        remove("player_1.action_board.json");
    } else if (player == 2){
        remove("player_2.action_board.json");
    }
}


void Client::initialize(unsigned int player, unsigned int board_size){

    this->player = player;
    this->board_size = board_size;
    this->initialized = true;
    string fileName;

    if(player == 1){
        fileName = "player_1.action_board.json";
    } else if (player == 2){
        fileName = "player_2.action_board.json";
    } else throw 20;

    // Create a new ofstream with fileName from above
    ofstream startupBoardFile(fileName);

    // In brackets so that cereal can terminate and correctly write the JSON file
    {
        // Creates a 2D vector of the new board to go into a JSON file
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

    // In brackets so that cereal can terminate and correctly write the JSON file
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

    // Checks to see if the file is available
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

    // Array for the Regex matches
    string input[2];
    // Regex looking for a -digit or normal digit
    try {
        regex re("(-\\d)|(\\d)");
        sregex_iterator next(resultString.begin(), resultString.end(), re);
        sregex_iterator end;
        int i = 0;
        while (next != end) {
            smatch match = *next;
            input[i] = match.str();
            i++;
            next++;
        }
        result = stoi(input[0]);
    } catch (regex_error& e){
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
    string fileName;

    if(player == 1){
        fileName = "player_1.action_board.json";
    } else if (player == 2){
        fileName = "player_2.action_board.json";
    } else throw 20;

    ifstream is(fileName);
    vector<vector<int>> board;

    // In brackets so that cereal can terminate and correctly write the JSON file
    {
        cereal::JSONInputArchive archive(is);
        archive(board);
    }

    is.close();
    if(player == 1){
        remove("player_1.action_board.json");
    } else if (player == 2){
        remove("player_2.action_board.json");
    }

    board.at(y).at(x) = result;

    // Create new file using the contents of the updated 2D board vector
    ofstream of(fileName);
    {
        cereal::JSONOutputArchive archive(of);
        archive(CEREAL_NVP(board));
    }
    of.close();
}


string Client::render_action_board(){

    string fileName;

    if(player == 1){
        fileName = "player_1.action_board.json";
    } else if (player == 2){
        fileName = "player_2.action_board.json";
    } else throw 20;

    ifstream is(fileName);
    vector<vector<int>> board;

    // In brackets so that cereal can terminate and correctly write the JSON file
    // Reads in the JSON
    {
        cereal::JSONInputArchive archive(is);
        archive(board);
    }
    is.close();

    std::string board_string = "";

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            string point = to_string(board.at(i).at(j));
             board_string += point;
        }
        board_string.append(1,'\n');
    }
    return board_string;
}