/// <summary>
/// Pixel Renderer module that uses Glad (OpenGL Driver) as a base
/// </summary>

#pragma once

#include <dependencies/glad/glad.h>

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Data
------------------------------------------------------------------------------*/

// Logging Callback
typedef void (*calypso_framework_renderer_pixel_opengl_es_log_callback_t)(const char* log_msg, const unsigned char  log_type);
calypso_framework_renderer_pixel_opengl_es_log_callback_t _calypso_framework_renderer_pixel_opengl_es_log_callback;

// State
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_NULL                         0b00000000
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT                         0b00000001
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_ERROR                        0b00000010
unsigned int _calypso_framework_renderer_pixel_opengl_es_state =                      CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_NULL;

// Open GL Version
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_MAJOR_VERSION                      3
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_MINOR_VERSION                      3
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_CONTEXT_PROFILE                    3

// OpenGL Object
unsigned int _calypso_framework_renderer_pixel_opengl_es_vao;
unsigned int _calypso_framework_renderer_pixel_opengl_es_vbo;
unsigned int _calypso_framework_renderer_pixel_opengl_es_ibo;

// Shader Program
unsigned int _calypso_framework_renderer_pixel_opengl_es_shader_program;

// Texture
unsigned int _calypso_framework_renderer_pixel_opengl_es_texture;

// Pixel Buffer
#define CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT        4 // RGBA
typedef struct calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t
{
    GLubyte* buffer;
    unsigned int buffer_width;
    unsigned int buffer_height;
    unsigned int buffer_length;
} calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t;

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Log Callback
------------------------------------------------------------------------------*/

void calypso_framework_renderer_pixel_opengl_es_set_log_callback(calypso_framework_renderer_pixel_opengl_es_log_callback_t log_callback)
{
    _calypso_framework_renderer_pixel_opengl_es_log_callback = log_callback;
}

void calypso_framework_renderer_pixel_opengl_es_do_log_callback(const char* log_msg, const unsigned char  log_type)
{
    if (_calypso_framework_renderer_pixel_opengl_es_log_callback == NULL)
        return;

    _calypso_framework_renderer_pixel_opengl_es_log_callback(log_msg,log_type);
}

void calypso_framework_renderer_pixel_opengl_es_log_graphics_card()
{
    // Check If We Are Init
    if (_calypso_framework_renderer_pixel_opengl_es_state != CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_log_graphics_card\n",3);
        return;
    }

    // Log
    const char* vendor = (const char*)glGetString(GL_VENDOR);
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    calypso_framework_renderer_pixel_opengl_es_do_log_callback("Graphics Card[",1);
    calypso_framework_renderer_pixel_opengl_es_do_log_callback(vendor,1);
    calypso_framework_renderer_pixel_opengl_es_do_log_callback(":",1);
    calypso_framework_renderer_pixel_opengl_es_do_log_callback(renderer,1);
    calypso_framework_renderer_pixel_opengl_es_do_log_callback("]\n",1);
}

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Init / Deinit
------------------------------------------------------------------------------*/
  
int calypso_framework_renderer_pixel_opengl_es_is_init()
{
    return _calypso_framework_renderer_pixel_opengl_es_state == CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT;
}

