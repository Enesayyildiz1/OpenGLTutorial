#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string loadShaderSrc(const char* filename);

void
processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int
main() {

	//***********************************CREATING WINDOW***************************************//

	std::cout << "rtx 4060";

	int success;
	char infoLog[512];

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//compiling vertex shader
	unsigned int vertexShader;
	//returning shader-id
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	//cacht error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error with vertex shader comp." << std::endl << infoLog << std::endl;
	}

	//compiling fragment shader
	unsigned int fragmentShaders[2];
	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);

	glCompileShader(fragmentShaders[0]);

	//cacht error
	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
		std::cout << "Error with fragment shader comp." << std::endl << infoLog << std::endl;
	}

	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc2 = loadShaderSrc("assets/fragment_core2.glsl");
	const GLchar* fragShader2 = fragShaderSrc2.c_str();
	glShaderSource(fragmentShaders[1], 1, &fragShader2, NULL);

	glCompileShader(fragmentShaders[1]);

	//cacht error
	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
		std::cout << "Error with fragment shader comp." << std::endl << infoLog << std::endl;
	}

	//create a program

	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();

	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);

	//catch errors

	glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "Link error: " << std::endl << infoLog << std::endl;
	}

	shaderPrograms[1] = glCreateProgram();

	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);

	//catch errors

	glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderPrograms[1], 512, NULL, infoLog);
		std::cout << "Link error: " << std::endl << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);


	//vertex array

	float vertices[] = {
		//positions              //colors
	 -0.25f, -0.5f, 0.0f,		1.0f, 1.0f, 0.5f,
	  0.15f,  0.0f, 0.0f,		0.5f, 1.0f, 0.7f,
	  0.0f,   0.5f, 0.0f,		0.6f, 1.0f, 0.2f ,
	  0.5f,   -0.4f, 0.0f,		1.0f, 0.2f, 1.0f

	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 1, 2,
	};

	//VAO, VBO 

	unsigned int VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attrib Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUseProgram(shaderPrograms[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		//draw shapes
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(VAO);


		glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glUseProgram(shaderPrograms[1]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(3*sizeof(float)));
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	/*glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;*/

	std::cout << vec.x << vec.y << vec.z << std::endl;

	glm::mat4 trans2 = glm::mat4(1.0f);
	trans = glm::rotate(trans2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	vec = trans2 * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;


	return 0;
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::string loadShaderSrc(const char* filename) {
	std::ifstream file;
	std::stringstream buf;

	std::string ret;

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Could not open" << std::endl;
	}
	return ret;
}