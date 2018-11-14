
#include <windows.h>  // for MS Windows
#include <glew/glew.h>
#include <freeglut/glut.h>  // GLUT, include glu.h and gl.h
#include <vector>
#include <mys.h>
#include <mazegen.h>
#include <ctime>
using namespace mys;
std::vector<mys::triangle3> StaticTriangles;
std::vector<mys::vec3> StaticTriangleColors;
std::vector<mys::segment2_flat> walls;
std::vector<int> wallDirs;


constexpr float WALL_BUFFER = 30;
constexpr float CORNER_BUFFER = 5;

inline void gen_wall(int x, int z, int side) {
	//vec2_i offs = compassToVec2_i(side);

	if (side == EAST) {
		float p0_x = (float)(x * 300 + 150);
		float p0_y = (float)(z * 300 + 150);
		float p1_x = (float)(x * 300 + 150);
		float p1_y = (float)(z * 300 + -150);
		walls.push_back(segment2_flat{ p0_x,p0_y,p1_x,p1_y });
	}

	if (side == WEST) {
		float p0_x = (float)(x * 300 - 150);
		float p0_y = (float)(z * 300 + 150);
		float p1_x = (float)(x * 300 - 150);
		float p1_y = (float)(z * 300 + -150);
		walls.push_back(segment2_flat{ p0_x,p0_y,p1_x,p1_y });
	}

	if (side == NORTH) {
		float p0_x = (float)(x * 300 - 150);
		float p0_y = (float)(z * 300 + 150);
		float p1_x = (float)(x * 300 + 150);
		float p1_y = (float)(z * 300 + 150);
		walls.push_back(segment2_flat{ p0_x,p0_y,p1_x,p1_y });
	}

	if (side == SOUTH) {
		float p0_x = (float)(x * 300 - 150);
		float p0_y = (float)(z * 300 - 150);
		float p1_x = (float)(x * 300 + 150);
		float p1_y = (float)(z * 300 - 150);
		walls.push_back(segment2_flat{ p0_x,p0_y,p1_x,p1_y });
	}



	wallDirs.push_back(side);
}

mys::camera defaultCamera;


char title[] = "Game 1";  // Windowed mode's title
int windowWidth = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX = 50;      // Windowed mode's top-left corner x
int windowPosY = 50;      // Windowed mode's top-left corner y

int refreshMillis = 15;      // Refresh period in milliseconds

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = true; // Full-screen or windowed mode?
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