void calypso_framework_renderer_pixel_opengl_es_init(void* opengl_proc_address, unsigned int width, unsigned int height)
{
    // Set State
    _calypso_framework_renderer_pixel_opengl_es_state = CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT;

    // OpenGL (Glad)
    gladLoadGLLoader(opengl_proc_address);

    // Generate shaders
    {
        // Vertex Shader Source
        const char* vertex_shader_source =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position_a;\n"
        "layout(location = 1) in vec2 texCoord_a;\n"
        "out vec2 texCoord_s;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position_a, 1.0);\n"
        "    texCoord_s = texCoord_a;\n"
        "}\n";

        // Fragment Shader Source
        const char* fragment_shader_source =
        "#version 330 core \n"
        "out vec4 fragColor_s; \n"
        "in vec2 texCoord_s; \n"
        "uniform sampler2D u_texture; \n"
        "void main() \n"
        "{\n"
        "  fragColor_s = texture(u_texture, texCoord_s);\n"
        "}\n";

        // Define Vertex Shader
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Define Fragment Shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

         // Compile Vertex Shader
        glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertexShader);

         // Compile Fragment Shader
        glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragmentShader);

        // Create Shader Program | Link shaders
        _calypso_framework_renderer_pixel_opengl_es_shader_program = glCreateProgram();
        glAttachShader(_calypso_framework_renderer_pixel_opengl_es_shader_program, vertexShader);
        glAttachShader(_calypso_framework_renderer_pixel_opengl_es_shader_program, fragmentShader);
        glLinkProgram(_calypso_framework_renderer_pixel_opengl_es_shader_program);

        // Delete Shaders (No Longer Needed When Program Is Created)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // OpenGl Object
    {
        // Vertex Buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void *)0);
        glEnableVertexAttribArray(0);

        // Verticies
        float vertices[] = 
        {
            // Position (XYZ)       // TextureCoords (UV)
             1.0f,  1.0f,  0.0f,    1.0f, 1.0f,             // top right
             1.0f, -1.0f,  0.0f,    1.0f, 0.0f,             // bottom right
            -1.0f, -1.0f,  0.0f,    0.0f, 0.0f,             // bottom left
            -1.0f,  1.0f,  0.0f,    0.0f, 1.0f              // top left
        };

        // Indicies
        unsigned int indices[] = 
        {
            0, 1, 3, // 1st Triangle
            1, 2, 3, // 2nd Triangle
        };

        // Generate objects
        glGenVertexArrays(1, &_calypso_framework_renderer_pixel_opengl_es_vao);
        glGenBuffers(1, &_calypso_framework_renderer_pixel_opengl_es_vbo);
        glGenBuffers(1, &_calypso_framework_renderer_pixel_opengl_es_ibo);
        glBindVertexArray(_calypso_framework_renderer_pixel_opengl_es_vao);

        // Set VBO Verticies
        glBindBuffer(GL_ARRAY_BUFFER, _calypso_framework_renderer_pixel_opengl_es_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set Ibo Indicies
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _calypso_framework_renderer_pixel_opengl_es_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position(XYZ) Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        
        // TextureCoord(UV) Attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind Our Buffer And Vertex Array
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Texture
    {
        // Generate | Bind Texture
        glGenTextures(1, &_calypso_framework_renderer_pixel_opengl_es_texture);
        glBindTexture(GL_TEXTURE_2D, _calypso_framework_renderer_pixel_opengl_es_texture);

        // Texture Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void calypso_framework_renderer_pixel_opengl_es_deinit(void)
{
    // De-allocate OpenGl Object
    glDeleteVertexArrays(1, &_calypso_framework_renderer_pixel_opengl_es_vao);
    glDeleteBuffers(1, &_calypso_framework_renderer_pixel_opengl_es_vbo);
    glDeleteBuffers(1, &_calypso_framework_renderer_pixel_opengl_es_ibo);

    // De-allocate OpenGl Shader Programs
    glDeleteProgram(_calypso_framework_renderer_pixel_opengl_es_shader_program);
}

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Renderer (Pixel Buffer)
------------------------------------------------------------------------------*/

void calypso_framework_renderer_pixel_opengl_es_create_pixel_buffer(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer, const int width, const int height)
{
    // Create Pixel Buffer
    pixel_buffer->buffer = (GLubyte*) malloc(width * height * CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT);
    pixel_buffer->buffer_width = width;
    pixel_buffer->buffer_height = height;
    pixel_buffer->buffer_length = width * height * CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT;

    // Set Default Values (White)
    for (int i = 0; i < pixel_buffer->buffer_length; i+= CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT)
    {
        pixel_buffer->buffer[i] = 0;
        pixel_buffer->buffer[i + 1] = 0;
        pixel_buffer->buffer[i + 2] = 0;
        pixel_buffer->buffer[i + 3] = 0;
    }
}

void calypso_framework_renderer_pixel_opengl_es_free_pixel_buffer(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer)
{
    free(pixel_buffer->buffer);
    pixel_buffer->buffer_width = 0;
    pixel_buffer->buffer_height = 0;
    pixel_buffer->buffer_length = 0;
}

/**
* \brief Set Pixel
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_set_pixel_buffer_pixel(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer, const int x, const int y, uint_fast8_t r, uint_fast8_t g, uint_fast8_t b, uint_fast8_t a)
{
    // Check If Pixel Buffer Is Valid
    if (pixel_buffer->buffer_width <= 0 || pixel_buffer->buffer_height <= 0 || pixel_buffer->buffer_length <= 0)
    {
         _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_set_pixel_buffer_pixel\n",3);
        return;
    }

    // Make sure we are in range
    if (x < 0 || x > pixel_buffer->buffer_width || y < 0 || y > pixel_buffer->buffer_height)
        return;

    // Get Index
    uint32_t index = ((y * pixel_buffer->buffer_width) + x) * CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT;

    // Set Pixel Data
    pixel_buffer->buffer[index] = r;
    pixel_buffer->buffer[index + 1] = g;
    pixel_buffer->buffer[index + 2] = b;
    pixel_buffer->buffer[index + 3] = a;
}

/**
* \brief Set pixel fill rect
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_set_pixel_fill_rect(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer, const int x, const int y, const int w, const int h, uint_fast8_t r,uint_fast8_t g, uint_fast8_t b, uint_fast8_t a)
{
     // Check If Pixel Buffer Is Valid
    if (pixel_buffer->buffer_width <= 0 || pixel_buffer->buffer_height <= 0 || pixel_buffer->buffer_length <= 0)
    {
         _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_set_pixel_buffer_pixel\n",3);
        return;
    }

    // Set Pixels
    for (int xi = x; x < w; xi++)
        for (int yi = x; y < h; yi++)
        {
            // Make sure we are in range
            if (xi < 0 || xi > pixel_buffer->buffer_width || yi < 0 || yi > pixel_buffer->buffer_height)
                continue;
            
            // Get Index
            uint32_t index = ((yi * pixel_buffer->buffer_width) + xi) * CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT;

            // Set Pixel Data
            pixel_buffer->buffer[index] = r;
            pixel_buffer->buffer[index + 1] = g;
            pixel_buffer->buffer[index + 2] = b;
            pixel_buffer->buffer[index + 3] = a;
        }
}

/**
* \brief Set pixel fill screen
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_set_pixel_fill_screen(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer, uint_fast8_t r,uint_fast8_t g, uint_fast8_t b, uint_fast8_t a)
{
    // Check If Pixel Buffer Is Valid
    if (pixel_buffer->buffer_width <= 0 || pixel_buffer->buffer_height <= 0 || pixel_buffer->buffer_length <= 0)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_set_pixel_fill_screen\n",3);
        return;
    }
    
    // Set Pixel Data
    for (int i = 0; i < pixel_buffer->buffer_length; i+= CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_PIXEL_BUFFER_CHANNEL_COUNT)
    {
        pixel_buffer->buffer[i] = r;
        pixel_buffer->buffer[i + 1] = g;
        pixel_buffer->buffer[i + 2] = b;
        pixel_buffer->buffer[i + 3] = a;
    }
}

/**
* \brief renderer render
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_render_pixel_buffer(calypso_framework_renderer_pixel_opengl_es_pixel_buffer_t* pixel_buffer, const unsigned int texture_slot)
{
    // Check If We Are Init
    if (_calypso_framework_renderer_pixel_opengl_es_state != CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_render_pixel_buffer\n",3);
        return;
    }

    // Check If Pixel Buffer Is Valid
    if (pixel_buffer->buffer_width <= 0 || pixel_buffer->buffer_height <= 0 || pixel_buffer->buffer_length <= 0)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Invalid Pixel Buffer : calypso_framework_renderer_pixel_opengl_es_render_pixel_buffer\n",3);
        return;
    }

    // Use Our Shader Program
    glUseProgram(_calypso_framework_renderer_pixel_opengl_es_shader_program);

    // Create Texture From Pixels | Bind It
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixel_buffer->buffer_width, pixel_buffer->buffer_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,pixel_buffer->buffer);
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    glBindTexture(GL_TEXTURE_2D,_calypso_framework_renderer_pixel_opengl_es_texture);

    const char* uniform_texture_parameter_name = "u_texture";
    int location = glGetUniformLocation(_calypso_framework_renderer_pixel_opengl_es_shader_program,uniform_texture_parameter_name);
    if (location == -1)
    {
        calypso_framework_renderer_pixel_opengl_es_do_log_callback("Renderer: Can't set shader program paramater float(",2);
        calypso_framework_renderer_pixel_opengl_es_do_log_callback(uniform_texture_parameter_name,2);
        calypso_framework_renderer_pixel_opengl_es_do_log_callback(")\n",2);
        return;
    }
    glUniform1i(location,texture_slot);
        
    // Render    
    glBindVertexArray(_calypso_framework_renderer_pixel_opengl_es_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Renderer (Clear)
------------------------------------------------------------------------------*/

/**
* \brief Set renderer's clear color
* \param r float
* \param g float
* \param b float
* \param a float
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_set_clear_color(const float r, const float g, const float b, const float a)
{
    glClearColor(r,g,b,a);
}

/**
* \brief Set renderer's clear color by byte color array
* \param color_array unsigned char [4]
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_set_clear_color_by_byte_color_array(const unsigned char  color_array[4])
{
    glClearColor(color_array[0] / 255.0f,color_array[1] / 255.0f,color_array[2] / 255.0f,color_array[3] / 255.0f);
}

/**
* \brief Clear renderer
* \return void
*/
void calypso_framework_renderer_pixel_opengl_es_clear()
{
    // Check If We Are Init
    if (_calypso_framework_renderer_pixel_opengl_es_state != CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_clear\n",3);
        return;
    }

    // Clear
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : Renderer (Resize)
------------------------------------------------------------------------------*/

void calypso_framework_renderer_pixel_opengl_es_renderer_resize(const int width, const int height)
{
     // Check If We Are Init
    if (_calypso_framework_renderer_pixel_opengl_es_state != CALYPSO_FRAMEWORK_RENDERER_PIXEL_OPENGL_ES_STATE_INIT)
    {
        _calypso_framework_renderer_pixel_opengl_es_log_callback("Renderer Not init : calypso_framework_renderer_pixel_opengl_es_renderer_resize\n",3);
        return;
    }

    // Set Viewport
    glViewport(0,0,width,height);
}

/*------------------------------------------------------------------------------
Calypso Framework Renderer Pixel OpenGL : license
------------------------------------------------------------------------------

MIT License
Copyright (c) 2023 Nicholas Gennady Korta
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/