#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const int n = 8;
int mobilityOfThePreviousBoard = -1;
int mobilityOfThePreviousBoardIsUs = false;
int moveNumber = 1;
int previousDepth = 0;
bool timeIsUp = false;
clock_t begin;


int inputAndCreateArray(char** board){
    int dimension = 0;
    printf("Enter the board dimension: ");
    scanf("%d", &dimension);
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            board[i][j] = 'U';
        }
    }
    board[dimension/2 - 1][dimension/2 - 1] = 'W';
    board[dimension/2][dimension/2] = 'W';
    board[dimension/2 - 1][dimension/2] = 'B';
    board[dimension/2][dimension/2 - 1] = 'B';
    return dimension;
}

void printBoard(char **board){
    printf("  ");
    for (int i = 0; i < n; i++) {
        printf("%c", 97 + i);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(j == 0){
                printf("%c ", 97 + i);
            }
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

bool checkLegalInDirection(bool change, char** board, int row, int col, char colour, int deltaRow, int deltaCol){
    for(int i = row + deltaRow, j = col + deltaCol; i >= 0 && j >= 0 && i < n && j < n; i += deltaRow, j += deltaCol){
        if(i == row + deltaRow && j == col + deltaCol && board[i][j] == colour){
            return false;
        }else if(board[i][j] == 'U'){
            return false;
        }else if(board[i][j] == colour){
            return true;
        }else{
            if(change){
                board[i][j] = colour;
            }
        }
    }
    return false;
}

bool checkLegalAll(char** board, int row, int col, char colour){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(!(i == 0 && j == 0) && checkLegalInDirection(false, board, row, col, colour, i, j)){
                return true;
            }
        }
    }
    return false;
}

bool gameover(char **board){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'U' && (checkLegalAll(board, i, j, 'B') || (checkLegalAll(board, i, j, 'W')))){
                return false;
            }
        }
    }
    return true;
}

bool computerFirst(void){
    char input = 'W';
    printf("Computer plays (B/W): ");
    scanf("\n%c", &input);
    if(input == 'B'){
        return true;
    }else {
        return false;
    }
}

bool gameNotFull(char **board){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'U'){
                return true;
            }
        }
    }
    return false;
}

bool checkMoveAv(char **board, char colour){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'U' && checkLegalAll(board, i, j, colour)){
                return true;
            }
        }
    }
    return false;
}

void makeAMove(char move[], char colour){
    move[0] = colour;
    printf("Enter move for colour %c (RowCol): ", colour);
    getchar();
    scanf(" %c%c", &move[1], &move[2]);
}

bool notTaken(char **board, int row, int col){
    return (board[row][col] == 'U');
}

bool positionInBounds(int row, int col){
    if(row > n - 1 || col > n - 1){
        return false;
    }else{
        return true;
    }
}

bool checkMove(char **board, char move[]){
    char colour = move[0];
    int row = move[1] - 97;
    int col = move[2] - 97;
    return (positionInBounds(row, col) && checkLegalAll(board, row, col, colour) && notTaken(board, row, col));
}

void putInBoard(char **board, char move[3]){
    board[move[1] - 97][move[2] - 97] = move[0];
}

void changeMove(char **board, char move[]){
    char colour = move[0];
    int row = move[1] - 97;
    int col = move[2] - 97;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(!(i == 0 && j == 0) && checkLegalInDirection(false, board, row, col, colour, i, j)){
                checkLegalInDirection(true, board, row, col, colour, i, j);
            }
        }
    }
}

void WinLoss(char **board){
    int B = 0, W = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'B'){
                B++;
            }else if(board[i][j] == 'W'){
                W++;
            }
        }
    }
    if(B > W){
        printf("B player wins.");
    }else if(B < W){
        printf("W player wins.");
    }else{
        printf("Draw!");
    }
}

int checkEdgeStableHorizontal(char **board, int row, int col, char opposite){
    for(int j = col + 1; j < n; j++){
        if(board[row][j] == opposite){
            for(int f = col - 1; f >= 0; f --){
                if(board[row][f] == 'U'){
                    return 0;
                }else if(board[row][f] == opposite){
                    return 2;
                }
            }
            return 2;
        }else if(board[row][j] == 'U'){
            for(int f = col - 1; f >= 0; f--){
                if(board[row][f] == 'U'){
                    return 1;
                }else if(board[row][f] == opposite){
                    return 0;
                }
            }
            return 2;
        }
    }
    return 2;
}

