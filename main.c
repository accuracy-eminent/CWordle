#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUF_LEN 8
#define WORD_LEN 5
#define NUM_GUESS 6

typedef struct {
    char guess[BUF_LEN];
    char ans[BUF_LEN];
    char status[BUF_LEN];
    int remain_guess;
    int solved;
} game_state_t;

void check_guess(game_state_t *state)
{
    state->solved = 1;
    for(int i = 0; i < WORD_LEN; i++)
    {
        char cur_letter = state->guess[i];
        char status = 'b';
        for(int j = 0; j < WORD_LEN; j++)
        {
            if(state->ans[j] == cur_letter)status = 'y';
        }
        if(state->ans[i] == cur_letter)status = 'g';
        state->status[i] = status;
        if(status != 'g')state->solved = 0;
    }
    // Check if all are correct
}

void print_guess(game_state_t *state)
{
    for(int i = 0; i < WORD_LEN; i++)
    {
        char open_char, close_char;
        switch(state->status[i])
        {
            case 'y':
                open_char = '{';
                close_char = '}';
                break;
            case 'g':
                open_char = '[';
                close_char = ']';
                break;
            default:
                open_char = '(';
                close_char = ')';
        }
        putchar(open_char);
        putchar(state->guess[i]);
        putchar(close_char);
    }
    putchar('\n');
}

void get_guess(game_state_t *state)
{
    printf("Enter guess: ");
    fgets(state->guess, BUF_LEN - 1, stdin);
    // Replace extra null terminators with spaces
    for(int i = 0; i < WORD_LEN; i++)
    {
        if(state->guess[i] == '\0' || state->guess[i] == '\r' || state->guess[i] == '\n') state->guess[i] = ' ';
    }
}

int read_word(game_state_t *state)
{
    FILE *fp;
    fp = fopen("words.txt", "rb");
    if(!fp)
    {
        printf("Couldn't open word list!\n");
        return -1;
    }
    // Get number of lines
    int count = 0;
    for(char c = getc(fp); c != EOF; c = getc(fp))
        if(c == '\n')
            count++;
    rewind(fp);
    // Choose a random line
    int rand_line = rand() % count;
    count = 0;
    for(char c = getc(fp); c != EOF && count < rand_line; c = getc(fp))
        if(c == '\n')
            count++;
    fseek(fp, -1L, SEEK_CUR);
    fgets(state->ans, WORD_LEN + 1, fp);
    fclose(fp);
    return 1;
}

void initialize(game_state_t *state)
{
    srand(time(NULL));
    for(int i = 0; i < BUF_LEN; i++)
    {
        state->guess[i] = ' ';
        state->status[i] = 'b';
        state->ans[i] = ' ';
    }
    state->remain_guess = NUM_GUESS;
    state->solved = 0;
}


int main(void)
{
    game_state_t state;
    printf("CWordle\n");
    while(1)
    {
        printf("***\nStarting round\n");
        initialize(&state);
        read_word(&state);
        for(state.remain_guess = NUM_GUESS; state.remain_guess >= 1; state.remain_guess--)
        {
            printf("%d guesses remaining\n", state.remain_guess);
            get_guess(&state);
            check_guess(&state);
            print_guess(&state);
            if(state.solved)
            {
                // If we solved it, end the current round, and decrement the guess counter to count this guess
                state.remain_guess--;
                break;
            }
            printf("----\n");
        }
        printf("Puzzle %ssolved with %d guesses remaining.\n", state.remain_guess ? "": "not ", state.remain_guess);
        printf("The word was: %s\n", state.ans);
        printf("Play again (y/n)? ");
        char buf[BUF_LEN];
        fgets(buf, BUF_LEN - 1, stdin);
        if(buf[0] != 'y') break;
    }
    return 0;
}