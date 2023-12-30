# box2d-raylib
This project shows how to integrate Box2D with Raylib. Raylib provides the windowing and drawing while Box2D provides rigid body physics.
- https://www.raylib.com/
- https://box2d.org/

I use CMake and FetchContent to bring together box2d and raylib with no submodules, packages, or file copying. No library configuration is needed.

The demo shows how to setup a simulation coordinate system and transform the results to the screen. I also left some notes showing how I verified the transformation.

Feel free to use this project as a starting point for making a game using raylib and Box2D.
