#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int InitializeBoard(int **connectNBoard, int numRowsInBoard);
void FillZero(int **my2DArray, int M);
int DisplayBoard(int **connectNBoard, int numRowsInBoard);
int MakeMove(int **connectNBoard, int numRowsInBoard, int playerID, int columnChosen);
int CheckWinner(int **connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID);
int noEmptyElements(int **connectNBoard, int numRowsInBoard, int columnChosen);
int convertNumToInt(char arr[]);

int main()
{
    const int MAXSIZE = 25;
    const int MINSIZE = 8;
    const int MINCONNECTNUM = 4;

    //Prompt for and read the size of the board (numRows)
    int numRows;
    while (1) {
        printf("Enter the number of squares along each edge of the board: \n");
        if (scanf("%d", &numRows) != 1) { 
            //If the value of numRows provided by the user was not a number print the error message
            printf("ERROR: The value provided was not an integer\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        //If the user enters an out of range value for the size of the board, print the appropriate error message
        if (numRows < MINSIZE) {
            printf("ERROR: Board size too small (<%d)\n", MINSIZE);
            while (getchar() != '\n'); // Clear input buffer
        } else if (numRows > MAXSIZE) {
            printf("ERROR: Board size too large (>%d)\n", MAXSIZE);
            while (getchar() != '\n'); // Clear input buffer
        }
        else {
            //numRows is an integer and within acceptable range
            break;
        }
    }

    //Prompt for and read the number of game pieces in a line that are needed to win the game (numToConnect)
    int numToConnect;
    while (1) {
        printf("Enter the number of pieces that must form a line to win\n");
        if (scanf("%d", &numToConnect) != 1) {
            //If the value of numToConnect provided by the user was not an integer print the error message
            printf("ERROR: The value provied was not an integer\n");
            while (getchar() != '\n'); //Clear input buffer
            continue;
        }
        //If the user enters an out of range value for the number of pieces that must be in a line
        //to win the game (numToConnect), print the appropriate error message
        if (numToConnect < MINCONNECTNUM) {
            printf("ERROR: Number to connect is too small (<%d)\n", MINCONNECTNUM);
            while (getchar() != '\n'); //Clear input buffer
        }
        else if (numToConnect > (numRows - MINCONNECTNUM)) {
            printf("ERROR: Number to connect is too large (>%d)\n", (numRows - MINCONNECTNUM));
            while (getchar() != '\n'); //Clear input buffer
        }
        else {
            //numToConnect is an integer and within the acceptable range
            break;
        }
    }

    // Dynamically allocate an integer array, myConnectNBoard, to hold a game board of the specified size
    // Do not initialize the array in the main program
    int **myConnectNBoard = (int **)malloc(numRows * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        myConnectNBoard[i] = (int *)malloc(numRows * sizeof(int));
    }

    int playernow = 0; // 1 = RED, 0 = BLACK
    int numattempts = 0;
    int win = 0;
    // Initialize the game board using the InitializeBoard function
    // Check to see that the board initialized correctly (InitializedBoard returned true)
    // If the board does not initialize correctly print the error message
    // ERROR: Could not initialize the game board
    // Then terminate the main program
    if (InitializeBoard(myConnectNBoard, numRows) == 0)
    {
        printf("ERROR: Could not initialize the game board");

        exit(1);
    }

    while (win == 0)
    {
        playernow = (playernow + 1) % 2;
        // Print two blank lines to the screen
        printf("\n\n");
        // Print the board to the screen using the DisplayBoard funtion
        //  If the board does not display correctly print the error message ERROR: Could not display the game board
        if (DisplayBoard(myConnectNBoard, numRows) == 0)
        {
            printf("ERROR: Could not display the game board");

            exit(1);
        }
        // Print a single line either
            //Red moves
            //Black moves
        if (playernow == 1)
        {
               printf("Red moves\n");
        }
        else
        {
            printf("Black moves\n");
        }
        // giving them three attempts to make a legal move
        for (numattempts = 1; numattempts <= 3; numattempts++)
        {
            printf("Enter the column number where you want to put your piece\n");
            int choosecolumn;
            /// If the entered value is not a number then print the error message
            if (scanf("%d", &choosecolumn) != 1)
            {
                printf("ERROR: The value provided was not a number\n");
                int clear;
                while ((clear = getchar()) != '\n' && clear != EOF)
                    ;
                continue;
            }
            int clear;
            while ((clear = getchar()) != '\n' && clear != EOF)
            {
                if (!isspace(clear))
                {
                }
            }

            // If the chosen column is not on the board, print the following error message and
            if (choosecolumn < 0 || choosecolumn > numRows - 1)
            {
                printf("ERROR: Column %d is not on the board: try again\n", choosecolumn);
                printf("ERROR: Column number should be >= 0 and < %d\n", numRows);
                continue;
            }
            // If all squares in the chosen column are occupied
            if (noEmptyElements(myConnectNBoard, numRows, choosecolumn) == 1)
            {
                // returns 1 if there are empty elements
                // returns 0 if there are no empty elements
                printf("\nERROR: Column %d is already completely full try again/n", choosecolumn);
                continue;
            }
            // if make move returns false
            if (MakeMove(myConnectNBoard, numRows, playernow, choosecolumn) == 0)
            {
                printf("illegal move\n");
                continue;
            }
            if (DisplayBoard(myConnectNBoard, numRows) == 0)
            {
                printf("ERROR: Could not display the game board");
                exit(1);
            }
            playernow = (playernow + 1) % 2;

            numattempts = 0;

            if (playernow == 1)
            {
                printf("Red moves\n");
               
            }
            else
            {
                printf("Black moves\n");
            }

            if (CheckWinner(myConnectNBoard, numRows, numToConnect, choosecolumn, playernow) == 1)
            {
                win = 1;
                break;
            }
        }

        if (win == 0)
        {
            playernow = (playernow + 1) % 2; // switching the player
        }
        if (win == 1)
        {
            if (playernow == 1)
            {
                printf("Black has won\n");
            }
            else
            {
                printf("Red has won\n");
            }
        }

        playernow = (playernow + 1) % 2;
        // forfeiting their turn
        if (numattempts == 4)
        {
            printf("TOO MANY ILLEGAL MOVES\n");
            if (playernow == 0)
            {
                printf("Black has forfeited a turn\n");
            }
            else
            {
                printf("Red has forfeited a turn\n");
            }
            numattempts = 0;     // resetting the count to so next player gets a fresh start

            continue;
        }
    }
    return 0;
}

// array connectNBoard is created in the calling program then passed into the function
// Initialize connectNBoard to have an 0 in each element
// Catch errors that would generate the following error messages:
// ERROR: Cannot initialize the board because the pointer to the board is NULL
// ERROR: illegal number of rows in board
// acceptable range of values for the size of the board is
// 8<=numRows<=25
// If either of these errors are generated the function will return false, otherwise return true
int InitializeBoard(int **connectNBoard, int numRowsInBoard)
{
    // connectNBoard is already created in the calling program
    // Initialize connectNBoard to have an 0 in each element
    if (connectNBoard == NULL)
    {
        printf("ERROR: Cannot initialize the board because the pointer to the board is NULL");
        return 0;
    }
    else if (numRowsInBoard < 8 || 25 < numRowsInBoard)
    {
        printf("ERROR: illegal number of rows in board");
        return 0;
    }
    else
    {
        // set all elements in a 2D array of size M rows by M columns to 0
        for (int i = 0; i < numRowsInBoard; i++)
        {
            for (int j = 0; j < numRowsInBoard; j++)
            {
                connectNBoard[i][j] = 0;
            }
        }
        return 1;
    }
}

// Be sure to catch the errors that would generate the following error messages
// ERROR: cannot print the board because the pointer to the board is NULL
// ERROR: illegal number of rows in board
// If either of these errors are generated the function will return false, otherwise return true
// Each number or character should be printed in a field 3 characters wide
int DisplayBoard(int **connectNBoard, int numRowsInBoard)
{
    if (connectNBoard == NULL)
    {
        printf("ERROR: cannot print the board because the pointer to the board is NULL\n");
        return 0;
    }
    else if (numRowsInBoard < 8 || 25 < numRowsInBoard)
    {
        printf("ERROR: illegal number of rows in board\n");
        return 0;
    }
    else
    {
        // prints the first row indicating the column numbers
        //    0  1  2  3  4  5  6 ... numRowsInBoard-1
        for (int i = -1; i < numRowsInBoard; i++)
        {
            if (i == -1)
            {
                printf("   ");
            }
            else
            {
                printf("%3d", i);
            }
        }
        printf("\n");
        // prints the remaining rows, with index at the start of each row
        for (int i = 0; i < numRowsInBoard; i++)
        {
            printf("%3d", i);
            // number of columns same as number of rows
            for (int j = 0; j < numRowsInBoard; j++)
            {
                if (connectNBoard[i][j] == 1)
                {
                    printf("%3c", 'R');
                }
                else if (connectNBoard[i][j] == 2)
                {
                    printf("%3c", 'B');
                }
                else
                {
                    printf("%3c", 'o');
                }
            }
            printf("\n");
        }
        return 1;
    }
}

// Be sure to catch the error that would generate the following error message
// ERROR: cannot print the board because the pointer to the board is NULL
// if this error is generated the function will return false
// If the move is not legal (the column chosen is not part of the board, or the column chosen
// has no empty elements) print a line of text to the screen that says
// Illegal move
// then return false
// Begin at the bottom (largest row number) of column indicated by the columnChosen. Find
// the bottomost element in column columnChosen that does not yet contain a game piece.
// Place the preset gamepiece into that location, in the connectNBoard array
// Print a line of text to the screen that says either
// Red has moved
// Black has moved
// Return true
int MakeMove(int **connectNBoard, int numRowsInBoard, int playerID, int columnChosen)
{
    int firstEmptyRow = 0;
    if (connectNBoard == NULL)
    {
        printf("ERROR: cannot print the board because the pointer to the board is NULL\n");
        return 0;
    }
    // the column chosen is not part of the board return false
    else if (columnChosen < 0 || numRowsInBoard < columnChosen)
    {
        printf("Illegal move 1\n");
        return 0;
    }
    // the column chosen is part of the board and
    // the column chosen has no empty elements return false
    else if (noEmptyElements(connectNBoard, numRowsInBoard, columnChosen))
    {
        printf("Illegal move 2\n");
        return 0;
    }
    // Begin at the the bottom (largest row number) of column indicated by the columnChosen. Find
    // the bottomost element in column columnChosen that does not yet contain a game piece.
    // Place the present gamepiece into that location in the connectNBoard array
    // Print a line of text to the screen that says either
    // Red has moved
    // Black has moved
    else
    {
        int notFound = 1;
        // if we get here then we know there is at least one empty element
        // so the while loop will eventually terminate
        while (notFound)
        {
            for (int i = 0; i < numRowsInBoard; i++)
            {
                if (connectNBoard[i][columnChosen] == 0)
                {
                    firstEmptyRow = i;
                    notFound = 0;
                }
            }
        }
        // Place the present gamepiece into that location in the connectNBoard array
        // Print a line of text to the screen that says either
        // Red has moved     playerID = 1 -> R   //same values as in array
        // Black has moved   playerID = 0 -> B
        if (playerID == 1)
        {
            // Red
            connectNBoard[firstEmptyRow][columnChosen] = 1;
            printf("Red has moved\n");
        }
        else
        {
            // Black
            connectNBoard[firstEmptyRow][columnChosen] = 2;
            printf("Black has moved\n");
        }
    }
    return 1;
}

int CheckWinner(int **connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID)
{
    int player;
    if (playerID == 0)
    {
        player = 1;
    }
    else
    {
        player = 2;
    }
    // If the move is not legal (the column chosen is not part of the board)
    if (columnChosen < 0 || numRowsInBoard < columnChosen)
    {
        // print a line of text to the screen that says
        printf("ERROR: invalid column chosen, cannot check for winner");
        // if this error is generated the function will return false
        return 0;
    }
    // Be sure to catch the error that would generate the following error message
    // ERROR: cannot print the board because the pointer to the board is NULL
    else if (connectNBoard == NULL)
    {
        printf("ERROR: cannot print the board because the pointer to the board is NULL");
        // if this error is generated the function will return false
        return 0;
    }
    // Begin in the column indicated by the columnChosen (the place the player is trying to put
    // his piece this turn)
    int beforecount = 0;
    int afterCount = 0;
    // Determine which row the newest gamepiece has been placed in (in columnChosen)
    int rows = 0;
    // Start at the top row and check if equal to playerID
    // If not equal then add 1 to row and continue until equal to playerID
    // We will eventually reach equal to playerID since placed a piece in columnChosen
    while (rows < numRowsInBoard && connectNBoard[rows][columnChosen] != player)
    {
        rows++;
    }

    // For player playerID
    // HORIZONTAL CHECK
    // Count the length of the row of this player's pieces along the row to the right of the
    // columnChosen(afterCount), and the length of the row of this player's pieces to the
    // left of the columnChosen (beforecount)
    // If the sum afterCount + beforecount + 1 >= numConnect then the player has won

    // for afterCount
    // starting to the right of columnChosen
    // we know that columnChosen has playerID in it already
    int col = columnChosen + 1;
    while (col < numRowsInBoard && connectNBoard[rows][col] == player)
    {
        afterCount++;
        col++;
    }
    // for beforecount
    // starting to the left of columnChosen
    // we know that columnChosen has playerID in it already
    col = columnChosen - 1;
    while (col >= 0 && connectNBoard[rows][col] == player)
    {
        beforecount++;
        col--;
    }
    if (afterCount + beforecount + 1 >= numConnect)
    {
        //printf("%d", player);
        //printf("test: won horizontal\n");
        return 1; // player has won horizontally yay
    }
    // VERTICAL CHECK
    // Count the length of the column of this player's pieces down the column
    // columnChosen(afterCount) below the newest piece, and the length of the row of
    // this player's pieces above the newest piece in the column columnChosen (beforecount)
    // If the sum afterCount + beforecount + 1 >= numConnect then the player has won
    // reseting to 0 for checking veritcally
    beforecount = 0;
    afterCount = 0;
    // starting below the row the newest gamepiece was placed
    // for afterCount
    int rowbelow = rows + 1;
    while (rowbelow < numRowsInBoard && connectNBoard[rowbelow][columnChosen] == player)
    {
        afterCount++;
        rowbelow++;
    }
    // starting above the row the newest gamepiece was placed
    // for beforecount
    rowbelow = rows - 1;
    while (rowbelow >= 0 && connectNBoard[rowbelow][columnChosen] == player)
    {
        beforecount++;
        rowbelow--;
    }
    if (afterCount + beforecount + 1 >= numConnect)
    {
        return 1; // player won yay
    }
    // DIAGONAL CHECK
    // Consider the length of the row of this player's pieces diagonally from UPPER LEFT to
    // LOWER RIGHT. Count the length of the diagonal row of this player's pieces to the right
    // of columnChosen (afterCount), and the length of the diagonal row of this player's
    // pieces to the left of the columnChosen (beforecount)
    // If the sum afterCount + beforecount + 1 >= numConnect then the player has won
    // resetting count for diagonally
    beforecount = 0;
    afterCount = 0;
    // afterCount
    int dialga = columnChosen + 1; // LOWER RIGHT is horizontal increase
    rowbelow = rows - 1;           // LOWER RIGHT is vertical decrease
    while (dialga < numRowsInBoard && rowbelow >= 0 && connectNBoard[rowbelow][dialga] == player)
    {
        afterCount++;
        dialga++;
        rowbelow--;
    }
    // beforecount
    dialga = columnChosen - 1; // UPPER LEFT is horizontal decrease
    rowbelow = rows + 1;       // UPPER LEFT is vertical increase
    while (dialga >= 0 && rowbelow < numRowsInBoard && connectNBoard[rowbelow][dialga] == player)
    {
        beforecount++;
        dialga--;
        rowbelow++;
    }
    if (afterCount + beforecount + 1 >= numConnect)
    {
        return 1;   // player won yay
    }
    //Consider the length of the row of this player's pieces diagonally from lower left to
    //upper right. Count the length of the diagonal row of this player's pieces to the right
    //of columnChosen (afterCount), and the length of the diagonal row of this player's
    //pieces to the left of the columnChosen (beforeCount)
    //If the sum afterCount + beforeCount + 1 >= numConnect then the player has won
    beforecount = 0;
    afterCount = 0;
    // afterCount
    dialga = columnChosen + 1;  //UPPER RIGHT is horizontal increase
    rowbelow = rows + 1;        //UPPER RIGHT is vertical increase
    while (dialga < numRowsInBoard && rowbelow < numRowsInBoard && connectNBoard[rowbelow][dialga] == player) {
        afterCount++;
        dialga++;
        rowbelow++;
    }
    // beforeCount
    dialga = columnChosen - 1;      //LOWER LEFT is horizontal decrease
    rowbelow = columnChosen -1;     //LOWER LEFT is vertical decrease
    while (dialga >=0 && rowbelow >= 0 && connectNBoard[rowbelow][dialga] == player) {
        beforecount = 0;
        dialga--;
        rowbelow--;
    }
    if (afterCount + beforecount + 1 >= numConnect)
    {
        return 1;   // player won yay
    }
    // player has not won in any direction, return false
    // printf("test: no winner\n");
    return 0;
}

// assume the column chosen is part of the board
// if there are no empty elements, return true
int noEmptyElements(int **connectNBoard, int numRowsInBoard, int columnChosen)
{
    int isEmpty = 0;
    for (int i = 0; i < numRowsInBoard; i++)
    {
        if (connectNBoard[i][columnChosen] == 0)
        {
            isEmpty++;
        }
    }
    if (isEmpty == 0)
    { // there are no empty elements return 1
        return 1;
    }
    return 0; // there are empty elements return 0
}

