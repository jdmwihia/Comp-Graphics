#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =======================
// DATA
// =======================
int mode = 1;

float earnings[] = {590, 850, 940, 1070, 800, 1020};
const int n = 6;

float minVal = 590;
float maxVal = 1070;

// =======================
// PADDING
// =======================
float padding = 0.12f;

// =======================
// SCALING
// =======================
float scaleY(float value) {
    float norm = (value - minVal) / (maxVal - minVal);
    return (-1.0f + padding) + norm * (2.0f - 2 * padding);
}

float getX(int i) {
    return (-1.0f + padding) + (2.0f - 2 * padding) * i / (n - 1);
}

// =======================
// AXES
// =======================
void drawAxes() {
    glColor3f(0, 0, 0);

    glBegin(GL_LINES);
        glVertex2f(-1.0f + padding, -1.0f + padding);
        glVertex2f( 1.0f - padding, -1.0f + padding);

        glVertex2f(-1.0f + padding, -1.0f + padding);
        glVertex2f(-1.0f + padding,  1.0f - padding);
    glEnd();
}

// =======================
// MODE 1
// =======================
void drawMode1() {
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex2f(getX(i), scaleY(earnings[i]));
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        float x = getX(i);
        float y = scaleY(earnings[i]);
        float s = 0.03f;

        glBegin(GL_LINES);
            glVertex2f(x - s, y);
            glVertex2f(x + s, y);

            glVertex2f(x, y - s);
            glVertex2f(x, y + s);
        glEnd();
    }
}

// =======================
// MODE 2
// =======================
void drawBox(float x, float y, float size) {
    glBegin(GL_QUADS);
        glVertex2f(x - size, y - size);
        glVertex2f(x + size, y - size);
        glVertex2f(x + size, y + size);
        glVertex2f(x - size, y + size);
    glEnd();
}

void drawMode2() {
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex2f(getX(i), scaleY(earnings[i]));
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        drawBox(getX(i), scaleY(earnings[i]), 0.03f);
    }
}

// =======================
// MODE 3
// =======================
void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (int i = 0; i <= 20; i++) {
        float angle = 2 * M_PI * i / 20;
        glVertex2f(x + cos(angle) * r, y + sin(angle) * r);
    }

    glEnd();
}

void drawMode3() {
    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex2f(getX(i), scaleY(earnings[i]));
    }
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < n; i++) {
        drawCircle(getX(i), scaleY(earnings[i]), 0.03f);
    }
}

// =======================
// INPUT
// =======================
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) mode = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) mode = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) mode = 3;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// =======================
// RENDER
// =======================
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    switch (mode) {
        case 1: drawMode1(); break;
        case 2: drawMode2(); break;
        case 3: drawMode3(); break;
    }
}

// =======================
// MAIN
// =======================
int main() {
    glfwInit();

    // MUST be compatibility profile for glBegin
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Earnings Graph", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    // FIX 1: projection reset (critical for visibility)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // FIX 2: background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // FIX 3: viewport
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}