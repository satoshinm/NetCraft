#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "matrix.h"
#include "util.h"

extern void render_scene(void);
extern void translate_camera_x_offset(float);

static int width;
static int height;

typedef struct {
    float transform[16];
    float viewport[4];
    float lensCenter[2];
    float h;
} EyeParameters;

static int hResolution;
static int vResolution;
static GLfloat hScreenSize;
static GLfloat vScreenSize;
static GLfloat interpupillaryDistance;
static GLfloat lensSeparationDistance;
static GLfloat eyeToScreenDistance;
static GLfloat distortionK[4];
static GLfloat chromaAbParameter[4];

static int worldFactor;

static EyeParameters left, right;

static GLuint framebuffer;
static GLuint depthrenderbuffer;
static GLuint texture;

static GLfloat scale[2];
static GLfloat scaleIn[2];

static GLuint quad_vertexbuffer;

static bool skipBarrelDistortion = false;

static struct {
    GLuint program;
    GLuint position;
    GLuint normal;
    GLuint uv;
    GLuint matrix;
    GLuint sampler;
    GLuint camera;
    GLuint timer;
    GLuint scale;
    GLuint scaleIn;
    GLuint lensCenter;
    GLuint hmdWarpParam;
    GLuint chromAbParam;
    GLuint skipBarrelDistortion;
} vr_attrib;


static void load_vr_shader() {
    GLuint program = load_program(
        "shaders/vr_vertex.glsl", "shaders/vr_fragment.glsl");
    vr_attrib.program = program;
    vr_attrib.position = glGetAttribLocation(program, "position");;
    vr_attrib.uv = -1; // unused
    vr_attrib.normal = -1; // unused
    vr_attrib.matrix = -1; // unused
    vr_attrib.sampler = glGetUniformLocation(program, "texid");
    vr_attrib.scale = glGetUniformLocation(program, "scale");
    vr_attrib.scaleIn = glGetUniformLocation(program, "scaleIn");
    vr_attrib.lensCenter = glGetUniformLocation(program, "lensCenter");
    vr_attrib.hmdWarpParam = glGetUniformLocation(program, "hmdWarpParam");
    vr_attrib.chromAbParam = glGetUniformLocation(program, "chromAbParam");
    vr_attrib.skipBarrelDistortion = glGetUniformLocation(program, "skipBarrelDistortion");
}

