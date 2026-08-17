# Some Basic Info
This is a project I made during my studies in Game Development. It is a simple 2D game engine / framework developed in C++.
You can use this to make fully functional 2D games.

The framework uses a similar pattern to how Unity works, in that it has Game Objects that you can add Components onto. You can also inherit from the BaseComponent class or any specialized classes to make your 
own custom versions of them, as can be seen in the Game folder. These can then also be attached to the Game Objects you make. If you're at all familiar with Unity it is very similar to how you would make a game with that 
Engine, minus the graphical user interface.

This project is fairly old at this point and I no longer have the documentation for it as that got lost on my school account which no longer exists. I might take the time at some point to go through it and try to document 
it properly again in this README file although that won't be likely to happen soon.

The Minigin folder holds the actual core engine. The Game folder holds a game that I made using the engine that is fully playable as a proof of functionality for the engine / framework.

The two build folders hold everything you need to play the example game I made using this framework.

## Controls for the game:
- Move: wasd
- Shoot: spacebar

## Side Note
The multithreading implementation is basically non-functional in this project, everything else does work though. This essentially means that preformance of games running on this engine could be better. 
Should still be more than fine enough for smaller 2D games though.
I'm unlikely to return to this project to try and fix the multithreading though as I have other projects I'm currently working on. Maybe someday I'll update this with a graphical user interface and fix the multithreading. 
For now though this project is permanetly on hold.
