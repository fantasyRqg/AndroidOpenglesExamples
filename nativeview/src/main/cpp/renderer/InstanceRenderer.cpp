//
// Created by ranqingguo on 3/16/17.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "InstanceRenderer.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>


bool InstanceRenderer::setUpInternal() {

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    GLuint vs = loadShader(mEglWrapper->getAssetManager(), "instance.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader(mEglWrapper->getAssetManager(), "instance.fs.glsl", GL_FRAGMENT_SHADER);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vs);
    glAttachShader(mProgram, fs);

    glLinkProgram(mProgram);

    glCommon::checkProgramAndPrint(mProgram);

    glDeleteShader(fs);
    glDeleteShader(vs);


    GLfloat *positions;
    GLuint *indices;

    int numIndices = esGenCube(0.1f, &positions, NULL, NULL, &indices);

    mNumIndicateds = numIndices;

    //indicates
    glGenBuffers(1, &mIndicateVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicateVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, indices, GL_STATIC_DRAW);
    free(indices);

    glGenBuffers(1, &mPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 3, positions, GL_STATIC_DRAW);
    free(positions);



    // Random color for each instance
    {
        GLubyte colors[NUM_INSTANCES][4];
        int instance;

        srandom(0);

        for (instance = 0; instance < NUM_INSTANCES; instance++) {
            colors[instance][0] = (GLubyte) (random() % 255);
            colors[instance][1] = (GLubyte) (random() % 255);
            colors[instance][2] = (GLubyte) (random() % 255);
            colors[instance][3] = 0;
        }

        glGenBuffers(1, &mColorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 4, colors, GL_STATIC_DRAW);
    }


    // Allocate storage to store MVP per instance
    {
        int instance;

        // Random angle for each instance, compute the MVP later
        for (instance = 0; instance < NUM_INSTANCES; instance++) {
            mAngle[instance] = (float) (random() % 32768) / 32767.0f * 360.0f;
        }

        glGenBuffers(1, &mMvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mMvpVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    update(0);

    return true;
}

bool InstanceRenderer::tearDownInternal() {

    glDeleteProgram(mProgram);
    glDeleteBuffers(1, &mMvpVBO);
    glDeleteBuffers(1, &mIndicateVBO);
    glDeleteBuffers(1, &mPositionVBO);
    glDeleteBuffers(1, &mColorVBO);
    glDeleteVertexArrays(1, &mVAO);
    return true;
}

bool InstanceRenderer::renderInternal(long timestampNs) {

//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);


    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(GLubyte), NULL);


    // Load each matrix row of the MVP.  Each row gets an increasing attribute location.
    glVertexAttribPointer(2 + 0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void *) NULL);
    glVertexAttribPointer(2 + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (const void *) (sizeof(GLfloat) * 4));
    glVertexAttribPointer(2 + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (const void *) (sizeof(GLfloat) * 8));
    glVertexAttribPointer(2 + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (const void *) (sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(2 + 0);
    glEnableVertexAttribArray(2 + 1);
    glEnableVertexAttribArray(2 + 2);
    glEnableVertexAttribArray(2 + 3);

    // One MVP per instance
    glVertexAttribDivisor(2 + 0, 1);
    glVertexAttribDivisor(2 + 1, 1);
    glVertexAttribDivisor(2 + 2, 1);
    glVertexAttribDivisor(2 + 3, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicateVBO);


    glDrawElementsInstanced(GL_TRIANGLES, mNumIndicateds, GL_UNSIGNED_INT, NULL, NUM_INSTANCES);


    return true;
}

void InstanceRenderer::update(long timestampNs) {
//    LOGD("update");

    float aspect = float(mEglWrapper->getWindowWidth()) / float(mEglWrapper->getWindowHeight());
    int numRows;
    int numColumns;
    glm::mat4 projMatrix = glm::perspective(60.0f, aspect, 1.0f, 20.0f);

//    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, -10),
//                                       glm::vec3(0, 0, 0),
//                                       glm::vec3(0, 1, 0));
//
//    glm::mat4 pv = projMatrix * viewMatrix;


    glBindBuffer(GL_ARRAY_BUFFER, mMvpVBO);
    glm::mat4 *mvpBuf = (glm::mat4 *) glMapBufferRange(GL_ARRAY_BUFFER, 0,
                                                       sizeof(glm::mat4) * NUM_INSTANCES,
                                                       GL_MAP_WRITE_BIT);

    numRows = (int) sqrtf(NUM_INSTANCES);

    numColumns = numRows;

    for (int i = 0; i < NUM_INSTANCES; ++i) {
        glm::mat4 model(1.0f);

        float translateX = ((float) (i % numRows) / (float) numRows) * 2.0f - 1.0f;
        float translateY = ((float) (i / numColumns) / (float) numColumns) * 2.0f - 1.0f;

        glm::mat4 translation = glm::translate(model, glm::vec3(translateX, translateY, -2.0f));

        mAngle[i] += timestampNs * 40.0f;

        if (mAngle[i] >= 360.0f) {
            mAngle[i] -= 360.0f;
        }

        glm::mat4 rotation = glm::rotate(model, mAngle[i], glm::vec3(1.0, 0.0, 1.0));

        mvpBuf[i] = projMatrix * translation * rotation * model;


        LOGD("mvp %d , %s", i, glm::to_string(mvpBuf[i]).c_str());
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

}


int InstanceRenderer::esGenCube(float scale, GLfloat **vertices, GLfloat **normals,
                                GLfloat **texCoords, GLuint **indices) {
    int i;
    int numVertices = 24;
    int numIndices = 36;

    GLfloat cubeVerts[] =
            {
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f,
                    0.5f, 0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    -0.5f, 0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
            };

    GLfloat cubeNormals[] =
            {
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
            };

    GLfloat cubeTex[] =
            {
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    1.0f, 0.0f,
                    1.0f, 1.0f,
                    0.0f, 1.0f,
                    0.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
            };

    // Allocate memory for buffers
    if (vertices != NULL) {
        *vertices = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*vertices, cubeVerts, sizeof(cubeVerts));

        for (i = 0; i < numVertices * 3; i++) {
            (*vertices)[i] *= scale;
        }
    }

    if (normals != NULL) {
        *normals = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*normals, cubeNormals, sizeof(cubeNormals));
    }

    if (texCoords != NULL) {
        *texCoords = (GLfloat *) malloc(sizeof(GLfloat) * 2 * numVertices);
        memcpy(*texCoords, cubeTex, sizeof(cubeTex));
    }


    // Generate the indices
    if (indices != NULL) {
        GLuint cubeIndices[] =
                {
                        0, 2, 1,
                        0, 3, 2,
                        4, 5, 6,
                        4, 6, 7,
                        8, 9, 10,
                        8, 10, 11,
                        12, 15, 14,
                        12, 14, 13,
                        16, 17, 18,
                        16, 18, 19,
                        20, 23, 22,
                        20, 22, 21
                };

        *indices = (GLuint *) malloc(sizeof(GLuint) * numIndices);
        memcpy(*indices, cubeIndices, sizeof(cubeIndices));
    }

    return numIndices;
}
