#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

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

const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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
// TEXT
// =======================
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
    }
}

// =======================
// AXES + TICKS
// =======================
void drawAxes() {
    glColor3f(0, 0, 0);

    float x0 = -1.0f + padding;
    float y0 = -1.0f + padding;
    float x1 =  1.0f - padding;
    float y1 =  1.0f - padding;

    // Axes
    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y0); // X-axis
        glVertex2f(x0, y0); glVertex2f(x0, y1); // Y-axis
    glEnd();

    // =========================
    // FIX 1: Lifted Y-axis label higher
    // =========================
    drawText(-0.95f, 0.92f, "Earnings (Ksh)");
    drawText(0.75f, -0.95f, "Days");

    // X-axis labels
    for (int i = 0; i < n; i++) {
        float x = getX(i);
        drawText(x - 0.03f, -0.98f, days[i]);
    }

    // =========================
    // Y-axis ticks (NO 1100)
    // =========================
    int ticks[] = {600, 700, 800, 900, 1000};
    int tickCount = 5;

    for (int i = 0; i < tickCount; i++) {
        float val = ticks[i];

        float yNorm = (val - minVal) / (maxVal - minVal);
        float y = (-1.0f + padding) + yNorm * (2.0f - 2 * padding);

        // tick mark
        glBegin(GL_LINES);
            glVertex2f(x0 - 0.01f, y);
            glVertex2f(x0 + 0.01f, y);
        glEnd();

        // label
        char buffer[16];
        sprintf(buffer, "%d", ticks[i]);
        drawText(x0 - 0.12f, y - 0.01f, buffer);
    }
}

// =======================
// MODE 1
// =======================
void drawAsterisk(float x, float y, float size) {
    glBegin(GL_LINES);

    glVertex2f(x - size, y); glVertex2f(x + size, y);
    glVertex2f(x, y - size); glVertex2f(x, y + size);
    glVertex2f(x - size, y - size); glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size); glVertex2f(x + size, y - size);

    glEnd();
}

void drawMode1() {
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex2f(getX(i), scaleY(earnings[i]));
    }
    glEnd();

    for (int i = 0; i < n; i++) {
        drawAsterisk(getX(i), scaleY(earnings[i]), 0.025f);
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
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glfwInit();

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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