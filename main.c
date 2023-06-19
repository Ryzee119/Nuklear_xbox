/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
//#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

#ifndef NXDK

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_SDL_GL2_IMPLEMENTATION
#include "Nuklear/nuklear.h"
#include "Nuklear/demo/sdl_opengl2/nuklear_sdl_gl2.h"

#else
#define NK_SDL_GL2_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES
#include <hal/video.h>
#include <SDL.h>
#include <pbgl.h>
#include "GL/gl.h"
#include <GL/glext.h>
#include "Nuklear/nuklear.h"
//#include "nuklear_pbgl.h"
#undef NK_API
#define NK_API __attribute__((weak))
void glPushAttrib(GLbitfield mask) {return;}
void glPopAttrib(void) {return;}
#include "nuklear_pbgl.h"
#endif

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the defines */
/*#define INCLUDE_ALL */
/*#define INCLUDE_STYLE */
/*#define INCLUDE_CALCULATOR */
/*#define INCLUDE_CANVAS */
/*#define INCLUDE_OVERVIEW */
/*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
#define INCLUDE_STYLE
#define INCLUDE_CALCULATOR
#define INCLUDE_CANVAS
#define INCLUDE_OVERVIEW
#define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
#include "Nuklear//demo/common/style.c"
#endif
#ifdef INCLUDE_CALCULATOR
#include "Nuklear/demo/common/calculator.c"
#endif
#ifdef INCLUDE_CANVAS
#include "Nuklear/demo/common/canvas.c"
#endif
#ifdef INCLUDE_OVERVIEW
#include "Nuklear/demo/common/overview.c"
#endif
#ifdef INCLUDE_NODE_EDITOR
#include "Nuklear/demo/common/node_editor.c"
#endif

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
int main(int argc, char *argv[])
{
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;

    NK_UNUSED(argc);
    NK_UNUSED(argv);

#ifndef NXDK
    SDL_Window *win;
    SDL_GLContext glContext;
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    win = SDL_CreateWindow("Demo",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);
    ctx = nk_sdl_init(win);
#else
    SDL_Window *win = NULL;
    win_width = WINDOW_WIDTH;
    win_height = WINDOW_HEIGHT;
    XVideoSetMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, REFRESH_DEFAULT);
    const int err = pbgl_init(GL_TRUE);
    pbgl_set_swap_interval(1);
    ctx = nk_sdl_init(WINDOW_WIDTH, WINDOW_HEIGHT);
#endif
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        //struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../Nuklear/extra_font/Roboto-Regular.ttf", 16, 0);
        nk_sdl_font_stash_end();
        //nk_style_load_all_cursors(ctx, atlas->cursors);
        //nk_style_set_font(ctx, &roboto->handle);
    }
#ifdef INCLUDE_STYLE
/* ease regression testing during Nuklear release process; not needed for anything else */
#ifdef STYLE_WHITE
    set_style(ctx, THEME_WHITE);
#elif defined(STYLE_RED)
    set_style(ctx, THEME_RED);
#elif defined(STYLE_BLUE)
    set_style(ctx, THEME_BLUE);
#elif defined(STYLE_DARK)
    set_style(ctx, THEME_DARK);
#endif
#endif
    bg.r = 1.00f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (running)
    {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        /* GUI */
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250), 0))
        {
            enum
            {
                EASY,
                HARD
            };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY))
                op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD))
                op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400)))
            {
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

/* -------------- EXAMPLES ---------------- */
#ifdef INCLUDE_CALCULATOR
        calculator(ctx);
#endif
#ifdef INCLUDE_CANVAS
        canvas(ctx);
#endif
#ifdef INCLUDE_OVERVIEW
        overview(ctx);
#endif
#ifdef INCLUDE_NODE_EDITOR
        node_editor(ctx);
#endif
        /* ----------------------------------------- */

#ifndef NXDK
        /* Draw */
        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
        nk_sdl_render(NK_ANTI_ALIASING_OFF);
        SDL_GL_SwapWindow(win);
#else
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_sdl_render(NK_ANTI_ALIASING_OFF);
        pbgl_swap_buffers();

#endif
    }

cleanup:
    //nk_sdl_shutdown();
    //SDL_GL_DeleteContext(glContext);
    //SDL_DestroyWindow(win);
    //SDL_Quit();
    return 0;
}
