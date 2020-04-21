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
#include "Server.hpp"
#include <cereal/archives/json.hpp>
#include <regex>


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file) {
    file->seekg(0, file->end);
    int length = file->tellg();
    file->seekg(0, file->beg);
    printf("The length of the file is: %d \n", length);
    return length;
}

ifstream player_1setup_board;
ifstream player_2setup_board;

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board) {



    this->board_size = board_size;
    player_1setup_board.open(p1_setup_board);
    player_2setup_board.open(p2_setup_board);


    if (p1_setup_board.compare("player_1.setup_board.txt") != 0 ||
        p2_setup_board.compare("player_2.setup_board.txt") != 0) {
        throw 20;
    }

    if (board_size != 10) {
        throw 20;
    }

    this->p1_setup_board = scan_setup_board(p1_setup_board);
    this->p2_setup_board = scan_setup_board(p2_setup_board);
}

Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){

    if(setup_board_name == "player_1.setup_board.txt"){

        BitArray2D *localBoard = new BitArray2D(BOARD_SIZE,BOARD_SIZE);
        unsigned int col = BOARD_SIZE; // y value
        unsigned int row = BOARD_SIZE; // x value
        unsigned int offset = 0;
        char pos;

        for (int i = 0; i < row; i++) {
            // The lines of the text files are 10 char becuase of \n,
            // this helps offset for each line
            switch (i) {
                case 0:
                    offset = 0;
                    break;
                case 1:
                    offset = 1;
                    break;
                case 2:
                    offset = 2;
                    break;
                case 3:
                    offset = 3;
                    break;
                case 4:
                    offset = 4;
                    break;
                case 5:
                    offset = 5;
                    break;
                case 6:
                    offset = 6;
                    break;
                case 7:
                    offset = 7;
                    break;
                case 8:
                    offset = 8;
                    break;
                case 9:
                    offset = 9;
                    break;
            }
            for (int j = 0; j < col; j++) {
                int off = ((i * 10) + j + offset);
                player_1setup_board.seekg(off, player_1setup_board.beg);
                player_1setup_board.get(pos);
                player_1setup_board.seekg(0, player_1setup_board.beg);
                if(pos != '_'){
                    localBoard->set(i,j);
                }
            }
        }
        return localBoard;

    } else if (setup_board_name == "player_2.setup_board.txt"){
        p2_setup_board = new BitArray2D(BOARD_SIZE,BOARD_SIZE);
        unsigned int col = BOARD_SIZE; // y value
        unsigned int row = BOARD_SIZE; // x value
        unsigned int offset;
        char pos;

        for (int i = 0; i < row; i++) {
            // The lines of the text files are 10 char becuase of \n,
            // this helps offset for each line
            switch (i) {
                case 0:
                    offset = 0;
                    break;
                case 1:
                    offset = 1;
                    break;
                case 2:
                    offset = 2;
                    break;
                case 3:
                    offset = 3;
                    break;
                case 4:
                    offset = 4;
                    break;
                case 5:
                    offset = 5;
                    break;
                case 6:
                    offset = 6;
                    break;
                case 7:
                    offset = 7;
                    break;
                case 8:
                    offset = 8;
                    break;
                case 9:
                    offset = 9;
                    break;
            }
            for (int j = 0; j < col; j++) {
                int off = ((i * 10) + j + offset);
                player_2setup_board.seekg(off, player_2setup_board.beg);
                player_2setup_board.get(pos);
                player_2setup_board.seekg(0, player_2setup_board.beg);
                if (pos != '_') {
                    p2_setup_board->set(i, j);
                }
            }
        }
        return p2_setup_board;
    }
    return nullptr;
}


int Server::evaluate_shot(unsigned int player, unsigned int xIn, unsigned int yIn) {
    char pos;
    int x = xIn; // row
    int y = yIn; // col
    bool hit;

    // Make sure player is either 1 or 2
    if (player == 0) {
        throw 20;
    } else if (player > 2) {
        throw 20;
    }

//    printf("value of x %d \n", x);
//    printf("value of y %d \n", y);

    // Make sure board is right size
    if (x >= 10) {
        printf("Out of Bounds x \n");
        return OUT_OF_BOUNDS;
    }
    if (y >= 10) {
        printf("Out of Bounds y \n");
        return OUT_OF_BOUNDS;
    }


    if (player == 1){
        hit = p1_setup_board->get(x,y);
    } else if (player == 2){
        hit = p2_setup_board->get(x,y);
    }

    if (hit) {
        return HIT;
    } else {
        return MISS;
    }
}


int Server::process_shot(unsigned int player) {
    string file;
    string xIn;
    string yIn;
    unsigned int intX;
    unsigned int intY;
    string coorIn;
    int result;

    if (player == 0) {
        throw 20;
    } else if (player == 1) {
        file = "player_1.shot.json";
    } else if (player == 2) {
        file = "player_2.shot.json";
    } else if (player >= 3) {
        throw 20;
    }
    ifstream shot_file(file);

    // Checks to see if there is a shotfile waiting to be read
    if (!shot_file.good()) {
        return NO_SHOT_FILE;
    }

    // Create a string from the shot_file
    std::string str((std::istreambuf_iterator<char>(shot_file)),
                    std::istreambuf_iterator<char>());
//        printf("Input: %s \n", str.c_str());
    string input[2];

    // try/catch for the regex expresion. Tries to find groups of digits within the shot_file string
    try {
        regex re("\\d+");
        sregex_iterator next(str.begin(), str.end(), re);
        sregex_iterator end;
        int i = 0;
        while (next != end) {
            smatch match = *next;
//                cout << match.str() << "\n";
            input[i] = match.str();
            i++;
            next++;
        }
    } catch (regex_error &e) {
    }
    shot_file.close();

    // Convert the input chars into ints
    intX = stoi(input[0]);
    intY = stoi(input[1]);

    // Send the x,y input to the evaluate_shot method
    result = evaluate_shot(player, intX, intY);

    // String of JSON format with the result int
    string result_str = "{\n"
                        "    \"result\": " + to_string(result) + "\n"
                                                                 "}";

    if(player == 1){
        remove("player_1.shot.json");
        file = "player_1.result.json";
    }else if(player == 2){
        remove("player_2.shot.json");
        file = "player_2.result.json";
    }

    // Write result to JSON file
    ofstream result_file(file);
    result_file << result_str;
    result_file.close();

    return SHOT_FILE_PROCESSED;
}