#include "kkr.hpp"
#include "renderer2d.hpp"

#include <glad/glad.h>

namespace Kokiri {
    namespace Graphics {
        namespace OpenGL {
            Renderer2D::Renderer2D(Core::Window &&window) {
                Renderer2D(std::move(window), Version::OPENGL_4_6);
            }

            Renderer2D::Renderer2D(Core::Window &&window, Version version, Type type) {
                int major = 2;
                int minor = 0;

                if (version == Version::OPENGL_3_1) {
                    major = 3;
                    minor = 1;
                } else if (version == Version::OPENGL_3_2) {
                    major = 3;
                    minor = 2;
                } else if (version == Version::OPENGL_4_0) {
                    major = 4;
                } else if (version == Version::OPENGL_4_5) {
                    major = 4;
                    minor = 5;
                } else if (version == Version::OPENGL_4_6) {
                    major = 4;
                    minor = 6;
                }

                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

                if (type == Type::COMPABILITY) {
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
                } else {
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                }

                auto context = SDL_GL_CreateContext(window.get_window());

                m_context = Core::make_shared<SDL_GLContext>(context);

                if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
                    // TODO: Improve error message
                    Core::Log::error("failed to create OpenGL context");
                    exit(1);
                }
            }

            Renderer2D::~Renderer2D() {
                Core::Log::info("destroying OpenGL Context");
                SDL_GL_DeleteContext(m_context.get());
            }


            void Renderer2D::draw() {

            }

            void Renderer2D::information() {
                Core::Log::info("Plataform: ", SDL_GetPlatform());
                Core::Log::info("OpenGL version: ", glGetString(GL_VERSION));
                Core::Log::info("OpenGL vendor: ", glGetString(GL_VENDOR));
                Core::Log::info("OpenGL renderer: ", glGetString(GL_RENDERER));
            #if !__WIN32__
                Core::Log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
            #endif // not adding more headers just to have this working, for now.
            }
        }
    }
}