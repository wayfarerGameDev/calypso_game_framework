/// <summary>
/// App module that uses SDL as a base
/// </summary>

#pragma once

// Dependancies
#include <stdio.h> // snprintf
#include <dependencies/SDL2/SDL.h>

// State
#define CALYPSO_FRAMEWORK_SDL2_APP_STATE_NULL                 0b00000000
#define CALYPSO_FRAMEWORK_SDL2_APP_STATE_INIT                 0b00000001
#define CALYPSO_FRAMEWORK_SDL2_APP_STATE_RUNNING              0b00000010
#define CALYPSO_FRAMEWORK_SDL2_APP_STATE_SHUTDOWN             0b00000011
#define CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR                0b00000100
unsigned int _calypso_framework_sdl2_app_state                = CALYPSO_FRAMEWORK_SDL2_APP_STATE_NULL;

// Window
#define CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_WIDTH_DEFAULT       1280 
#define CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_HEIGHT_DEFAULT      720
SDL_Window* _calypso_framework_sdl2_app_window_ptr;

// Open GL Context
SDL_GLContext _calypso_framework_sdl2_app_gl_context_ptr;

// Events
typedef void (*calypso_framework_sdl2_app_event_t)(void);
calypso_framework_sdl2_app_event_t _calypso_framework_sdl2_app_event_on_startup;
calypso_framework_sdl2_app_event_t _calypso_framework_sdl2_app_event_on_shutdown;
calypso_framework_sdl2_app_event_t _calypso_framework_sdl2_app_event_on_update;
calypso_framework_sdl2_app_event_t _calypso_framework_sdl2_app_event_on_resize;

// Custom Toolbar
int _calypso_framework_sdl2_app_custom_toolbar_height         = -1;


/*------------------------------------------------------------------------------
Calypso Framework SDL App : Title
------------------------------------------------------------------------------*/

void calypso_framework_sdl2_app_set_title(const char* title)
{
    SDL_SetWindowTitle(_calypso_framework_sdl2_app_window_ptr,title);
}

void calypso_framework_sdl2_app_set_icon_by_surface(SDL_Surface* surface_ptr)
{
    // Return If Icon Is NULL
    if (surface_ptr == NULL)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app","Failed to set window icon",3);
        #endif
    }

    SDL_SetWindowIcon(_calypso_framework_sdl2_app_window_ptr,surface_ptr);
}

void calypso_framework_sdl2_app_set_icon_bmp(char* file_path)
{
    // Load BMP | Create texture | Free
    SDL_Surface* surface = SDL_LoadBMP(file_path);
    
    // Return If Icon Is NULL
    if (surface == NULL)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app","Failed to set window icon",3);
        #endif
    }

    SDL_SetWindowIcon(_calypso_framework_sdl2_app_window_ptr,surface);
}

/*------------------------------------------------------------------------------
Calypso Framework SDL App : Window
------------------------------------------------------------------------------*/

SDL_Window* calypso_framework_sdl2_app_get_window_sdl2_window_ptr(void)
{
    return _calypso_framework_sdl2_app_window_ptr;
}

int calypso_framework_sdl2_app_get_window_width(void)
{
     // Not Valid Group
    if (!_calypso_framework_sdl2_app_window_ptr)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app","Failed to get window width (window is NULL)",3);
        #endif
        return 0;
    }

    int w = 0;
    int h = 0;
    SDL_GetWindowSize(_calypso_framework_sdl2_app_window_ptr,&w,&h);
    return w;
}

int calypso_framework_sdl2_app_get_window_height(void)
{
     // Not Valid Group
    if (!_calypso_framework_sdl2_app_window_ptr)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app","Failed to get window height (window is NULL)",3);
        #endif
        return 0;
    }

    int w = 0;
    int h = 0;
    SDL_GetWindowSize(_calypso_framework_sdl2_app_window_ptr,&w,&h);
    return h;
}

void calypso_framework_sdl2_app_set_window_resizable(const int is_resizable)
{
    SDL_SetWindowResizable(_calypso_framework_sdl2_app_window_ptr,is_resizable);
}

void calypso_framework_sdl2_app_maxamize_window()
{
    SDL_MaximizeWindow(_calypso_framework_sdl2_app_window_ptr);
}

void calypso_framework_sdl2_app_restore_window()
{
    SDL_RestoreWindow(_calypso_framework_sdl2_app_window_ptr);
}

