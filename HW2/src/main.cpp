#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iomanip>
using namespace std;

typedef struct grid // 用來記錄每一個格子的內容(value)、座標(x y)、是否被打開(revealed)
{
    string value;
    bool revealed;
    int x;
    int y;
}grid;

grid** creatBoard(int, int );                   //創建陣列
void revealed(grid**, int , int , int);         //打開Board中的格子
void printBoard(grid**, int);                   //將陣列打印出來
//void printBoardtest(grid**, int);             //將陣列內容印出來(測試用)

int main()
{
    bool gameover = false , win = false;
    int boardSize, mineNum;
    int error = true;
    cout << "Welcome to the MineSweeper Game !!\n\n";
    cout << "set the board Size and mines Num that you want to play\n";
    while(error){
        cout << endl;
        cout << "board Size = ";
        cin >> boardSize;
        cout << "mines Num = ";
        cin >> mineNum;
        if (mineNum < boardSize * boardSize){
            error = false;
        }
        if (error == true){
            cout << endl;
            cout << "ERROR: Mines number and board size are invalid ! \nPlease set again\n";
        }
    }
    int nonMineCount = boardSize*boardSize-mineNum; // 用來記錄當前還有幾個非炸彈的格子未被打開
    grid** Board = creatBoard(boardSize, mineNum);
    cout << endl;
    printBoard(Board, boardSize);
//    printBoardtest(Board, boardSize);
    while(!gameover){
        int x, y;
        cout << endl;
        cout << "enter the grid that you want to reveal:\n";
        cout << "x : ";
        cin >> x;
        cout << "y : ";
        cin >> y;
        while(Board[x][y].revealed){
            cout << endl;
            cout << "Error : the grid have been reveled, please enter the grid again.";
            cout << "enter the grid that you want to reveal:\n";
            cout << "x : ";
            cin >> x;
            cout << "y : ";
            cin >> y;
        }
        if (Board[x][y].value != "bomb"){
            revealed(Board, x, y, boardSize);
            if (Board[x][y].value != "0"){
                Board[x][y].revealed = true;
            }
        } else {
            Board[x][y].revealed = true;
        }
        cout << endl;
        printBoard(Board, boardSize);
        nonMineCount = 0;
        for (int i=0 ; i<boardSize ; ++i){
            for (int j=0 ; j<boardSize ; ++j){
                if (Board[i][j].revealed == false && Board[i][j].value != "bomb"){
                    nonMineCount++;
                }
            }
        }
//        printBoardtest(Board, boardSize);
        if (Board[x][y].value == "bomb"){
            gameover = true;
        }
        if (nonMineCount == 0){
            gameover = true;
            win = true;
        }
    }
    if (win == true){
        cout << endl << "The Game is over. You Win !!!\n";
    } else {
        cout << endl << "The Game is over. You Lose\n";
    }
    return 0;
}

