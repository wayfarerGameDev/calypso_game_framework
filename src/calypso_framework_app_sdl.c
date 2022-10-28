/// <summary>
/// App module that uses SDL as a base
/// </summary>

#pragma once

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<conio.h>
#include <SDL2/SDL.h>

// State
#define CALYPSO_FRAMEWORK_APP_SDL_STATE_NULL        0
#define CALYPSO_FRAMEWORK_APP_SDL_STATE_INIT        1
#define CALYPSO_FRAMEWORK_APP_SDL_STATE_RUNNING     2
#define CALYPSO_FRAMEWORK_APP_SDL_STATE_EXIT        3
#define CALYPSO_FRAMEWORK_APP_SDL_STATE_ERROR       4
unsigned int _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_NULL;

// Window
#define CALYPSO_FRAMEWORK_APP_SDL_SCREEN_WIDTH_DEFAULT  1280 
#define CALYPSO_FRAMEWORK_APP_SDL_SCREEN_HEIGHT_DEFAULT 720
SDL_Window* _calypso_framework_app_sdl_window;

// Time
#define CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_UNCAPPED -1
#define CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_DEFAULT CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_UNCAPPED
float _calypso_framework_app_sdl_time_frame_rate_target = CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_DEFAULT;
float _calypso_framework_app_sdl_time_frame_rate = 0;
float _calypso_framework_app_sdl_time_frame_time_target = 0;
float _calypso_framework_app_sdl_time_delta_time = 0;

// Systems
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED                 0
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ENABLED                  1
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ONESHOT                  2
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_STARTUP        0
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_STARTUP         1
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_STARTUP              2
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_SHUTDOWN       3
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_SHUTDOWN        4
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_SHUTDOWN             5
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_UPDATE         6
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_UPDATE          7
#define CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE               8
typedef void (*calypso_framework_app_sdl_system)(void);
calypso_framework_app_sdl_system* _calypso_framework_app_sdl_systems;
int* _calypso_framework_app_sdl_systems_states;
int* _calypso_framework_app_sdl_systems_app_stages;
unsigned int _calypso_framework_app_sdl_system_count = 0;

/**
 * Gets app's sdl window
 * 
 * @return SDL_Window*
*/
SDL_Window* calypso_framework_app_sdl_get_sdl_window()
{
    return _calypso_framework_app_sdl_window;
}

/**
 * Sets window's icon
 * 
 * @param icon SDL_Surface*
 * @return void
*/
void calypso_framework_app_sdl_set_window_icon(SDL_Surface* icon)
{
    // Return If Icon Is NULL
    if (icon == NULL)
    {
        printf("\033[0;31m"); // Red
        printf("App Error: Failed To Set Window Icon\n");
        printf("\033[0;00m"); // White
        return;
    }

    SDL_SetWindowIcon(_calypso_framework_app_sdl_window,icon);
}

/**
 * Sets window's title
 * 
 * @param title const char*
 * @return void
*/
void calypso_framework_app_sdl_set_window_title(const char* title)
{
    SDL_SetWindowTitle(_calypso_framework_app_sdl_window,title);
}

/**
 * Sets window's ability to be resizable enabled
 * @param bIsResizable const bool
 * @return void
*/
void calypso_framework_app_sdl_set_window_resizable(const bool bIsResizable)
{
    SDL_SetWindowResizable(_calypso_framework_app_sdl_window,bIsResizable);
}

/**
 * Sets app's time frame rate to be uncapped
 * @return void
*/
void calypso_framework_app_sdl_set_time_frame_rate_target_uncapped()
{
    // Time
    _calypso_framework_app_sdl_time_frame_rate_target = CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_UNCAPPED;
    _calypso_framework_app_sdl_time_frame_time_target = 0;
}

