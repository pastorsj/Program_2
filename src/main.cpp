#define USER1 "pastorsj"
#define USER2 "persinme"

//Add your program1.cpp code to this file

//Near the top, add an include to "ShaderManager.h"

//Replace the shader setup calls with
//calls to ShaderManager methods. See
//the program1-shaderManager.cpp example
//on the course webpage.

//You can then remove the functions:
//printProgramLinkLog()
//printProgramCompileLog()


#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#endif

#define GL3W
#ifdef GLEW
#define GLEW_STATIC
#include "glew/glew.h"
#endif
#ifdef GL3W
#include "gl3w/gl3w.h"
#endif
#include <SFML/Window.hpp>
#include "simplePNG.h"
#include "ShaderManager.h"
#include "WorldState.h"
#include "GLHelper.h"


#define RESOLUTION 512

GLuint shaderProg;

GLuint positionBuffer;
GLuint colorBuffer;
GLuint vertexArray;
GLint positionSlot;
GLint colorSlot;

//TODO: replace these with your scene data
GLfloat positions[10] = {-1,-1, 1,1, -0.5,-0.366, 0,0.5, 0.5,-0.366};
GLfloat colors[15] = {1,1,1, 0.5,0.5,0.5, 1,0,0, 0,1,0, 0,0,1};

/*
 Draw a single frame
 */
void display(WorldState & state)
{
	// Clear the color bits in the display buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Use a simple shader to render the line
	glUseProgram(shaderProg);
	checkGLError("useProgram");

	//use the vertex array
	glBindVertexArray(vertexArray);
	
    GLuint timeSlot = glGetUniformLocation(shaderProg, "timeVal");
    glUniform1f(timeSlot, state.getCurrentTime());

	GLuint xOffset = glGetUniformLocation(shaderProg, "xOffset");
	glUniform1f(xOffset, state.getXOffset());

	GLuint yOffset = glGetUniformLocation(shaderProg, "yOffset");
	glUniform1f(yOffset, state.getYOffset());
    
	//get uniform slot id
	//TODO: send a uniform value to the shader
	checkGLError("uniform");
	
	// Render using vertex attributes (data already on GPU) (~2008, 3.0)
	// https://web.archive.org/web/20150225192608/http://www.arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
	
	// Tell OpenGL we want to use a buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	checkGLError("bind pos buffer");
	
	// Tell OpenGL what shader data slot we want to use
	glEnableVertexAttribArray(positionSlot);
	checkGLError("enable pos slot");
	
	// Tell OpenGL how to interpret the data
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
	checkGLError("vertex attrib pos");
	
	// Do the same thing for colors
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glEnableVertexAttribArray(colorSlot);
	glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
	checkGLError("color array setup");
	
	
	
	// Draw some primitives as: glDrawArrays(type, first, count)
	//TODO: replace these with your draw calls
	glDrawArrays(GL_TRIANGLES, 2, 3);
	
	
	
	//done with the vertex array
	glBindVertexArray(0);

	// Tell OpenGL we are done with the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Tell OpenGL we are done with the shader
	glUseProgram(0);
	checkGLError("unbind");
}

/*
 Initialize the graphics state
 */
void graphicsInit()
{
	//Use ShaderManager to prepare shader
	//char const * vertSource = "attribute vec2 pos; attribute vec3 color; varying vec4 smoothColor; void main() { gl_Position=vec4(pos.xy, 0, 1); smoothColor=vec4(color.xyz, 1); }";
	//char const * fragSource = "varying vec4 smoothColor; void main() { gl_FragColor = smoothColor; }";
	//shaderProg = ShaderManager::shaderFromString(&vertSource, &fragSource, 1, 1);
	
	char const * vertPath = "shaders/simple.vert";
	char const * fragPath = "shaders/simple.frag";
	shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
	
	// The data we will render needs to be on the GPU
	// These commands upload the data
	
	// Find out where the shader expects the data
	positionSlot = glGetAttribLocation(shaderProg, "pos");
	colorSlot = glGetAttribLocation(shaderProg, "color");
	if(positionSlot < 0 || colorSlot < 0) {
		fprintf(stderr, "Could not get input location\n");
	}

	glGenBuffers(1, &positionBuffer); //make the position buffer
	glGenBuffers(1, &colorBuffer);
	if(positionBuffer == 0) fprintf(stderr, "GenBuffer failed\n");
	if(colorBuffer == 0) fprintf(stderr, "GenBuffer failed\n");
	
	glGenVertexArrays(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer); //activate the position buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); //alloc and upload
	glBindBuffer(GL_ARRAY_BUFFER, 0); //deactivate the position buffer
	checkGLError("pos buffer");
	
	// Do the same thing for the color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGLError("color buffer");
}

