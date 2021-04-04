#ifndef CVRP_DRAWGL_H
#define CVRP_DRAWGL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <MSI/AntSystem/Tour.h>

namespace msi::cvrp {

class GraphElements {
public:
    // struct VertGL {
    //     float x, y;
    //     float r, g, b;
    // };
    // std::vector<VertGL> point;
    // std::vector<VertGL> line;
    struct {
        float x, y;
        float r, g, b;
    } point[51];
    struct {
        float x, y;
        float r, g, b;
    } line[198];
    void translate_vert_into_edges(msi::ant_system::Graph& g);
};

class Opengl {
public:
    void draw(msi::cvrp::GraphElements& ge);
    static const char* vertex_shader_text;
    static const char* fragment_shader_text;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

using VertId = std::size_t;

}// namespace msi::cvrp

#endif //CVRP_DRAWGL_H
