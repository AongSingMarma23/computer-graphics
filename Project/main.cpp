#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>

// Global Variables for Animation and Interaction
float carPosition = 0.0f;
float wheelAngle = 0.0f;
float celestialScale = 1.0f; // Renamed from moonScale to generic celestialScale
bool celestialGrowing = true;
float treeShear = 0.0f;
bool treeSwayRight = true;
int flowOffset = 0;
int windowWidth = 800;
int windowHeight = 600;

// Boat Variables
float boatPosition = 0.0f;
bool boatMoving = false;

// Day/Night Mode Toggle
bool isNight = false;

// --- 1. PRIMITIVE DRAWING ALGORITHMS ---

// Helper to set a pixel
void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// 1. DDA Line Drawing Algorithm
void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        drawPixel(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
}

// 1.5 Dashed Line Drawing Algorithm (Variant of DDA for Water)
void drawLineDashed(int x1, int y1, int x2, int y2, int shift) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        // Dash pattern: Uses modulo arithmetic with a shift for animation
        // 20 pixel cycle: 10 pixels on, 10 pixels off
        if ((i + shift) % 20 < 10) {
            drawPixel(round(x), round(y));
        }
        x += xIncrement;
        y += yIncrement;
    }
}

// 2. Bresenham's Line Drawing Algorithm
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x = x1;
    int y = y1;

    // Determine direction
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    // Swap if slope > 1 to generalize algorithm
    bool swap = false;
    if (dy > dx) {
        std::swap(dx, dy);
        swap = true;
    }

    int p = 2 * dy - dx; // Decision parameter

    for (int i = 0; i <= dx; i++) {
        drawPixel(x, y);
        if (p >= 0) {
            if (swap) x += sx; else y += sy;
            p -= 2 * dx;
        }
        if (swap) y += sy; else x += sx;
        p += 2 * dy;
    }
}

// 3. Midpoint Circle Algorithm (Using P value)
void drawCircleMidpoint(int r) {
    int x = 0;
    int y = r;
    int p = 1 - r; // Initial decision parameter P

    // Plot initial points
    while (x <= y) {
        // Plot in all 8 octants
        drawPixel(x, y);
        drawPixel(y, x);
        drawPixel(-x, y);
        drawPixel(-y, x);
        drawPixel(-x, -y);
        drawPixel(-y, -x);
        drawPixel(x, -y);
        drawPixel(y, -x);

        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * x - 2 * y + 1;
        }
    }
}

// 3.5 Filled Midpoint Circle Algorithm (For Moon/Sun/Lamps)
void drawFilledCircleMidpoint(int r) {
    int x = 0;
    int y = r;
    int p = 1 - r;

    while (x <= y) {
        glBegin(GL_LINES);
        // Horizontal lines for symmetry (Scan-line filling)
        glVertex2i(-x, y); glVertex2i(x, y);
        glVertex2i(-x, -y); glVertex2i(x, -y);
        glVertex2i(-y, x); glVertex2i(y, x);
        glVertex2i(-y, -x); glVertex2i(y, -x);
        glEnd();

        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * x - 2 * y + 1;
        }
    }
}

// --- NEW SCENE DETAILS ---

void drawGradientSky() {
    glBegin(GL_QUADS);
    if (isNight) {
        // Night Sky
        glColor3f(0.05f, 0.05f, 0.3f); // Dark Night Blue at top
        glVertex2f(-400, 300);
        glVertex2f(400, 300);
        glColor3f(0.7f, 0.4f, 0.2f); // Dusk Orange/Brown at horizon
        glVertex2f(400, -60);
        glVertex2f(-400, -60);
    } else {
        // Day Sky
        glColor3f(0.0f, 0.7f, 1.0f); // Bright Sky Blue at top
        glVertex2f(-400, 300);
        glVertex2f(400, 300);
        glColor3f(1.0f, 1.0f, 1.0f); // White/Hazy at horizon
        glVertex2f(400, -60);
        glVertex2f(-400, -60);
    }
    glEnd();

    // Stars (Only in Night Mode)
    if (isNight) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(2.0);
        glBegin(GL_POINTS);
            glVertex2i(-350, 250); glVertex2i(-200, 280); glVertex2i(0, 290);
            glVertex2i(200, 260); glVertex2i(350, 280); glVertex2i(100, 270);
            glVertex2i(-100, 250); glVertex2i(300, 220); glVertex2i(-250, 200);
        glEnd();
        glPointSize(1.0);
    }
}

