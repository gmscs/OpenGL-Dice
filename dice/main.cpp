#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "aux.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include "stb/stb_image_write.h"

const char* die_fragment_shader =
"#version 330 core\n"
""
"in vec3 fragmentNormals;"
"in vec3 texcoords;"
"in mat4 total_matrix;"
""
"uniform samplerCube sampler;"
""
"out vec4 glass;"
""
"void main() {"
"	float ratio = 1.00 / 1.52;"
"	vec3 refraction = refract(normalize(texcoords), normalize(fragmentNormals), ratio);"
""
"	vec3 refraction_dir = (total_matrix * vec4(refraction, 0.0)).xyw;"
"	glass = texture(sampler, refraction_dir);"
"	glass.a = 0.9;"
"}";

const char* die_vertex_shader =
"#version 330 core\n"
"layout(location = 0) in vec3 vertices;"
"layout(location = 1) in vec3 normals;"
"layout(location = 4) in vec3 indices;"
""
"uniform mat4 die_matrix;"
"uniform mat4 model_matrix;"
"uniform vec3 camera_position;"
""
"out vec3 fragmentNormals;"
"out vec3 texcoords;"
"out mat4 total_matrix;"
""
"void main() {"
"	fragmentNormals = vec3(vec4(normals, 0.0) * model_matrix);"
"	texcoords = vec3(model_matrix * vec4(vertices, 1.0) - vec4(camera_position, 1.0));"
"	total_matrix = die_matrix * model_matrix;"
""
"   gl_Position = total_matrix * vec4(vertices, 1.0);"
"}";

const char* sphere_fragment_shader = 
"#version 330 core\n"
""
"in vec3 texcoords;"
""
"void main()"
"{"
"	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
"}";

const char* sphere_vertex_shader = 
"#version 330 core\n"
"layout(location = 0) in vec3 vertices;"
"layout(location = 1) in vec3 colors;"
"layout(location = 2) in vec3 indices;"
""
"uniform mat4 sphere_matrix;"
"uniform mat4 model_matrix;"
""
"out vec3 texcoords;"
""
"void main()" 
"{"
	"texcoords = colors;"
	"mat4 total_matrix = sphere_matrix * model_matrix;"
	""
	"gl_Position = total_matrix * vec4(vertices, 1.0);"
"}";

const char* skybox_vertex_shader =
"#version 330 core\n"
"layout(location = 0) in vec3 vertices;"
""
"uniform mat4 skybox_matrix;"
"uniform mat4 model_matrix;"
""
"out vec3 texcoords;"
""
"void main()"
"{"
"	texcoords = vertices;"
"	gl_Position = skybox_matrix * model_matrix * vec4(vertices, 1.0);"
"}";

const char* skybox_fragment_shader =
"#version 330 core\n"
""
"in vec3 texcoords;"
""
"uniform samplerCube sampler;"
""
"void main()"
"{"
"	gl_FragColor = texture(sampler, texcoords);"
"}";

typedef struct projection
{
	float fov;
	float aspect_ratio;
	float near;
	float far;
}PROJECTION;

typedef struct view
{
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 axis;
}VIEW;

int load_obj_file(const std::string &file, std::vector <glm::vec3> &Vertices, std::vector <glm::vec3> &Normals, std::vector <glm::vec3> &Texcoords, std::vector<glm::vec3> &Tangents, std::vector<unsigned int> &Indices) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate);
	if(!scene) {
		std::cerr << "Failed to load scene." << "\n";
		return -1;
	}
	
	const aiMesh* mesh = scene->mMeshes[0];
	
	for(int i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D& mesh_vert = mesh->mVertices[i];
		Vertices.push_back(glm::vec3(mesh_vert.x,mesh_vert.y,mesh_vert.z));
		if(mesh->HasNormals()) {
			const aiVector3D& mesh_norm = mesh->mNormals[i];
			Normals.push_back(glm::vec3(mesh_norm.x,mesh_norm.y,mesh_norm.z));
		}
		if(mesh->HasTextureCoords(0)) {
			const aiVector3D& mesh_tex = mesh->mTextureCoords[0][i];
			Texcoords.push_back(glm::vec3(mesh_tex.x, mesh_tex.y, mesh_tex.z));
		}
		if(mesh->HasTangentsAndBitangents()) {
			const aiVector3D& mesh_tan = mesh->mTangents[i];
			Tangents.push_back(glm::vec3(mesh_tan.x, mesh_tan.y, mesh_tan.z));
		}
	}
	
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		Indices.push_back(face.mIndices[0]);
		Indices.push_back(face.mIndices[1]);
		Indices.push_back(face.mIndices[2]);
	}
	
	return mesh->mNumVertices;
}

