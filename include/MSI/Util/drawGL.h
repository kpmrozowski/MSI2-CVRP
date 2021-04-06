#ifndef CVRP_DRAWGL_H
#define CVRP_DRAWGL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <cstdlib>
#include <cstdio>
#include <MSI/CVRP/CVRP.h>

namespace msi::util {

class GraphElements {
public:
    struct {
        float x, y;
        float r, g, b;
    } point[51];
    struct {
        float x, y;
        float r, g, b;
    } line[110];
    void translate_vert_into_edges(msi::cvrp::Tour& tour);
};

class Opengl {
public:
    void draw(GraphElements& ge);
    static const char* vertex_shader_text;
    static const char* fragment_shader_text;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

using VertId = std::size_t;

}// namespace msi::cvrp

#endif //CVRP_DRAWGL_H
