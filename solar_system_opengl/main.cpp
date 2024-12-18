#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

static float m=0, M=0, v=0, V=0, E=0, e=0,  j=0, J=0, s=0,r=0, R=0, S=0, U=0, u=0, n=0, N=0, p=0, P=0, z=0;
float angleX = 0.0, angleY = 0.0, zoom = -20.0;
int prevMouseX = -1, prevMouseY = -1;

GLfloat diffuseMaterial[4] = {0.8, 0.8, 0.8, 1.0};

void renderText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Enlarged and bold font
    }
}

void myinit(void)
{
    glClearColor(0.10, 0.01, 0.05, 0.0); // Darker space background
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {2.0, 2.0, 2.0, 0.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void drawOrbit(float radius)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.5, 0.5, 0.5);
    for (int i = 0; i < 360; i++)
    {
        float theta = i * (M_PI / 180);
        glVertex3f(radius * cos(theta), 0.0, radius * sin(theta));
    }
    glEnd();
}

void drawStars()
{
    glDisable(GL_LIGHTING);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 10; i++)   // Static stars with reduced blinking effect
    {
        float x = (rand() % 200 - 100) / 10.0;
        float y = (rand() % 200 - 100) / 10.0;
        float z = (rand() % 200 - 100) / 10.0;
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void display(void)
{
    GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    drawStars();

    // Sun
    glPushMatrix();
    glRotatef((GLfloat)z, 1.0, 1.0, 1.0);
    glDisable(GL_LIGHTING);

    // Bright glow effect for the Sun
    for (float i = 1.6; i <= 2.0; i += 0.1)
    {
        glColor4f(1.0, 0.9, 0.0, 0.3); // Semi-transparent yellowish glow
        glutSolidSphere(i, 40, 16);
    }

    glColor3f(1.0, 0.8, 0.0); // Core color of the Sun
    glutSolidSphere(1.5, 40, 16);
    renderText(-0.4, -2.0, "Sun");

    glEnable(GL_LIGHTING);
    glPopMatrix();



    // Planets with Orbits
    float planetDistances[] = {3.5, 5.0, 6.5, 8.0, 11.0, 13.0, 15.0, 17.0};
    float planetSizes[] = {0.2, 0.3, 0.35, 0.25, 0.8, 0.7, 0.6, 0.5};
    const char *planetNames[] = {"Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
    float planetColors[][3] = {
        {0.7, 0.7, 0.7}, // Mercury (grey)
        {0.9, 0.6, 0.2}, // Venus (yellow-orange)
        {0.1, 0.6, 0.8}, // Earth (blue-green)
        {0.8, 0.4, 0.1}, // Mars (red-orange)
        {0.9, 0.8, 0.5}, // Jupiter (beige)
        {0.9, 0.9, 0.7}, // Saturn (pale yellow)
        {0.6, 0.8, 0.9}, // Uranus (light blue)
        {0.3, 0.3, 0.8}  // Neptune (dark blue)
    };
    float *rotations[] = {&M, &V, &E, &R, &J, &S, &U, &N};
    float *selfRotations[] = {&m, &v, &e, &r, &j, &s, &u, &n};


    for (int i = 0; i < 8; i++)
    {
        drawOrbit(planetDistances[i]);
        glPushMatrix();
        glRotatef(*(rotations[i]), 0.0, 1.0, 0.0);
        glTranslatef(planetDistances[i], 0.0, 0.0);
        glRotatef(*(selfRotations[i]), 0.0, 1.0, 0.0);
        glColor3f(planetColors[i][0], planetColors[i][1], planetColors[i][2]);
        glutSolidSphere(planetSizes[i], 20, 8);
        renderText(-0.2, -0.4, planetNames[i]); // Enhanced visibility and bold text
        glPopMatrix();
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 50.0); // Adjusted for closer view
    glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
    m += 0.1f;
    M += 0.02f;
    v += 0.08f;
    V += 0.015f;
    e += 0.05f;
    E += 0.01f;
    j += 0.005f;
    J += 0.002f;
    r += 0.03f;
    R += 0.007f;
    s += 0.004f;
    S += 0.0015f;
    u += 0.003f;
    U += 0.001f;
    n += 0.002f;
    N += 0.0005f;
    z += 0.05f;

    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    if (prevMouseX >= 0 && prevMouseY >= 0)
    {
        angleY += (x - prevMouseX) * 0.15f;
        angleX += (y - prevMouseY) * 0.15f;
    }
    prevMouseX = x;
    prevMouseY = y;
    glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        zoom -= 1.0f;
    }
    else
    {
        zoom += 1.0f;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solar System");
    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseWheel);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
