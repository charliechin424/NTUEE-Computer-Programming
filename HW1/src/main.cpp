#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int check_end(int num[][3], char* result); // �P�_�C���O�_����

void print_table (char table[][5]); // �L�X�C�@�����a�e���Ϯ׮�l

int check_input (int num[][3], int x, int y); // �ˬd���a��J����l�y�ЬO�_���ġB�w�Q�ϥιL

void correspond(int* x, int* y); // �⪱�a��J��num��l�y���ഫ��table����l�y��

int main()
{
    srand(time(0));
    int who_start = rand() % 2 + 1; // �H���P�_���Ӫ��a���}�l
    char result = 'NULL'; // �ΨӧP�_���G
    int turn = -1; // �N���U���Ӫ��a���^�X
    int x, y; // ���a��J�� x y �y��
    int wrong; // �ΨӧP�_���a��J�y�ЬO�_���~
    char table[5][5]; // ��X�����
    int num[3][3]; // �Ptable���������A�Ψӷ�@�ǤJcheck_end()�禡���Ѽ�

    //�C���٨S�}�l�ɪ�l�Ƭ�-1
    for (int i=0 ; i<3 ; ++i){
        for (int j=0 ; j<3 ; ++j){
            num[i][j] = -1;
        }
    }

    // ��l�ƪ��˦�
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

    // ��}�l�M�w���Ӫ��a���}�l(���)
    if (who_start == 1){
        cout << "���aA���}�l\n";
    }
    if (who_start == 2){
        cout << "���aB���}�l\n";
    }

    // ���a�C��J�@���y�дN�P�_�C���O�_����
    while(check_end(num, &result)){
        if (who_start % 2 == 1){
            cout << "----------���쪱�aA,�п�J��l";
        }
        if (who_start % 2 == 0){
            cout << "----------���쪱�aB,�п�J��l";
        }
        who_start++;
        turn++; // �C���@�� turn �N�h 1

        //���a��J�y�СA�çP�_�X�k��
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
        print_table(table); // �L�X��e�����Ϯ׼˦�
    }

    // ���}while�j��A�C������
    cout << "--------------------";
    cout << "�C�������A";

    // �P�_�C�����G
    if (result == 'A'){
        cout << "���aA���";
    }
    if (result == 'B'){
        cout << "���aB���";
    }
    if (result == 'C'){
        cout << "�������襭��";
    }
    return 0;
}

int check_end(int num[][3], char* result)
{
    // �ˬd�C�@�C
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

    // �ˬd�C�@��
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

    // �ˬd�﨤�u
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

    //�ˬd�O�_����
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
    // �W�L������
    if (x>3 || x<1 || y>3 || y<1){
        cout << "��J���~�A�Э��s��J��l";
        return 1;
    }

    //��l�w�Q�ϥ�
    if (num[x-1][y-1] != -1){
        cout << "����l�w�Q�e�L�A�Э��s��J��l";
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