/**
 * Sets app's time frame rate target
 * @param frame_rate const unsigned int
 * @return void
*/
void calypso_framework_app_sdl_set_time_frame_target_rate(const unsigned int frame_rate)
{
    _calypso_framework_app_sdl_time_frame_rate_target = frame_rate;
    _calypso_framework_app_sdl_time_frame_time_target = 1000 / frame_rate;
}

/**
 * Gets app's time frame rate
 * @return float
*/
float calypso_framework_app_sdl_get_time_frame_rate()
{
    return _calypso_framework_app_sdl_time_frame_rate;
}

/**
 * Gets app's time frame rate as ptr
 * @return float
*/
float* calypso_framework_app_sdl_get_time_frame_rate_ptr()
{
    return &_calypso_framework_app_sdl_time_frame_rate;
}

/**
 * Gets app's time delta time as pointer
 * @return float
*/
float calypso_framework_app_sdl_get_time_delta_time()
{
    return _calypso_framework_app_sdl_time_delta_time;
}

float* calypso_framework_app_sdl_get_time_delta_time_ptr()
{
    return &_calypso_framework_app_sdl_time_delta_time;
}

/**
 * Prints app's time data
 * @return void
*/
void calypso_framework_app_sdl_print_time()
{
    printf("\033[0;32m"); // Green
    printf("App (Time)\n");
    printf("\033[0;00m"); // White
    printf("frame rate : %i\n",(int)_calypso_framework_app_sdl_time_frame_rate);
    printf("delta time : %f\n",_calypso_framework_app_sdl_time_delta_time);
}

/**
 * Enable app system
 * @param system void function with no paramaters
*/
void calypso_framework_app_sdl_enable_system(calypso_framework_app_sdl_system system)
{
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems[i] == system)
        {
            _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ENABLED;
            return;
        }
}

/**
 * Disable app system
 * @param system void function with no paramaters
*/
void calypso_framework_app_sdl_disable_system(calypso_framework_app_sdl_system system)
{
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems[i] == system)
        {
            _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED;
            return;
        }
}

/**
 * Oneshot app system
 * @param system void function with no paramaters
*/
void calypso_framework_app_sdl_one_shot_system(calypso_framework_app_sdl_system system)
{
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems[i] == system)
        {
            _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ONESHOT;
            return;
        }
}

/**
 * Add app system
 * 
 * @param system void function with no paramaters
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_STARTUP || 0 : late startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_STARTUP || 1 : early startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_STARTUP || 2 : start
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_SHUTDOWN || 3 : late shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_SHUTDOWN || 4 : early shutdown
 * @param app_stage CALYPSO_FclsRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_SHUTDOWN || 5 : shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_UPDATE || 6 : early update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_UPDATE || 7 : late update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE || 8 : update
 * @return void
*/
void calypso_framework_app_sdl_add_system(calypso_framework_app_sdl_system system, int app_stage)
{
    // Not Valid Group
    if (app_stage < 0 || app_stage > 8)
    {
        printf("\033[0;31m"); // Red
        printf ("App Error: Failed to add system (invalid app_stage)\n app_stage is '%i",app_stage);
        printf("\033[0;00m"); // White
        return;
    }

    // Already Added
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
    {
        if (_calypso_framework_app_sdl_systems[i] == system)
        {
            printf("\033[0;31m"); // Red
            printf ("App Error: Failed to add system (system already added)");
            printf("\033[0;00m"); // White
            return;
        }
    }

    // Add System
    _calypso_framework_app_sdl_system_count++;
    _calypso_framework_app_sdl_systems = realloc(_calypso_framework_app_sdl_systems,_calypso_framework_app_sdl_system_count * sizeof(calypso_framework_app_sdl_system));
    _calypso_framework_app_sdl_systems_states = realloc(_calypso_framework_app_sdl_systems_states,_calypso_framework_app_sdl_system_count * sizeof(int));
    _calypso_framework_app_sdl_systems_app_stages = realloc(_calypso_framework_app_sdl_systems_app_stages,_calypso_framework_app_sdl_system_count * sizeof(int));
    _calypso_framework_app_sdl_systems[_calypso_framework_app_sdl_system_count - 1] = system;
    _calypso_framework_app_sdl_systems_states[_calypso_framework_app_sdl_system_count - 1] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ENABLED;
    _calypso_framework_app_sdl_systems_app_stages[_calypso_framework_app_sdl_system_count - 1] = app_stage;
    
}

