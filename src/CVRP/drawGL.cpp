#include <MSI/CVRP/drawGL.h>
#include <fmt/core.h>

namespace msi::cvrp {

void Vertices::translate_vert_into_edges(msi::ant_system::Graph& g) {
    int ii = 0, jj = 0;
    for(int aa = 0; aa < g.m_vert_count; aa++)
        for(int bb = 0; bb < g.m_vert_count; bb++) {
            auto edge = g.m_edges[aa * g.m_vert_count + bb];
            if (edge.has_value()) {
                this->point[ii].x = edge->pos.x1 * 2 / 70. - 1.1; // {for 0<x<10 and 0<y<10: " * 0.2 - 1; " }
                this->point[ii].y = edge->pos.y1 * 2 / 70. - 1.1;
                this->point[ii].r = 1.f;
                this->point[ii].g = 1.f;
                this->point[ii].b = 1.f;
                ii++;
                if(ii % 10 == 0) fmt::print("i={} ", ii);
                this->point[ii].x = edge->pos.x2 * 2 / 70. - 1.1;
                this->point[ii].y = edge->pos.y2 * 2 / 70. - 1.1;
                this->point[ii].r = 1.f;
                this->point[ii].g = 1.f;
                this->point[ii].b = 1.f;
                ii++;
                if(ii % 10 == 0) fmt::print("i={} ", ii);
            }
        }
    fmt::print("\n");
    for(int aa = 0; aa < g.m_vert_count; aa++)
        for(int bb = 0; bb < g.m_vert_count; bb++) {
            auto edge = g.m_edges[aa * g.m_vert_count + bb];
            if (edge.has_value() && edge->pheromone > 0.4) {
                this->line[ii].x = edge->pos.x1 * 0.2 - 1;
                this->line[ii].y = edge->pos.y1 * 0.2 - 1;
                this->line[ii].r = 0.f;
                this->line[ii].g = 1.f;
                this->line[ii].b = 0.f;
                jj++;
                if(ii % 10 == 0) fmt::print("j={} ", jj);
                this->line[ii].x = edge->pos.x2 * 0.2 - 1;
                this->line[ii].y = edge->pos.y2 * 0.2 - 1;
                this->line[ii].r = 0.f;
                this->line[ii].g = 1.f;
                this->line[ii].b = 0.f;
                jj++;
                if(ii % 10 == 0) fmt::print("j={} ", jj);
            }
        }
    fmt::print("\n");
}

void Opengl::draw(Vertices& vc) {
    // Dynamic alocation of array to be drawn
    // VertGL *pointGl;
    // pointGl = new VertGL[vc.point.size()];
    // for(int i = 0; i < vc.point.size(); i++) {
    //     pointGl[i].x = point[i].x;
    //     pointGl[i].y = point[i].y;
    //     pointGl[i].r = point[i].r;
    //     pointGl[i].g = point[i].g;
    //     pointGl[i].b = point[i].b;
    // }
    // VertGL pointgl{*pointGl};
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
    glPointSize(10);
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(vc.point), vc.point, GL_STATIC_DRAW);
        glDrawArrays(GL_POINTS, 0, sizeof(vc.point)/sizeof(vc.point[0]));
        // glLineWidth(0.5);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vc.point), vc.point, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, sizeof(vc.point)/sizeof(vc.point[0]));
        // glLineWidth(3);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vc.line), vc.line, GL_STATIC_DRAW);
        // glDrawArrays(GL_LINES, 0, sizeof(vc.line)/sizeof(vc.line[0]));
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