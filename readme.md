The game must be playable by both by 2 people and vs. an AI. 

Your implementation must check for the winning condition, and display that on the right hand side the same way the current setup does for tic-tac-toe. 

The stopGame() method is expected to clean up and delete any memory allocated.

For the submission, create a new github based on the above code-base and provide a link to that along with a complete readme.md on how your implementation works.

IMPLEMENTATION

I viewed the process of implementing a functional Connect4 game in ImGUI as consisting of three main parts:

    - Display System (How does the game present this information?)
    - Win Checking System (How does the game check for terminal states?)
    - AI Evaluation System (How does the AI assign value to moves?)

Display

    - The first step I took was understanding and implementing the Grid class for intializing the board needed for Connect4.