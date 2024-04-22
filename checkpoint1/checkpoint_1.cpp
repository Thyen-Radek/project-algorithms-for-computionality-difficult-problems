#include <iostream>
#include <vector>
#include <tuple>
#include <list>

int solve(std::vector<std::vector<char>>& parkingLot, int N, int W, int H);
std::tuple<int, int, int> findCarHead(const std::vector<std::vector<char>>& parkingLot, int W, int H);
int moveCar(std::vector<std::vector<char>>& parkingLot, int N, int W, int H, int posX, int posY, int directionH, int directionV, int steps, int checkIfBlocked, int previousH, std::list<std::string>& moves, int fromTop = 0, int needToMove = -1);
void printMap(std::vector<std::vector<char>>& parkingLot, int W, int H);

int main() {
    // width, height, max N steps for solution
    int W, H, N;
    std::cin >> W >> H >> N;

    std::vector<std::vector<char>> parkingLot(H, std::vector<char>(W));

    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            std::cin >> parkingLot[i][j];
        }
    }

    solve(parkingLot, N, W, H);

    return 0;
}
const std::vector<char> carNamesVertical = {'x', 'y', 'z', 'w'};
const std::vector<char> carNamesHorizontal = {'a', 'b'};
std::tuple<int, int, int> carHead;

int findSizeVertical(const std::vector<std::vector<char>>& parkingLot, int W, int H, int x, int y) {
    int size = 0;
    // the sizes are from 2 to 4 and starts from x, y and then z and u for vertical cars
    // make vector of x,y,z,u
    for (int i = 0; i < 4; i++){
        if (parkingLot[y + i][x] == carNamesVertical[i]){
            size++;
        }else {
            break;
        }
    }
    return size;
}

int findSizeHorizontal(const std::vector<std::vector<char>>& parkingLot, int W, int H, int x, int y) {
    int size = 0;
    // the sizes are from 2 to 4 and starts from x, y and then z and u for vertical cars
    // make vector of x,y,z,u
    for (int i = 0; i < 4; i++){
        if (parkingLot[y + i][x] == carNamesHorizontal[i]){
            size++;
        }else {
            break;
        }
    }
    return size;
}


std::tuple<int, int, int> findCarHead(const std::vector<std::vector<char>>& parkingLot, int W, int H) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (parkingLot[i][j] == 'o') {
                int size = 1;
                // check vertically
                for (int k = i + 1; k < H && parkingLot[k][j] == 'o'; k++) {
                    size++;
                }
                return std::make_tuple(j, i, size);
            }
        }
    }
    return std::make_tuple(-1, -1, -1);
}


int solve(std::vector<std::vector<char>>& parkingLot, int N, int W, int H) {
    // to be implemented
    // find car of type 'o' and (coordinates of the car)
    auto carHead2 = findCarHead(parkingLot, W, H);
    // taget x, y = 0 (PAIR), x = carHead.x, y = 0
    std::pair<int, int> carTarget = {std::get<0>(carHead), 0};

    carHead = carHead2;


    // print the coordinates of the car
    // std::cout << "Car head, X:" << std::get<0>(carHead) << ", Y:" << std::get<1>(carHead) << ", Size:" << std::get<2>(carHead) << std::endl;
    // std::cout << "Car target, X:" << carTarget.first << ", Y:" << carTarget.second << std::endl;

    // from top Y = 1 to Y = carHead.y ( i want to find all horizontal cars and move them)
    // make list with string for appending moves
    std::list<std::string> moves;

    moveCar(parkingLot, N, W, H, std::get<0>(carHead), 0, 0, 1, 0,0,1, moves,1,-1);

    // going from top to bottom on x = 2, whenever a is found we will check x = 1 and 3 if on this position is # change it to b
    for (int i = 1; i < H; i++) {
        if (parkingLot[i][2] == 'a') {
            if (parkingLot[i][1] == '#') {
                parkingLot[i][1] = 'b';
            }
            if (parkingLot[i][3] == '#') {
                parkingLot[i][3] = 'b';
            }
        }
    }

    // printMap(parkingLot, W, H);

    // push X Y position of car 'o' and U and by Y moves example: '1 10 U 10'
    moves.push_back(std::to_string(std::get<0>(carHead)) + " " + std::to_string(std::get<1>(carHead)) + " U " + std::to_string(std::get<1>(carHead)+1));

    // std cout number of moves
    std::cout << moves.size() << std::endl;

    // print move
    for (const auto& move : moves) {
        std::cout << move << std::endl;
    }



    return 0;
}