int checkEdgeStableVertical(char **board, int row, int col, char opposite){
    for(int i = row + 1; i < n; i++){
        if(board[i][col] == opposite){
            for(int k = row - 1; k >= 0; k--){
                if(board[k][col] == 'U'){
                    return 0;
                }else if(board[k][col] == opposite){
                    return 2;
                }
            }
            return 2;
        }else if(board[i][col] == 'U'){
            for(int k = row - 1; k >= 0; k--){
                if(board[k][col] == 'U'){
                    return 1;
                }else if(board[k][col] == opposite){
                    return 0;
                }
            }
            return 2;
        }
    }
    return 2;
}

int checkEdgeStableAllCount(char **board, char us, char oppo){
    int value = 0;
    int edgeValuesBaseOnStable[3][8][8] =
    {
        {
            { 0 ,-25, 75, 50, 50, 75,-25, 0 },
            {-25, 0 , 0 , 0 , 0 , 0 , 0 ,-25},
            { 75, 0 , 0 , 0 , 0 , 0 , 0 , 75},
            { 50, 0 , 0 , 0 , 0 , 0 , 0 , 50},
            { 50, 0 , 0 , 0 , 0 , 0 , 0 , 50},
            { 75, 0 , 0 , 0 , 0 , 0 , 0 , 75},
            {-25, 0 , 0 , 0 , 0 , 0 , 0 ,-25},
            { 0 ,-25, 75, 50, 50, 75,-25, 0 }
        },
        {
            { 0 ,200,200,200,200,200,200, 0 },
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            {200, 0 , 0 , 0 , 0 , 0 , 0 ,200},
            { 0 ,200,200,200,200,200,200, 0 }
        },
        {
            { 700,1200,1000,1000,1000,1000,1200, 700},
            {1200,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1200},
            {1000,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1000},
            {1000,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1000},
            {1000,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1000},
            {1000,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1000},
            {1200,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,1200},
            { 700,1200,1000,1000,1000,1000,1200, 700}
        }
    };

    for(int i = 0; i < n; i += 7){
        for(int j = 0; j < n; j++){
            if(board[i][j] == us){
                if((i+j) == 0 ||(i+j) == 7 || (i+j) == 14){
                    value += 700;
                }else{
                    value += edgeValuesBaseOnStable[checkEdgeStableHorizontal(board, i, j, oppo)][i][j];
                }
            }else if(board[i][j] == oppo){
                if((i+j) == 0 ||(i+j) == 7 || (i+j) == 14){
                    value -= 700;
                }else{
                    value -= edgeValuesBaseOnStable[checkEdgeStableHorizontal(board, i, j, us)][i][j];
                }
            }
        }
    }

    for(int j = 0; j < n; j += 7){
        for(int i = 1; i < n-1; i++){
            if(board[i][j] == us){
                value += edgeValuesBaseOnStable[checkEdgeStableVertical(board, i, j, oppo)][i][j];
            }else if(board[i][j] == oppo){
                value -= edgeValuesBaseOnStable[checkEdgeStableVertical(board, i, j, us)][i][j];
            }
        }
    }

    return value;
}

int countMoveAv(char **board, char colour){
    int count = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 'U' && checkLegalAll(board, i, j, colour)){
                count++;
            }
        }
    }
    return count;
}

int currentMobility(char **board, char us, char oppo){
    int Moppo = 0, Mus = 0;
    if(mobilityOfThePreviousBoardIsUs){
        Mus = mobilityOfThePreviousBoard;
        Moppo = countMoveAv(board, oppo);
    }else{
        Moppo = mobilityOfThePreviousBoard;
        Mus = countMoveAv(board, us);
    }
    int MinMobility = Mus;
    if(Moppo < Mus){
        MinMobility = Moppo;
    }
    int currentMobility = (1000*(Mus - Moppo))/(MinMobility + 2);
    return currentMobility;
}


