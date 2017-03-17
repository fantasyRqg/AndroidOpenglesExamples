//
// Created by ranqingguo on 3/16/17.
//


#include "InstanceRenderer.h"
#include <algorithm>

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
        0.583f, 0.771f, 0.014f,
        0.609f, 0.115f, 0.436f,
        0.327f, 0.483f, 0.844f,
        0.822f, 0.569f, 0.201f,
        0.435f, 0.602f, 0.223f,
        0.310f, 0.747f, 0.185f,
        0.597f, 0.770f, 0.761f,
        0.559f, 0.436f, 0.730f,
        0.359f, 0.583f, 0.152f,
        0.483f, 0.596f, 0.789f,
        0.559f, 0.861f, 0.639f,
        0.195f, 0.548f, 0.859f,
        0.014f, 0.184f, 0.576f,
        0.771f, 0.328f, 0.970f,
        0.406f, 0.615f, 0.116f,
        0.676f, 0.977f, 0.133f,
        0.971f, 0.572f, 0.833f,
        0.140f, 0.616f, 0.489f,
        0.997f, 0.513f, 0.064f,
        0.945f, 0.719f, 0.592f,
        0.543f, 0.021f, 0.978f,
        0.279f, 0.317f, 0.505f,
        0.167f, 0.620f, 0.077f,
        0.347f, 0.857f, 0.137f,
        0.055f, 0.953f, 0.042f,
        0.714f, 0.505f, 0.345f,
        0.783f, 0.290f, 0.734f,
        0.722f, 0.645f, 0.174f,
        0.302f, 0.455f, 0.848f,
        0.225f, 0.587f, 0.040f,
        0.517f, 0.713f, 0.338f,
        0.053f, 0.959f, 0.120f,
        0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f,
        0.820f, 0.883f, 0.371f,
        0.982f, 0.099f, 0.879f
};

bool InstanceRenderer::setUpInternal() {


    float aspect = float(mEglWrapper->getWindowWidth()) / float(mEglWrapper->getWindowHeight());


    mProjectMatrix = glm::perspective(45.0f, aspect, 1.0f, 100.0f);
    mViewMatrix = glm::lookAt(
            glm::vec3(0, 0, 20), // Camera is here
            glm::vec3(0, 0, 0), // and looks here
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

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

    glGenBuffers(1, &mPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                 GL_STATIC_DRAW);

    {


        glGenBuffers(1, &mColorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data,
                     GL_STATIC_DRAW);
    }

    mPVmatLocation = glGetUniformLocation(mProgram, "PVmat");
    mTimestampLocation = glGetUniformLocation(mProgram, "timestampS");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    update(0);


    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


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


bool InstanceRenderer::renderInternal(long timestampMills) {

    static long lastTime = 0;
    static int nbFrames = 0;

    if (lastTime == 0) {
        lastTime = timestampMills;
    }

    nbFrames++;
    if (timestampMills - lastTime >= 1000) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        LOGI("%f ms/frame\n", 1000.0 / double(nbFrames));
        LOGV("%d frame/sec\n", nbFrames);
        nbFrames = 0;
        lastTime += 1000;
    }


//    glUseProgram(mTriangleProgram);
//    glBindVertexArray(mTriangleVertexArray);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//    glDrawArrays(GL_TRIANGLES, 0, 3);


    glUseProgram(mProgram);
    glBindVertexArray(mVAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glm::mat4 pv = mProjectMatrix * mViewMatrix;

    glUniformMatrix4fv(mPVmatLocation, 1, GL_FALSE, &pv[0][0]);
    glUniform1f(mTimestampLocation, GLfloat(timestampMills) / GLfloat(1e3));

//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glDrawArrays(GL_TRIANGLES,0,36)
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);


    return true;
}

void InstanceRenderer::update(long timestampNs) {
//    LOGD("update");

    int numRows;
    int numColumns;

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

        glm::mat4 rotation = glm::rotate(model, glm::radians(mAngle[i]),
                                         glm::vec3(1.0f, 0.0f, 1.0f));

        mvpBuf[i] = glm::transpose(translation * rotation * model);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);


    //read back

//    glBindBuffer(GL_ARRAY_BUFFER, mMvpVBO);
//    mvpBuf = (glm::mat4 *) glMapBufferRange(GL_ARRAY_BUFFER, 0,
//                                            sizeof(glm::mat4) * NUM_INSTANCES,
//                                            GL_MAP_READ_BIT);
//
//    for (int i = 0; i < NUM_INSTANCES; ++i) {
//        LOGD("mvp %d , %s", i, glm::to_string(mvpBuf[i]).c_str());
//    }
//
//    glUnmapBuffer(GL_ARRAY_BUFFER);
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
