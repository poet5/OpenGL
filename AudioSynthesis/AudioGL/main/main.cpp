#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>


// Shader Class, loading objects relative to screenspace.
#include "shaderClass/shader.h" // Lovely shader class <3
#include "VBO/VBO.h"
#include "VAO/VAO.h"
#include "EBO/EBO.h"

// OpenAL, Audio Libraries.

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "AL/efx-creative.h"
#include "AL/efx-presets.h"
#include "AL/efx.h"


#include <windows.h>

// GLM. (3D Imagery.)

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#define PI 3.14


// Vertices coordinates (triangle shape.)
GLfloat vertices[] =
{
	0.5f, 1, 0.5f, // upper right corner.
	-0.5f, 0.5f, 0.0, // lower left corner.
	0.5f, -0.5f,0.0, // lower  right corner.
	-0.5f, -1, 0.0
};
GLuint indices[] = 
{
	3, 1, 2,
	2, 3, 0
};

#include <fstream>
#include <cstring>



int main()
{	
	// Startup OpenAL.

	// Open Default device windows has selected.
	ALCdevice * device = alcOpenDevice(nullptr); 
	if(!device)
	{
		std::cout << "OpenAL Device Failed!" << std::endl;
		return -1;
	}

	// Capture device.
	alcCaptureOpenDevice(NULL, 4700, AL_FREQUENCY, 24000);

	ALCcontext * context = alcCreateContext(device,NULL);

    alcMakeContextCurrent(context);

	// Get any errors.
	alGetError();

	// Create sound segment for audio.
	char buffer[10000];
	alBufferi(buffer[0], AL_FREQUENCY, 5000); 

	// Create iterator for each segment of audio.
	int FreqIterator = 0;

	// TODO:
	// Free memory from buffer.

	// Option 2: Single-sample computations
    // Initialize GLFW.
    glfwInit();

    // Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Make a window using glfwCreateWindow
    GLFWwindow* window = glfwCreateWindow(1920, 1200, "OpenGL", NULL, NULL);

    // Check if the window works.
    if (window == NULL) 
    {
        printf("Window failed! \n");
        return -1;
    }


	// Put window on display.
    glfwMakeContextCurrent(window);

	// Load OpenGL using Glad.
	gladLoadGL();

	glViewport(0,0,1920,1080);// Change the viewport for visibility of window content.
	// Load shader for Vertex (shape) and Fragment (texture).

	Shader shaderobj("shader/crown/default.vert", "shader/crown/default.frag");

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Pulsating Effect.

	// Get Scale of object.
	GLuint scaleID = glGetUniformLocation(shaderobj.ID, "scale");

	float scale = 0.0f;

	// Audio generation.
		
	GLfloat prevtime = glfwGetTime();
	

	int audioPlay = PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME | SND_ASYNC);

	float rotation = 0.0f;

	glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
			GLfloat crntTime = glfwGetTime();
			
			if (glfwGetTime() - crntTime >1.0f / 75.0f)
			{
				scale+=0.2f ;
				crntTime = glfwGetTime();
			}

			alBufferi(buffer[0], AL_FREQUENCY, 4700); 

			glUniform1f(scaleID, 0.05f * cos(buffer[FreqIterator++] * 2* PI * crntTime));

			if(FreqIterator == 10000) {
				FreqIterator = 0;
			}

			//GLAD Events. (Color.)

			glClearColor(0.94f, 0.9024f, 0.9306f, 1.0f);  // Change the color value.

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT); // Prepare it.
            // Tell OpenGL which Shader Program we want to use
			shaderobj.Activate();

			// Create GLM Object to project shape.
			glm::mat4 model = glm::mat4(1.0f); 
			glm::mat4 view = glm::mat4(1.0f); 
			glm::mat4 proj = glm::mat4(1.0f);

			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
			proj = glm::perspective(glm::radians(45.0f), (float)(1920/1200), 0.1f, 100.0f);

			if (crntTime - prevtime >= 1/60) 
			{
				rotation += 0.5f;
				prevtime = crntTime;
			}

			// Generate a matrix of coordinates for each mat4 variable
			int modelLoc = glGetUniformLocation(shaderobj.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			int viewLoc = glGetUniformLocation(shaderobj.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			int projLoc = glGetUniformLocation(shaderobj.ID, "proj");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


            // Bind the VAO so OpenGL knows to use it
            VAO1.Bind();
            // Draw the triangle using the GL_TRIANGLES primitive and draw it along with EBO, or indices.
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window); // Swap window with each new frame.
            glfwPollEvents(); // Poll each event each time the loop occurs.
    }
    // Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// Delete window before ending the program
	shaderobj.Delete();
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
}