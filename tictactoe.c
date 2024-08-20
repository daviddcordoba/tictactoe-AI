#include "assert.h" /*  */
#include "stdbool.h" /* tipo de los booleanos  */
#include "stdio.h" /* printf().. */
#include "stdlib.h" /*  */
#include <math.h>
/* define es una manera de generar constantes, se reemplazan todas las apariciones por sus valores durante la compilacion */

#define BOARD_SIZE 3
#define CELL_MAX (BOARD_SIZE * BOARD_SIZE - 1)
// Declaración de funciones
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizing);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
char get_winner(char board[BOARD_SIZE][BOARD_SIZE]);
bool has_free_cell(char board[BOARD_SIZE][BOARD_SIZE]);

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
    int cell = 0;
    printf("\t .................................................\n");
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int column = 0; column < BOARD_SIZE; ++column) {
            printf("\t | %d: %c ", cell, board[row][column]);
            ++cell;
        }
        printf("\t | \n");
        printf("\t .................................................\n");
    }
} 

char get_winner(char board[BOARD_SIZE][BOARD_SIZE])
{
    char winner = '-'; // Inicialmente no hay ganador

    // Verificar las filas
    for (int row = 0; row < BOARD_SIZE; row++){
        char first = board[row][0]; 
        if(first != '-' && first != ' '){ 
            bool all_match = true; 
            for (int column = 1; column < BOARD_SIZE; column++) {
                if (first != board[row][column]) { 
                    all_match = false;
                    break;
                }
            }
            if (all_match) {
                return first;
            }
        }
    }

    // Verificar las columnas
    for (int column = 0; column < BOARD_SIZE; column++){
        char first = board[0][column]; 
        if(first != '-' && first != ' '){ 
            bool all_match = true; 
            for (int row = 1; row < BOARD_SIZE; row++) {
                if (first != board[row][column]) { 
                    all_match = false;
                    break;
                }
            }
            if (all_match) {
                return first;
            }
        }
    }

    //Verificar diagonal principal

    char first_diag_princ = board[0][0];
    if(first_diag_princ != '-' && first_diag_princ != ' '){
        bool all_match = true;
        for (int i = 1; i < BOARD_SIZE; i++){
            if(first_diag_princ != board[i][i]){
                all_match = false;
                break;
            }
        }
        if(all_match){
            return first_diag_princ;
        }
    }

    //Verificar diagonal secundaria

    char first_diag_sec = board[0][BOARD_SIZE-1];// board[0][2=board_size-1] board_size = 3 
    if(first_diag_sec != '-' && first_diag_sec != ' '){
        bool all_match = true;
        for (int i = 1; i < BOARD_SIZE; i++){
            if(first_diag_sec != board[i][BOARD_SIZE - 1 - i]){
                all_match = false;
                break;
            }
        }
        if(all_match){
            return first_diag_sec;
        }
    }
    return winner; // si llego a este punto, no hay ganador, si no ya hubiese sido retornado
}

bool has_free_cell(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    int free_cells = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            if(board[i][j] == '-'){
                free_cells++;
            }
        }
    }
    return free_cells > 0;
}  

int find_best_move(char board[BOARD_SIZE][BOARD_SIZE]) {
    int bestMove = -1;
    float bestValue = INFINITY;

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        int row = i / BOARD_SIZE;
        int col = i % BOARD_SIZE;
        if (board[row][col] == '-') {
            board[row][col] = 'O';  // Supongamos que 'O' es la IA
            int moveValue = minimax(board, 0, true);  // Evaluamos la jugada
            board[row][col] = '-';  // Deshacemos la jugada

            if (moveValue < bestValue) {
                bestMove = i;
                bestValue = moveValue;
            }
        }
    }
    return bestMove;
}

int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizing) {
    char winner = get_winner(board);
    if (winner == 'X') return 10 - depth;  // Maximizing player (X) wins
    if (winner == 'O') return depth - 10;  // Minimizing player (O) wins
    if (!has_free_cell(board)) return 0;   // Empate

    if (isMaximizing) {
        float maxEval = -INFINITY;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == '-') {
                    board[i][j] = 'X'; // Intento de movimiento
                    int eval = minimax(board, depth + 1, false); // Llamada recursiva
                    board[i][j] = '-'; // Deshacer movimiento
                    maxEval = eval > maxEval ? eval : maxEval;
                }
            }
        }
        return maxEval;
    } else {
        float minEval = INFINITY;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == '-') {
                    board[i][j] = 'O'; // Intento de movimiento
                    int eval = minimax(board, depth + 1, true); // Llamada recursiva
                    board[i][j] = '-'; // Deshacer movimiento
                    minEval = eval < minEval ? eval : minEval;
                }
            }
        }
        return minEval;
    }
}


int main(void) /* cuando un compila en c, lo primero que se llama es a main(), sino seria una libreria mas. retorna 0 para q el sist.op sepa que salio todo bien */
{
    printf("TicTacToe\n");

    char board[BOARD_SIZE][BOARD_SIZE] = {
        {'-', '-', '-'},
        {'-', '-', '-'},
        {'-', '-', '-'}
    };

    char turn = 'X'; // Para mi, siempre arrancara X
    char winner = '-';
    int cell = 0;

    while (winner == '-' && has_free_cell(board)) {
    print_board(board); // Muestro el tablero
    if (turn == 'X') {
        printf("\nTurno %c - Elija donde marcar (número del 0 al 8): ", turn);
        int scanf_result = scanf("%d", &cell);

        if (scanf_result <= 0) {
            printf("Error al leer un número desde teclado\n");
            exit(EXIT_FAILURE);
        }
    } else {
        cell = find_best_move(board);  // La IA elige la mejor jugada
        printf("\nLa IA elige la celda %d\n", cell);
    }

    if (cell >= 0 && cell <= CELL_MAX) {
        int row = cell / BOARD_SIZE;
        int colum = cell % BOARD_SIZE;

        if (board[row][colum] == '-') { // Si la celda está libre
            board[row][colum] = turn;
            turn = turn == 'X' ? 'O' : 'X'; // Cambio de turno
            winner = get_winner(board);
        } else {
            printf("\nCelda ocupada!\n");
        }

    } else {
        printf("\nCelda inválida!\n");
    }
}


    print_board(board);

    if (winner == '-') {
        printf("Empate!\n");
    } else {
        printf("Ganó %c\n", winner);
    } 
    return 0;
}