int staticEvaluation(char **board, char us, char oppo){
    int EVAL = (int)(5 + moveNumber/4) * checkEdgeStableAllCount(board, us, oppo) + (int)(1 + moveNumber/20) * currentMobility(board, us, oppo);
    return EVAL;
}

int endEvaluation(char **board, char us, char oppo){
    int end = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == us){
                end++;
            }else if(board[i][j] == oppo){
                end--;
            }
        }
    }
    return end;
}

int minMax(char **board, int depth, int alpha, int beta, bool maxmizingPlayer, char us, char oppo){
    if(timeIsUp){
        return -1;
    }else if(((double)(clock() - begin))/CLOCKS_PER_SEC > (0.9)){
        timeIsUp = true;
        return -1;
    }
    
    if(depth == 0){
        return staticEvaluation(board, us, oppo);
    }else if(!gameNotFull(board)){
        return endEvaluation(board, us, oppo);
    }

    if(depth == 1){
        if(maxmizingPlayer){
            mobilityOfThePreviousBoard = countMoveAv(board, us);
            mobilityOfThePreviousBoardIsUs = true;
        }else {
            mobilityOfThePreviousBoard = countMoveAv(board, oppo);
            mobilityOfThePreviousBoardIsUs = false;
        }
    }

    if(maxmizingPlayer){
        bool moved = false;
        int maxEval = -999999999;
        char **fboard;
        fboard = malloc(n * sizeof(char *));
        for(int r = 0; r < n; r++){
            fboard[r] = malloc(n * sizeof(char));
        }
        for(int i = 0; i < n; i++){
            bool doubleBreak = false;
            for(int j = 0; j < n; j++){
                if(board[i][j] == 'U' && checkLegalAll(board, i, j, us)){
                    moved = true;
                    for(int r = 0; r < n; r++){
                        for(int c = 0; c < n; c++){
                            fboard[r][c] = board[r][c];
                        }
                    }
                    fboard[i][j] = us;
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){
                            if(!(a == 0 && b == 0) && checkLegalInDirection(false, fboard, i, j, us, a, b)){
                                checkLegalInDirection(true, fboard, i, j, us, a, b);
                            }
                        }
                    }
                    int eval = minMax(fboard, depth -1, alpha, beta, false, us, oppo);
                    if(eval > maxEval){
                        maxEval = eval;
                    }
                    if(eval > alpha){
                        alpha = eval;
                    }
                    if(beta <= alpha){
                        doubleBreak = true;
                        break;
                    }
                }
            }
            if(doubleBreak){
                break;
            }
        }
        for(int k = 0; k < n; k++){
            free(fboard[k]);
        }
        free(fboard);
        if(!moved){
            int eval = minMax(board, depth -1, alpha, beta, false, us, oppo);
            if(eval > maxEval){
                maxEval = eval;
            }
            if(eval > alpha){
                alpha = eval;
            }
        }
        return maxEval;
    }else{
        bool moved = false;
        int minEval = 999999999;
        char **fboard;
        fboard = malloc(n * sizeof(char *));
        for(int r = 0; r < n; r++){
            fboard[r] = malloc(n * sizeof(char));
        }
        for(int i = 0; i < n; i++){
            bool doubleBreak = false;
            for(int j = 0; j < n; j++){
                if(board[i][j] == 'U' && checkLegalAll(board, i, j, oppo)){//find a avalible move
                    moved = true;
                    for(int r = 0; r < n; r++){
                        for(int c = 0; c < n; c++){
                            fboard[r][c] = board[r][c];
                        }
                    }
                    fboard[i][j] = oppo;
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){
                            if(!(a == 0 && b == 0) && checkLegalInDirection(false, fboard, i, j, oppo, a, b)){
                                checkLegalInDirection(true, fboard, i, j, oppo, a, b);
                            }
                        }
                    }
                    int eval = minMax(fboard, depth -1, alpha, beta, true, us, oppo);
                    if(eval < minEval){
                        minEval = eval;
                    }
                    if(eval < beta){
                        beta = eval;
                    }
                    if(beta <= alpha){
                        doubleBreak = true;
                        break;
                    }
                }
            }
            if(doubleBreak){
                break;
            }
        }
        for(int k = 0; k < n; k++){
            free(fboard[k]);
        }
        free(fboard);
        if(!moved){
            int eval = minMax(board, depth -1, alpha, beta, true, us, oppo);
            if(eval < minEval){
                minEval = eval;
            }
            if(eval < beta){
                beta = eval;
            }
        }
        return minEval;
    }
}