grid** creatBoard(int boardSize, int mineNum) {
    srand(time(0));
    grid** board= new grid* [boardSize];
    for (int i=0 ; i<boardSize ; ++i){
        board[i] = new grid [boardSize];
    }
    for (int i=0 ; i<boardSize ;++i){
        for (int j=0 ; j<boardSize ; ++j){
            board[i][j].value = "0";
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].revealed = false;
        }
    }

    // 隨機放置炸彈
    int mineCount = 0;

    while (mineCount < mineNum) {
        int x = rand() % boardSize;
        int y = rand() % boardSize;
        if (board[x][y].value == "0") {
            board[x][y].value = "bomb";
            mineCount++;
        }
    }

    for (int r = 0 ; r < boardSize ; r++) {
        for (int c = 0 ; c < boardSize ; c++) {
            if (board[r][c].value == "bomb"){
                continue;
            }

            // Top
            if (r > 0 && board[r-1][c].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Top Right
            if (r > 0 && c < boardSize - 1 && board[r-1][c+1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Right
            if (c < boardSize - 1 && board[r][c+1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Bottom Right
            if (r < boardSize - 1 && c < boardSize - 1 && board[r+1][c+1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Bottom
            if (r < boardSize - 1 && board[r+1][c].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Bottom Left
            if (r < boardSize - 1 && c > 0 && board[r+1][c-1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Left
            if (c > 0 && board[r][c-1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }

            // Top Left
            if (r > 0 && c > 0 && board[r-1][c-1].value == "bomb"){
                board[r][c].value = to_string(stoi(board[r][c].value)+1);
            }
        }
    }
    return board;
}

void revealed(grid** arr, int x, int y, int boardSize) {
    vector <grid> show;
    show.push_back(arr[x][y]);
    while(show.size() != 0){
        grid one = show[show.size()-1];
//        one.x = show[show.size()-1].x;
//        one.y = show[show.size()-1].y;
//        one.value = show[show.size()-1].value;
//        one.revealed = show[show.size()-1].revealed;
        show.pop_back();
        int i=one.x;
        int j=one.y;
        if(!one.revealed){
            one.revealed = true;
        }
        if(one.value != "0"){
            break;
        }

        // top left

        if(i>0 && j>0 && arr[i-1][j-1].value == "0" && !arr[i-1][j-1].revealed){
            show.push_back(arr[i-1][j-1]);
        }

        // bottom right

        if(i<boardSize-1 && j<boardSize-1 && arr[i+1][j+1].value == "0" && !arr[i+1][j+1].revealed){
            show.push_back(arr[i+1][j+1]);
        }

        // top right

        if(i>0 && j<boardSize-1 && arr[i-1][j+1].value == "0" && !arr[i-1][j+1].revealed){
            show.push_back(arr[i-1][j+1]);
        }

        // bottom left

        if(i<boardSize-1 && j>0 && arr[i+1][j-1].value == "0" && !arr[i+1][j-1].revealed){
            show.push_back(arr[i+1][j-1]);
        }

        // top
        if(i>0 && arr[i-1][j].value == "0" && !arr[i-1][j].revealed){
            show.push_back(arr[i-1][j]);
        }

        // right

        if(j<boardSize-1 && arr[i][j+1].value == "0" && !arr[i][j+1].revealed){
            show.push_back(arr[i][j+1]);
        }

        // bottom

        if(i<boardSize-1 && arr[i+1][j].value == "0" && !arr[i+1][j].revealed){
            show.push_back(arr[i+1][j]);
        }

        // left

        if(j>0 && arr[i][j-1].value == "0" && !arr[i][j-1].revealed){
            show.push_back(arr[i][j-1]);
        }


        // start revealing the grid

        if (i>0 && j>0 && !arr[i-1][j-1].revealed){
            //Top Left Reveal
            arr[i-1][j-1].revealed = true;
        }

        if (j>0 && !arr[i][j-1].revealed) {
        // Left Reveal
            arr[i][j-1].revealed = true;
        }

        if (i<boardSize-1 && j>0 && !arr[i+1][j-1].revealed){
        //Bottom Left Reveal
            arr[i+1][j-1].revealed = true;
        }

        if (i>0 && !arr[i-1][j].revealed){
        //Top Reveal
            arr[i-1][j].revealed = true;
        }

        if (i<boardSize-1 && !arr[i+1][j].revealed){
        // Bottom Reveal
            arr[i+1][j].revealed = true;
        }

        if (i>0 && j<boardSize-1 && !arr[i-1][j+1].revealed){
        // Top Right Reveal
            arr[i-1][j+1].revealed = true;
        }

        if (j<boardSize-1 && !arr[i][j+1].revealed) {
        //Right Reveal
            arr[i][j+1].revealed = true;
        }

        if (i<boardSize-1 && j<boardSize-1 && !arr[i+1][j+1].revealed){
        // Bottom Right Reveal
            arr[i+1][j+1].revealed = true;
        }
    }
}

void printBoard(grid** Board, int boardSize){
    cout << "  Y";
    for (int i=0 ; i<boardSize ; ++i){
        cout << "   " << setfill('0') << setw(2) << i << " ";
    }
    cout << endl << "X                   ";
    cout << endl;
    for (int i=0 ; i<boardSize ; ++i) {
        cout << setfill('0') << setw(2) << i << " ";
        for (int j=0 ; j<boardSize ; ++j){
            if (Board[i][j].revealed == true){
                cout << setfill(' ') << setw(4) << Board[i][j].value << "  ";
            } else {
                cout << setfill(' ') << setw(4) << "*" << "  ";
            }
        }
        cout << endl;
    }
}

//void printBoardtest(grid** Board, int boardSize){
//    for (int i=0 ; i<boardSize ; i++) {
//        for (int j=0 ; j<boardSize ; ++j){
//            cout << setw(4) << Board[i][j].value << "  ";
//        }
//        cout << endl;
//    }
//}