void drawMountains() {
    if (isNight) {
        glColor3f(0.1f, 0.1f, 0.2f); // Dark silhouette mountains
    } else {
        glColor3f(0.2f, 0.5f, 0.2f); // Greenish mountains in day
    }

    glBegin(GL_TRIANGLES);
    glVertex2f(-400, -60); glVertex2f(-150, 180); glVertex2f(100, -60);
    glVertex2f(-100, -60); glVertex2f(200, 150); glVertex2f(500, -60);
    glVertex2f(150, -60); glVertex2f(500, 200); glVertex2f(850, -60);
    glEnd();

    // Snow on mountain peaks (Jagged/Cartoonish style)
    glColor3f(1.0f, 1.0f, 1.0f); // White snow

    // Peak 1 snow (Jagged Cap)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-150, 180); // Peak
    glVertex2f(-120, 130); // Right Base
    glVertex2f(-135, 145); // Zig
    glVertex2f(-150, 130); // Mid
    glVertex2f(-165, 145); // Zag
    glVertex2f(-180, 130); // Left Base
    glEnd();

    // Peak 2 snow (Jagged Cap)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(200, 150);
    glVertex2f(220, 110);
    glVertex2f(210, 120);
    glVertex2f(200, 110);
    glVertex2f(190, 120);
    glVertex2f(180, 110);
    glEnd();

    // Peak 3 snow (Jagged Cap)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(500, 200);
    glVertex2f(530, 150);
    glVertex2f(515, 165);
    glVertex2f(500, 150);
    glVertex2f(485, 165);
    glVertex2f(470, 150);
    glEnd();
}

void drawStreetLamp() {
    // Pole
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(-2, 0, 2, 60);

    // Light Glow (Night Only)
    if (isNight) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.9f, 0.0f, 0.4f); // Semi-transparent yellow glow
        glPushMatrix();
        glTranslatef(0, 60, 0);
        drawFilledCircleMidpoint(12); // Glow radius (larger than bulb)
        glPopMatrix();
        glDisable(GL_BLEND);
    }

    // Lamp Head (Bulb)
    if (isNight) {
        glColor3f(1.0f, 1.0f, 0.8f); // Bright yellow (ON)
    } else {
        glColor3f(0.3f, 0.3f, 0.35f); // Dark Grey/Blueish (OFF)
    }

    glPushMatrix();
    glTranslatef(0, 60, 0);
    drawFilledCircleMidpoint(6); // Filled bulb
    glPopMatrix();
}

// --- SCENE OBJECTS ---

void drawMoon() {
    glColor3f(1.0f, 1.0f, 1.0f); // White Moon

    glPushMatrix();
    glScalef(celestialScale, celestialScale, 1.0f);
    drawFilledCircleMidpoint(40);
    glPopMatrix();
}

void drawSun() {
    glColor3f(1.0f, 0.6f, 0.0f); // Orange Sun

    glPushMatrix();
    glScalef(celestialScale, celestialScale, 1.0f);
    drawFilledCircleMidpoint(40);

    // Sun rays using DDA
    glColor3f(1.0f, 0.8f, 0.0f); // Yellowish Rays
    for(int i = 0; i < 360; i += 30) {
        float rad = i * 3.14159 / 180.0;
        int x2 = cos(rad) * 60;
        int y2 = sin(rad) * 60;
        drawLineDDA(0, 0, x2, y2);
    }

    glPopMatrix();
}