int current_ticks, delta_ticks;
float fElapsedTime;
/* Callback handler for window re-paint event */
void display() {
	current_ticks = glutGet(GLUT_ELAPSED_TIME);
	bool moving = false;
	float nx = defaultCamera.position.x;
	float nz = defaultCamera.position.z;

	if (keyaisdown)
	{
		moving = true;

		nx += cos(defaultCamera.angleXZ + M_PI_2) * 600 * fElapsedTime;
		nz += sin(defaultCamera.angleXZ + M_PI_2) * 600 * fElapsedTime;
	}
	if (keydisdown)
	{
		moving = true;
		nx += cos(defaultCamera.angleXZ - M_PI_2) * 600 * fElapsedTime;
		nz += sin(defaultCamera.angleXZ - M_PI_2) * 600 * fElapsedTime;
	}

	if (keywisdown)
	{
		moving = true;
		nx += cos(defaultCamera.angleXZ) * 600 * fElapsedTime;
		nz += sin(defaultCamera.angleXZ) * 600 * fElapsedTime;
	}

	if (keysisdown)
	{
		moving = true;
		nx += -cos(defaultCamera.angleXZ) * 600 * fElapsedTime;
		nz += -sin(defaultCamera.angleXZ) * 600 * fElapsedTime;
	}
	if (moving) {
		float nnx = nx;
		float nnz = nz;
		int ct = 0;
		segment2_flat wall1;
		int side1;

		segment2_flat wall2;
		int side2;
		vec2 player{ defaultCamera.position.x,defaultCamera.position.z };
		vec2 veloc{ nx - player.x,nz - player.y };
		for (int i = 0;i < walls.size();i++) {
			segment2_flat wall = walls[i];
			int side = wallDirs[i];
			if (checkCircleSegmentCollision(wall, player.x, player.y, WALL_BUFFER)) {
				ct++;
				if (ct == 1) {
					wall1 = wall;
					side1 = side;
				}
				if (ct == 2) {

					wall2 = wall;
					side2 = side;
					break;
				}
			}


		}

		if (ct == 1) {

			//	vec2 close = closest(wall1, player);

			vec2 diff = subtract(vec2{ wall1.p1_x,wall1.p1_y }, vec2{ wall1.p0_x,wall1.p0_y });
			//vec2 orth = perpendicular2(diff);
			vec2 nv = veloc;
			if (dotProduct(veloc, vec2_itovec2(compassOppositeVec2_i(side1))) < STANDARD_EPSILON)
				nv = projection(veloc, diff);
			nnx = player.x + nv.x;
			nnz = player.y + nv.y;




		}
		if (ct == 2) {
			if (side1 == side2) {
				//	vec2 close = closest(wall1, player);

				vec2 diff = subtract(vec2{ wall1.p1_x,wall1.p1_y }, vec2{ wall1.p0_x,wall1.p0_y });
				//vec2 orth = perpendicular2(diff);
				vec2 nv = veloc;
				if (dotProduct(veloc, vec2_itovec2(compassOppositeVec2_i(side1))) < STANDARD_EPSILON)
					nv = projection(veloc, diff);
				nnx = player.x + nv.x;
				nnz = player.y + nv.y;

			}
			else {
				int concavity = ConcaveOrConvexCorner(wall1, wall2, player);
				if (concavity == CONVEX)
				{


					bool has1 = false;
					bool has2 = false;
					float ix1, ix2, iy1, iy2;
					int whichCase = 0;
					float dist1;
					float dist2;
					vec2 normv = normalize(veloc);
					vec2 junction = getJunction(wall1, wall2);
					if (get_line_intersection_special(wall1, junction, player, veloc, ix1, iy1))
					{
						has1 = true;

					}
					if (get_line_intersection_special(wall2, junction, player, veloc, ix2, iy2))
					{
						has2 = true;

					}

					if (has1)
						dist1 = magnitude(vec2{ ix1 - player.x,iy1 - player.y });
					if (has2)
						dist2 = magnitude(vec2{ ix2 - player.x,iy2 - player.y });


					if (!has1 && !has2) {
						whichCase = 0;
					}
					if (has1 && !has2)
					{
						whichCase = 1;

					}
					if (!has1 && has2)
					{
						whichCase = 2;
					}
					if (has1&&has2) {
						if (dist1 == dist2) {
							whichCase == 3;
						}
						if (dist1 < dist2) {
							whichCase = 1;
						}
						if (dist1 > dist2) {
							whichCase = 2;
						}

					}




					if (whichCase == 0) {
						
						
					

						nnx = player.x + veloc.x;
						nnz = player.y + veloc.y;

						
					}

					if (whichCase == 1) {
						vec2 diff = subtract(vec2{ wall1.p1_x,wall1.p1_y }, vec2{ wall1.p0_x,wall1.p0_y });
						//vec2 orth = perpendicular2(diff);
						vec2 nv = veloc;
						if (dotProduct(veloc, vec2_itovec2(compassOppositeVec2_i(side1))) < STANDARD_EPSILON)
							nv = projection(veloc, diff);
						nnx = player.x + nv.x;
						nnz = player.y + nv.y;
					}

					if (whichCase == 2) {
						vec2 diff = subtract(vec2{ wall2.p1_x,wall2.p1_y }, vec2{ wall2.p0_x,wall2.p0_y });
						//vec2 orth = perpendicular2(diff);
						vec2 nv = veloc;
						if (dotProduct(veloc, vec2_itovec2(compassOppositeVec2_i(side2))) < STANDARD_EPSILON)
							nv = projection(veloc, diff);
						nnx = player.x + nv.x;
						nnz = player.y + nv.y;
					}
					if (whichCase == 3) {
						

					}


				}

				if (concavity == CONCAVE) {

					vec2 nv = veloc;
					vec2 axis1 = vec2_itovec2(compassOppositeVec2_i(side1));

					if (dotProduct(nv, axis1) < STANDARD_EPSILON)
					{
						switch (side1) {
						case EAST:
							nv.x = 0;
							break;
						case NORTH:
							nv.y = 0;
							break;
						case WEST:
							nv.x = 0;
							break;
						case SOUTH:
							nv.y = 0;
							break;

						}
					}


					vec2 axis2 = vec2_itovec2(compassOppositeVec2_i(side2));

					if (dotProduct(nv, axis2) < STANDARD_EPSILON)
					{
						switch (side2) {
						case EAST:
							nv.x = 0;
							break;
						case NORTH:
							nv.y = 0;
							break;
						case WEST:
							nv.x = 0;
							break;
						case SOUTH:
							nv.y = 0;
							break;

						}
					}


					nnx = player.x + nv.x;
					nnz = player.y + nv.y;



				}





			}
		}
		defaultCamera.position.x = nnx;
		defaultCamera.position.z = nnz;





	}


	if (keyleftisdown)
	{

		defaultCamera.angleXZ += (float)2 * M_PI*fElapsedTime;
	}


	if (keyrightisdown)
	{

		defaultCamera.angleXZ -= (float)2 * M_PI* fElapsedTime;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
	glLoadIdentity();              // Reset model-view matrix





//draw here
	for (int i = 0;i < StaticTriangles.size();i++) {
		mys::triangle3 T = mys::adjustTriangle3ToCamera(defaultCamera, StaticTriangles[i]);
		mys::triangle3 result1;
		mys::triangle3 result2;
		int oneOrTwoTriangles;
		mys::vec3 col = StaticTriangleColors[i];
		if (mys::triangle3CullAndClipBehindNearPlane(T, result1, result2, oneOrTwoTriangles)) {
			if (oneOrTwoTriangles == 1) {
				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result1, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom, col, result1, defaultCamera.focalDistance);
			}

			if (oneOrTwoTriangles == 2) {

				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result1, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom, col, result1, defaultCamera.focalDistance);
				mys::triangle2CullAndClipOutsideWindowAndRender(mys::projectTriangle(result2, defaultCamera.focalDistance), clipAreaXLeft, clipAreaXRight, clipAreaYTop, clipAreaYBottom, col, result2, defaultCamera.focalDistance);
			}
		}
	}



	glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)

	delta_ticks = glutGet(GLUT_ELAPSED_TIME) - current_ticks; //the time, in ms, that took to render the scene
	if (delta_ticks > 0)
		fElapsedTime = (float)delta_ticks / (float)1000;

	glutPostRedisplay();
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

