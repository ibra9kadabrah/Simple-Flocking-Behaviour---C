# Simple-Flocking-Behaviour---C
A particle simulation built with raylib that shows local flocking behaviour. Particles respond to mouse movement and create dynamic swirling patterns when clicked

## Features
- Cool particle movement and simple physics
- Mouse-following
- Interactive vortex effect with left mouse click
- Smooth particle transitions and respawnin

## Dependencies
This project requires raylib. The current implementation expects raylib to be in a specific directory structure, check main.c

to compile, gcc -o main main.c -I../raylib/src -L../raylib/build/raylib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