// Updated to accept Color arguments
void drawBuilding(int width, int height, float r, float g, float b) {
    // Fill with simple OpenGL quad for aesthetics (background for the lines)
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(1, 1);
    glVertex2f(width-1, 1);
    glVertex2f(width-1, height-1);
    glVertex2f(1, height-1);
    glEnd();

    // Draw outline using Bresenham (White for contrast)
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLineBresenham(0, 0, width, 0);          // Bottom
    drawLineBresenham(width, 0, width, height); // Right
    drawLineBresenham(width, height, 0, height);// Top
    drawLineBresenham(0, height, 0, 0);         // Left

    // Windows using DDA
    if (isNight) {
        glColor3f(1.0f, 1.0f, 0.6f); // Warm Lit windows
    } else {
        glColor3f(0.2f, 0.3f, 0.5f); // Dark/Reflective windows in day
    }

    for(int y = 20; y < height - 20; y+=30) {
        for(int x = 10; x < width - 10; x+=20) {
            drawLineDDA(x, y, x+10, y);
            drawLineDDA(x+10, y, x+10, y+10);
            drawLineDDA(x+10, y+10, x, y+10);
            drawLineDDA(x, y+10, x, y);
        }
    }

    // Door (New detail)
    glColor3f(0.2f, 0.1f, 0.0f); // Dark Wood color
    glRectf(width/2 - 10, 0, width/2 + 10, 25);

    // Antenna (New detail)
    glColor3f(0.5f, 0.5f, 0.5f);
    drawLineBresenham(width/2, height, width/2, height+20);
    drawLineBresenham(width/2, height+20, width/2-10, height+30);
    drawLineBresenham(width/2, height+20, width/2+10, height+30);
}

void drawCar() {
    // Car Body using Bresenham
    glColor3f(0.9f, 0.2f, 0.2f); // Bright Red

    // Bottom chassis
    drawLineBresenham(-50, 0, 50, 0);
    drawLineBresenham(50, 0, 50, 20);
    drawLineBresenham(50, 20, 25, 20); // Hood
    drawLineBresenham(25, 20, 15, 40); // Windshield
    drawLineBresenham(15, 40, -40, 40); // Roof
    drawLineBresenham(-40, 40, -50, 20); // Rear window
    drawLineBresenham(-50, 20, -50, 0); // Rear

    // Fill Car Body (Optional Quad)
    glBegin(GL_QUADS);
    glVertex2f(-50, 0); glVertex2f(50, 0); glVertex2f(50, 20); glVertex2f(-50, 20);
    glVertex2f(-40, 20); glVertex2f(25, 20); glVertex2f(15, 40); glVertex2f(-40, 40);
    glEnd();

    // Headlight
    if (isNight) glColor3f(1.0f, 1.0f, 0.0f); // On
    else glColor3f(0.9f, 0.9f, 0.8f); // Off/Reflective

    glBegin(GL_POLYGON);
    glVertex2f(45, 5); glVertex2f(50, 5); glVertex2f(50, 15); glVertex2f(45, 15);
    glEnd();

    // Transformation: Rotation for wheels
    // Front Wheel
    glPushMatrix();
    glTranslatef(30, 0, 0);
    glRotatef(wheelAngle, 0, 0, 1); // Rotate wheel based on movement
    glColor3f(0.1f, 0.1f, 0.1f); // Dark tire
    drawCircleMidpoint(12);
    // Spokes
    glColor3f(0.8f, 0.8f, 0.8f); // Silver spokes
    drawLineDDA(0, 12, 0, -12);
    drawLineDDA(12, 0, -12, 0);
    glPopMatrix();

    // Rear Wheel
    glPushMatrix();
    glTranslatef(-30, 0, 0);
    glRotatef(wheelAngle, 0, 0, 1);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark tire
    drawCircleMidpoint(12);
    // Spokes
    glColor3f(0.8f, 0.8f, 0.8f); // Silver spokes
    drawLineDDA(0, 12, 0, -12);
    drawLineDDA(12, 0, -12, 0);
    glPopMatrix();
}

void drawBoat() {
    // Simple boat - bigger and simpler like car
    glColor3f(1.0f, 0.0f, 0.0f); // Brown wood color

    // Simple boat hull - bigger like car size
    glBegin(GL_QUADS);
    glVertex2f(-60, 0);   // Bottom left (bigger than car)
    glVertex2f(60, 0);    // Bottom right
    glVertex2f(50, 25);   // Top right
    glVertex2f(-50, 25);  // Top left
    glEnd();

    // Hull outline using Bresenham (like car)
    glColor3f(0.2f, 0.1f, 0.05f); // Darker brown outline
    drawLineBresenham(-60, 0, 60, 0);    // Bottom
    drawLineBresenham(60, 0, 50, 25);    // Right side
    drawLineBresenham(50, 25, -50, 25);  // Top
    drawLineBresenham(-50, 25, -60, 0);  // Left side

    // Simple mast - just a vertical line
    glColor3f(0.4f, 0.2f, 0.1f); // Medium brown
    drawLineBresenham(0, 25, 0, 70);

    // Simple rectangular sail (bigger)
    glColor3f(1.0f, 1.0f, 1.0f); // White sail
    glBegin(GL_QUADS);
    glVertex2f(5, 50);   // Bottom left of sail
    glVertex2f(40, 50);  // Bottom right of sail
    glVertex2f(40, 65);  // Top right of sail
    glVertex2f(5, 65);   // Top left of sail
    glEnd();

    // Sail outline using Bresenham
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray outline
    drawLineBresenham(5, 50, 40, 50);   // Bottom
    drawLineBresenham(40, 50, 40, 65);  // Right
    drawLineBresenham(40, 65, 5, 65);   // Top
    drawLineBresenham(5, 65, 5, 50);    // Left
}