int moveCarUp(std::vector<std::vector<char>>& parkingLot, int W, int H, int posX, int posY, int size, std::list<std::string>& moves){
    // if we can move up
    // std::cout << "Size of the car: " << size << std::endl;
    int maxY = posY;
    int flag = 0;
    // std::cout << "Position: " << posX << ", " << posY << std::endl;
    while (maxY > 0) {
        maxY--;
        // std::cout << parkingLot[maxY][posX] << std::endl;
        if (parkingLot[maxY][posX] == 'b' || parkingLot[maxY][posX] == 'a'){
            flag = 1;
            maxY++;
            break;
        }
        if (parkingLot[maxY][posX] == 'y' || parkingLot[maxY][posX] == 'z' || parkingLot[maxY][posX] == 'u' || parkingLot[maxY][posX] == 'o' || parkingLot[maxY][posX] == '#'){
            // move the next car up
            // std::cout << "Moving car up" << std::endl;
            maxY++;
            break;
        }
    }
    // std::cout << "Flag: " << flag << std::endl;
    // std::cout << "Max Y: " << maxY << std::endl;
    if (flag == 0) {
        for (int y = maxY; y < posY; y++) {
            // check surroundings
            int blocked = 0;
            
            for (int j = 0; j < size; j++) {
                if (parkingLot[y + j][2] == 'a' || parkingLot[y + j][2] == 'b') {
                    // std::cout << "Position: " << parkingLot[y+j][2] << " " << y+j << std::endl;
                    blocked = 1;
                    break;
                }
            }
            // std::cout << "Blocked: " << blocked << " " << y << std::endl;
            if (blocked == 0){
                // std::cout << "Car can be moved up" << std::endl;
                // move the car up
                //MOVING
                moves.push_back(std::to_string(posX) + " " + std::to_string(posY) + " U " + std::to_string(posY - y));
                for (int i = 0; i < size; i++) {
                    parkingLot[posY + i][posX] = '.';
                }

                for (int i = 0; i < size; i++) {
                    parkingLot[y + i][posX] = carNamesVertical[i];
                }
                return 1;
            }
        }
    }else {
        // std::cout << "Car is blocked" << std::endl;
        // move to the closest one and move it up
        // std::cout << "Car is blocked" << std::endl;
        // std::cout << "POsition: " << posX << ", " << posY << std::endl;
        for (int y = posY-1; y >= maxY; y--) {
            // check surroundings
            int blocked = 0;
            for (int j = 0; j < size; j++) {
                if (parkingLot[y + j][2] == 'a' || parkingLot[y + j][2] == 'b') {
                    // std::cout << "Position: " << parkingLot[i-j][2] << " " << i-j << std::endl;
                    blocked = 1;
                    break;
                }
            }
            // std::cout << "Blocked: " << blocked << " " << y << std::endl;
            // std::cout << "Blocked: " << blocked << " " << i << std::endl;
            if (blocked == 0){
                // std::cout << "Car can be moved up" << std::endl;
                // move the car up
                //MOVING
                moves.push_back(std::to_string(posX) + " " + std::to_string(posY) + " U " + std::to_string(posY - y));
                for (int i = 0; i < size; i++) {
                    parkingLot[posY + i][posX] = '.';
                }
                // std::cout << "Size: " << size << std::endl;
                for (int i = 0; i < size; i++) {
                    parkingLot[y+i][posX] = carNamesVertical[i];
                }
                // lock head of it 
                parkingLot[y][posX] = '#';

                return 1;
            }
        }
    }
    return 0;
}

