
#include <windows.h>  // for MS Windows
#include <glew/glew.h>
#include <freeglut/glut.h>  // GLUT, include glu.h and gl.h
#include <vector>
#include <mys.h>
std::vector<mys::triangle3> StaticTriangles;
std::vector<mys::vec3> StaticTriangleColors;
mys::camera defaultCamera;
using mys::ShaderProgram;
using mys::ww;
using mys::wh;
char title[] = "Game 1";  // Windowed mode's title
int windowWidth = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX = 50;      // Windowed mode's top-left corner x
int windowPosY = 50;      // Windowed mode's top-left corner y

int refreshMillis = 15;      // Refresh period in milliseconds

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = false; // Full-screen or windowed mode?
bool paused = false;         // Movement paused or resumed
GLfloat xSpeedSaved, ySpeedSaved;  // To support resume

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

bool keyaisdown = false;
bool keysisdown = false;
bool keywisdown = false;
bool keydisdown = false;
bool keyrightisdown = false;
bool keyleftisdown = false;


/* Callback handler for window re-paint event */
void display() {
	if (keyaisdown)
	{
	
		defaultCamera.position.x += cos(defaultCamera.angleXZ + M_PI_2) * 10;
		defaultCamera.position.z += sin(defaultCamera.angleXZ + M_PI_2) * 10;
	}
	if (keydisdown)
	{

		defaultCamera.position.x += cos(defaultCamera.angleXZ - M_PI_2) * 10;
		defaultCamera.position.z += sin(defaultCamera.angleXZ - M_PI_2) * 10;
	}

	if (keywisdown)
	{

		defaultCamera.position.x += cos(defaultCamera.angleXZ) * 10;
		defaultCamera.position.z += sin(defaultCamera.angleXZ ) * 10;
	}

	if (keysisdown)
	{

		defaultCamera.position.x -= cos(defaultCamera.angleXZ) * 10;
		defaultCamera.position.z -= sin(defaultCamera.angleXZ) * 10;
	}


	if (keyleftisdown)
	{

		defaultCamera.angleXZ += M_PI / (float)32;
	}


	if (keyrightisdown)
	{

		defaultCamera.angleXZ -= M_PI / (float)32;
	}
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
	glLoadIdentity();              // Reset model-view matrix





//draw here
	for (int i = 0;i < StaticTriangles.size();i++) {
		mys::triangle3 T = mys::adjustTriangle3ToCamera(defaultCamera,StaticTriangles[i]);
		mys::triangle3 result1;
		mys::triangle3 result2;
		int oneOrTwoTriangles;
		mys::vec3 col = StaticTriangleColors[i];
		if (mys::triangle3CullAndClipBehindNearPlane(T,result1,result2,oneOrTwoTriangles)) {
			if (oneOrTwoTriangles == 1) {
				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result1, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom,col,result1,defaultCamera.focalDistance);
			}

			if (oneOrTwoTriangles == 2) {
			
				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result1, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom,col,result1, defaultCamera.focalDistance);
				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result2, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom,col,result2, defaultCamera.focalDistance);
			}
		}
	}



	glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)


}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) {
		clipAreaXLeft = -1.0 * aspect;
		clipAreaXRight = 1.0 * aspect;
		clipAreaYBottom = -1.0;
		clipAreaYTop = 1.0;
	}
	else {
		clipAreaXLeft = -1.0;
		clipAreaXRight = 1.0;
		clipAreaYBottom = -1.0 / aspect;
		clipAreaYTop = 1.0 / aspect;
	}
	clipAreaXLeft *= mys::ONE_METER;
	clipAreaYBottom *= mys::ONE_METER;
	clipAreaXRight *= mys::ONE_METER;
	clipAreaYTop *= mys::ONE_METER;

	gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
	glScissor(0, 0, width, height);
	glEnable(GL_SCISSOR_TEST);
	ww = width;
	wh = height;
}

