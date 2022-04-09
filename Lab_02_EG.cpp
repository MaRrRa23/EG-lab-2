#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

GLuint VBO;
GLuint gWorldLocation;
GLint gScaleLocation;

const char * VertexShaderCode = "#version 330\nlayout (location = 0) in vec3 Position;\nuniform mat4 gWorld;void main(){gl_Position = gWorld * vec4(Position, 1.0);}";

const char* FragmentShaderCode = "#version 330\nout vec4 FragColor;void main(){FragColor = vec4(1.0, 0.0, 0.0, 1.0);}";

void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;

    glm::mat4x4 Matrix(
        1.0f, 0.0f, 0.0f, sinf(Scale),
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    Scale += 0.0001f;
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &Matrix[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}

void initializeWindow(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Tutorial 01");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);
    glClearColor(0.5f, 0.2f, 0.8f, 0.0f);
}

void attachShader(const char* pShaderText, GLint shaderType, GLuint shaderProgram)
{
    GLuint ShaderObj = glCreateShader(shaderType);

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
    }

    glAttachShader(shaderProgram, ShaderObj);
}

void linkProgram(GLuint shaderProgram) 
{
    glLinkProgram(shaderProgram);

    GLint Success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
    }

    glValidateProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

int main(int argc, char** argv)
{
    initializeWindow(argc, argv);

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::cout << "Error during GLEW initialization";
        return 1;
    }

    GLuint shaderProgram = glCreateProgram();
    attachShader(VertexShaderCode, GL_VERTEX_SHADER, shaderProgram);
    attachShader(FragmentShaderCode, GL_FRAGMENT_SHADER, shaderProgram);
    linkProgram(shaderProgram);

    glm::vec3 Vertices[3] = {
        { -1.0f, -1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
