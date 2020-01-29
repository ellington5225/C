/* Resources:
I didn't use any outside code but used these sources to clarify the rules and to learn about certain programming concepts in C.
https://en.wikipedia.org/wiki/Mastermind_(board_game)
https://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c (for the srand(time(NULL)))
https://codereview.stackexchange.com/questions/138339/a-simple-mastermind-game-in-c (did not look at the sample code, just rules)
*/


#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

//variables used to track instances and frequency of colors in the computer's sequence and in the guessed sequence
//colors: red, orange, yellow, green, blue, purple, white
int compr = 0;
int compo = 0;
int compy = 0;
int compg = 0;
int compb = 0;
int compp = 0;
int compw = 0;

int guessr = 0;
int guesso = 0;
int guessy = 0;
int guessg = 0;
int guessb = 0;
int guessp = 0;
int guessw = 0;

//guess_num tracks the number of valid guesses made
int guess_num = 0;

void assign_color(char array[], int index, int n);
void change_frequency(char character, int change_num, string source);
int compare_frequencies(char character);
bool checkzero(char character);

int main(int argc, string argv[])
{
    //make number generation more random
    srand(time(NULL));

    //make sure correct usage
    if (argc != 2)
    {
        printf("Usage: ./mastermind [3 <= x <= 5] or ./mastermind rules\n");
        return 1;
    }
    else if (strcmp(argv[1], "rules") == 0)
    {
        printf("\n   In Mastermind, your goal is to guess the sequence of colored marbles the computer has selected.\n\n");
        printf("   Usage is ./mastermind [3 <= x <= 5], where x is the number of marbles in the computer's sequence.\n\n");
        printf("   The colors the computer can choose from are red (R), orange (O), yellow (Y), green (G), blue (B), purple (P),");
        printf(" and white (W)\n\n");
        printf("   To make a guess about the sequence, simply type in a string of the letters corresponding to the numbers.\n\n");
        printf("   For example, a guess could be BYGPOR (although capitalization doesn't matter).\n\n");
        printf("   The computer will then return a series of minuses (-) and pluses (+).\n\n");
        printf("   A plus (+) means your guessed color and position of one of the marbles were correct. A minus (-) means your");
        printf(" guessed color was correct, but your guessed position was not.\n\n");
        printf("   No symbol will be returned if your guessed color and position were both incorrect.\n\n");
        printf("   Keep guessing until you get all pluses!\n\n");
        printf("   Happy playing!\n\n");
        return 0;
    }
    else if (atoi(argv[1]) < 3 || atoi(argv[1]) > 5)
    {
        printf("Please choose a number between 3 and 5, inclusive.\n");
        return 2;
    }

    //if correct usage, proceed to get user's specified # of marbles and make an array for their colors
    int marbles_num = atoi(argv[1]);
    char colors[marbles_num];

    //fill the array with colors with the function assign_color
    for (int i = 0; i < marbles_num; i++)
    {
        int number = rand();
        assign_color(colors, i, number);
    }

    //variable counts the number of pluses to determine if the sequence has been guessed correctly or not
    int plus_counter = 0;

    //each time this while loop runs, it goes through one round
    while (plus_counter < marbles_num)
    {
        //error_prompted will keep track of whether or not the user has gotten an error message
        bool error_prompted = false;

        compr = 0;
        compo = 0;
        compy = 0;
        compg = 0;
        compb = 0;
        compp = 0;
        compw = 0;

        guessr = 0;
        guesso = 0;
        guessy = 0;
        guessg = 0;
        guessb = 0;
        guessp = 0;
        guessw = 0;

        plus_counter = 0;

        //get a guess from the user
        string guess;

        guess = get_string("Guess: ");
        if (strlen(guess) != marbles_num)
        {
            printf("Please use the correct number of marbles.\n");
            guess = get_string("Guess: ");
        }

        int guess_array[marbles_num];

        //array will determine whether or not a specific guessed marble's correctness status has been determined yet
        //they all start as false
        bool determined[marbles_num];
        for (int k = 0; k < marbles_num; k++)
        {
            determined[k] = false;
        }

        //put the characters in the guess into the array guess_array for comparison
        for (int i = 0; i < marbles_num; i++)
        {
            //put the characters in the guess into the array guess_array for comparison
            guess_array[i] = toupper(guess[i]);
        }

        //get frequencies of colors in the computer's sequence and the guessed sequence
        for (int m = 0; m < marbles_num; m++)
        {
            change_frequency(colors[m], 1, "computer");
            change_frequency(guess_array[m], 1, "guess");
        }

        //check for invalid characters
        for (int v = 0; v < marbles_num; v++)
        {
            int comparison = compare_frequencies(guess_array[v]);
            if (comparison == 2)
                {
                    if (error_prompted == false)
                    {
                        printf("Oops, looks like you've used an invalid character! Remember, the marbles will only be one of the");
                        printf(" following colors: ROYBGPW");
                        error_prompted = true;
                    }
                }
        }
        //check for color/position matches (pluses)
        for (int j = 0; j < marbles_num; j++)
        {
            if (error_prompted == false) //only continue to this part if no error message has been prompted
            {
                if (guess_array[j] == (int) colors[j])
                {
                    printf("+");
                    //decreases the computer and guess frequencies by 1 - frequencies ~ frequency of the remaining marbles
                    //basically, means that one of the marbles of that color has already been guessed
                    //so later, the program will only look at the remaining, unguessed marbles
                    change_frequency(colors[j], -1, "computer");
                    change_frequency(guess_array[j], -1, "guess");
                    determined[j] = true;
                    plus_counter++;
                }
            }
        }

        //check for color but not position matches (minuses)
        for (int n = 0; n < marbles_num; n++)
        {
            if (error_prompted == false) //only continue to this part if no error message has been prompted
            {
                //if the marble hasn't already been guessed and assigned a plus
                if (determined[n] == false)
                {
                    int comparison = compare_frequencies(guess_array[n]);
                    if (comparison == 0 || comparison == 1) //if the frequencies match or the computer frequency > guess frequency
                    {
                        if (checkzero(guess_array[n]) == false) //and the frequencies are nonzero
                        {
                            //user gets a minus, subtract from the frequency
                            printf("-");
                            change_frequency(guess_array[n], -1, "computer");
                            change_frequency(guess_array[n], -1, "guess");
                        }
                        //else the guess frequency is zero, then should get nothing
                    }
                    else if (comparison < 0) //if guess frequency > computer frequency
                    {
                        if (checkzero(guess_array[n]) == false) //if the guess frequency is nonzero
                        {
                            if (checkzero(colors[n]) == false) //if the computer frequency is nonzero
                            {
                                //get a minus and subtract from the frequency
                                printf("-");
                                change_frequency(guess_array[n], -1, "computer");
                                change_frequency(guess_array[n], -1, "guess");
                            }
                        }
                        //else none of that color and you get nothing
                    }
                }
            }
        }
        printf("\n");

        //only count guesses that have all correct characters
        if (error_prompted == false)
        {
            guess_num++;
        }
    }

    //displays green congratulatory message and number of guesses
    printf("\033[32m");
    printf("Congratulations! You have won Mastermind!\n");
    printf("Number of guesses: %i\n", guess_num);
    printf("\033[39m");
}

