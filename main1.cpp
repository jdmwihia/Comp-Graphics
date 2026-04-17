#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// DATA

int mode = 1;
float earnings[] = {590, 850, 940, 1070, 800, 1020};
const int n = 6;
const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

float minVal = 590;
float maxVal = 1070;
float padding = 0.15f;

// Aspect ratio for shape correction
float aspectRatio = 1.0f;


// SCALING

float scaleY(float value) {
    float norm = (value - minVal) / (maxVal - minVal);
    return (-1.0f + padding) + norm * (2.0f - 2 * padding);
}

float getX(int i) {
    return (-1.0f + padding) + (2.0f - 2 * padding) * i / (n - 1);
}


// TEXT

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}


// AXES + TICKS

void drawAxes() {
    glColor3f(0, 0, 0);
    float x0 = -1.0f + padding;
    float y0 = -1.0f + padding;
    float x1 =  1.0f - padding;
    float y1 =  1.0f - padding;

    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y0); // X-axis
        glVertex2f(x0, y0); glVertex2f(x0, y1); // Y-axis
    glEnd();

    drawText(-0.95f, 0.92f, "Earnings (Ksh)");
    drawText(0.88f, -0.95f, "Days"); 

    for (int i = 0; i < n; i++) {
        drawText(getX(i) - 0.05f / aspectRatio, -0.95f, days[i]);
    }

    int ticks[] = {600, 700, 800, 900, 1000};
    for (int i = 0; i < 5; i++) {
        float y = scaleY((float)ticks[i]);
        glBegin(GL_LINES);
            glVertex2f(x0 - 0.02f / aspectRatio, y); glVertex2f(x0 + 0.02f / aspectRatio, y);
        glEnd();
        char buffer[16];
        sprintf(buffer, "%d", ticks[i]);
        drawText(x0 - 0.18f / aspectRatio, y - 0.02f, buffer);
    }
}


// SHAPE HELPERS

void drawAsterisk(float x, float y, float size) {
    float sx = size / aspectRatio; // Scale horizontal component
    glBegin(GL_LINES);
        glVertex2f(x - sx, y); glVertex2f(x + sx, y);
        glVertex2f(x, y - size); glVertex2f(x, y + size);
        glVertex2f(x - sx, y - size); glVertex2f(x + sx, y + size);
        glVertex2f(x - sx, y + size); glVertex2f(x + sx, y - size);
    glEnd();
}

void drawBox(float x, float y, float s) {
    float sx = s / aspectRatio;
    glBegin(GL_QUADS);
        glVertex2f(x - sx, y - s); glVertex2f(x + sx, y - s);
        glVertex2f(x + sx, y + s); glVertex2f(x - sx, y + s);
    glEnd();
}

void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * M_PI * i / 30.0f;
        glVertex2f(x + (cos(angle) * r) / aspectRatio, y + sin(angle) * r);
    }
    glEnd();
}


// MODES

void drawMode1() {
    glColor3f(0, 0, 0);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    for (int i = 0; i < n; i++) drawAsterisk(getX(i), scaleY(earnings[i]), 0.03f);
}

void drawMode2() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    glLineWidth(1.0f);
    for (int i = 0; i < n; i++) drawBox(getX(i), scaleY(earnings[i]), 0.03f);
}

void drawMode3() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < n; i++) drawCircle(getX(i), scaleY(earnings[i]), 0.03f);
}


// SYSTEM CALLBACKS

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (height == 0) height = 1;
    aspectRatio = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Maintain coordinate system from -1 to 1
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

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
    glLoadIdentity();
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Earnings Graph Implementaion 1", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Initial call to set up the projection/aspect ratio
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    framebuffer_size_callback(window, w, h);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}