/**
 * MIT License
 *
 * Copyright (c) 2016 Rafael C. Nunes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE  SOFTWARE.
 */

#include <iostream>
#include <cstdint>

#include <SDL2/SDL.h>

#include "graphics/window.hpp"
#include "graphics/renderer.hpp"
#include "graphics/primitives.hpp"
#include "utils/loaders/object_loader.hpp"
#include "utils/debug/debug.hpp"

/**
 * @brief Prints the usage of the renderer.
 */
void usage();

int main(int argc, char *argv[]) {

    bool running = true;
    bool debug = false;
    SDL_Event event;

    Window window(800, 600, "Kokiri Framework",
                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    ObjectLoader *loader = nullptr;

    SDL_GLContext gl = SDL_GL_CreateContext(window.get_window());
    // Send the gl context to the renderer
    Renderer opengl_renderer = Renderer(gl);

    uint32_t frame = 0;
    uint32_t fps = 60;

    if (argc > 1) {
        const std::string object_path = argv[1];
        auto const dot_pos = object_path.find_last_of('.');

        if (object_path.substr(dot_pos+1) == "obj") {
            //loader = new ObjectLoader(object_path);

        } else {
            usage();
            exit(0);
        }
    }

    // TODO: Render the loaded object onto the screen.

    // if it isn't rendering an .obj file there's an enum to switch the rendered
    // polyhedron.
    enum Polyhedron {
        cube = 0,
        sphere = 2,
        tetrahedron = 4
    };

    Polyhedron render_polyhedron = Polyhedron::cube;

    while (running) {
        frame = SDL_GetTicks();

        SDL_PollEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            /**
             * This is synchronous state.
             */
            switch(event.key.keysym.sym) {
            case SDLK_UP:
                opengl_renderer.rotate(2, -1, 0, 0);
                break;
            case SDLK_DOWN:
                opengl_renderer.rotate(2, 1, 0, 0);
                break;
            case SDLK_LEFT:
                opengl_renderer.rotate(2, 0, -1, 0);
                break;
            case SDLK_RIGHT:
                opengl_renderer.rotate(2, 0, 1, 0);
                break;
            case SDLK_ESCAPE:
                running = false;
                break;
            case SDLK_d:
                debug = !debug;
                break;
            case SDLK_1:
                render_polyhedron = Polyhedron::cube;
                break;
            case SDLK_2:
                render_polyhedron = Polyhedron::sphere;
                break;
            case SDLK_3:
                render_polyhedron = Polyhedron::tetrahedron;
                break;
            }
        default:
#ifdef DEBUG
            std::cout << "Event: " << event.type << std::endl;
#endif // DEBUG
            break;
        }

        opengl_renderer.render_view();

        switch (render_polyhedron) {
        case cube:
            Primitives::cube();
            break;
        case sphere:
            Primitives::sphere();
            break;
        case tetrahedron:
            Primitives::tetrahedron();
            break;
        }

        SDL_GL_SwapWindow(window.get_window());

        double delay = 1000/fps-(SDL_GetTicks()-frame);

        if (1000/fps > (SDL_GetTicks() - frame)) {
            SDL_Delay(delay);
        }

#ifdef DEBUG
            Debug::log("Delay: ", delay);
#endif // DEBUG
    }

    Debug::log("Destroying context/renderer/window");

    SDL_GL_DeleteContext(gl);

    Debug::log("Exiting SDL2\n");

    SDL_Quit();

    // Cleaning everything else.
    delete loader;

    return 0;
}

void usage() {
    std::cerr << ERROR_COLOR
              << "This program should be run with one of the current ways:\n"
              << "\n";
#ifdef __WIN32__
    std::cerr << "\t1) renderer.exe <path_to_object.obj>\n"
              << "\t2) renderer.exe";
#else
    std::cerr << "\t1) renderer.out <path_to_object.obj>\n"
              << "\t2) renderer.out";
#endif
    std::cerr << RESET_COLOR << std::endl;
}
