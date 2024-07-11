# Maze Project for Holberton Coding School

This is a maze game project created using SDL2 for the Holberton Coding School. The game renders a 3D maze where the player navigates to reach the finish line.

## Table of Contents
1. [Features](#features)
2. [Requirements](#requirements)
3. [Installation](#installation)
4. [Usage](#usage)
5. [File Structure](#file-structure)
6. [Authors](#authors)

## Features
- **3D Maze Rendering**: The game utilizes a 2D raycasting technique to render a 3D maze.
- **Player Movement**: The player can move forward, backward, and strafe left and right.
- **Mouse Look**: The player can look around using the mouse.
- **Textured Walls, Floor, and Sky**: The game includes textures for walls, floor, and sky.
- **Frame Counter and Coordinates**: The game displays the current frame count, average FPS, and player coordinates on the screen.
- **Maze Solvability Check**: The game ensures the maze is solvable from the starting position using BFS algorithm.

## Requirements
- SDL2
- SDL2_image
- SDL2_ttf

Ensure that you have the necessary libraries installed on your system. You can install them using your package manager or download them from the SDL website.

## Installation
1. **Clone the Repository**:
    ```bash
    git clone https://github.com/xov121/amazeng.git
    ```
2. **Navigate to the Project Directory**:
    ```bash
    cd maze
    ```
3. **Compile the Code**:
    ```bash
    gcc -o maze main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lm
    ```

## Usage
1. **Run the Game**:
    ```bash
    ./maze
    ```
2. **Controls**:
    - `W`: Move forward
    - `S`: Move backward
    - `A`: Strafe left
    - `D`: Strafe right
    - `Mouse`: Look around
    - `ESC`: Exit the game

## File Structure
- **maze.c**: The main source file containing the game logic.
- **images/**: Directory containing texture files for the floor, sky, and walls.
    - `blackfloor.jpg`: Floor texture.
    - `sky.jpeg`: Sky texture.
    - `brick.bmp`: Wall texture.
- **README.md**: This readme file.

## Authors
- **Javier Marrero** - [xov121](https://github.com/xov121)

This project was developed as part of the curriculum for Holberton Coding School. If you have any questions or suggestions, feel free to contact me through GitHub.