void captureScene(int screenshot_number, int win_width, int win_height) {
	GLubyte* pixels = new GLubyte[3 * win_width * win_height];
	glReadPixels(0, 0, win_width, win_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	char name[55];
	int curr_height = win_height - 1;
	int pix;
	snprintf(name, 55 * sizeof(char), "screenshot%d.ppm", screenshot_number);
	FILE *f = fopen(name, "w");
	fprintf(f, "P3\n%d %d\n%d\n", win_width, curr_height, 255);
	for(int i = 0; i < curr_height; i++) {
		for(int j = 0; j < win_width; j++) {
			pix = 3 * ((curr_height - i) * win_width + j);
			fprintf(f, "%3d %3d %3d ", pixels[pix], pixels[pix + 1], pixels[pix + 2]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
	std::cout << "Screenshot taken!\n";
}

GLuint load_cube_tex(const char* dir, int channels) {
	int width;
	int height;
	int comp;
	char* img;
	unsigned char* tex_data;
	GLuint texture;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	img = (char*)"right.jpg"; 
	char* right = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(right, dir);
	strcat(right, img);
	tex_data = stbi_load(right, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load right texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(right);
	
	img = (char*)"left.jpg"; 
	char* left = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(left, dir);
	strcat(left, img);
	tex_data = stbi_load(left, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load left texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(left);

	img = (char*)"top.jpg"; 
	char* top = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(top, dir);
	strcat(top, img);
	tex_data = stbi_load(top, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load top texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(top);
	
	img = (char*)"bottom.jpg"; 
	char* bottom = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(bottom, dir);
	strcat(bottom, img);
	tex_data = stbi_load(bottom, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load bottom texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(bottom);
	
	img = (char*)"front.jpg"; 
	char* front = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(front, dir);
	strcat(front, img);
	tex_data = stbi_load(front, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load front texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(front);
	
	img = (char*)"back.jpg"; 
	char* back = (char*)malloc((strlen(dir) + strlen(img) + 1) *  sizeof(char));
	strcpy(back, dir);
	strcat(back, img);
	tex_data = stbi_load(back, &width, &height, &comp, channels);
	if(!tex_data) {
		std::cerr << "Failed to load back texture.\n";
		stbi_image_free(tex_data);
		return -1;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
	stbi_image_free(tex_data);
	free(back);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texture;
}

int main() {
	if(!glfwInit()) {
		std::cerr << "glfwInit failed." << std::endl;
		return 1;
	}

	int win_width = 800;
	int win_height = 800;
	
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Project 2", NULL, NULL);
	if(window == NULL) {
		std::cerr << "Window creation failed." << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		std::cerr << "glewInit failed." << std::endl;
		return 1;
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	int screenshot_number = 0;
	
	// MESH LOADING
	std::vector<glm::vec3> cube_vertices;
	std::vector<glm::vec3> cube_normals;
	std::vector<glm::vec3> cube_texcoords;
	std::vector<glm::vec3> cube_tangents;
	std::vector<unsigned int> cube_indices;
	int cube_v_total = load_obj_file("assets/cube.obj", cube_vertices, cube_normals, cube_texcoords, cube_tangents, cube_indices);
	if(cube_v_total == -1) {
		std::cerr << "Failed to load .obj file." << std::endl;
		return 1;
	}
	std::cout << "Loaded Skybox Mesh\n";

	std::vector<glm::vec3> die_vertices;
	std::vector<glm::vec3> die_normals;
	std::vector<glm::vec3> die_texcoords;
	std::vector<glm::vec3> die_tangents;
	std::vector<unsigned int> die_indices;
	int die_v_total = load_obj_file("assets/die.obj", die_vertices, die_normals, die_texcoords, die_tangents, die_indices);
	if(die_v_total == -1) {
		std::cerr << "Failed to load .obj file." << std::endl;
		return 1;
	}
	std::cout << "Loaded Die Mesh\n";

	std::vector<glm::vec3> sphere_vertices;
	std::vector<glm::vec3> sphere_normals;
	std::vector<glm::vec3> sphere_texcoords;
	std::vector<glm::vec3> sphere_tangents;
	std::vector<unsigned int> sphere_indices;
	int sphere_v_total = load_obj_file("assets/sphere.obj", sphere_vertices, sphere_normals, sphere_texcoords, sphere_tangents, sphere_indices);
	if(sphere_v_total == -1) {
		std::cerr << "Failed to load .obj file." << std::endl;
		return 1;
	}
	std::cout << "Loaded Sphere Mesh\n";
	
	// SKYBOX TEXTURE LOADING
	GLuint skybox_texture = load_cube_tex("skybox/", 0);
	if(skybox_texture == -1) {
		std::cerr << "Failed to load textures. Exiting.\n";
		return 1;
	}
	std::cout << "Loaded SkyBox Texture\n";
	
	// SKYBOX VBO
	GLuint skybox_vbo = 0;
	glGenBuffers(1, &skybox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(cube_vertices[0]), &cube_vertices[0], GL_STATIC_DRAW);
	
	// SKYBOX VAO
	GLuint skybox_vao = 0;
	glGenVertexArrays(1, &skybox_vao);
	glBindVertexArray(skybox_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	// DIE VBO
	GLuint die_vertex_vbo = 0;
	glGenBuffers(1, &die_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, die_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, die_vertices.size() * sizeof(die_vertices[0]), &die_vertices[0], GL_STATIC_DRAW);
	GLuint die_normals_vbo = 0;
	glGenBuffers(1, &die_normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, die_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, die_normals.size() * sizeof(die_normals[0]), &die_normals[0], GL_STATIC_DRAW);
	GLuint die_color_vbo = 0;
	glGenBuffers(1, &die_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, die_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, die_texcoords.size() * sizeof(die_texcoords[0]), &die_texcoords[0], GL_STATIC_DRAW);
	GLuint die_i_vbo = 0;
	glGenBuffers(1, &die_i_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, die_i_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, die_indices.size() * sizeof(die_indices[0]), &die_indices[0], GL_STATIC_DRAW);
	
	// DIE VAO
	GLuint die_vao = 0;
	glGenVertexArrays(1, &die_vao);
	glBindVertexArray(die_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, die_vertex_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, die_normals_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, die_color_vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, die_i_vbo);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// SPHERE VBO
	GLuint sphere_vertex_vbo = 0;
	glGenBuffers(1, &sphere_vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(sphere_vertices[0]), &sphere_vertices[0], GL_STATIC_DRAW);
	GLuint sphere_color_vbo = 0;
	glGenBuffers(1, &sphere_color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere_texcoords.size() * sizeof(sphere_texcoords[0]), &sphere_texcoords[0], GL_STATIC_DRAW);
	GLuint sphere_i_vbo = 0;
	glGenBuffers(1, &sphere_i_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_i_vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere_indices.size() * sizeof(sphere_indices[0]), &sphere_indices[0], GL_STATIC_DRAW);

	GLuint sphere_vao = 0;
	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vertex_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_i_vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	// CREATE SHADER PROGRAMS ////////////////////////////////////////
	GLuint die_vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(die_vs, 1, &die_vertex_shader, NULL);
    glCompileShader(die_vs);
    GLuint die_fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(die_fs, 1, &die_fragment_shader, NULL);
    glCompileShader(die_fs);
    GLuint die_shader = glCreateProgram();
    glAttachShader(die_shader, die_vs);
    glAttachShader(die_shader, die_fs);
    glLinkProgram(die_shader);
    
    GLuint skybox_vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skybox_vs, 1, &skybox_vertex_shader, NULL);
    glCompileShader(skybox_vs);
    GLuint skybox_fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skybox_fs, 1, &skybox_fragment_shader, NULL);
    glCompileShader(skybox_fs);
    GLuint skybox_shader = glCreateProgram();
    glAttachShader(skybox_shader, skybox_vs);
    glAttachShader(skybox_shader, skybox_fs);
    glLinkProgram(skybox_shader);

	GLuint sphere_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sphere_vs, 1, &sphere_vertex_shader, NULL);
	glCompileShader(sphere_vs);
	GLuint sphere_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sphere_fs, 1, &sphere_fragment_shader, NULL);
	glCompileShader(sphere_fs);
	GLuint sphere_shader = glCreateProgram();
	glAttachShader(sphere_shader, sphere_vs);
	glAttachShader(sphere_shader, sphere_fs);
	glLinkProgram(sphere_shader);
    //////////////////////////////////////////////////////////////////
    
    glm::mat4 skybox_model_matrix = glm::mat4(1.0f);

	glm::mat4 model_matrix = glm::translate(aux::mat4_identity, glm::vec3(-0.8f, 0.0f, 0.0f));
	glm::mat4 sphere_model_matrix = glm::translate(aux::mat4_identity, glm::vec3(-0.8f, 0.0f, 0.0f));

	glm::mat4 model_matrix2 = glm::translate(aux::mat4_identity, glm::vec3(0.8f, 0.0f, 0.0f));
	glm::mat4 sphere_model_matrix2 = glm::translate(aux::mat4_identity, glm::vec3(0.8f, 0.0f, 0.0f));

	model_matrix = glm::rotate(model_matrix, aux::degrees_to_radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix2 = glm::rotate(model_matrix2, aux::degrees_to_radians(14.3f), glm::vec3(0.0f, 0.0f, 1.0f));

	sphere_model_matrix = glm::rotate(sphere_model_matrix, aux::degrees_to_radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	sphere_model_matrix2 = glm::rotate(sphere_model_matrix2, aux::degrees_to_radians(14.3f), glm::vec3(0.0f, 0.0f, 1.0f));

	sphere_model_matrix = glm::scale(sphere_model_matrix, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 sphere1_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 sphere21_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(6.0f, 3.0f, -3.0f));
	glm::mat4 sphere22_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(6.0f, -3.0f, 3.0f));
	glm::mat4 sphere31_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 6.0f, -3.0f));
	glm::mat4 sphere32_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(0.0f, 6.0f, 0.0f));
	glm::mat4 sphere33_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 6.0f, 3.0f));
	glm::mat4 sphere41_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -6.0f, 3.0f));
	glm::mat4 sphere42_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -6.0f, -3.0f));
	glm::mat4 sphere43_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -6.0f, -3.0f));
	glm::mat4 sphere44_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -6.0f, 3.0f));
	glm::mat4 sphere51_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, -3.0f, 3.0f));
	glm::mat4 sphere52_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, -3.0f, -3.0f));
	glm::mat4 sphere53_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 3.0f, 3.0f));
	glm::mat4 sphere54_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 3.0f, -3.0f));
	glm::mat4 sphere55_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 0.0f, 0.0f));
	glm::mat4 sphere61_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -3.0f, -6.0f));
	glm::mat4 sphere62_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 0.0f, -6.0f));
	glm::mat4 sphere63_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 3.0f, -6.0f));
	glm::mat4 sphere64_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -3.0f, -6.0f));
	glm::mat4 sphere65_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 0.0f, -6.0f));
	glm::mat4 sphere66_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 3.0f, -6.0f));

	sphere_model_matrix2 = glm::scale(sphere_model_matrix2, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 sphere1_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 sphere21_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(6.0f, 3.0f, -3.0f));
	glm::mat4 sphere22_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(6.0f, -3.0f, 3.0f));
	glm::mat4 sphere31_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 6.0f, -3.0f));
	glm::mat4 sphere32_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(0.0f, 6.0f, 0.0f));
	glm::mat4 sphere33_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 6.0f, 3.0f));
	glm::mat4 sphere41_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -6.0f, 3.0f));
	glm::mat4 sphere42_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -6.0f, -3.0f));
	glm::mat4 sphere43_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -6.0f, -3.0f));
	glm::mat4 sphere44_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -6.0f, 3.0f));
	glm::mat4 sphere51_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, -3.0f, 3.0f));
	glm::mat4 sphere52_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, -3.0f, -3.0f));
	glm::mat4 sphere53_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 3.0f, 3.0f));
	glm::mat4 sphere54_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 3.0f, -3.0f));
	glm::mat4 sphere55_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 0.0f, 0.0f));
	glm::mat4 sphere61_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -3.0f, -6.0f));
	glm::mat4 sphere62_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 0.0f, -6.0f));
	glm::mat4 sphere63_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 3.0f, -6.0f));
	glm::mat4 sphere64_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -3.0f, -6.0f));
	glm::mat4 sphere65_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 0.0f, -6.0f));
	glm::mat4 sphere66_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 3.0f, -6.0f));
    
	glm::vec3 die_camera_position = {0.0f, 0.0f, 5.0f};
	glm::vec3 skybox_cam_position = {0.0f, 0.0f, 0.7f};

	PROJECTION projection_info[1] = {
		{aux::degrees_to_radians(45.0f), aux::get_aspect_ratio(win_width, win_height), 0.1f, 100.0f}
		};
	VIEW view_info[3] = {
		{ glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
		};

	glm::mat4 projection_matrix = glm::perspective(projection_info[0].fov, projection_info[0].aspect_ratio, projection_info[0].near, projection_info[0].far);
	glm::mat4 die_view_matrix = glm::lookAt(view_info[0].position, view_info[0].lookAt, view_info[0].axis);
	glm::mat4 skybox_view_matrix = glm::lookAt(view_info[1].position, view_info[1].lookAt, view_info[1].axis);
	glm::mat4 sphere_view_matrix = glm::lookAt(view_info[2]. position, view_info[2].lookAt, view_info[2].axis);

	glm::mat4 skybox_matrix;
	glm::mat4 die_matrix;
	glm::mat4 sphere_matrix;

	bool s_key_pressed = false;
	bool o_key_pressed = false;
	bool key1_pressed = false;
	bool key2_pressed = false;
	bool key3_pressed = false;
	bool orbit = false;

	float prev_time = glfwGetTime();
	
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetWindowSize(window, &win_width, &win_height);
		glfwGetFramebufferSize(window, &win_width, &win_height);
		if(win_height == win_width) { //I want to keep the 1:1 aspect ratio
			glViewport(0, 0, win_height, win_height);
			projection_info[0].aspect_ratio = aux::get_aspect_ratio(win_height, win_height);
		} else if (win_width > win_height) {
			glViewport(0, 0, win_height, win_height);
			projection_info[0].aspect_ratio = aux::get_aspect_ratio(win_height, win_height);
		} else {
			glViewport(0, 0, win_width, win_width);
			projection_info[0].aspect_ratio = aux::get_aspect_ratio(win_width, win_width);
		}

		float time = glfwGetTime();
				
		glDepthMask(GL_FALSE);
		glUseProgram(skybox_shader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
    	skybox_matrix = projection_matrix * skybox_view_matrix;
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "skybox_matrix"), 1, GL_FALSE, glm::value_ptr(skybox_matrix));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(skybox_model_matrix));
		glBindVertexArray(skybox_vao);
		glDrawArrays(GL_TRIANGLES, 0, cube_v_total);
		glDepthMask(GL_TRUE);

		glUseProgram(sphere_shader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		sphere_matrix = projection_matrix * sphere_view_matrix;
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "sphere_matrix"), 1, GL_FALSE, glm::value_ptr(sphere_matrix));
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere1_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere21_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere22_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere31_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere32_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere33_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere41_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere42_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere43_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere44_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere51_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere52_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere53_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere54_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere55_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere61_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere62_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere63_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere64_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere65_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere66_model_matrix));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);

		glUseProgram(die_shader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
		die_matrix = projection_matrix * die_view_matrix;
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "camera_position"), 1, GL_FALSE, glm::value_ptr(die_camera_position));
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "die_matrix"), 1, GL_FALSE, glm::value_ptr(die_matrix));
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(die_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, die_i_vbo);
		glDrawElements(GL_TRIANGLES, die_indices.size(), GL_UNSIGNED_INT, NULL);

		glUseProgram(sphere_shader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		sphere_matrix = projection_matrix * sphere_view_matrix;
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "sphere_matrix"), 1, GL_FALSE, glm::value_ptr(sphere_matrix));
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere1_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere21_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere22_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere31_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere32_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere33_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere41_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere42_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere43_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere44_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere51_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere52_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere53_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere54_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere55_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere61_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere62_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere63_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere64_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere65_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);
		
		glUniformMatrix4fv(glGetUniformLocation(sphere_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(sphere66_model_matrix2));
		glBindVertexArray(sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_vbo);
		glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, NULL);

		glUseProgram(die_shader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
		die_matrix = projection_matrix * die_view_matrix;
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "camera_position"), 1, GL_FALSE, glm::value_ptr(die_camera_position));
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "die_matrix"), 1, GL_FALSE, glm::value_ptr(die_matrix));
		glUniformMatrix4fv(glGetUniformLocation(die_shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix2));
		glBindVertexArray(die_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, die_i_vbo);
		glDrawElements(GL_TRIANGLES, die_indices.size(), GL_UNSIGNED_INT, NULL);

		//AUTO ORBITTING CAMERA
		if(orbit == true) {
			float delta_time = float(time - prev_time);
			glm::quat quaternion = glm::quat(glm::vec3(-1 * aux::degrees_to_radians(delta_time * 3), aux::degrees_to_radians(delta_time * 10), 0));
			glm::mat4 rot = glm::mat4_cast(quaternion);

			skybox_model_matrix = rot * skybox_model_matrix;
			
			model_matrix = rot * model_matrix;
			sphere1_model_matrix = rot * sphere1_model_matrix;
			sphere21_model_matrix = rot * sphere21_model_matrix;
			sphere22_model_matrix = rot * sphere22_model_matrix;
			sphere31_model_matrix = rot * sphere31_model_matrix;
			sphere32_model_matrix = rot * sphere32_model_matrix;
			sphere33_model_matrix = rot * sphere33_model_matrix;
			sphere41_model_matrix = rot * sphere41_model_matrix;
			sphere42_model_matrix = rot * sphere42_model_matrix;
			sphere43_model_matrix = rot * sphere43_model_matrix;
			sphere44_model_matrix = rot * sphere44_model_matrix;
			sphere51_model_matrix = rot * sphere51_model_matrix;
			sphere52_model_matrix = rot * sphere52_model_matrix;
			sphere53_model_matrix = rot * sphere53_model_matrix;
			sphere54_model_matrix = rot * sphere54_model_matrix;
			sphere55_model_matrix = rot * sphere55_model_matrix;
			sphere61_model_matrix = rot * sphere61_model_matrix;
			sphere62_model_matrix = rot * sphere62_model_matrix;
			sphere63_model_matrix = rot * sphere63_model_matrix;
			sphere64_model_matrix = rot * sphere64_model_matrix;
			sphere65_model_matrix = rot * sphere65_model_matrix;
			sphere66_model_matrix = rot * sphere66_model_matrix;
			
			model_matrix2 = rot * model_matrix2;
			sphere1_model_matrix2 = rot * sphere1_model_matrix2;
			sphere21_model_matrix2 = rot * sphere21_model_matrix2;
			sphere22_model_matrix2 = rot * sphere22_model_matrix2;
			sphere31_model_matrix2 = rot * sphere31_model_matrix2;
			sphere32_model_matrix2 = rot * sphere32_model_matrix2;
			sphere33_model_matrix2 = rot * sphere33_model_matrix2;
			sphere41_model_matrix2 = rot * sphere41_model_matrix2;
			sphere42_model_matrix2 = rot * sphere42_model_matrix2;
			sphere43_model_matrix2 = rot * sphere43_model_matrix2;
			sphere44_model_matrix2 = rot * sphere44_model_matrix2;
			sphere51_model_matrix2 = rot * sphere51_model_matrix2;
			sphere52_model_matrix2 = rot * sphere52_model_matrix2;
			sphere53_model_matrix2 = rot * sphere53_model_matrix2;
			sphere54_model_matrix2 = rot * sphere54_model_matrix2;
			sphere55_model_matrix2 = rot * sphere55_model_matrix2;
			sphere61_model_matrix2 = rot * sphere61_model_matrix2;
			sphere62_model_matrix2 = rot * sphere62_model_matrix2;
			sphere63_model_matrix2 = rot * sphere63_model_matrix2;
			sphere64_model_matrix2 = rot * sphere64_model_matrix2;
			sphere65_model_matrix2 = rot * sphere65_model_matrix2;
			sphere66_model_matrix2 = rot * sphere66_model_matrix2;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		//TAKE SCREENSHOT
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && s_key_pressed == false) { //WOW glfw callbacks are awful, even lambda cant fix them
			s_key_pressed = true;
			captureScene(screenshot_number, win_width, win_height);
			screenshot_number++;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && s_key_pressed == true) {
			s_key_pressed = false;
		}

		//ORBIT CAM
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && o_key_pressed == false) {
			o_key_pressed = true;
			if (orbit == false) orbit = true;
			else orbit = false;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE && o_key_pressed == true) {
			o_key_pressed = false;
		}

		//CHANGE SKYBOX
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && key1_pressed == false) {
			key1_pressed = true;
			skybox_texture = load_cube_tex("skybox/", 0);
			if(skybox_texture == -1) {
				std::cerr << "Failed to load textures. Exiting.\n";
				return 1;
			}
			std::cout << "Loaded SkyBox Texture\n";
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && key1_pressed == true) {
			key1_pressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && key2_pressed == false) {
			key2_pressed = true;
			skybox_texture = load_cube_tex("skybox2/", 0);
			if(skybox_texture == -1) {
				std::cerr << "Failed to load textures. Exiting.\n";
				return 1;
			}
			std::cout << "Loaded SkyBox2 Texture\n";
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && key2_pressed == true) {
			key2_pressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && key3_pressed == false) {
			key3_pressed = true;
			skybox_texture = load_cube_tex("skybox3/", 0);
			if(skybox_texture == -1) {
				std::cerr << "Failed to load textures. Exiting.\n";
				return 1;
			}
			std::cout << "Loaded SkyBox3 Texture\n";
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && key3_pressed == true) {
			key3_pressed = false;
		}

		//ROTATE SCENE
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { //repeated code better than unreadable code
			float delta_time = float(time - prev_time);
			glm::quat quaternion = glm::quat(glm::vec3(0, -1 * aux::degrees_to_radians(delta_time * 30), 0));
			glm::mat4 rot = glm::mat4_cast(quaternion);
			skybox_model_matrix = rot * skybox_model_matrix;
			
			model_matrix = rot * model_matrix;
			sphere1_model_matrix = rot * sphere1_model_matrix;
			sphere21_model_matrix = rot * sphere21_model_matrix;
			sphere22_model_matrix = rot * sphere22_model_matrix;
			sphere31_model_matrix = rot * sphere31_model_matrix;
			sphere32_model_matrix = rot * sphere32_model_matrix;
			sphere33_model_matrix = rot * sphere33_model_matrix;
			sphere41_model_matrix = rot * sphere41_model_matrix;
			sphere42_model_matrix = rot * sphere42_model_matrix;
			sphere43_model_matrix = rot * sphere43_model_matrix;
			sphere44_model_matrix = rot * sphere44_model_matrix;
			sphere51_model_matrix = rot * sphere51_model_matrix;
			sphere52_model_matrix = rot * sphere52_model_matrix;
			sphere53_model_matrix = rot * sphere53_model_matrix;
			sphere54_model_matrix = rot * sphere54_model_matrix;
			sphere55_model_matrix = rot * sphere55_model_matrix;
			sphere61_model_matrix = rot * sphere61_model_matrix;
			sphere62_model_matrix = rot * sphere62_model_matrix;
			sphere63_model_matrix = rot * sphere63_model_matrix;
			sphere64_model_matrix = rot * sphere64_model_matrix;
			sphere65_model_matrix = rot * sphere65_model_matrix;
			sphere66_model_matrix = rot * sphere66_model_matrix;

			model_matrix2 = rot * model_matrix2;
			sphere1_model_matrix2 = rot * sphere1_model_matrix2;
			sphere21_model_matrix2 = rot * sphere21_model_matrix2;
			sphere22_model_matrix2 = rot * sphere22_model_matrix2;
			sphere31_model_matrix2 = rot * sphere31_model_matrix2;
			sphere32_model_matrix2 = rot * sphere32_model_matrix2;
			sphere33_model_matrix2 = rot * sphere33_model_matrix2;
			sphere41_model_matrix2 = rot * sphere41_model_matrix2;
			sphere42_model_matrix2 = rot * sphere42_model_matrix2;
			sphere43_model_matrix2 = rot * sphere43_model_matrix2;
			sphere44_model_matrix2 = rot * sphere44_model_matrix2;
			sphere51_model_matrix2 = rot * sphere51_model_matrix2;
			sphere52_model_matrix2 = rot * sphere52_model_matrix2;
			sphere53_model_matrix2 = rot * sphere53_model_matrix2;
			sphere54_model_matrix2 = rot * sphere54_model_matrix2;
			sphere55_model_matrix2 = rot * sphere55_model_matrix2;
			sphere61_model_matrix2 = rot * sphere61_model_matrix2;
			sphere62_model_matrix2 = rot * sphere62_model_matrix2;
			sphere63_model_matrix2 = rot * sphere63_model_matrix2;
			sphere64_model_matrix2 = rot * sphere64_model_matrix2;
			sphere65_model_matrix2 = rot * sphere65_model_matrix2;
			sphere66_model_matrix2 = rot * sphere66_model_matrix2;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			float delta_time = float(time - prev_time);
			glm::quat quaternion = glm::quat(glm::vec3(0, aux::degrees_to_radians(delta_time * 30), 0));
			glm::mat4 rot = glm::mat4_cast(quaternion);
			skybox_model_matrix = rot * skybox_model_matrix;
			
			model_matrix = rot * model_matrix;
			sphere1_model_matrix = rot * sphere1_model_matrix;
			sphere21_model_matrix = rot * sphere21_model_matrix;
			sphere22_model_matrix = rot * sphere22_model_matrix;
			sphere31_model_matrix = rot * sphere31_model_matrix;
			sphere32_model_matrix = rot * sphere32_model_matrix;
			sphere33_model_matrix = rot * sphere33_model_matrix;
			sphere41_model_matrix = rot * sphere41_model_matrix;
			sphere42_model_matrix = rot * sphere42_model_matrix;
			sphere43_model_matrix = rot * sphere43_model_matrix;
			sphere44_model_matrix = rot * sphere44_model_matrix;
			sphere51_model_matrix = rot * sphere51_model_matrix;
			sphere52_model_matrix = rot * sphere52_model_matrix;
			sphere53_model_matrix = rot * sphere53_model_matrix;
			sphere54_model_matrix = rot * sphere54_model_matrix;
			sphere55_model_matrix = rot * sphere55_model_matrix;
			sphere61_model_matrix = rot * sphere61_model_matrix;
			sphere62_model_matrix = rot * sphere62_model_matrix;
			sphere63_model_matrix = rot * sphere63_model_matrix;
			sphere64_model_matrix = rot * sphere64_model_matrix;
			sphere65_model_matrix = rot * sphere65_model_matrix;
			sphere66_model_matrix = rot * sphere66_model_matrix;

			model_matrix2 = rot * model_matrix2;
			sphere1_model_matrix2 = rot * sphere1_model_matrix2;
			sphere21_model_matrix2 = rot * sphere21_model_matrix2;
			sphere22_model_matrix2 = rot * sphere22_model_matrix2;
			sphere31_model_matrix2 = rot * sphere31_model_matrix2;
			sphere32_model_matrix2 = rot * sphere32_model_matrix2;
			sphere33_model_matrix2 = rot * sphere33_model_matrix2;
			sphere41_model_matrix2 = rot * sphere41_model_matrix2;
			sphere42_model_matrix2 = rot * sphere42_model_matrix2;
			sphere43_model_matrix2 = rot * sphere43_model_matrix2;
			sphere44_model_matrix2 = rot * sphere44_model_matrix2;
			sphere51_model_matrix2 = rot * sphere51_model_matrix2;
			sphere52_model_matrix2 = rot * sphere52_model_matrix2;
			sphere53_model_matrix2 = rot * sphere53_model_matrix2;
			sphere54_model_matrix2 = rot * sphere54_model_matrix2;
			sphere55_model_matrix2 = rot * sphere55_model_matrix2;
			sphere61_model_matrix2 = rot * sphere61_model_matrix2;
			sphere62_model_matrix2 = rot * sphere62_model_matrix2;
			sphere63_model_matrix2 = rot * sphere63_model_matrix2;
			sphere64_model_matrix2 = rot * sphere64_model_matrix2;
			sphere65_model_matrix2 = rot * sphere65_model_matrix2;
			sphere66_model_matrix2 = rot * sphere66_model_matrix2;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			float delta_time = float(time - prev_time);
			glm::quat quaternion = glm::quat(glm::vec3(aux::degrees_to_radians(delta_time * 30), 0, 0));
			glm::mat4 rot = glm::mat4_cast(quaternion);
			skybox_model_matrix = rot * skybox_model_matrix;
			
			model_matrix = rot * model_matrix;
			sphere1_model_matrix = rot * sphere1_model_matrix;
			sphere21_model_matrix = rot * sphere21_model_matrix;
			sphere22_model_matrix = rot * sphere22_model_matrix;
			sphere31_model_matrix = rot * sphere31_model_matrix;
			sphere32_model_matrix = rot * sphere32_model_matrix;
			sphere33_model_matrix = rot * sphere33_model_matrix;
			sphere41_model_matrix = rot * sphere41_model_matrix;
			sphere42_model_matrix = rot * sphere42_model_matrix;
			sphere43_model_matrix = rot * sphere43_model_matrix;
			sphere44_model_matrix = rot * sphere44_model_matrix;
			sphere51_model_matrix = rot * sphere51_model_matrix;
			sphere52_model_matrix = rot * sphere52_model_matrix;
			sphere53_model_matrix = rot * sphere53_model_matrix;
			sphere54_model_matrix = rot * sphere54_model_matrix;
			sphere55_model_matrix = rot * sphere55_model_matrix;
			sphere61_model_matrix = rot * sphere61_model_matrix;
			sphere62_model_matrix = rot * sphere62_model_matrix;
			sphere63_model_matrix = rot * sphere63_model_matrix;
			sphere64_model_matrix = rot * sphere64_model_matrix;
			sphere65_model_matrix = rot * sphere65_model_matrix;
			sphere66_model_matrix = rot * sphere66_model_matrix;

			model_matrix2 = rot * model_matrix2;
			sphere1_model_matrix2 = rot * sphere1_model_matrix2;
			sphere21_model_matrix2 = rot * sphere21_model_matrix2;
			sphere22_model_matrix2 = rot * sphere22_model_matrix2;
			sphere31_model_matrix2 = rot * sphere31_model_matrix2;
			sphere32_model_matrix2 = rot * sphere32_model_matrix2;
			sphere33_model_matrix2 = rot * sphere33_model_matrix2;
			sphere41_model_matrix2 = rot * sphere41_model_matrix2;
			sphere42_model_matrix2 = rot * sphere42_model_matrix2;
			sphere43_model_matrix2 = rot * sphere43_model_matrix2;
			sphere44_model_matrix2 = rot * sphere44_model_matrix2;
			sphere51_model_matrix2 = rot * sphere51_model_matrix2;
			sphere52_model_matrix2 = rot * sphere52_model_matrix2;
			sphere53_model_matrix2 = rot * sphere53_model_matrix2;
			sphere54_model_matrix2 = rot * sphere54_model_matrix2;
			sphere55_model_matrix2 = rot * sphere55_model_matrix2;
			sphere61_model_matrix2 = rot * sphere61_model_matrix2;
			sphere62_model_matrix2 = rot * sphere62_model_matrix2;
			sphere63_model_matrix2 = rot * sphere63_model_matrix2;
			sphere64_model_matrix2 = rot * sphere64_model_matrix2;
			sphere65_model_matrix2 = rot * sphere65_model_matrix2;
			sphere66_model_matrix2 = rot * sphere66_model_matrix2;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			float delta_time = float(time - prev_time);
			glm::quat quaternion = glm::quat(glm::vec3(-1 * aux::degrees_to_radians(delta_time * 30), 0, 0));
			glm::mat4 rot = glm::mat4_cast(quaternion);
			skybox_model_matrix = rot * skybox_model_matrix;
			
			model_matrix = rot * model_matrix;
			sphere1_model_matrix = rot * sphere1_model_matrix;
			sphere21_model_matrix = rot * sphere21_model_matrix;
			sphere22_model_matrix = rot * sphere22_model_matrix;
			sphere31_model_matrix = rot * sphere31_model_matrix;
			sphere32_model_matrix = rot * sphere32_model_matrix;
			sphere33_model_matrix = rot * sphere33_model_matrix;
			sphere41_model_matrix = rot * sphere41_model_matrix;
			sphere42_model_matrix = rot * sphere42_model_matrix;
			sphere43_model_matrix = rot * sphere43_model_matrix;
			sphere44_model_matrix = rot * sphere44_model_matrix;
			sphere51_model_matrix = rot * sphere51_model_matrix;
			sphere52_model_matrix = rot * sphere52_model_matrix;
			sphere53_model_matrix = rot * sphere53_model_matrix;
			sphere54_model_matrix = rot * sphere54_model_matrix;
			sphere55_model_matrix = rot * sphere55_model_matrix;
			sphere61_model_matrix = rot * sphere61_model_matrix;
			sphere62_model_matrix = rot * sphere62_model_matrix;
			sphere63_model_matrix = rot * sphere63_model_matrix;
			sphere64_model_matrix = rot * sphere64_model_matrix;
			sphere65_model_matrix = rot * sphere65_model_matrix;
			sphere66_model_matrix = rot * sphere66_model_matrix;
			
			model_matrix2 = rot * model_matrix2;
			sphere1_model_matrix2 = rot * sphere1_model_matrix2;
			sphere21_model_matrix2 = rot * sphere21_model_matrix2;
			sphere22_model_matrix2 = rot * sphere22_model_matrix2;
			sphere31_model_matrix2 = rot * sphere31_model_matrix2;
			sphere32_model_matrix2 = rot * sphere32_model_matrix2;
			sphere33_model_matrix2 = rot * sphere33_model_matrix2;
			sphere41_model_matrix2 = rot * sphere41_model_matrix2;
			sphere42_model_matrix2 = rot * sphere42_model_matrix2;
			sphere43_model_matrix2 = rot * sphere43_model_matrix2;
			sphere44_model_matrix2 = rot * sphere44_model_matrix2;
			sphere51_model_matrix2 = rot * sphere51_model_matrix2;
			sphere52_model_matrix2 = rot * sphere52_model_matrix2;
			sphere53_model_matrix2 = rot * sphere53_model_matrix2;
			sphere54_model_matrix2 = rot * sphere54_model_matrix2;
			sphere55_model_matrix2 = rot * sphere55_model_matrix2;
			sphere61_model_matrix2 = rot * sphere61_model_matrix2;
			sphere62_model_matrix2 = rot * sphere62_model_matrix2;
			sphere63_model_matrix2 = rot * sphere63_model_matrix2;
			sphere64_model_matrix2 = rot * sphere64_model_matrix2;
			sphere65_model_matrix2 = rot * sphere65_model_matrix2;
			sphere66_model_matrix2 = rot * sphere66_model_matrix2;
		}

		//RESET SCENE
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			skybox_model_matrix = aux::mat4_identity;

			model_matrix = glm::translate(aux::mat4_identity, glm::vec3(-0.8f, 0.0f, 0.0f));
			sphere_model_matrix = glm::translate(aux::mat4_identity, glm::vec3(-0.8f, 0.0f, 0.0f));

			model_matrix2 = glm::translate(aux::mat4_identity, glm::vec3(0.8f, 0.0f, 0.0f));
			sphere_model_matrix2 = glm::translate(aux::mat4_identity, glm::vec3(0.8f, 0.0f, 0.0f));

			model_matrix = glm::rotate(model_matrix, aux::degrees_to_radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_matrix2 = glm::rotate(model_matrix2, aux::degrees_to_radians(14.3f), glm::vec3(0.0f, 0.0f, 1.0f));

			sphere_model_matrix = glm::rotate(sphere_model_matrix, aux::degrees_to_radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			sphere_model_matrix2 = glm::rotate(sphere_model_matrix2, aux::degrees_to_radians(14.3f), glm::vec3(0.0f, 0.0f, 1.0f));

			sphere_model_matrix = glm::scale(sphere_model_matrix, glm::vec3(0.1f, 0.1f, 0.1f));
			sphere_model_matrix2 = glm::scale(sphere_model_matrix2, glm::vec3(0.1f, 0.1f, 0.1f));

			sphere1_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(0.0f, 0.0f, 6.0f));
			sphere21_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(6.0f, 3.0f, -3.0f));
			sphere22_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(6.0f, -3.0f, 3.0f));
			sphere31_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 6.0f, -3.0f));
			sphere32_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(0.0f, 6.0f, 0.0f));
			sphere33_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 6.0f, 3.0f));
			sphere41_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -6.0f, 3.0f));
			sphere42_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -6.0f, -3.0f));
			sphere43_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -6.0f, -3.0f));
			sphere44_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -6.0f, 3.0f));
			sphere51_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, -3.0f, 3.0f));
			sphere52_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, -3.0f, -3.0f));
			sphere53_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 3.0f, 3.0f));
			sphere54_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 3.0f, -3.0f));
			sphere55_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-6.0f, 0.0f, 0.0f));
			sphere61_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, -3.0f, -6.0f));
			sphere62_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 0.0f, -6.0f));
			sphere63_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(-3.0f, 3.0f, -6.0f));
			sphere64_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, -3.0f, -6.0f));
			sphere65_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 0.0f, -6.0f));
			sphere66_model_matrix = glm::translate(sphere_model_matrix, glm::vec3(3.0f, 3.0f, -6.0f));

			sphere1_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(0.0f, 0.0f, 6.0f));
			sphere21_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(6.0f, 3.0f, -3.0f));
			sphere22_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(6.0f, -3.0f, 3.0f));
			sphere31_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 6.0f, -3.0f));
			sphere32_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(0.0f, 6.0f, 0.0f));
			sphere33_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 6.0f, 3.0f));
			sphere41_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -6.0f, 3.0f));
			sphere42_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -6.0f, -3.0f));
			sphere43_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -6.0f, -3.0f));
			sphere44_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -6.0f, 3.0f));
			sphere51_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, -3.0f, 3.0f));
			sphere52_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, -3.0f, -3.0f));
			sphere53_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 3.0f, 3.0f));
			sphere54_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 3.0f, -3.0f));
			sphere55_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-6.0f, 0.0f, 0.0f));
			sphere61_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, -3.0f, -6.0f));
			sphere62_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 0.0f, -6.0f));
			sphere63_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(-3.0f, 3.0f, -6.0f));
			sphere64_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, -3.0f, -6.0f));
			sphere65_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 0.0f, -6.0f));
			sphere66_model_matrix2 = glm::translate(sphere_model_matrix2, glm::vec3(3.0f, 3.0f, -6.0f));
			
			projection_info[0].fov = aux::degrees_to_radians(45.0f);
			projection_matrix = glm::perspective(projection_info[0].fov, projection_info[0].aspect_ratio, projection_info[0].near, projection_info[0].far);

			orbit = false;
		}

		//ZOOM IN AND OUT ZOOMZOOM
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && projection_info[0].fov < 2.5) {
			projection_info[0].fov += aux::degrees_to_radians(1.0f);
			projection_matrix = glm::perspective(projection_info[0].fov, projection_info[0].aspect_ratio, projection_info[0].near, projection_info[0].far);
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && projection_info[0].fov > 0.1) {
			projection_info[0].fov -= aux::degrees_to_radians(1.0f);
			projection_matrix = glm::perspective(projection_info[0].fov, projection_info[0].aspect_ratio, projection_info[0].near, projection_info[0].far);
		}

		prev_time = time;
	}
	
	//CLEAN-UP ON AISLE 4

	//CLEAR DIE BUFFERS
	glBindVertexArray(die_vao);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(4);
	glDeleteVertexArrays(1, &die_vao);

	glDeleteBuffers(1, &die_vertex_vbo);
    glDeleteBuffers(1, &die_color_vbo);
    glDeleteBuffers(1, &die_normals_vbo);
    glDeleteBuffers(1, &die_i_vbo);

	//CLEAR SKYBOX BUFFERS
	glBindVertexArray(skybox_vao);
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &skybox_vao);
	
	glDeleteBuffers(1, &skybox_vbo);

	//CLEAR SPHERE BUFFERS
	glBindVertexArray(sphere_vao);
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &sphere_vao);

	glDeleteBuffers(1, &sphere_vertex_vbo);
	glDeleteBuffers(1, &sphere_color_vbo);
	glDeleteBuffers(1, &sphere_i_vbo);

	//DELETE SHADERS
	glDeleteShader(die_vs);
	glDeleteShader(die_fs);
	glDeleteShader(skybox_vs);
	glDeleteShader(skybox_fs);
	glDeleteShader(sphere_vs);
	glDeleteShader(sphere_fs);

	glDeleteProgram(die_shader);
	glDeleteProgram(skybox_shader);
	glDeleteProgram(sphere_shader);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
