# Minigin

## Design Choices


- The game utilizes a component system to manage its entities.
- During the gameloop, the Input is processed first, followed by the update, fixed update, and finally the render.
- The Time singleton is employed to keep track of deltaSeconds, which measures the time between frames.
- SDL2_Mixer is utilized as a Sound Service locator, operating with a single thread.
- JSON files are employed to load all the levels, using the NlohmannJson library for this purpose.
- The JSON manager is also responsible for loading most of the resources for the game's objects.
- Additional libraries used in the game include glm, SDL2, SDL2_Image, and SDL2_ttf.
- The observer pattern is employed for managing the health and score of the players.
- The World singleton is responsible for various tasks, such as object placement for each level, collision handling, and managing different     gamemodes.
- Input from players can be received through controllers and/or a keyboard, providing a versatile input system.




