//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Sphere.h"

const unsigned int width = 1000;
const unsigned int height = 1000;
const double M_PI = 3.14159;
const size_t number_of_planets = 8;
int main()
{
	srand(time(NULL));
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "projekt2", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	std::vector<Shader> planetPrograms(number_of_planets);
	for (int i = 0; i < number_of_planets; i++)
	{
		Shader temp("default.vert", "default.frag");
		planetPrograms.push_back(temp);
	}

	std::vector<Sphere>planets(number_of_planets);
	for (int i = 0; i < number_of_planets; i++)
	{
		planets.push_back(Sphere());
	}

	planets[0].init(1, 1, 0);
	planets[1].init(0.4, 0.1, 0.1);
	planets[2].init(0.5, 0.5, 0.1);
	planets[3].init(0, 0, 1);
	planets[4].init(0.3, 0.3, 0);
	planets[5].init(0.4, 0.4, 0);
	planets[6].init(0.7,0.7, 0);
	planets[7].init(0.4, 0.4, 0.8);

	std::vector<float> timers({0,0,0,0,0,0,0,0});
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);


	// Initializes matrices so they are not the null matrix
	std::vector<glm::mat4> models(number_of_planets);
	for (int i = 0; i < number_of_planets; i++) models[i] = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);


	double translations[number_of_planets][3] =
	{
		{0,0,0},
		{5,		1,		0.005},
		{5,		1.5,		0.005},
		{5,		2,		0.005},
		{4,		2.5,		0.005},
		{4,		3,		0.005},
		{3,		2.25,	0.005},
		{3,		1.75,		0.005}
	};


	double scaling[number_of_planets] = {
		350 * 0.001,
		10 * 0.001,
		15 * 0.001,
		20 * 0.001,
		14 * 0.001,
		80 * 0.001,
		70 * 0.001,
		60 * 0.001
	};


	for (int i = 0; i < number_of_planets; i++)
	{
		models[i] = glm::scale(models[i], glm::vec3(1 + scaling[i], 1 + scaling[i], 1 + scaling[i]));		//skalowanie
	}



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0,0,0, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		for (int i = 0; i < number_of_planets; i++)
		{
			planetPrograms[i].Activate();
		}

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1/60 )
		{
			timers[1] += 0.0006512341f;
			timers[2] += 0.0006034563f;
			timers[3] += 0.0005034563f;
			timers[4] += 0.00040645f;
			timers[5] += 0.00030635f;
			timers[6] += 0.0002034562f;
			timers[7] += 0.000103456f;
			prevTime = crntTime;
		}
		for (int i = 0; i < number_of_planets; i++)
		{
			//models[i] = glm::rotate(models[i], glm::radians(1.0f), glm::vec3(1,0,0));					//obrót wokó³ w³asnej osi
			models[i] = glm::translate(models[i], glm::vec3(
				cos(2.f * M_PI * timers[i])			 * translations[i][0] / 10000,
				cos(2.f * M_PI * timers[i])				 * translations[i][1] / 10000,
				sin((double)(i / 10) + 2.f * M_PI * timers[i])				 * translations[i][2] / 10000));	//obrót wokó³ œrodka uk³adu
		}
		

		view = glm::translate(view, glm::vec3(.0f,.0f,.0f));
		proj = glm::perspective(glm::radians(158.0f), (float)width / height, 0.1f, 1000.0f);
		
		for (int i = 0; i < number_of_planets; i++)
		{
			GLuint sphereModelLoc = glGetUniformLocation(planetPrograms[i].ID, "model");
			glUniformMatrix4fv(sphereModelLoc, 1, GL_FALSE, glm::value_ptr(models[i]));
			GLuint sphereViewLoc = glGetUniformLocation(planetPrograms[i].ID, "view");
			glUniformMatrix4fv(sphereViewLoc, 1, GL_FALSE, glm::value_ptr(view));
			GLuint sphereProjLoc = glGetUniformLocation(planetPrograms[i].ID, "proj");
			glUniformMatrix4fv(sphereProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
			planets[i].draw();
		}
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	for(int i = 0; i < number_of_planets; i++) planets[i].cleanup();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}