// fucntuon which prints map
void printMap(std::vector<std::vector<char>>& parkingLot, int W, int H) {
    int i = 0;
    for(const auto& row : parkingLot) {
        for(const auto& spot : row) {
            std::cout << spot;
        }
        std::cout << " " << i << std::endl;
        i++;
    }
}
// recursive function moveCar
int moveCar(std::vector<std::vector<char>>& parkingLot, int N, int W, int H, int posX, int posY, int directionH, int directionV, int steps, int checkIfBlocked, int previousH, std::list<std::string>& moves, int fromTop, int needToMove) {
    // base case
    if (needToMove != -1 && posY > needToMove){
        // means can move
        return 1;
    }
    if (parkingLot[posY][posX] == 'o' || parkingLot[posY][posX] == '#') {
        return 0;
    }
    // for horizontal cars
    int canMove = 0;
    
    char temp = parkingLot[posY][posX];
    int size;
    int direction;
    if (temp == '.' && checkIfBlocked == 1) {
        // std::cout << "Car is not blocked can be moved" << std::endl;
        return 1;
    }
    if (temp != '.' && temp != '#') {
        // std::cout << "Car is blocking at position: " << posX << ", " << posY << std::endl;

        if (temp == 'a'){
            // std::cout << "PositionH: " << posX << ", " << posY << std::endl;
            // if (previousH == 1 && posY > std::get<1>(carHead) && posY < 50){
            // if (previousH == 1 && posY > std::get<1>(carHead)){
            //     // std::cout << "GOING FROM TO POSY:" << posY << std::endl;
            //     return 0;
            // }
            // in this place i tihnk checkIfBlocked should be 0
            // we will move to the right
            size = 2;
            direction = 1;
            canMove = moveCar(parkingLot, N, W, H, posX + size, posY, directionH, directionV, steps + 1, 1,1, moves,0,-1);
            
        } else if (temp == 'b') {
            // std::cout << "PositionH: " << posX << ", " << posY << std::endl;
            // if (previousH == 1 && posY > std::get<1>(carHead) && posY < 50){
            // if (previousH == 1 && posY > std::get<1>(carHead)){
            //     //  std::cout << "GOING FROM TO POSY:" << posY << std::endl;
            //     return 0;
            // }
            // same here
            // we will move to the left
            if (posY > needToMove && needToMove != -1){
                return 0;
            }
            size = 2;
            direction = -1;
            // std::cout << "Car is horizontal" << posX-size << ":" << posY<< std::endl;
            canMove = moveCar(parkingLot, N, W, H, posX - size, posY, directionH, directionV, steps + 1, 1,1, moves,0,-1);
            
        } else {
            // std::cout << "FROM WHICH POSITION IS PREVIOUSLY CALLE: " << fromTop << std::endl;
            // std::cout << "Position: " << posX << ", " << posY << std::endl;
            // if (previousH == 1 && fromTop == 1 && parkingLot[posY-1][posX] == '.'){
            //     // position
            //     // std::cout << "GOING FROM TOP" << std::endl;
            //     return 0;
            // }
            // std::cout << "Car is not horizontal" << std::endl;
            // we will move up
            // if we move up we can move only if the car is not blocked
            // std::cout << "At pos 10 3" << parkingLot[10][3] << std::endl;
            char temp = parkingLot[posY][posX];
            int size;
            if (temp != 'x'){
                // it means its the middle of car
                while (parkingLot[posY][posX] != 'x'){
                    posY--;
                }
            }
            size = findSizeVertical(parkingLot, W, H, posX, posY);
            int result = 0;
            if (previousH == 1) {
                // if we are in horizontal car we will move up
                // if we are in vertical car we will move down
                result = moveCarUp(parkingLot, W, H, posX, posY,size, moves);
            }
            if (result == 1) {
                // std::cout << "Car can be moved up" << std::endl;
                return 1;
            }
            // if not possible to move up, we will move down
            // there at first i need to find possible moves and recursively go for all possible moves
            // std::cout << "We will be going down from here" << std::endl;
            // also i need to move at the end of horizontal car i am right now checking
            posY = posY + size;
            // std::cout << "Position: " << posX << ", " << posY << std::endl;
            int initialPosY = posY-1;
            int bestMove = posY-1;
            // i will find first position he need to move to
            int positionHeShouldMoveTo = posY-1;
            int tmpPosY = posY;
            int myMoves = -1;
            if (needToMove != -1){
                // i calculate
                // std::cout << "Need to move: " << needToMove << std::endl;
                // std::cout << "Position: " << posX << ", " << posY << std::endl;
                myMoves = needToMove - (posY - size) + 1;
                // std::cout << "Moves: " << myMoves << std::endl;
            }
            // std::cout << "SOMETHING" << std::endl;

            while (true){
                int flag = 0;
                for (int i = 0; i < size; i++) {
                    if ((parkingLot[tmpPosY-i][2] == 'b' && (tmpPosY-i) <= std::get<1>(carHead)) || (parkingLot[tmpPosY-i][2] == 'a' && (tmpPosY-i) <= std::get<1>(carHead))) {
                        // std::cout << "FLAGING: " << posX << ", " << tmpPosY << std::endl;
                        flag = 1;
                        break;
                    }
                }
                myMoves--;
                if (flag == 0 && myMoves <= 0) {
                    // std::cout << "My moves: " << myMoves << std::endl;
                    // std::cout << "Position he should move to: " << posX << ", " << tmpPosY << std::endl;
                    positionHeShouldMoveTo = tmpPosY;
                    break;
                }
                tmpPosY++;
                if (parkingLot[tmpPosY][posX] == '#'){
                    // std::cout << "MYposition: " << posX << ", " << tmpPosY << std::endl;
                    // printMap(parkingLot, W, H);
                    tmpPosY--;
                    break;
                }


            }
            if (positionHeShouldMoveTo == initialPosY){
                return 0;
            }
            // std::cout << "Position he should move to: " << posX << ", " << positionHeShouldMoveTo << std::endl;

            // if positionHeSHouldMove is the same as initialPosY we will return 0
            while (parkingLot[posY][posX] != 'x' && parkingLot[posY][posX] != 'a' && parkingLot[posY][posX] != 'b'){
                if (parkingLot[posY][posX] == '#'){
                    // we wil not go rekursively but just move the best if we can
                    // actually there will be return because of #
                    if (bestMove == initialPosY){
                        return 0;
                    }
                    // MOVING
                    moves.push_back(std::to_string(posX) + " " + std::to_string(initialPosY) + " D " + std::to_string(bestMove - initialPosY));
                    for (int i = 0; i < size; i++) {
                        // clear initial pos
                        parkingLot[initialPosY-i][posX] = '.';
                    }
                    for (int i = 0; i < size; i++) {
                        // set new pos
                        parkingLot[bestMove-i][posX] = carNamesVertical[size-1-i];
                        // std::cout << "Parking lot: " << parkingLot[bestMove-i][posX] << std::endl;
                    }
                    // std::cout << "MYposition: " << posX << ", " << posY << std::endl;
                
                    return 1;
                }
                // there i will check surroundings 
                int flag = 0;
                for (int i = 0; i < size; i++) {
                    if (parkingLot[posY-i][2] == 'a' || parkingLot[posY-i][2] == 'b') {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    // std::cout << "MYposition: " << posX << ", " << posY << std::endl;
                    // std::cout << "MYposition: " << posX << ", " << posY << std::endl;
                    bestMove = posY;
                                        // thats the first good move
                    // // move then
                    // // moves.pushback
                    // moves.push_back(std::to_string(posX) + " " + std::to_string(initialPosY) + " D " + std::to_string(bestMove - initialPosY));
                    // for (int i = 0; i < size; i++) {
                    //     // clear initial pos
                    //     parkingLot[initialPosY-i][posX] = '.';
                    // }
                    // for (int i = 0; i < size; i++) {
                    //     // set new pos
                    //     parkingLot[bestMove-i][posX] = carNamesVertical[size-1-i];
                    //     // std::cout << "Parking lot: " << parkingLot[bestMove-i][posX] << std::endl;
                    // }
                    // std::cout << "MYposition: " << posX << ", " << posY << std::endl;
                    // return 1;
                }
                posY++;
            }
            // print whats postion and at this position we can move
            // std::cout << "Position: " << posX << ", " << posY << std::endl;
            // std::cout << "at this pos" << parkingLot[posY][posX] << std::endl;
            // std::cout << "Both: " << posY << ", " << positionHeShouldMoveTo << std::endl;
            if (positionHeShouldMoveTo < posY){
                // print both
                
                return 0;
            }
            int canMove = moveCar(parkingLot, N, W, H, posX, posY, 0, 1, steps + 1, 0,0, moves,1,positionHeShouldMoveTo);

            if (canMove == 1){
                // now i move by moves i counted myMove
                moves.push_back(std::to_string(posX) + " " + std::to_string(initialPosY) + " D " + std::to_string(positionHeShouldMoveTo - initialPosY));
                for (int i = 0; i < size; i++) {
                    // clear initial pos
                    parkingLot[initialPosY-i][posX] = '.';
                }

                for (int i = 0; i < size; i++) {
                    // set new pos
                    parkingLot[positionHeShouldMoveTo-i][posX] = carNamesVertical[size-1-i];
                    // std::cout << "Parking lot: " << parkingLot[bestMove-i][posX] << std::endl;
                }
                return 1;
            }

            // if (canMove == 1){
            //     // std::cout << "SEARCHING FOR NEW BEST MOVE" << std::endl;
            //     // std::cout << "Position: " << posX << ", " << posY << std::endl;
            //     while (parkingLot[posY][posX] != 'x' && parkingLot[posY][posX] != '#' && parkingLot[posY][posX] != 'a' && parkingLot[posY][posX] != 'b'){
            //         posY++;
            //     }
            //     posY--;
            //     bestMove = posY;
            //     if (bestMove == initialPosY){
            //         return 0;
            //     }
            //     // MOVING
            //     moves.push_back(std::to_string(posX) + " " + std::to_string(initialPosY) + " D " + std::to_string(bestMove - initialPosY));
            //     for (int i = 0; i < size; i++) {
            //         // clear initial pos
            //         parkingLot[initialPosY-i][posX] = '.';
            //     }
            //     for (int i = 0; i < size; i++) {
            //         // set new pos
            //         parkingLot[bestMove-i][posX] = carNamesVertical[size-1-i];
            //         // std::cout << "Parking lot: " << parkingLot[bestMove-i][posX] << std::endl;
            //     }
            //     // printMap(parkingLot, W, H);
            //     return 1;
            // }


            // in loop i will check each position if can be moved down and save last possible position
            // whenever i find a position that is another car i will recursively call moveCar and try move that car
            // same situation for that car and etc.
            // if there wont be better move than last possible move i will move the car to that position
            // it there will, i will find best possible position (backtrack) by just simply moving to the last possible position
            // for horizontal there will be backtracking as well just to move from the last horizontal to the first horizontal



            // i tihnk first horizontal recursion should in there but from there i can use call below that code ??????
            // i think checkIfBlocked should be 0 from here 
            // i think canMove also should receive from recursive call

        }
    }

    if (checkIfBlocked == 1) {
        return 0;
    }
    // std::cout << "WE ARE MOVING DOWN" << std::endl;
    // checkifblocked in call should be 0?? #TODO
    moveCar(parkingLot, N, W, H, posX + directionH, posY + directionV, directionH, directionV, steps + 1, checkIfBlocked, 1, moves,1,needToMove);

    if (canMove == 1) {
        // std::cout << "Car can be moved CANMOVE" << std::endl;
        // std::cout << "Position: " << posX << ", " << posY << std::endl;
        // move the car
        // MOVING
        if (direction == -1){
            // left
            moves.push_back(std::to_string(posX) + " " + std::to_string(posY) + " L " + "1");
        }else {
            moves.push_back(std::to_string(posX) + " " + std::to_string(posY) + " R " + "1");
        }
        // std::cout << "Can move: " << posX << ", " << posY << std::endl;
        for (int i = 0; i < size; i++) {
            parkingLot[posY][posX + i * direction] = '.';
        }
        for (int i = 0; i < size; i++) {
            parkingLot[posY][posX + i * direction + direction] = carNamesHorizontal[i];
        }
        // change to hash 
        parkingLot[posY][posX + size * direction] = '#';
        // printMap(parkingLot, W, H);
        return 1;
    }
    
    return 0;
    
}