/**
 * Add app system that is disabled
 * 
 * @param system void function with no paramaters
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_STARTUP || 0 : late startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_STARTUP || 1 : early startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_STARTUP || 2 : start
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_SHUTDOWN || 3 : late shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_SHUTDOWN || 4 : early shutdown
 * @param app_stage CALYPSO_FclsRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_SHUTDOWN || 5 : shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_UPDATE || 6 : early update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_UPDATE || 7 : late update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE || 8 : update
 * @return void
*/
void calypso_framework_app_sdl_add_system_disabled(calypso_framework_app_sdl_system system, int app_stage)
{
    calypso_framework_app_sdl_add_system(system,app_stage);
    calypso_framework_app_sdl_disable_system(system);
}

/**
 * Add app system that is to play once and than disable
 * 
 * @param system void function with no paramaters
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_STARTUP || 0 : late startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_STARTUP || 1 : early startup
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_STARTUP || 2 : start
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_SHUTDOWN || 3 : late shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_SHUTDOWN || 4 : early shutdown
 * @param app_stage CALYPSO_FclsRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_SHUTDOWN || 5 : shutdown
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_UPDATE || 6 : early update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_UPDATE || 7 : late update
 * @param app_stage CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE || 8 : update
 * @return void
*/
void calypso_framework_app_sdl_add_system_one_shot(calypso_framework_app_sdl_system system, int app_stage)
{
    calypso_framework_app_sdl_add_system(system,app_stage);
    calypso_framework_app_sdl_one_shot_system(system);
}

