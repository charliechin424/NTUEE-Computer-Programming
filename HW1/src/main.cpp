#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int check_end(int num[][3], char* result); // 判斷遊戲是否結束

void print_table (char table[][5]); // 印出每一輪玩家畫的圖案格子

int check_input (int num[][3], int x, int y); // 檢查玩家輸入的格子座標是否有效、已被使用過

void correspond(int* x, int* y); // 把玩家輸入的num格子座標轉換成table的格子座標

int main()
{
    srand(time(0));
    int who_start = rand() % 2 + 1; // 隨機判斷哪個玩家先開始
    char result = 'NULL'; // 用來判斷結果
    int turn = -1; // 代表當下哪個玩家的回合
    int x, y; // 玩家輸入的 x y 座標
    int wrong; // 用來判斷玩家輸入座標是否有誤
    char table[5][5]; // 輸出的表格
    int num[3][3]; // 與table對應之表格，用來當作傳入check_end()函式的參數

    //遊戲還沒開始時初始化為-1
    for (int i=0 ; i<3 ; ++i){
        for (int j=0 ; j<3 ; ++j){
            num[i][j] = -1;
        }
    }

    // 初始化表格樣式
    for (int i=0 ; i<5 ; i++){
        for (int j=0 ; j<5 ; ++j){
            if (i%2 == 0 && j%2 == 0){
                table[i][j] = ' ';
            } else if (i%2 == 1 && j%2 == 1) {
                table[i][j] = '+';
            } else if (i%2==0 && j%2==1){
                table[i][j] = '|';
            } else if (i%2==1 && j%2==0){
                table[i][j] = '-';
            }
        }
    }

    // 剛開始決定哪個玩家先開始(圈圈)
    if (who_start == 1){
        cout << "玩家A先開始\n";
    }
    if (who_start == 2){
        cout << "玩家B先開始\n";
    }

    // 玩家每輸入一次座標就判斷遊戲是否結束
    while(check_end(num, &result)){
        if (who_start % 2 == 1){
            cout << "----------輪到玩家A,請輸入格子";
        }
        if (who_start % 2 == 0){
            cout << "----------輪到玩家B,請輸入格子";
        }
        who_start++;
        turn++; // 每玩一輪 turn 就多 1

        //玩家輸入座標，並判斷合法性
        do {
            cin >> x >> y;
            wrong = check_input(num, x, y);
        } while(wrong);

        if (who_start % 2 == 1){
            num[x-1][y-1] = 1;
        }
        if (who_start % 2 == 0){
            num[x-1][y-1] = 2;
        }

        correspond(&x, &y);
        if (turn % 2 + 1 == 1){
            table[x][y] = 'O';
        }
        if (turn % 2 + 1 == 2){
            table[x][y] = 'X';
        }
        print_table(table); // 印出當前的表格圖案樣式
    }

    // 離開while迴圈，遊戲結束
    cout << "--------------------";
    cout << "遊戲結束，";

    // 判斷遊戲結果
    if (result == 'A'){
        cout << "玩家A獲勝";
    }
    if (result == 'B'){
        cout << "玩家B獲勝";
    }
    if (result == 'C'){
        cout << "此局雙方平手";
    }
    return 0;
}

int check_end(int num[][3], char* result)
{
    // 檢查每一列
    for (int i=0 ; i<3 ; ++i){
        if (num[i][0] == num[i][1] && num[i][1] == num[i][2]){
            if (num[i][2] == 1){
                *result = 'B';
                return 0;
            }
            if (num[i][2] == 2){
                *result = 'A';
                return 0;
            }
        }
    }

    // 檢查每一行
    for (int j=0 ; j<3 ; ++j){
        if (num[0][j] == num[1][j] && num[1][j] == num[2][j]){
            if (num[2][j] == 1){
                *result = 'B';
                return 0;
            }
            if (num[2][j] == 2){
                *result = 'A';
                return 0;
            }
        }
    }

    // 檢查對角線
    if (num[0][0] == num[1][1] && num[1][1] == num[2][2]){
        if (num[2][2] == 1){
            *result = 'B';
            return 0;
        }
        if (num[2][2] == 2){
            *result = 'A';
            return 0;
        }
    }
    if (num[0][2] == num[1][1] && num[1][1] == num[2][0]){
        if (num[2][0] == 1){
            *result = 'B';
            return 0;
        }
        if (num[2][0] == 2){
            *result = 'A';
            return 0;
        }
    }

    //檢查是否平手
    int pease = 1;
    for (int i=0 ; i<3 ; ++i){
        for (int j=0 ; j<3 ; ++j){
            if (num[i][j] == -1){
                pease = 0;
            }
        }
    }
    if (pease == 1){
        *result = 'C';
        return 0;
    }
    return 1;
}

void print_table (char table[][5])
{
    for (int i=0 ; i<5 ; ++i){
        for (int j=0 ; j<5 ; ++j){
            cout << table[i][j];
        }
        cout << "\n";
    }
}

int check_input (int num[][3], int x, int y)
{
    // 超過表格邊界
    if (x>3 || x<1 || y>3 || y<1){
        cout << "輸入錯誤，請重新輸入格子";
        return 1;
    }

    //格子已被使用
    if (num[x-1][y-1] != -1){
        cout << "此格子已被畫過，請重新輸入格子";
        return 1;
    }
    return 0;
}

void correspond(int* x, int* y)
{
    if ((*x)==1){
        (*x)--;
    }
    if ((*x)==3){
        (*x)++;
    }
    if ((*y)==1){
        (*y)--;
    }
    if ((*y)==3){
        (*y)++;
    }
}



