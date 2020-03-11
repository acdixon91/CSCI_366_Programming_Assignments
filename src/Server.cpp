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
int get_file_length(ifstream *file){
    file->seekg(0, file->end);
    int length = file->tellg();
    file->seekg(0, file->beg);
    printf("The length of the file is: %d \n", length);
    return length;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

    this->board_size = board_size;
    this->p1_setup_board.open(p1_setup_board);
    this->p2_setup_board.open(p2_setup_board);


    if (p1_setup_board.compare("player_1.setup_board.txt") != 0 || p2_setup_board.compare("player_2.setup_board.txt") != 0 ){
        throw 20;
    }

    if (board_size != 10){
        throw 20;
    }



//    ifstream ifstr1(p1_setup_board);
//    int size1 = get_file_length(&ifstr1);
//    assert(size1 == 110);
//    ifstream ifstr2(p2_setup_board);
//    int size2 = get_file_length(&ifstr2);
//    assert(size2 == 110);

}


int Server::evaluate_shot(unsigned int player, unsigned int xIn, unsigned int yIn) {
    char pos;
    int x = xIn;
    int y = yIn;

    if (player == 0){
        throw 20;
    } else if (player > 2){
        throw 20;
    }

//    printf("value of x %d \n", x);
//    printf("value of y %d \n", y);
    if(x >= 10)
    {
        printf("Out of Bounds x \n");
        return OUT_OF_BOUNDS;
    }
    if (y >= 10){
        printf("Out of Bounds y \n");
        return OUT_OF_BOUNDS;
    }

    if(player == 1){
        int offset;

        switch(y){
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


        int off = ((y * 10) + x + offset);
        cout << "off: " << off << "\n";
        p2_setup_board.seekg(off,p2_setup_board.beg);
        p2_setup_board.get(pos);
        p2_setup_board.seekg(0,p2_setup_board.beg);
        printf("Ship on board 2: %c \n", pos);
    }

    if(player == 2){
        int off = (y * 10) + x;
        p1_setup_board.seekg(off,p1_setup_board.beg);
        p1_setup_board.get(pos);
        p1_setup_board.seekg(0,p1_setup_board.beg);
        printf("Ship on board 1: %c \n", pos);
    }

    cout << "what was hit: " << pos << "\n";

    if ( pos == '_' ){
        return MISS;
    } else {
        return HIT;
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

    if(player == 0){
        throw 20;
    }else if(player == 1){
        file = "player_1.shot.json";
    }else if(player == 2){
        file = "player_2.shot.json";
    }else if (player >= 3){
        throw 20;
    }

    if(player == 1){
        ifstream shot_file("player_1.shot.json");

        if(!shot_file.good()){
            return NO_SHOT_FILE;
        }

//        getline(shot_file, coorIn);
//        getline(shot_file, coorIn);
//        std::ifstream t("file.txt");
        std::string str((std::istreambuf_iterator<char>(shot_file)),
                        std::istreambuf_iterator<char>());
//        printf("Input: %s \n", str.c_str());

        string input[2];
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
        } catch (regex_error& e){
            // Syntax error in the regular expression
        }
        shot_file.close();
        cout << input[0] << " input 0 of shot\n";
        cout << input[1] << " input 1 of shot\n";
        intX = stoi(input[0]);
        intY = stoi(input[1]);

        result = evaluate_shot(player,intX,intY);

        string result_str = "{\n"
                            "    \"result\": "+to_string(result)+"\n"
                                                                 "}";
        ofstream result_file("player_1.result.json");
        result_file << result_str;
        result_file.close();
        remove("player_1.shot.json");
    }

    if(player == 2){
        ifstream shot_file("player_2.shot.json");

        if(!shot_file.good()){
            return NO_SHOT_FILE;
        }

        std::string str((std::istreambuf_iterator<char>(shot_file)),
                        std::istreambuf_iterator<char>());

//        getline(shot_file, coorIn);
//        printf("Input: %s \n", coorIn.c_str());

        string input[2];
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
        } catch (regex_error& e){
            // Syntax error in the regular expression
        }

        shot_file.close();

        intX = stoi(input[0]);
        intY = stoi(input[1]);

        result = evaluate_shot(player,intX,intY);

        string result_str = "{\n"
                            "    \"result\": "+to_string(result)+"\n"
                                                                 "}";
        ofstream result_file("player_2.result.json");
        result_file << result_str;
        result_file.close();
        remove("player_2.shot.json");
    }


   return SHOT_FILE_PROCESSED;
}