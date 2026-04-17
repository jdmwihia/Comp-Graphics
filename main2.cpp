#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <cstdio>

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
// PADDING & SCALING
// =======================
float padding = 0.15f;

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
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

// =======================
// SHAPE HELPERS
// =======================
void drawBox(float x, float y, float s) {
    glBegin(GL_QUADS);
        glVertex2f(x - s, y - s); glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s); glVertex2f(x - s, y + s);
    glEnd();
}

void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * M_PI * i / 30.0f;
        glVertex2f(x + cos(angle) * r, y + sin(angle) * r);
    }
    glEnd();
}

// =======================
// AXES + TICKS
// =======================
void drawAxes() {
    glColor3f(0, 0, 0);
    glLineWidth(1.0f); 
    float x0 = -1.0f + padding;
    float y0 = -1.0f + padding;
    float x1 =  1.0f - padding;
    float y1 =  1.0f - padding;

    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y0); // X-axis
        glVertex2f(x0, y0); glVertex2f(x0, y1); // Y-axis
    glEnd();

    drawText(-0.95f, 0.92f, "Earnings (Ksh)");
    drawText(0.92f, -0.95f, "Days");

    for (int i = 0; i < n; i++) {
        drawText(getX(i) - 0.05f, -0.95f, days[i]);
    }

    int ticks[] = {600, 700, 800, 900, 1000};
    for (int i = 0; i < 5; i++) {
        float y = scaleY(ticks[i]);
        glBegin(GL_LINES);
            glVertex2f(x0 - 0.02f, y); glVertex2f(x0 + 0.02f, y);
        glEnd();
        char buffer[16];
        sprintf(buffer, "%d", ticks[i]);
        drawText(x0 - 0.15f, y - 0.02f, buffer);
    }
}

// =======================
// MODES
// =======================

// CASE 1: Bold Line + Small Solid Boxes
void drawMode1() {
    glColor3f(0, 0, 0);
    glLineWidth(2.0f); 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    
    for (int i = 0; i < n; i++) drawBox(getX(i), scaleY(earnings[i]), 0.025f);
}

// CASE 2: Bold line + Slightly Larger Boxes 
void drawMode2() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();

    for (int i = 0; i < n; i++) drawBox(getX(i), scaleY(earnings[i]), 0.035f);
}

// CASE 3: Colored Graph with Bold Line + Circles
void drawMode3() {
    glColor3f(0.0f, 0.0f, 1.0f); 
    glLineWidth(2.0f); 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f); 
    for (int i = 0; i < n; i++) drawCircle(getX(i), scaleY(earnings[i]), 0.03f);
}

// =======================
// CORE LOGIC
// =======================
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) mode = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) mode = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) mode = 3;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void render() {
    if (mode == 3) glClearColor(1.0f, 0.992f, 0.816f, 1.0f); 
    else glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 

    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    
    switch (mode) {
        case 1: drawMode1(); break;
        case 2: drawMode2(); break;
        case 3: drawMode3(); break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Earnings Visualization", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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