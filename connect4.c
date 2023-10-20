#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

enum
{
    EMPTY = 0,
    RED,
    BLUE,
};

typedef char brd_t[4][5];
typedef char player_t;

void init_board(brd_t *board)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            (*board)[row][col] = '0';
        }
    }
}

player_t otherPlayer(player_t player)
{
    switch (player)
    {
    case 'R':
        return 'B';
    case 'B':
        return 'R';
    default:
        assert(0);
        return 0;
    }
}

int has_won(brd_t board, player_t player)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            if (board[row][col] == player && board[row][col + 1] == player &&
                board[row][col + 2] == player && board[row][col + 3] == player)
            {
                return 1;
            }
        }
    }

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            if (board[row][col] == player && board[row + 1][col] == player &&
                board[row + 2][col] == player && board[row + 3][col] == player)
            {
                return 1;
            }
        }
    }

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            if (board[row][col] == player && board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player && board[row + 3][col + 3] == player)
            {
                return 1;
            }
        }
    }

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 3; col < 5; ++col)
        {
            if (board[row][col] == player && board[row + 1][col - 1] == player &&
                board[row + 2][col - 2] == player && board[row + 3][col - 3] == player)
            {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(brd_t board)
{
    for (int j = 0; j < 5; ++j)
    {
        if (board[0][j] == '0')
        {
            return 0;
        }
    }
    return 1;
}

int score(brd_t board, player_t player)
{
    assert(has_won(board, player));
    int score = 10;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (board[i][j] == player)
            {
                score -= 1;
            }
        }
    }
    return score;
}

typedef struct
{
    int col;
    int score;
} move_t;

int drop_piece(brd_t board, int col, player_t player)
{
    assert(board[3][col] == '0' || board[2][col] == '0' || board[1][col] == '0' || board[0][col] == '0');
    for (int row = 3; row >= 0; --row)
    {
        if (board[row][col] == '0')
        {
            board[row][col] = player;
            return row;
        }
    }
    return -1; 
}

void print_board(brd_t board)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            printf("%c ", board[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

move_t best_move(brd_t board, player_t player, int depth, int alpha, int beta)
{
    move_t response;
    move_t candidate = {.col = -1, .score = (player == 'B') ? INT_MIN : INT_MAX};

    if (has_won(board, 'R'))
    {
        candidate.score = -10;
        return candidate;
    }
    if (has_won(board, 'B'))
    {
        candidate.score = 10;
        return candidate;
    }

    if (is_full(board) || depth == 0)
    {
        candidate.score = 0;
        return candidate;
    }

    for (int col = 0; col < 5; ++col)
    {
        if (board[0][col] == '0')
        {
            int row = drop_piece(board, col, player);
            response = best_move(board, (player == 'B') ? 'R' : 'B', depth - 1, alpha, beta);
            board[row][col] = '0';

            if (player == 'B')
            {
                if (response.score > candidate.score)
                {
                    candidate.col = col;
                    candidate.score = response.score;
                }
                alpha = (alpha > response.score) ? alpha : response.score;
            }
            else // player == 'R'
            {
                if (response.score < candidate.score)
                {
                    candidate.col = col;
                    candidate.score = response.score;
                }
                beta = (beta < response.score) ? beta : response.score;
            }
            if (beta <= alpha)
            {
                break; 
            }
        }
    }

    return candidate;
}

int main()
{
    brd_t board;
    init_board(&board);

    char user_choice;
    printf("Choose Red (R) or Blue (B)? ");
    scanf(" %c", &user_choice);

    if (user_choice != 'R' && user_choice != 'B')
    {
        printf("Invalid choice. Please select 'R' or 'B'.\n");
        return 1;
    }

move_t response;
    player_t current = 'R';
    int depth = 6; // Set the depth for Computer search

    while (1)
    {
        print_board(board);

        if ((user_choice == 'R' && current == 'R') || (user_choice == 'B' && current == 'B'))
        {
            printf("Enter your move (0-4): ");
            int move;
            scanf("%d", &move);

            while (move < 0 || move > 4 || board[0][move] != '0')
            {
                printf("Invalid move. Please try again: ");
                scanf("%d", &move);
            }

            drop_piece(board, move, current);
        }
        else
        {
            response = best_move(board, current, depth, INT_MIN, INT_MAX);

            if (response.col == -1)
            {
                printf("It's a draw!\n");
                break;
            }
            printf("Computer chooses column %d\n", response.col);

            drop_piece(board, response.col, current);
        }

        if (has_won(board, current))
        {
            print_board(board);
            printf("Player %c has won!\n", current);
            break;
        }
        else if (is_full(board))
        {
            print_board(board);
            printf("It's a draw!\n");
            break;
        }
        current = (current == 'R') ? 'B' : 'R';
    }

    return 0;
}