int roomOccupation(vec2_i rm, int roomRad, int* rooms) {
	if (rm.x<-roomRad || rm.x>roomRad || rm.y<-roomRad || rm.y>roomRad)
		return -1;

	if (rooms[(roomRad * 2 + 1)*(roomRad - rm.y) + (roomRad + rm.x)])
		return 1;

	return 0;
}
void setRoomOpen(vec2_i rm, int roomRad, int* rooms) {
	rooms[(roomRad * 2 + 1)*(roomRad - rm.y) + (roomRad + rm.x)] = 0;
}
void cleanup() {

}
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

	std::atexit(cleanup);
	defaultCamera = mys::camera{ mys::vec3{0.0,0.0,0.0},(float)M_PI_2,0.0,500 };
	const int roomRad = 15;
	std::vector<mys::vec2_i> roomList;
	mazegen::push_maze(roomRad, roomList);
	int rooms[(roomRad * 2 + 1)*(roomRad * 2 + 1)];
	for (int i = 0;i < roomRad*roomRad;i++) {
		rooms[i] = 1;
	}
	for (int i = 0;i < roomList.size();i++) {
		setRoomOpen(roomList[i], roomRad, rooms);
	}

	for (int i = 0;i < roomList.size();i++) {
		vec2_i rmloc = roomList[i];

		StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + -150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + -150),(float)-50,(float)(rmloc.y * 300 - 150)} });
		StaticTriangleColors.push_back(vec3{ 0,0,1 });
		StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 + -150),(float)-50,(float)(rmloc.y * 300 - 150)} });
		StaticTriangleColors.push_back(vec3{ 0,0,1 });


		StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + -150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + -150),(float)50,(float)(rmloc.y * 300 - 150)} });
		StaticTriangleColors.push_back(vec3{ 0,1,0 });
		StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 + -150),(float)50,(float)(rmloc.y * 300 - 150)} });
		StaticTriangleColors.push_back(vec3{ 0,1,0 });

		if (abs(roomOccupation(vec2_i{ rmloc.x + 1,rmloc.y }, roomRad, rooms) == 1)) {

			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 1,0, 0 });



			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 1,0, 0 });
			gen_wall(rmloc.x, rmloc.y, EAST);

		}


		if (abs(roomOccupation(vec2_i{ rmloc.x - 1,rmloc.y }, roomRad, rooms) == 1)) {

			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 - 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 - 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 1,0, 1 });



			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 - 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 1,0, 1 });

			gen_wall(rmloc.x, rmloc.y, WEST);

		}

		if (abs(roomOccupation(vec2_i{ rmloc.x ,rmloc.y + 1 }, roomRad, rooms) == 1)) {

			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 - 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 + 150)} });
			StaticTriangleColors.push_back(vec3{ 0.5,0.5, 0.8 });


			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 + 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 + 150)} });
			StaticTriangleColors.push_back(vec3{ 0.5,0.5, 0.8 });

			gen_wall(rmloc.x, rmloc.y, NORTH);

		}
		if (abs(roomOccupation(vec2_i{ rmloc.x ,rmloc.y - 1 }, roomRad, rooms) == 1)) {

			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 - 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 0.7,0.6, 0.2 });


			StaticTriangles.push_back(triangle3{ vec3{(float)(rmloc.x * 300 + 150),(float)-50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 + 150),(float)50,(float)(rmloc.y * 300 - 150)},vec3{(float)(rmloc.x * 300 - 150),(float)-50,(float)(rmloc.y * 300 - 150)} });
			StaticTriangleColors.push_back(vec3{ 0.7,0.6, 0.2 });

			gen_wall(rmloc.x, rmloc.y, SOUTH);


		}


	}

	int ri = rand() % roomList.size();
	defaultCamera.position.x = (float)(roomList[ri].x) * (float)300;
	defaultCamera.position.z = (float)(roomList[ri].y) * (float)300;

	vec2_i dirs[] = { vec2_i{1,0},vec2_i{0,1},vec2_i{-1,0},vec2_i{0,-1} };
	int  randNum = rand() % 4;
	while (roomOccupation(vec2_i{ roomList[ri].x + dirs[randNum].x,roomList[ri].y + dirs[randNum].y }, roomRad, rooms) != 0) {
		randNum = rand() % 4;
	}
	switch (randNum) {
	case 0: defaultCamera.angleXZ = (float)0; break;
	case 1: defaultCamera.angleXZ = M_PI_2; break;
	case 2:defaultCamera.angleXZ = M_PI;  break;
	case 3:defaultCamera.angleXZ = (float)3 * M_PI_2; break;
	}



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
			printf("%c", errorLog[i]);
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

	glScissor(0, 0, windowWidth, windowHeight);
	glEnable(GL_SCISSOR_TEST);


	glutDisplayFunc(display);     // Register callback handler for window re-paint
	glutReshapeFunc(reshape);     // Register callback handler for window re-shape
	//glutTimerFunc(0, Timer, 0);   // First timer call immediately
	glutSpecialFunc(specialKeys); // Register callback handler for special-key event
	glutSpecialUpFunc(specialKeys2); // Register callback handler for special-key event
	glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
	glutKeyboardUpFunc(keyboard2);   // Register callback handler for special-key event
	glutFullScreen();             // Put into full screen
	glutMouseFunc(mouse);   // Register callback handler for mouse event
	glutIgnoreKeyRepeat(1);
	initGL();                     // Our own OpenGL initialization
	glutMainLoop();               // Enter event-processing loop

	return 0;
}