void calypso_framework_sdl2_app_maxamize_restore_toggle_window()
{
    // Check if the window is maximized
    if (SDL_GetWindowFlags(_calypso_framework_sdl2_app_window_ptr) & SDL_WINDOW_MAXIMIZED) 
        SDL_RestoreWindow(_calypso_framework_sdl2_app_window_ptr);
    else 
        SDL_MaximizeWindow(_calypso_framework_sdl2_app_window_ptr);
}

void calypso_framework_sdl2_app_minimize_window()
{
    SDL_MinimizeWindow(_calypso_framework_sdl2_app_window_ptr);
}

/*------------------------------------------------------------------------------
Calypso Framework SDL App : Window Custom Toolbar
------------------------------------------------------------------------------*/

SDL_HitTestResult calypso_framework_sdl2_app_window_custom_toolbar_callback(SDL_Window* window, const SDL_Point* area, void* data) 
{
    // Get Mouse Position
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Get window Rect
    SDL_Rect window_rect;
    SDL_GetWindowPosition(window, &window_rect.x, &window_rect.y);
    SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);
    
    // Check If Mouse Is In Custom Toolbar Area
    if (mouse_x >= window_rect.x && mouse_x < window_rect.x + window_rect.w && mouse_y >= window_rect.y && mouse_y < window_rect.y + _calypso_framework_sdl2_app_custom_toolbar_height)
        return SDL_HITTEST_DRAGGABLE;

    // Return Normal
    return SDL_HITTEST_NORMAL;
}

void calypso_framework_sdl2_app_enable_window_custom_toolbar(const int is_enabled, const int toolbar_height)
{
    // Disable
    if (is_enabled <= 0)
    {
        SDL_SetWindowHitTest(_calypso_framework_sdl2_app_window_ptr, NULL, NULL);
        SDL_SetWindowBordered(_calypso_framework_sdl2_app_window_ptr,SDL_TRUE);
        _calypso_framework_sdl2_app_custom_toolbar_height = -1;
    }

    // Enable
    else        
    {
        // Register hit test callback function
        SDL_SetWindowBordered(_calypso_framework_sdl2_app_window_ptr,SDL_FALSE);
        SDL_SetWindowHitTest(_calypso_framework_sdl2_app_window_ptr, calypso_framework_sdl2_app_window_custom_toolbar_callback, NULL);
        _calypso_framework_sdl2_app_custom_toolbar_height = toolbar_height;
    }
}

/*------------------------------------------------------------------------------
Calypso Framework SDL App : Swap
------------------------------------------------------------------------------*/

void calypso_framework_sdl2_app_set_swap_interval(const int interval)
{
    SDL_GL_SetSwapInterval(interval);
}

void calypso_framework_sdl2_app_set_swap_buffers()
{
    SDL_GL_SwapWindow(_calypso_framework_sdl2_app_window_ptr);
}

/*------------------------------------------------------------------------------
Calypso Framework SDL App : Main
------------------------------------------------------------------------------*/

void calypso_framework_sdl2_app_init()
{
    // Only Init Once
    if (_calypso_framework_sdl2_app_state != CALYPSO_FRAMEWORK_SDL2_APP_STATE_NULL)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init","Already initalized",2);
        #endif
        return;
    }

    // Set State To Init By Default
    _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_INIT;

    // Init SDL (Everything)
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init","Failed to initalize",3);
        CALYPSO_FRAMEWORK_LOG_MESSAGE("\t",SDL_GetError(),3);
        #endif
        _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR;
        return;
    }

    // Create SDL Window
    _calypso_framework_sdl2_app_window_ptr = SDL_CreateWindow("App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_WIDTH_DEFAULT, CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_HEIGHT_DEFAULT,SDL_WINDOW_OPENGL);
    if(!_calypso_framework_sdl2_app_window_ptr)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init","Failed to initalize",3);
        CALYPSO_FRAMEWORK_LOG_MESSAGE("\t",SDL_GetError(),3);
        #endif
        _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR;
        return;
    }
}