void init_vr(GLFWwindow *window) {
    load_vr_shader();

    glfwGetFramebufferSize(window, &hResolution, &vResolution);
    if (hResolution == 1280 && vResolution == 800) {
        // based on https://github.com/mrdoob/three.js/blob/36565aa86a44d02cdb9c8af4ba91816928180fab/examples/js/effects/OculusRiftEffect.js#L13
        // DK1
        hScreenSize = 0.14976;
        vScreenSize = 0.0936;
        interpupillaryDistance = 0.064;
        lensSeparationDistance = 0.064;
        eyeToScreenDistance = 0.041;
        distortionK[0] = 1.0;
        distortionK[1] = 0.22;
        distortionK[2] = 1.24;
        distortionK[3] = 0.0;
        chromaAbParameter[0] = 0.996;
        chromaAbParameter[1] = -0.004;
        chromaAbParameter[2] = 1.014;
        chromaAbParameter[3] = 0.0;
    } else if (hResolution == 1920 && vResolution == 1080) {
        // DK2
        hScreenSize = 0.12576;
        vScreenSize = 0.07074;
        interpupillaryDistance = 0.0635;
        lensSeparationDistance = 0.0635;
        eyeToScreenDistance = 0.041;
        distortionK[0] = 1.0;
        distortionK[1] = 0.22;
        distortionK[2] = 1.24;
        distortionK[3] = 0.0;
        chromaAbParameter[0] = 0.996;
        chromaAbParameter[1] = -0.004;
        chromaAbParameter[2] = 1.014;
        chromaAbParameter[3] = 0.0;
    } else {
        // TODO: Google Cardboard
        // https://github.com/toji/webvr.info/blob/e00559c5b8bd1bc7ad050e9be07991ccf8c986ad/samples/js/third-party/webvr-polyfill.js#L2279
        /*
  CardboardV1:
    label: 'Cardboard I/O 2014',
    fov: 40,
    interLensDistance: 0.060,
    baselineLensDistance: 0.035,
    screenLensDistance: 0.042,
    distortionCoefficients: [0.441, 0.156],
    inverseCoefficients: [-0.4410035, 0.42756155, -0.4804439, 0.5460139,
      -0.58821183, 0.5733938, -0.48303202, 0.33299083, -0.17573841,
      0.0651772, -0.01488963, 0.001559834]

  CardboardV2:
    id: 'CardboardV2',
    label: 'Cardboard I/O 2015',
    fov: 60,
    interLensDistance: 0.064,
    baselineLensDistance: 0.035,
    screenLensDistance: 0.039,
    distortionCoefficients: [0.34, 0.55],
    inverseCoefficients: [-0.33836704, -0.18162185, 0.862655, -1.2462051,
      1.0560602, -0.58208317, 0.21609078, -0.05444823, 0.009177956,
      -9.904169E-4, 6.183535E-5, -1.6981803E-6]
*/
        hScreenSize = 0.12576;
        vScreenSize = 0.07074;
        interpupillaryDistance = 0.0635;
        lensSeparationDistance = 0.0635;
        eyeToScreenDistance = 0.041;
        distortionK[0] = 1.0;
        distortionK[1] = 0.22;
        distortionK[2] = 1.24;
        distortionK[3] = 0.0;
        chromaAbParameter[0] = 0.996;
        chromaAbParameter[1] = -0.004;
        chromaAbParameter[2] = 1.014;
        chromaAbParameter[3] = 0.0;
    }

    worldFactor = 1;
    skipBarrelDistortion = false;


    // Compute aspect ratio and FOV
    float aspect = hResolution / (2.0*vResolution);

    // Fov is normally computed with:
    //   ( 2*atan2(vScreenSize,2*eyeToScreenDistance) );
    // But with lens distortion it is increased (see Oculus SDK Documentation)
    float r = -1.0 - (4 * (hScreenSize/4 - lensSeparationDistance/2) / hScreenSize);
    float distScale = (distortionK[0] + distortionK[1] * powf(r,2) + distortionK[2] * powf(r,4) + distortionK[3] * powf(r,6));
    //float fov = 2*atan2f(vScreenSize*distScale, 2*eyeToScreenDistance); // TODO: use in perspective matrix transform!

    // Compute x translation offset (h) for projection matrix
    double h = 4 * (hScreenSize/4 - interpupillaryDistance/2) / hScreenSize;

    left.h = h;
    right.h = -h;

    // Compute camera transformation matrices
    mat_translate(left.transform, -worldFactor * interpupillaryDistance/2, 0.0, 0.0);
    mat_translate(right.transform, worldFactor * interpupillaryDistance/2, 0.0, 0.0);

    // Compute Viewport
    left.viewport[0] = 0;
    left.viewport[1] = 0;
    left.viewport[2] = hResolution/2;
    left.viewport[3] = vResolution;

    right.viewport[0] = hResolution/2;
    right.viewport[1] = 0;
    right.viewport[2] = hResolution/2;
    right.viewport[3] = vResolution;

    // Distortion shader parameters
    float lensShift = 4 * (hScreenSize/4 - lensSeparationDistance/2) / hScreenSize;
    left.lensCenter[0] = lensShift;
    left.lensCenter[1] = 0.0;

    right.lensCenter[0] = -lensShift;
    right.lensCenter[1] = 0.0;

    scale[0] = 1.0/distScale;
    scale[1] = 1.0*aspect/distScale;

    scaleIn[0] = 1.0;
    scaleIn[1] = 1.0/aspect;

    // Setup offscreen framebuffer to render to, instead of directly to screen
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture);
    glfwGetFramebufferSize(window, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("error setting up framebuffer l: %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        exit(1);
    }

    // Render directly to screen by default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // The (half-)fullscreen quad's FBO
    static const GLfloat quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer_data), quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void vr_toggle_skip_barrel_distortion() {
    skipBarrelDistortion = !skipBarrelDistortion;
}

void vr_update_viewport(int width, int height) {
    left.viewport[0] = width/2 - hResolution/2;
    left.viewport[1] = height/2 - vResolution/2;
    left.viewport[2] = hResolution/2;
    left.viewport[3] = vResolution;

    right.viewport[0] = width/2;
    right.viewport[1] = height/2 - vResolution/2;
    right.viewport[2] = hResolution/2;
    right.viewport[3] = vResolution;
}

static void render_vr_eye(EyeParameters *eye) {
    // Render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(eye->viewport[0], eye->viewport[1], eye->viewport[2], eye->viewport[3]);

    // Use our shader
    glUseProgram(vr_attrib.program);

    // Bind our texture in Texture Unit
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(vr_attrib.sampler, 4);

    glUniform2fv(vr_attrib.scale, 1, scale);
    glUniform2fv(vr_attrib.scaleIn, 1, scaleIn);
    glUniform2fv(vr_attrib.lensCenter, 1, eye->lensCenter);
    glUniform4fv(vr_attrib.hmdWarpParam, 1, distortionK); // hmdWarpParam = distortionK
    glUniform4fv(vr_attrib.chromAbParam, 1, chromaAbParameter); // chromAbParam = chromaAbParameter
    glUniform1i(vr_attrib.skipBarrelDistortion, skipBarrelDistortion);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(vr_attrib.position);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    glDisableVertexAttribArray(vr_attrib.position);
}

void vr_render() {
    // left eye
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    // Translate camera by IPD to achieve 3D effect
    // TODO: is another translation needed? https://github.com/satoshinm/NetCraft/issues/90
    translate_camera_x_offset(left.h);
    render_scene();
    translate_camera_x_offset(-left.h);
    render_vr_eye(&left);

    // right eye
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    translate_camera_x_offset(right.h);
    render_scene();
    translate_camera_x_offset(-right.h);
    render_vr_eye(&right);
}