/* Called back when the timer expired */
void Timer(int value) {
	glutPostRedisplay();    // Post a paint request to activate display()
	glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:     // ESC key
		exit(0);
		break;


	case 'a':  keyaisdown = true;break;
		
	case 's': keysisdown = true;break;
	case 'w':  keywisdown = true;break;
	case 'd': 
		 keydisdown = true; break;
	case 'f': fullScreenMode = !fullScreenMode;         // Toggle state
		if (fullScreenMode) {                     // Full-screen mode
			windowPosX = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
			windowPosY = glutGet(GLUT_WINDOW_Y);
			windowWidth = glutGet(GLUT_WINDOW_WIDTH);
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();                      // Switch into full screen
		}
		else {                                         // Windowed mode
			glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
			glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
		} break;


	}
	

}
/* Callback handler for normal-key event */
void keyboard2(unsigned char key, int x, int y) {
	switch (key) {
	case 27:     // ESC key
		exit(0);
		break;


	case 'a': keyaisdown = false;break;

	case 's': keysisdown = false;break;
	case 'w':  keywisdown = false;break;
	case 'd':  keydisdown = false; break;

	}

}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_RIGHT:    // Right: increase x speed
		keyrightisdown = true;
	 break;
	case GLUT_KEY_LEFT:     // Left: decrease x speed
		keyleftisdown = true;
		 break;


	}
}

/* Callback handler for special-key event */
void specialKeys2(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_RIGHT:    // Right: increase x speed
		keyrightisdown = false;
		break;
	case GLUT_KEY_LEFT:     // Left: decrease x speed
		keyleftisdown = false;
		break;


	}
}

/* Callback handler for mouse event */
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
		/*
		paused = !paused;         // Toggle state
		if (paused) {
		
		}
		else {
			
		}*/
	}
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {


	defaultCamera = mys::camera{ mys::vec3{0.0,0.0,0.0},(float)M_PI_2,0.0,500 };
	
	StaticTriangles.push_back(mys::newTriangle3(mys::vec3{ -150,-50,150 }, mys::vec3{ -150,50,150 }, mys::vec3{ 150,50,150 }));
	StaticTriangles.push_back(mys::newTriangle3(mys::vec3{ -150,-50,150 }, mys::vec3{ 150,-50,150 }, mys::vec3{ 150,50,150 }));
	StaticTriangleColors.push_back(mys::vec3{0.1,0.7,0.7});
	StaticTriangleColors.push_back(mys::vec3{ 0.2,0.6,0.1 });

	StaticTriangles.push_back(mys::newTriangle3(mys::vec3{ -150,-50,-150 }, mys::vec3{ -150,50,-150 }, mys::vec3{ 150,50,-150 }));
	StaticTriangles.push_back(mys::newTriangle3(mys::vec3{ -150,-50,-150 }, mys::vec3{ 150,-50,-150 }, mys::vec3{ 150,50,-150 }));
	StaticTriangleColors.push_back(mys::vec3{ 0.1,0.65,0.2 });
	StaticTriangleColors.push_back(mys::vec3{ 0.8,0.6,0.2 });
	
	StaticTriangles.push_back(mys::newTriangle3(mys::vec3{ -150,-50,-150 }, mys::vec3{ -150,50,-150 }, mys::vec3{ -150,50,150 }));

	StaticTriangleColors.push_back(mys::vec3{ 0.2,0.6,0.1 });
	



	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
	glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
	glutCreateWindow(title);      // Create window with given title
	glewInit();

	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSourceARB(vertexShader, 1, &mys::vertex_shader, nullptr);

	glShaderSourceARB(fragmentShader, 1, &mys::fragment_shader, nullptr);
	glCompileShaderARB(vertexShader);

	glCompileShaderARB(fragmentShader);
	GLint success = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
			for (int i = 0;i < errorLog.size();i++) {
				printf("%c",errorLog[i]);
			}
		
	
	}
	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);

	glLinkProgram(ShaderProgram);
	glUseProgram(ShaderProgram);

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glutDisplayFunc(display);     // Register callback handler for window re-paint
	glutReshapeFunc(reshape);     // Register callback handler for window re-shape
	glutTimerFunc(0, Timer, 0);   // First timer call immediately
	glutSpecialFunc(specialKeys); // Register callback handler for special-key event
	glutSpecialUpFunc(specialKeys2); // Register callback handler for special-key event
	glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
	glutKeyboardUpFunc(keyboard2);   // Register callback handler for special-key event
	//glutFullScreen();             // Put into full screen
	glutMouseFunc(mouse);   // Register callback handler for mouse event
	glutIgnoreKeyRepeat(1);
	initGL();                     // Our own OpenGL initialization
	glutMainLoop();               // Enter event-processing loop

	return 0;
}