void calypso_framework_sdl2_app_init_with_opengl(const int major_version, const int minor_version, const int context_profile)
{
    // Setup OpenGL Attributes
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,major_version);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,minor_version);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,context_profile); //MAY NEED TO CHANGE HOW THIS IS DONE
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    }

    // Only Init Once
    if (_calypso_framework_sdl2_app_state != CALYPSO_FRAMEWORK_SDL2_APP_STATE_NULL)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init_with_opengl","Already initalized",2);
        #endif
        return;
    }

    // Set State To Init By Default
    _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_INIT;

    // Init SDL (Everything)
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init_with_opengl","Failed to initalize",3);
        CALYPSO_FRAMEWORK_LOG_MESSAGE("\t",SDL_GetError(),3);
        #endif
        _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR;
        return;
    }

    // Create SDL Window
    _calypso_framework_sdl2_app_window_ptr = SDL_CreateWindow("App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_WIDTH_DEFAULT, CALYPSO_FRAMEWORK_SDL2_APP_SCREEN_HEIGHT_DEFAULT,SDL_WINDOW_OPENGL);
    if(!_calypso_framework_sdl2_app_window_ptr)
    {
        #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
        CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init_with_opengl","Failed to initalize",3);
        CALYPSO_FRAMEWORK_LOG_MESSAGE("\t",SDL_GetError(),3);
        #endif
        _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR;
        return;
    }

    //Open GL Context
    {
        _calypso_framework_sdl2_app_gl_context_ptr = SDL_GL_CreateContext(_calypso_framework_sdl2_app_window_ptr);
        if (!_calypso_framework_sdl2_app_gl_context_ptr)
        {
            #ifdef CALYPSO_FRAMEWORK_LOG_MESSAGE_ENABLED
            CALYPSO_FRAMEWORK_LOG_MESSAGE("sdl_app->init_with_opengl","Failed to initalize",3);
            CALYPSO_FRAMEWORK_LOG_MESSAGE("\t",SDL_GetError(),3);
            #endif
            _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_ERROR;
            return;
        }
    }
}

void calypso_framework_sdl2_app_swap_buffer()
{
    SDL_GL_SwapWindow(_calypso_framework_sdl2_app_window_ptr);
}

void* calypso_framework_sdl2_app_get_open_gl_proc_address()
{
    return SDL_GL_GetProcAddress;
}

void calypso_framework_sdl2_app_shutdown(void)
{
     _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_SHUTDOWN;
}

void calypso_framework_sdl2_app_set_events(calypso_framework_sdl2_app_event_t event_on_startup, calypso_framework_sdl2_app_event_t event_on_shutdown, calypso_framework_sdl2_app_event_t event_on_update, calypso_framework_sdl2_app_event_t event_on_resize)
{
    _calypso_framework_sdl2_app_event_on_startup = event_on_startup;
    _calypso_framework_sdl2_app_event_on_shutdown = event_on_shutdown;
    _calypso_framework_sdl2_app_event_on_update = event_on_update;
    _calypso_framework_sdl2_app_event_on_resize = event_on_resize;
}

void calypso_framework_sdl2_app_run(void)
{
    // Only Run If State Is Initalized (Not Yet Running)
    if (_calypso_framework_sdl2_app_state != CALYPSO_FRAMEWORK_SDL2_APP_STATE_INIT)
        return;

    // On Event Startup
    if (_calypso_framework_sdl2_app_event_on_startup != NULL)
        _calypso_framework_sdl2_app_event_on_startup();

    // Set State To Running
    _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_RUNNING;

    // Run
    while(_calypso_framework_sdl2_app_state == CALYPSO_FRAMEWORK_SDL2_APP_STATE_RUNNING)
    {
        // On Event Update
        if (_calypso_framework_sdl2_app_event_on_update != NULL)
           _calypso_framework_sdl2_app_event_on_update();

        // Event Loop
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT: _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_SHUTDOWN; break;
                    case SDL_WINDOWEVENT: if (event.window.event == SDL_WINDOWEVENT_RESIZED) _calypso_framework_sdl2_app_event_on_resize(); break;
                    default: break;
                }
            }
        }
    }

    // On Event Shutdown
    if (_calypso_framework_sdl2_app_event_on_shutdown != NULL)
        _calypso_framework_sdl2_app_event_on_shutdown();

    // Quit
    {
        // SDL
        SDL_DestroyWindow(_calypso_framework_sdl2_app_window_ptr);
        SDL_Quit();

        // OpenGL
        if (_calypso_framework_sdl2_app_gl_context_ptr)
            SDL_GL_DeleteContext(_calypso_framework_sdl2_app_gl_context_ptr);
    }

    // No State
    _calypso_framework_sdl2_app_state = CALYPSO_FRAMEWORK_SDL2_APP_STATE_NULL;
}