//assign_color uses the randomly generated number n to assign a color to each marble in the computer's sequence
void assign_color(char array[], int index, int n)
{
    int chunk = RAND_MAX / 7;

    if (n < chunk)
    {
        array[index] = 'R';
    }
    else if (n > chunk && n < 2 * chunk)
    {
        array[index] = 'O';
    }
    else if (n > 2 * chunk && n < 3 * chunk)
    {
        array[index] = 'Y';
    }
    else if (n > 3 * chunk && n < 4 * chunk)
    {
        array[index] = 'G';
    }
    else if (n > 4 * chunk && n < 5 * chunk)
    {
        array[index] = 'B';
    }
    else if (n > 5 * chunk && n < 6 * chunk)
    {
        array[index] = 'P';
    }
    else if (n > 6 * chunk && n < RAND_MAX)
    {
        array[index] = 'W';
    }
    return;
}

//changes the frequency of colors of remaining marbles in the computer or guess sequences
void change_frequency(char character, int change_num, string source)
{
    if (strcmp(source, "computer") == 0)
    {
        switch(character)
        {
            case 'R':
                compr = compr + change_num;
                break;

            case 'O':
                compo = compo + change_num;
                break;

            case 'Y':
                compy = compy + change_num;
                break;

            case 'G':
                compg = compg + change_num;
                break;

            case 'B':
                compb = compb + change_num;
                break;

            case 'P':
                compp = compp + change_num;
                break;

            case 'W':
                compw = compw + change_num;
                break;
        }
    }
    else if (strcmp(source, "guess") == 0)
    {
        switch(character)
        {
            case 'R':
                guessr = guessr + change_num;
                break;

            case 'O':
                guesso = guesso + change_num;
                break;

            case 'Y':
                guessy = guessy + change_num;
                break;

            case 'G':
                guessg = guessg + change_num;
                break;

            case 'B':
                guessb = guessb + change_num;
                break;

            case 'P':
                guessp = guessp + change_num;
                break;

            case 'W':
                guessw = guessw + change_num;
                break;
        }
    }
}

//compares the frequencies of a certain character (color) in the computer's and the guessed sequences
//returns 0 if they match, 1 if the computer's is greater, and -1 if the guessed is greater, returns 2 otherwise
int compare_frequencies(char character)
{
    switch(character)
    {
        case 'R':
            if (compr == guessr)
            {
                return 0;
            }
            else if (compr < guessr)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'O':
            if (compo == guesso)
            {
                return 0;
            }
            else if (compo < guesso)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'Y':
            if (compy == guessy)
            {
                return 0;
            }
            else if (compy < guessy)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'G':
            if (compg == guessg)
            {
                return 0;
            }
            else if (compg < guessg)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'B':
            if (compb == guessb)
            {
                return 0;
            }
            else if (compb < guessb)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'P':
            if (compp == guessp)
            {
                return 0;
            }
            else if (compp < guessp)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        case 'W':
            if (compw == guessw)
            {
                return 0;
            }
            else if (compw < guessw)
            {
                return -1;
            }
            else
            {
                return 1;
            }
            break;

        default:
            return 2;
            break;
    }
}

//checks if a certain character's frequency is zero or not in a sequence
bool checkzero(char character)
{
    switch (character)
    {
        case 'R':
            if (compr == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'O':
            if (compo == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'Y':
            if (compy == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'G':
            if (compg == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'B':
            if (compb == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'P':
            if (compp == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        case 'W':
            if (compw == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;

        default:
            return false;
            break;
    }
}