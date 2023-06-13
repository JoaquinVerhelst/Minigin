# Minigin

## Design Choices


- It has a component system.
- The gameloop processes Input first then update, fixed update and lastly render.
- There is a Time singleton for deltaSeconds.
- A Sound Service locator using SDL2_Mixer with one thread.
- The levels are all loaded in using a JSON file. The NlohmannJson is used for this. 
- Most objects resources are also loaded through the JSON manager.
- Other libraries used are: glm, SDL2, SDL2_Image, SDL2_ttf.
- The health and score for the players uses the observer pattern.
- There is a World singelton, that takes care of the placement of objects for each level, collisions and handles the different gamemodes.
- There is input for both players through controllers and/or a keyboard.




