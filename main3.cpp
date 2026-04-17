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
float earnings[] = {590, 850, 940, 1070, 800, 1020};
const int n = 6;
const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

float minVal = 590;
float maxVal = 1070;
float padding = 0.15f;

// =======================
// HELPERS
// =======================
float scaleY(float value) {
    float norm = (value - minVal) / (maxVal - minVal);
    return (-1.0f + padding) + norm * (2.0f - 2 * padding);
}

float getX(int i) {
    return (-1.0f + padding) + (2.0f - 2 * padding) * i / (n - 1);
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
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
// AXES
// =======================
void drawAxes() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(1.0f); 
    float x0 = -1.0f + padding;
    float y0 = -1.0f + padding;
    float x1 =  1.0f - padding;
    float y1 =  1.0f - padding;

    glBegin(GL_LINES);
        glVertex2f(x0, y0); glVertex2f(x1, y0); 
        glVertex2f(x0, y0); glVertex2f(x0, y1); 
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
// CALLBACKS
// =======================
// This function runs every time you resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Ensure we don't divide by zero
    if (height == 0) height = 1;
    
    // Update the viewport to the new window dimensions
    glViewport(0, 0, width, height);

    // Reset the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Maintain a coordinate system from -1 to 1
    // This ensures the graph stays centered and scales
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

// =======================
// CORE RENDERING
// =======================
void render() {
    glClearColor(1.0f, 0.992f, 0.816f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    // Line - Blue
    glColor3f(0.0f, 0.0f, 1.0f); 
    glLineWidth(2.5f); 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex2f(getX(i), scaleY(earnings[i]));
    }
    glEnd();

    // Points - Red Circles
    glColor3f(1.0f, 0.0f, 0.0f); 
    for (int i = 0; i < n; i++) {
        drawCircle(getX(i), scaleY(earnings[i]), 0.025f);
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Earnings Graph Implementation 3", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    
    // Register the resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Enable Anti-aliasing for smoother lines/circles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    // Set initial viewport and projection
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    framebuffer_size_callback(window, screenWidth, screenHeight);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}