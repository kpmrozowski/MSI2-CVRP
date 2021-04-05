#include <MSI/CVRP/drawGL.h>
#include <fmt/core.h>

namespace msi::cvrp {

void GraphElements::translate_vert_into_edges(msi::ant_system::Graph& g) {
    double scale_x = 2/70.;
    double translate_x = - 1.4;
    double scale_y = scale_x;
    double translate_y = -1.05;
    for(VertId i = 0; i < g.m_vert_count+1; i++) {
        this->point[i].x = g.m_vertices[i].x * scale_x + translate_x; // {for 0<x<10 and 0<y<10: " * 0.2 - 1; " }
        this->point[i].y = g.m_vertices[i].y * scale_y + translate_y;
        this->point[i].r = 0.f;
        this->point[i].g = 1.f;
        this->point[i].b = 0.f;
    }
    std::size_t ii = 0;
    for(std::size_t aa = 0; aa < g.m_vert_count-1; aa++)
        for(std::size_t bb = 0; bb < g.m_vert_count; bb++) {
            auto edge = g.m_edges[aa * g.m_vert_count + bb];
            if (edge.has_value() && edge->pheromone > 0.15) {
                this->line[ii].x = edge->pos.x1 * scale_x + translate_x; // {for 0<x<10 and 0<y<10: " * 0.2 - 1; " }
                this->line[ii].y = edge->pos.y1 * scale_y + translate_y;
                this->line[ii].r = 1.f;
                this->line[ii].g = 1.f;
                this->line[ii].b = 1.f;
                ii++;
                // fmt::print("{} ", ii);
                this->line[ii].x = edge->pos.x2 * scale_x + translate_x;
                this->line[ii].y = edge->pos.y2 * scale_y + translate_y;
                this->line[ii].r = 1.f;
                this->line[ii].g = 1.f;
                this->line[ii].b = 1.f;
                ii++;
                // fmt::print("{} ", ii);
            }
        }
    fmt::print("\n");
}

void Opengl::draw(GraphElements& ge) {
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1280, 960, "ACO-CVRP graph", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(15);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        mat4x4_identity(m);
        // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ge.line), ge.line, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, sizeof(ge.line)/sizeof(ge.line[0]));
        glClear(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ge.point), ge.point, GL_STATIC_DRAW);
        glDrawArrays(GL_POINTS, 0, sizeof(ge.point)/sizeof(ge.point[0]));
        glClear(GL_ARRAY_BUFFER);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

const char* Opengl::vertex_shader_text =
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

const char* Opengl::fragment_shader_text =
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

void Opengl::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void Opengl::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

}// namespace msi::cvrp