# GraphColoring
GraphColoring is based on [graph coloring](https://en.wikipedia.org/wiki/Graph_coloring), which is an important part of [graph theory](https://en.wikipedia.org/wiki/Graph_theory).

GraphColoring has a [website](http://pommicket.com/graph-coloring) with a guide to the game.

## Running GraphColoring
You can either build GraphColoring from source or just run the executable which you can find in the "Releases" section on GitHub, or on [itch.io](https://pommicket.itch.io/graph-coloring).

If you want to delete all of your progress, just delete the saves folder.

## Building on GNU/Linux
If you're using a Debian/Ubuntu-based distribution, you can install the dependencies using
```bash
sudo apt-get install cmake libfreetype6-dev libgtk-3-dev libcairo2-dev
```
To build it, just run
```bash
cmake .
make
```
You can add the `-j4` flag to `make` to make it compile faster.

## Building for Windows
Just run `./build-windows.sh` to build for Windows.

## Writing levels
Levels are stored as XML configuration files in the `assets/levels` folder. `LEVELS.md` contains the full documentation on writing levels.

## Contact
If you would like to report an issue, please do it through GitHub. You can email me at pommicket@gmail.com.
