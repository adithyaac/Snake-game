# Snake Game

In this project, I made a playable snake game. If you're not familiar with snake, [you can try out a demo at this link](https://www.xarg.org/project/html5-snake/). Please follow directions below if you want to make the game yourself:

1. Log in to GitHub in VS Code by clicking the user icon in the bottom left corner (above gearbox). In the VSCode terminal, clone the repo to your local machine using the repo URL obtained while accepting the assignment.  
```sh
     git clone <your-repo-url>
```
     
2. Attempt the tasks mentioned in [https://cs61c.org/fa23/projects/proj1/#conceptual-overview](https://cs61c.org/fa23/projects/proj1/#conceptual-overview) and make changes to only `src/snake.c`, `src/state.c`, and `src/custom_tests.c`.
   
3. Run the following commands locally to see how your code is doing.
```sh
   make run-unit-tests 
   make run-unit-tests-<1 to 6 except 2>  
   make run-integration-tests
```

4. commit your changes to the GitHub repo by running
  ```sh
   git add -A
   git commit -m "<commit message>"
   git push origin main
  ```
    
5. For seeing the logs/cmd line output of auto-grading:   
   Go to your repo URL > open the actions tab > click latest workflow run > autograding

6. For playing the game, run the following commands
```sh
   make interactive-snake 
   ./interactive-snake.
```
   Use the wasd keys to control your snake!

7. To speed up or slow down the game,  run 
```sh
   make interactive-snake 
   ./interactive-snake.
```
   (replacing 0.5 with the number of seconds between time steps). During the game, you can also press ] to move faster and [ to move slower.
   
