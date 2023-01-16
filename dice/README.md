#Project 2
Work by Guilherme Serpa - 82078

- To compile the project run 'make' on the project's home folder, where the makefile is located.

- The OpenGL, glfw and glew development libraries need to be installed on the linux system. The makefile links them.

- After the project's compilation is complete, a new file will appear in the project's home directory called 'dice'. Run this executable to see the final result of the project.

- 'aux.h' is used for some of its auxiliary functions.

- The 'stb' folder are public domain libraries that are used to load the cubemap faces. The specific functions used are 'stbi_load' (to load the image) and 'stbi_image_free' to free the memory. The public repo can be found here: https://github.com/nothings/stb

- MGL libraries are not used, an attempt to write something equivalent from scratch was made.
    - Shaders can be found at the beginning of the file.
    - obj files are loaded in the 'load_obj_file' function.
    - Loading cubemaps/textures is done in the 'load_cube_tex' function.
    - Shader programs are linked and created in the main() function.
    - Buffer deletion and cleaning is done during program termination.
    - Shader deletion is done durin program termination, after buffer deletion.
    - Window creation is done at the beginning of the main() function.

- The keys used to interact with the program are:
    - Left arrow key to rotate the scene left (continuous pressing).
    - Right arrow key to rotate the scene right (continuous pressing).
    - Up arrow key to rotate the scene up (continuous pressing).
    - Down arrow key to rotate the scene down (continuous pressing).
    - Left CTRL to zoom out (continuous pressing).
    - Left SHIFT to zoom in (continuous pressing).
    - O key to automatically "o"rbit around the scene (press O again to pause and unpause).
    - R key to "r"eset the scene (not the skybox).
    - S key to take a screenshot of the scene (screenshot saved in project home dir).
    - 2 key to change to second skybox.
    - 3 key to change to third skybox.
    - 1 key to return to first skybox.

- In the project's home folder you can find:
    - The 'stb' folder and the 'glm' folder (external libraries used for the project).
    - 3 'skybox' folders with the skybox textures, made in Gimp, used and for switching them around.
    - The 'assets' folder containing the .obj files used and imported.
    - 'aux.h' for auxiliary functions.
    - 'main.cpp' the actual project.
    - 'makefile' for building the project.
    - The updated proposal as a pdf file.
    - 'README.md' is this file.
    - A zip file containing all of the above as well.