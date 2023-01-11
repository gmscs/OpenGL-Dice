/*
 * By Guilherme Serpa, 82078
 *
*/

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <sstream>
#include "glm/ext.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace aux {

//I have bad enough headaches already without having to think about Win vs Linux
//or C++ versioning. I'm putting Pi here
const double PI = 3.141592653589793238462643383279502884;
glm::mat3 mat3_identity(1.0);
glm::mat4 mat4_identity(1.0);

class sstream : public std::stringstream
{
    public:
    sstream() {
        precision(6);
    }
};

std::string to_string(glm::vec1 v)
{
    std::stringstream stream;

    stream << "[" << v[0] << "]";
    return stream.str();
}

std::string to_string(glm::vec2 v)
{
    std::stringstream stream;

    stream << "[" << v[0] << ", " << v[1] << "]";
    return stream.str();
}

std::string to_string(glm::vec3 v)
{
    std::stringstream stream;

    stream << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    return stream.str();
}

std::string to_string(glm::vec4 v)
{
    std::stringstream stream;

    stream << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "]";
    return stream.str();
}

std::string to_string(glm::quat q)
{
    sstream stream;

    stream << "(" << q[3] << ", {" << q[0] << ", " << q[1] << ", " << q[2] << "})";
    return stream.str();
}

float angle_between(glm::vec2 v1, glm::vec2 v2)
{
    glm::vec2 normalized_v1 = glm::normalize(v1);
    glm::vec2 normalized_v2 = glm::normalize(v2);

    float dot = glm::dot(normalized_v1, normalized_v2);
    return std::acos(dot);
}

float angle_between(glm::vec3 v1, glm::vec3 v2)
{
    glm::vec3 normalized_v1 = glm::normalize(v1);
    glm::vec3 normalized_v2 = glm::normalize(v2);

    float dot = glm::dot(normalized_v1, normalized_v2);
    return std::acos(dot);
}

glm::vec3 rand_int_vec3(int min, int max)
{
    glm::vec3 v;
    int i = 0;
    // Always call at start of main() but here so i dont forget
    // srand((unsigned) time(NULL));
    while(i < 3) {
        int val = min + rand() % max;
        v[i] = val;
        i++;
    }
    return v;
}

glm::vec3 rand_float_vec3(int min, int max)
{
    glm::vec3 v;
    int i = 0;
    // Always call at start of main() but here so i dont forget
    // srand((unsigned) time(NULL));
    while(i < 3) {
        float val = min + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(max-min));
        v[i] = val;
        i++;
    }
    return v;
}

glm::mat3 rand_int_mat3(int min, int max)
{
    glm::mat3 m;
    int i = 0;
    int j = 0;
    // Always call at start of main() but here so i dont forget
    // srand((unsigned) time(NULL));
    while(i < 3) {
        while (j < 3) {
            int val = min + rand() % max;
            m[i][j] = val;
            j++;
        }
        j = 0;
        i++;
    }
    return m;
}

glm::mat3 rand_double_mat3(int min, int max)
{
    glm::mat3 m;
    int i = 0;
    int j = 0;
    // Always call at start of main() but here so i dont forget
    // srand((unsigned) time(NULL));
    while(i < 3) {
        while (j < 3) {
            double val = min + static_cast <double> (rand()) / static_cast <double> (RAND_MAX/(max-min));
            m[i][j] = val;
            j++;
        }
        j = 0;
        i++;
    }
    return m;
}

glm::mat4 get_projection_matrix(float near, float far, float fov, float width, float height)
{
    glm::mat4 proj((GLfloat)0.0);
    float aspect_ratio = width / height;
    proj[0][0] = (GLfloat)(1.0f / tanf(fov/2)) / aspect_ratio;
    proj[1][1] = (GLfloat)(1.0f / tanf(fov/2));
    proj[2][2] = (GLfloat)(-far - near) / (far - near);
    proj[3][2] = (GLfloat)-1.0;
    proj[2][3] = (GLfloat)(-2 * far * near) / (far - near);
    return proj;
}

glm::mat4 get_view_matrix(glm::vec3 camera_position, float yaw, glm::vec3 axis)
{
    glm::mat4 translation = glm::translate(mat4_identity, camera_position);
    glm::mat4 rotation = glm::rotate(mat4_identity, yaw, axis);
    glm::mat4 view = rotation * translation;
    return view;
}

//I'm accepting parameters only as floating-points for the sake of simplicity
template <typename T>
T radians_to_degrees(T radians)
{
    assert((void("radians_to_degrees only accepts floating point parameters"), std::numeric_limits<T>::is_iec559));
    return radians * (180/PI);
}

template <typename T>
T degrees_to_radians(T degrees)
{
    assert((void("degrees_to_radians only accepts floating point parameters"), std::numeric_limits<T>::is_iec559));
    return degrees * (PI/180);
}

void print_mat4(glm::mat4 m)
{
    for(int i = 0; i < 4; i++) {
        std::cout << to_string(glm::row(m, i));
        std::cout << "\n";
    }
}

float get_aspect_ratio(int width, int height) {
	return width/height;
}

//bool load_obj_file(const char* file, )

}//namespace_aux