void AImove(char** board, char us, char oppo){
    int eval = -999999999, maxEval = -999999999, maxRow = 0, maxCol = 0, maxRowOfPrevious = 0, maxColOfPrevious = 0, alpha =  -999999999, beta = 999999999;
    begin = clock();
    char **fboard;
    fboard = malloc(n * sizeof(char *));
    for(int r = 0; r < n; r++){
        fboard[r] = malloc(n * sizeof(char));
    }
    int boardLeft = 61 - moveNumber;
    int depth = 3;
    
    if(boardLeft < (previousDepth + 3)){
        depth = boardLeft;
    }
    
    for( ; ; depth += 1){
        for(int i = 0; i < n; i++){
            bool doubleBreak = false;
            for(int j = 0; j < n; j++){
                if(board[i][j] == 'U' && checkLegalAll(board, i, j, us)){
                    for(int r = 0; r < n; r++){
                        for(int c = 0; c < n; c++){
                            fboard[r][c] = board[r][c];
                        }
                    }

                    fboard[i][j] = us;

                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){
                            if(!(a == 0 && b == 0) && checkLegalInDirection(false, fboard, i, j, us, a, b)){
                                checkLegalInDirection(true, fboard, i, j, us, a, b);
                            }
                        }
                    }
                    eval = minMax(fboard, depth, alpha, beta, false, us, oppo);
                    if(eval > maxEval){
                        maxEval = eval;
                        maxRow = i;
                        maxCol = j;
                    }
                    if(eval > alpha){
                        alpha = eval;
                    }
                    if(beta <= alpha){
                        doubleBreak = true;
                        break;
                    }
                }
                if(timeIsUp){
                    doubleBreak = true;
                    break;
                }
            }
            if(doubleBreak){
                break;
            }
        }
        if(!timeIsUp){
            maxRowOfPrevious = maxRow;
            maxColOfPrevious = maxCol;
        }else{
            depth--;
            break;
        }
    }
    timeIsUp = false;
    previousDepth = depth;
    for(int k = 0; k < n; k++){
        free(fboard[k]);
    }
    free(fboard);
    board[maxRowOfPrevious][maxColOfPrevious] = us;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(!(i == 0 && j == 0) && checkLegalInDirection(false, board, maxRowOfPrevious, maxColOfPrevious, us, i, j)){
                checkLegalInDirection(true, board, maxRowOfPrevious, maxColOfPrevious, us, i, j);
            }
        }
    }
    printf("Computer places %c at %c%c.\n", us, maxRowOfPrevious + 97, maxColOfPrevious + 97);
}

int main(int argc, const char * argv[]) {
    char **board;
    char move[3];
    board = malloc(n * sizeof(char *));
    for(int i = 0; i < n; i++){
        board[i] = malloc(n * sizeof(int));
    }
    inputAndCreateArray(board);
    printBoard(board);

    char shift = 'B', us = 'W', oppo = 'B';

    if(computerFirst()){
        us = 'B';
        oppo = 'W';
    }
    while(gameNotFull(board)){
        if(checkMoveAv(board, shift)){
            if(us == shift){
                AImove(board, us, oppo);
                moveNumber++;
                printBoard(board);
            }else{
                makeAMove(move, shift);
                if(checkMove(board, move)){
                    putInBoard(board, move);
                    changeMove(board, move);
                    moveNumber++;
                    printBoard(board);
                }else{
                    printf("Invalid move.\n");
                    printf("%c player wins.", us);
                    return 0;
                }
            }
        }else{
            if(shift == 'W'){
                if(!checkMoveAv(board, 'B')){
                    break;
                }
            }else{
                if(!checkMoveAv(board, 'W')){
                    break;
                }
            }
            printf("%c player has no valid move.\n", shift);
        }
        if(shift == 'W'){
            shift = 'B';
        }else{
            shift = 'W';
        }
    }
    WinLoss(board);
}





