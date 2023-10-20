#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

typedef char board_t[15];
typedef char player_t;

player_t next_player(player_t player)
{
    switch (player) {
        case RED: return BLUE;
        case BLUE: return RED;
        default: assert(0);
    }
}

int has_won(board_t board, player_t player)
{
    int f = 1;
    int z[2] = {2, 3};
    int y[3] = {3, 5, 6};
    int x[4] = {4, 7, 9, 10};

    for (int i = 5; i < 8; i++) {
        for (int j = i + 1; j <= 8; j++) {
            if (board[i] == next_player(player) &&
                board[j] == next_player(player) &&
                board[j + y[i - 5]] == next_player(player)) {
                return 1;
            }
        }
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            if (board[i] == next_player(player) &&
                board[j] == next_player(player) &&
                board[j + x[i]] == next_player(player)) {
                return 1;
            }
        }
    }

    for (int i = 9; i < 11; i++) {
        for (int j = i + 1; j <= 11; j++) {
            if (board[i] == next_player(player) &&
                board[j] == next_player(player) &&
                board[j + z[i - 9]] == next_player(player)) {
                return 1;
            }
        }
    }

for (int i = 12; i < 13; i++) {
        for (int j = i + 1; j <= 13; j++) {
            if (board[i] == next_player(player) &&
                board[j] == next_player(player) &&
                board[j + f] == next_player(player)) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board) {
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            return 0; // The board is not full
        }
    }
    return 1; // The board is full
}

typedef struct {
    int line;
    int score;
} move_t;

move_t best_move(board_t board, player_t player)
{
    move_t player_num;
    int non_player = 1;
    move_t move1;

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            if (has_won(board, player)) {
                board[i] = NO;
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }

    player_t oplayer = next_player(player);
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = oplayer;
            if (has_won(board, oplayer)) {
                board[i] = NO;
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
             move1= best_move(board, next_player(player));
            board[i] = NO;

            if (move1.score == -1) {
                return (move_t){i, 1};
            } else if (move1.score == 0) {
                player_num = (move_t){i, 0};
                non_player = 0;
            } else {
                if (non_player) {
                    player_num = (move_t){i, -1};
                    non_player = 0;
                }
            }
            if (is_full(board)) {
                board[i] = NO;
                return (move_t) {i, 0};
            }
        }
    }

    return player_num;
}

void print_board(board_t board) {
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R "); break;
        case BLUE: printf("B "); break;
        case NO: printf(". "); break;
        }
    }
    printf("\n");
}


int main()
{
    board_t board = {NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO};
    int play;
    int num;
    int i;
    printf("You want to be player 1 or 2, with our RED colour: ");
    scanf("%d", &num);
    player_t currentPlayer;

    if (num == 1) 
    {
        currentPlayer = RED;
    } 
    else if (num == 2) {
        currentPlayer = BLUE;
    } else 
    {
        printf("\nInvalid choice");
        return 1; 
    }

    while (1) {
        print_board(board);
        printf("\n");

        if (currentPlayer == RED) {
            printf("Enter a line (0-14): ");
            scanf("%d", &play);
            i = play;
            if (i >= 0 && i < 15 && board[i] == NO) {
                board[i] = currentPlayer;
            } 
            else {
                printf("Not a valid move\n");
                continue;
            }
        } else {
            move_t move2 = best_move(board, currentPlayer);
            int l = move2.line;
            board[l] = currentPlayer;
        }

        if (has_won(board, currentPlayer)) {
            print_board(board);
            printf("Player %c wins!\n", (currentPlayer == RED) ? 'R' : 'B');
            break;
        }
        else if (is_full(board)) {
            print_board(board);
            printf("It's a draw!\n");
            break;
        }

        currentPlayer = next_player(currentPlayer);  ## to switch the players
    }

    return 0;
}
