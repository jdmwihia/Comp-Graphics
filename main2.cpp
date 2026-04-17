// GEORGE OTIENO NGIYE ,SCT211-0427/2023
// JEREMY MWIHIA ,SCT211-0028/2023


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// GLOBAL DATA

int mode = 1;
float earnings[] = {590, 850, 940, 1070, 800, 1020};
const int n = 6;
const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

float minVal = 590;
float maxVal = 1070;
float padding = 0.15f;

// Aspect ratio tracking
float aspectRatio = 1.0f;


// SCALING LOGIC

float scaleY(float value) {
    float norm = (value - minVal) / (maxVal - minVal);
    return (-1.0f + padding) + norm * (2.0f - 2 * padding);
}

float getX(int i) {
    return (-1.0f + padding) + (2.0f - 2 * padding) * i / (n - 1);
}


// DRAWING HELPERS

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void drawBox(float x, float y, float s) {
    glBegin(GL_QUADS);
        // We scale the width of the box by aspectRatio to keep it square
        glVertex2f(x - s / aspectRatio, y - s); 
        glVertex2f(x + s / aspectRatio, y - s);
        glVertex2f(x + s / aspectRatio, y + s); 
        glVertex2f(x - s / aspectRatio, y + s);
    glEnd();
}

void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * M_PI * i / 30.0f;
        // Adjust X by aspect ratio to keep the circle round
        glVertex2f(x + (cos(angle) * r) / aspectRatio, y + sin(angle) * r);
    }
    glEnd();
}


// UI COMPONENTS

void drawAxes() {
    glColor3f(0, 0, 0);
    glLineWidth(1.5f);
    float x0 = -1.0f + padding;
    float y0 = -1.0f + padding;
    float x1 =  1.0f - padding;
    float y1 =  1.0f - padding;

    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y0); // X-axis
        glVertex2f(x0, y0); glVertex2f(x0, y1); // Y-axis
    glEnd();

    drawText(-0.95f, 0.92f, "Earnings (Ksh)");
    drawText(0.85f, -0.95f, "Days");

    for (int i = 0; i < n; i++) {
        drawText(getX(i) - 0.04f, -0.95f, days[i]);
    }

    int ticks[] = {600, 700, 800, 900, 1000};
    for (int i = 0; i < 5; i++) {
        float y = scaleY((float)ticks[i]);
        glBegin(GL_LINES);
            glVertex2f(x0 - 0.02f, y); glVertex2f(x0 + 0.02f, y);
        glEnd();
        char buffer[16];
        sprintf(buffer, "%d", ticks[i]);
        drawText(x0 - 0.18f / aspectRatio, y - 0.02f, buffer);
    }
}


// MODES

void drawMode1() {
    glColor3f(0, 0, 0);
    glLineWidth(2.0f); 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    for (int i = 0; i < n; i++) drawBox(getX(i), scaleY(earnings[i]), 0.025f);
}

void drawMode2() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();
    for (int i = 0; i < n; i++) drawBox(getX(i), scaleY(earnings[i]), 0.035f);
}

void drawMode3() {
    glColor3f(0.0f, 0.4f, 0.8f); 
    glLineWidth(2.5f); 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) glVertex2f(getX(i), scaleY(earnings[i]));
    glEnd();

    glColor3f(0.9f, 0.1f, 0.1f); 
    for (int i = 0; i < n; i++) drawCircle(getX(i), scaleY(earnings[i]), 0.03f);
}


// SYSTEM CALLBACKS

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    // Update aspect ratio for shape correction
    if (height == 0) height = 1;
    aspectRatio = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // This ensures the coordinate system stays -1 to 1 
    // but accounts for window width/height ratios
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
    if (mode == 3) glClearColor(1.0f, 0.98f, 0.9f, 1.0f); 
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
    // Glut init for text rendering
    glutInit(&argc, argv);

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Earnings Graph Implementation 2", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // Trigger initial size logic
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