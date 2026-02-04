#include <iostream>
using namespace std;

int main(){
    int board[840][840];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++)
            board[i][j] = '_';
        for (int k = 0; k < 9; k++)
            cout << "|";
    }




    return 0;
}
    