void drawTree() {
    // Transformation: Shearing (Wind effect)
    GLfloat shearMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        treeShear, 1.0f, 0.0f, 0.0f, // x depends on y (shear in x)
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glPushMatrix();

    // Draw Trunk (Static)
    glColor3f(0.55f, 0.27f, 0.07f); // Saddle Brown
    glRectf(-5, 0, 5, 40);

    // Apply Shear only to the leaves
    glTranslatef(0, 40, 0); // Move to top of trunk
    glMultMatrixf(shearMatrix); // Apply shear
    glTranslatef(0, -40, 0); // Move back (pivot point)

    // Draw Leaves (Triangle)
    glColor3f(0.13f, 0.55f, 0.13f); // Forest Green
    glBegin(GL_TRIANGLES);
    glVertex2f(-20, 40);
    glVertex2f(20, 40);
    glVertex2f(0, 100);
    glEnd();

    // Snow on tree top (Jagged Cap)
    glColor3f(1.0f, 1.0f, 1.0f); // White snow
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 100); // Tip
    glVertex2f(8, 85);  // Right
    glVertex2f(4, 90);  // Zig
    glVertex2f(0, 85);  // Mid
    glVertex2f(-4, 90); // Zag
    glVertex2f(-8, 85); // Left
    glEnd();

    glPopMatrix();
}

void drawScene() {
    // 1. Draw Sky Gradient & Stars (or Blue Sky)
    drawGradientSky();

    // 2. Draw Moon or Sun
    glPushMatrix();
    glTranslatef(300, 200, 0);
    if (isNight) {
        drawMoon();
    } else {
        drawSun();
    }
    glPopMatrix();

    // 3. Draw Mountains
    drawMountains();

    // 4. Draw Buildings (City Skyline) with Colors
    glPushMatrix();
    glTranslatef(-350, -50, 0);
    drawBuilding(80, 200, 0.2f, 0.6f, 0.8f); // Cyan/Blue

    glTranslatef(90, 0, 0);
    drawBuilding(100, 300, 0.8f, 0.2f, 0.8f); // Magenta

    glTranslatef(110, 0, 0);
    drawBuilding(60, 150, 0.4f, 0.8f, 0.2f); // Lime Green

    glTranslatef(70, 0, 0);
    drawBuilding(120, 250, 0.9f, 0.5f, 0.1f); // Orange
    glPopMatrix();

    // 5. Draw Street Lamps
    glPushMatrix();
    glTranslatef(-200, -60, 0);
    drawStreetLamp();
    glTranslatef(150, 0, 0);
    drawStreetLamp();
    glTranslatef(150, 0, 0);
    drawStreetLamp();
    glTranslatef(150, 0, 0);
    drawStreetLamp();
    glPopMatrix();

    // 6. Draw Trees with Shearing (Wind)
    glPushMatrix();
    glTranslatef(280, -50, 0);
    drawTree();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200, -50, 0); // Extra tree
    drawTree();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(360, -50, 0); // Extra tree
    drawTree();
    glPopMatrix();

    // 7. Draw Road
    // Sidewalk/Pavement (New Detail)
    glColor3f(0.5f, 0.5f, 0.5f); // Grey Pavement
    glRectf(-400, -60, 400, -50);

    // Road surface
    glColor3f(0.3f, 0.3f, 0.35f); // Dark Grey asphalt
    glRectf(-400, -80, 400, -60);
    // Road Line
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLineBresenham(-400, -60, 400, -60);

    // 8. Draw Car (Character controlled by key)
    glPushMatrix();
    glTranslatef(carPosition, -45, 0); // Transformation: Translation (Move car)
    drawCar();
    glPopMatrix();

    // 9. Draw Boat (Animated on water)
    glPushMatrix();
    glTranslatef(boatPosition, -120, 0); // Fixed position in water, no bobbing
    drawBoat();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Background color handled by gradient
    glLoadIdentity();

    // Set view
    gluOrtho2D(-400, 400, -300, 300);

    // --- MAIN SCENE ---
    drawScene();

    // --- TRANSFORMATION: REFLECTION ---
    // Reflect the city in the "water" below y = -80
    glPushMatrix();
    glTranslatef(0, -160, 0); // Move down
    glScalef(1.0f, -0.6f, 1.0f); // Scale Y by -0.6 (Reflection + slightly squashed)

    // Draw semi-transparent rectangle to simulate water tint
    drawScene(); // Draw the whole scene again upside down

    glPopMatrix();

    // Water surface tint overlay
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (isNight) {
        glColor4f(0.0f, 0.3f, 0.7f, 0.45f); // More realistic Deep Azure Blue, slightly more opaque
    } else {
        glColor4f(0.0f, 0.5f, 0.8f, 0.3f); // Brighter water for Day
    }
    glRectf(-400, -300, 400, -80);
    glDisable(GL_BLEND);

    // Water surface flow lines (Dashed & Flowing across body)
    if (isNight) glColor3f(0.4f, 0.7f, 1.0f); // Brighter surface line
    else glColor3f(0.8f, 0.9f, 1.0f); // White lines for day

    // Draw lines every 20 pixels to create "waves" across the entire water body
    for(int y = -80; y > -300; y -= 20) {
        // Use y coordinate to offset the phase of the wave so they don't all move in perfect unison
        drawLineDashed(-400, y, 400, y, flowOffset + abs(y));
    }

    glFlush();
}