void saveBuffer(sf::Window const & window)
{
    unsigned char *dest;
    unsigned int w = window.getSize().x;
    unsigned int h = window.getSize().y;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
    
    simplePNG_write("_program1.png", w, h, dest);
    free(dest);
}

float getVer()
{
    float ver = 0.0f;
#ifdef GLEW
	glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    if (GLEW_VERSION_1_1) { ver = 1.1f; }
    if (GLEW_VERSION_1_2) { ver = 1.2f; }
    if (GLEW_VERSION_1_3) { ver = 1.3f; }
    if (GLEW_VERSION_1_4) { ver = 1.4f; }
    if (GLEW_VERSION_1_5) { ver = 1.5f; }
    
    if (GLEW_VERSION_2_0) { ver = 2.0f; }
    if (GLEW_VERSION_2_1) { ver = 2.1f; }
    
    if (GLEW_VERSION_3_0) { ver = 3.0f; }
    if (GLEW_VERSION_3_1) { ver = 3.1f; }
    if (GLEW_VERSION_3_2) { ver = 3.2f; }
    if (GLEW_VERSION_3_3) { ver = 3.3f; }
    
    if (GLEW_VERSION_4_0) { ver = 4.0f; }
    if (GLEW_VERSION_4_1) { ver = 4.1f; }
    if (GLEW_VERSION_4_2) { ver = 4.2f; }
    if (GLEW_VERSION_4_3) { ver = 4.3f; }
    if (GLEW_VERSION_4_4) { ver = 4.4f; }
    if (GLEW_VERSION_4_5) { ver = 4.5f; }
#endif
	
#ifdef GL3W
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
	}
	
	if (gl3wIsSupported(1, 1)) { ver = 1.1f; }
    if (gl3wIsSupported(1, 2)) { ver = 1.2f; }
    if (gl3wIsSupported(1, 3)) { ver = 1.3f; }
    if (gl3wIsSupported(1, 4)) { ver = 1.4f; }
    if (gl3wIsSupported(1, 5)) { ver = 1.5f; }
    
    if (gl3wIsSupported(2, 0)) { ver = 2.0f; }
    if (gl3wIsSupported(2, 1)) { ver = 2.1f; }
    
    if (gl3wIsSupported(3, 0)) { ver = 3.0f; }
    if (gl3wIsSupported(3, 1)) { ver = 3.1f; }
    if (gl3wIsSupported(3, 2)) { ver = 3.2f; }
    if (gl3wIsSupported(3, 3)) { ver = 3.3f; }
    
    if (gl3wIsSupported(4, 0)) { ver = 4.0f; }
    if (gl3wIsSupported(4, 1)) { ver = 4.1f; }
    if (gl3wIsSupported(4, 2)) { ver = 4.2f; }
    if (gl3wIsSupported(4, 3)) { ver = 4.3f; }
    if (gl3wIsSupported(4, 4)) { ver = 4.4f; }
    if (gl3wIsSupported(4, 5)) { ver = 4.5f; }
#endif
    
    return ver;
}

class GLBox
{
private:
	sf::Window *App;
	
	void handleEvents(sf::Window & window, WorldState & state)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				state.setRunning(false);
			
			// Escape key : exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				state.setRunning(false);
		}

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
		{
			state.moveRight();
		}
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
		{
			state.moveLeft();
		}
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
		{
			state.moveUp();
		}
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) )
		{
			state.moveDown();
		}
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            state.rotateClockWise();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            state.rotateCounterClockWise();
        }
	}

	float getContextVersion()
	{
	}
	
public:
	GLBox()
	{
		//sf::err().rdbuf(NULL); //hide errors
		
#ifdef __APPLE__
		//int nullFD = open("/dev/null", O_WRONLY);
		//int oldFD = dup(2); // Duplicate the old file descriptor, so it can be restored
		//dup2(nullFD, 2); // Redirect
#endif
		
		sf::VideoMode mode(RESOLUTION, RESOLUTION, 32);
#ifdef __linux__
		sf::ContextSettings settings(32, 0, 0, 3, 3);
#else
		sf::ContextSettings settings(0, 0, 0, 3, 3);
#endif
		sf::Window window(mode, "glver", sf::Style::Default, settings);
		float ver = getVer();
		
#ifdef __APPLE__
		//dup2(oldFD, 2); // Redirect back
		//close(oldFD); // Not needed anymore
#endif
		
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
		
		
		sf::Clock time;
		WorldState state;
		graphicsInit();
		
		while (state.isRunning())
		{
			this->handleEvents(window, state);
			state.timeStep( time.getElapsedTime().asSeconds() );
			display(state);
			window.display();
		}
		window.close();
	}
	
	~GLBox()
	{
		glDeleteBuffers(1, &positionBuffer);
	}
};

int main()
{
	printf("Program by %s+%s\n", USER1, USER2);
	GLBox prog;
	
    return EXIT_SUCCESS;
}