/**`
 * Initializes app
 * @return void
*/
void calypso_framework_app_sdl_init()
{
    // Only Init Once
    if (_calypso_framework_app_sdl_state != CALYPSO_FRAMEWORK_APP_SDL_STATE_NULL)
        return;

    // Set State To Init By Default
    _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_INIT;

    // Init SDL (Everything)
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("\033[0;31m"); // Red
        printf("App Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        printf("\033[0;00m"); // White
        _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_ERROR;
        return;
    }

    // Create SDL Window
    _calypso_framework_app_sdl_window = SDL_CreateWindow("App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CALYPSO_FRAMEWORK_APP_SDL_SCREEN_WIDTH_DEFAULT, CALYPSO_FRAMEWORK_APP_SDL_SCREEN_HEIGHT_DEFAULT, 0);
    if(!_calypso_framework_app_sdl_window)
    {
        printf("\033[0;31m"); // Red
        printf("App Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        printf("\033[0;00m"); // White
        _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_ERROR;
        return;
    }
}

/**
 * Runs app, will halt code below call until app is no longer running
 * @return void
*/
void calypso_framework_app_sdl_run()
{
    // Only Run If State Is Initalized (Not Yet Running)
    if (_calypso_framework_app_sdl_state != CALYPSO_FRAMEWORK_APP_SDL_STATE_INIT)
        return;

    // Run Early Startup App Stage Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_STARTUP)
                _calypso_framework_app_sdl_systems[i]();
            
    // Run Startup App Stage Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_STARTUP)
                _calypso_framework_app_sdl_systems[i]();

    // Run Late Startup App Stage Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_STARTUP)
                _calypso_framework_app_sdl_systems[i]();

    // Set State To Running
    _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_RUNNING;

    // Time Data To Do Calculations
    int time_time_last = 0;
    int time_time_delay = 0;
    int time_frame_count = 0;
    int time_frame_last = 0;

    // Run
    while(_calypso_framework_app_sdl_state == CALYPSO_FRAMEWORK_APP_SDL_STATE_RUNNING)
    {
        // Do Time (Capped Frame Rate)
        if (_calypso_framework_app_sdl_time_frame_rate_target != CALYPSO_FRAMEWORK_APP_SDL_TIME_FPS_UNCAPPED)
        {
            // Delay Our Timestep By Frame Rate Target
            time_time_delay =  _calypso_framework_app_sdl_time_frame_time_target - (SDL_GetTicks() - time_time_last);
                if (time_time_delay > 0 && time_time_delay < _calypso_framework_app_sdl_time_frame_time_target)
                {
                    time_frame_count -= time_time_delay;
                    time_frame_last += time_time_delay;
                    SDL_Delay(time_time_delay);
                }

            // Get Frame Rate Every 1 Seconds
            ++time_frame_count;
            if (SDL_GetTicks() - time_frame_last >= 1000.f)
            {
                _calypso_framework_app_sdl_time_frame_rate = time_frame_count;
                time_frame_count = 0;
                time_frame_last = SDL_GetTicks();
            }

             // Get Delta Time (As Seconds) | Set Time Last To Time Current
            _calypso_framework_app_sdl_time_delta_time = (SDL_GetTicks() - time_time_last) * 0.001f;;
            time_time_last = SDL_GetTicks();
        }

        // Do Time (Uncapped Frame Rate)
        else
        {
            // Get Frame Rate Every 1 Seconds
            ++time_frame_count;
            if (SDL_GetTicks() - time_frame_last >= 1000.f)
            {
                _calypso_framework_app_sdl_time_frame_rate = time_frame_count;
                time_frame_count = 0;
                time_frame_last = SDL_GetTicks();
            }

            // Get Delta Time (As Seconds) | Set Time Last To Time Current
            _calypso_framework_app_sdl_time_delta_time = (SDL_GetTicks() - time_time_last) * 0.001f;
            time_time_last = SDL_GetTicks();
        }

        // Event Loop
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT: _calypso_framework_app_sdl_state = CALYPSO_FRAMEWORK_APP_SDL_STATE_EXIT; break;
                    default: break;
                }
            }
        }

        // Run Early Update App Stage Systems
        for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
            if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
                if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE)
                {
                    if (_calypso_framework_app_sdl_systems_states[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ONESHOT)
                        _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED;
                    _calypso_framework_app_sdl_systems[i]();
                }

        // Run Update App Stage Systems
        for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
            if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
                if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE)
                {
                    if (_calypso_framework_app_sdl_systems_states[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ONESHOT)
                        _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED;
                    _calypso_framework_app_sdl_systems[i]();
                }

        // Run Late Update App Stage Systems
        for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
            if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
                if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_UPDATE)
                {
                    if (_calypso_framework_app_sdl_systems_states[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_ONESHOT)
                        _calypso_framework_app_sdl_systems_states[i] = CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED;
                    _calypso_framework_app_sdl_systems[i]();
                }
    }

    // Run Early Shutdown App Stage Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_EARLY_SHUTDOWN)
                _calypso_framework_app_sdl_systems[i]();
            
    // Run Shutdown App Stage Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_SHUTDOWN)
                _calypso_framework_app_sdl_systems[i]();

    // Run Late Shutdown App Stage  Systems
    for (int i = 0; i < _calypso_framework_app_sdl_system_count; i++)
        if (_calypso_framework_app_sdl_systems_states[i] != CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_STATE_DISABLED)
            if (_calypso_framework_app_sdl_systems_app_stages[i] == CALYPSO_FRAMEWORK_APP_SDL_SYSTEM_APP_STAGE_LATE_SHUTDOWN)
                _calypso_framework_app_sdl_systems[i]();

    // Clear Systems
    free(_calypso_framework_app_sdl_systems);
    free(_calypso_framework_app_sdl_systems_app_stages);
    _calypso_framework_app_sdl_system_count = 0;

    // Quit
    SDL_DestroyWindow(_calypso_framework_app_sdl_window);
    SDL_Quit();
}