// --- ANIMATION LOGIC ---
void timer(int value) {
    // Animate Celestial Body (Subtle Pulsing)
    if(celestialGrowing) {
        celestialScale += 0.002f;
        if(celestialScale > 1.05f) celestialGrowing = false;
    } else {
        celestialScale -= 0.002f;
        if(celestialScale < 1.0f) celestialGrowing = true;
    }

    // Animate Tree (Swaying/Shearing) - SLOWER
    if(treeSwayRight) {
        treeShear += 0.002f; // Reduced from 0.01f
        if(treeShear > 0.2f) treeSwayRight = false;
    } else {
        treeShear -= 0.002f; // Reduced from 0.01f
        if(treeShear < -0.2f) treeSwayRight = true;
    }

    // Animate Water Flow (Dash Offset)
    flowOffset = (flowOffset + 1) % 20;

    // Animate Boat
    if (boatMoving) {
        boatPosition += 2.0f; // Move faster like car
        if (boatPosition > 450) boatPosition = -450;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// --- INPUT HANDLING ---
void handleSpecialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:
            carPosition += 5.0f;
            wheelAngle -= 10.0f; // Rotate wheels negative when moving right
            if (carPosition > 400) carPosition = -400;
            break;
        case GLUT_KEY_LEFT:
            carPosition -= 5.0f;
            wheelAngle += 10.0f; // Rotate wheels positive when moving left
            if (carPosition < -400) carPosition = 400;
            break;
    }
    glutPostRedisplay();
}

void handleStandardKeys(unsigned char key, int x, int y) {
    if (key == 'd' || key == 'D') isNight = false; // Switch to Day
    if (key == 'n' || key == 'N') isNight = true;  // Switch to Night
    if (key == 'b' || key == 'B') {
        boatMoving = !boatMoving; // Toggle boat movement
        if (!boatMoving) {
            // Stop the boat but keep it visible
            boatPosition = boatPosition; // Keep current position
        }
    }
    if (key == 'c' || key == 'C') {
        carPosition += 20.0f; // Move car forward when 'C' is pressed
        wheelAngle -= 20.0f;
        if (carPosition > 400) carPosition = -400;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("City Scenery - OpenGL Project");

    glutDisplayFunc(display);
    glutSpecialFunc(handleSpecialKeys); // For Arrow Keys
    glutKeyboardFunc(handleStandardKeys); // For 'D' and 'N' keys
    glutTimerFunc(0, timer, 0);  // Start Animation

    glutMainLoop();
    return 0;
}
