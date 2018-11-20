#pragma once
#include <cmath>    
#include "corecrt_math_defines.h"
#include <freeglut/glut.h>  // GLUT, include glu.h and gl.h
#include <vector>
#include <iostream>
#include <algorithm>
namespace mys {


	GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
	int ww, wh;
	constexpr float STANDARD_EPSILON = 0.001;
	constexpr float NEAR_PLANE = 5.0;
	constexpr float FAR_PLANE = 1500;
	constexpr float ONE_METER = 300;
	constexpr int EAST = 0;
	constexpr int NORTH = 1;
	constexpr int WEST = 2;
	constexpr int SOUTH = 3;
	constexpr int X_AXIS = 0;
	constexpr int Y_AXIS = 1;
	constexpr int CONCAVE = 0;
	constexpr int CONVEX = 1;
	
	GLint ShaderProgram;
	GLint scp;
	struct shaderuniformlocationstorage {
		GLint w, h,colRGB, origin,normal,top,right,focalDistance;

	} shaderuniformlocations;
	
	const char* vertex_shader = R"(


in vec3 a_position;




//the shader entry point is the main method
void main()
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
}

)";
	//credit https://stackoverflow.com/a/53349063/5166365 for helping me fix this
	const char* fragment_shader = R"(
#version 430 core
uniform vec3 colRGB;
uniform vec3 origin;
uniform vec3 normal;
uniform float focalDistance;
uniform int w;
uniform int h;
uniform float right;
uniform float top;
out vec4 color;
  
float far = 1500;

void main()
{
float cx =(-1.0+2.0*gl_FragCoord.x/w)*right;
	float cy = (-1.0+2.0*gl_FragCoord.y/h)*top;
	cx /= focalDistance;
	cy /= focalDistance;
	vec3 dirvec = vec3( cx,cy,1 );
	float t = dot(normal, origin) / dot(normal,dirvec);

		
		

float dpth =length(t*dirvec);
if(dpth>far)
{discard;
return;}
//color = vec4(vec3(dpth)/far,1);
color = vec4(clamp(1-(dpth+50)/far,0,1)*colRGB.xyz,1);

gl_FragDepth = dpth/far;
return;


}
     
)";


	const char* scpfs = R"(
#version 430 core
uniform vec3 colRGB;
out vec4 color;
  

void main()
{
	color = vec4(colRGB.xyz,1);
gl_FragDepth=0;
}

		
		
     
)";
	struct vec2_i{
	int x,y;
	};
	struct vec2 {
		float x, y;
	};

	struct vec3 {
		float x, y,z;
	};
	
	struct mat2 {
		float entry11, entry12,entry21,entry22;
	};

	struct mat3 {
		float entry11, entry12,entry13, entry21, entry22,entry23,entry31,entry32,entry33;
	};
	float magnitude(const vec2& v) {
		return sqrt(v.x*v.x+ v.y*v.y );
	}
	float magnitude(const vec3& v) {
		return sqrt(v.x*v.x + v.y*v.y+v.z*v.z);
	}
	vec2 add(const vec2& a, const vec2& b) {
		return vec2{ a.x + b.x,a.y + b.y };
	}
	vec3 add(const vec3& a, const vec3& b) {
		return vec3{ a.x + b.x,a.y + b.y ,a.z+b.z};
	}
	vec2 subtract(const vec2& a, const vec2& b) {
		return vec2{ a.x - b.x,a.y - b.y };
	}
	vec3 subtract(const vec3& a, const vec3& b) {
		return vec3{ a.x - b.x,a.y - b.y ,a.z-b.z};
	}
	vec2 scalarMultiply(float s, const vec2& v) {
		return vec2{ s*v.x,s*v.y };
	}
	vec3 scalarMultiply(float s, const vec3& v) {
		return vec3{ s*v.x,s*v.y,s*v.z };
	}
	float dotProduct(const vec2& a, const vec2& b) {
		return a.x*b.x + a.y*b.y;
	}
	float dotProduct(const vec3& a, const vec3& b) {
		return a.x*b.x + a.y*b.y+a.z*b.z;
	}
	vec3 crossProduct(const vec3& a,const vec3& b) {
		return vec3{ a.y*b.z - b.y*a.z,a.z*b.x - a.x*b.z,a.x*b.y - a.y*b.x };
	}
	float crossProduct(const vec2& a, const vec2& b) {
		return a.x*b.y - b.x*a.y;
	}
	vec2 perpendicular2(const vec2& v) { return vec2{v.y,-v.x}; }

	float component (const vec2& target, const vec2& base) {
		return (target.x*base.x + target.y*base.y) / sqrt(base.x*base.x + base.y*base.y );
	}
	float component(const vec3& target, const vec3& base) {
		return (target.x*base.x + target.y*base.y + target.z*base.z) / sqrt(base.x*base.x + base.y*base.y + base.z*base.z);
	}

	vec2 projection(const vec2& target, const vec2& base) {
		float s = (target.x*base.x + target.y*base.y) / (base.x*base.x + base.y*base.y);
		return vec2{ s*base.x,s*base.y };
	}
	vec3 projection(const vec3& target, const vec3& base) {
		float s = (target.x*base.x + target.y*base.y+target.z*base.z) / (base.x*base.x + base.y*base.y+base.z*base.z);
		return vec3{ s*base.x,s*base.y ,s*base.z};
	}
	
	float angle(const vec2& v) {
		
		float val = atan2(v.y, v.x);
		if (val < 0)
			val += (float)2 * M_PI;
	
		return val;
	}


	float angleBetween(const vec2& v1, const vec2& v2) {
		return acos((dotProduct(v1, v2)) / (magnitude(v1)*magnitude(v2)));
	}
	float angleBetween(const vec3& v1, const vec3 &v2) {
		return acos((dotProduct(v1, v2)) / (magnitude(v1)*magnitude(v2)));
	}






	//add vector projection if needed later
	float determinant(const mat2& m) {
		return m.entry11*m.entry22 - m.entry12*m.entry21;
	}
	float determinant(const mat3& m) {
		return m.entry11*(m.entry22*m.entry33 - m.entry23*m.entry32) - m.entry12*(m.entry21*m.entry33 - m.entry23*m.entry31) + m.entry13*(m.entry21*m.entry32-m.entry22*m.entry31);
	}
	vec2 normalize(const vec2& v) {
		return scalarMultiply((float)1/magnitude(v),v);
	}

	vec3 normalize(const vec3& v) {
		return scalarMultiply((float)1 / magnitude(v), v);
	}

	
	bool inverse(const mat2& m,mat2& result) {
		float det = determinant(m);
		if (abs(det) < STANDARD_EPSILON) {
			return false;
		}
		float detInv = (float)1 / det;
		float detInvNegated = ((float)(-1))*detInv;
		result =mat2{detInv*m.entry22,detInvNegated*m.entry12,detInvNegated*m.entry21,detInv*m.entry11};
		return true;
	}

	bool inverse(const mat3& m, mat3&result) {
		float det = determinant(m);
		
		if (abs(det) < STANDARD_EPSILON)
			return false;
		float detInv = (float)1 / det;
		mat3 cofactor;
		mat3 adjoint;
		
		cofactor.entry11 = m.entry22*m.entry33 - m.entry23*m.entry32;
		cofactor.entry12 = m.entry21*m.entry33 - m.entry23*m.entry31;
		cofactor.entry13 = m.entry21*m.entry32 - m.entry22*m.entry31;
		cofactor.entry21 = m.entry12*m.entry33 - m.entry13*m.entry32;
		cofactor.entry22 = m.entry11*m.entry33 - m.entry13*m.entry31;
		cofactor.entry23 = m.entry11*m.entry32 - m.entry12*m.entry31;
		cofactor.entry31 = m.entry12*m.entry23 - m.entry13*m.entry22;
		cofactor.entry32 = m.entry11*m.entry23 - m.entry13*m.entry21;
		cofactor.entry33 = m.entry11*m.entry22 - m.entry12*m.entry21;

		adjoint.entry11 = cofactor.entry11;
		adjoint.entry12 = cofactor.entry21;
		adjoint.entry13 = cofactor.entry31;
		adjoint.entry21 = cofactor.entry12;
		adjoint.entry22 = cofactor.entry22;
		adjoint.entry23 = cofactor.entry32;
		adjoint.entry31 = cofactor.entry13;
		adjoint.entry32 = cofactor.entry23;
		adjoint.entry33 = cofactor.entry33;


		result = mat3{ detInv*adjoint.entry11,detInv*adjoint.entry12,detInv*adjoint.entry13,detInv*adjoint.entry21,detInv*adjoint.entry22,
			detInv*adjoint.entry23,detInv*adjoint.entry31,detInv*adjoint.entry32,detInv*adjoint.entry33 };
		return true;
	}

	vec2 matrixMultiply(const mat2& m, const vec2& v) {
		return vec2{ v.x*m.entry11 + v.y*m.entry12,v.x*m.entry21 + v.y*m.entry22 };
	}

	vec3 matrixMultiply(const mat3& m, const vec3& v) {
		return vec3{ v.x*m.entry11 + v.y*m.entry12 + v.z*m.entry13,v.x*m.entry21 + v.y*m.entry22 + v.z*m.entry23,v.x*m.entry31 + v.y*m.entry32 + v.z*m.entry33 };
	}
	struct triangle3 {
		vec3 a, b, c, normal;
	};
	struct triangle2 {
		vec2 a, b, c;
	};
	struct camera {
		vec3 position; float angleXZ, angleZY,focalDistance;
	};
	struct segment2_flat {
		float p0_x, p0_y, p1_x, p1_y;
	};
	vec2 perspectiveProject(const vec3& point, float focalDistance) {
		return scalarMultiply(focalDistance / point.z, vec2{ point.x,point.y });
	}
	vec3 getShaderPoint(const vec2& point,const triangle3& T, float focalDistance) {
		float cx = point.x;
		float cy = point.y;
		cx /= focalDistance;
		cy /= focalDistance;
		vec3 dirvec = { cx,cy,1 };
		float t = dotProduct(T.normal, T.a) / dotProduct(T.normal, dirvec);

		return scalarMultiply(t, dirvec);
	}

	int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
		float p2_x, float p2_y, float p3_x, float p3_y, float &i_x, float &i_y)
	{
		/*
		glBegin(GL_LINES);
		glVertex2f(p0_x,p0_y);
		glVertex2f(p1_x, p1_y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(p2_x, p2_y);
		glVertex2f(p3_x, p3_y);
		glEnd();*/

		float goal1 = p0_x - p2_x;
		float goal2 = p0_y - p2_y;

		float topleft = p0_x - p1_x;
		float bottomleft = p0_y - p1_y;
		float topright = p3_x - p2_x;
		float bottomright = p3_y - p2_y;

		float det = topleft * bottomright - topright * bottomleft;
		if (abs(det) < STANDARD_EPSILON){
			return 0; //singular matrix no intersection
			
		}

		float u = (bottomright*goal1 - topright * goal2) / det;
		float v = (topleft*goal2 - bottomleft * goal1) / det;
		if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
			i_x = p0_x - u * topleft;
			i_y = p0_y - u * bottomleft;
			
			return 1;
		}
	


		return 0;
	}




	int get_line_intersection_special(const segment2_flat &wall,const vec2& junction, const vec2& player, const vec2& veloc,float& i_x,float& i_y)
	{
		
		vec2 nonJunction;
		if (magnitude(subtract(junction, vec2{ wall.p0_x,wall.p0_y })) < 20)
			nonJunction = vec2{ wall.p1_x,wall.p1_y };
		if (magnitude(subtract(junction, vec2{ wall.p1_x,wall.p1_y })) < 20)
			nonJunction = vec2{ wall.p0_x,wall.p0_y };
		float p0_x = player.x;
		float p0_y = player.y;
		float p1_x = player.x + veloc.x;
		float p1_y = player.y + veloc.y;

		float p2_x = junction.x;
		float p2_y = junction.y;
		float p3_x = nonJunction.x;
		float p3_y = nonJunction.y;

	


	
		float goal1 = p0_x - p2_x;
		float goal2 = p0_y - p2_y;

		float topleft = p0_x - p1_x;
		float bottomleft = p0_y - p1_y;
		float topright = p3_x - p2_x;
		float bottomright = p3_y - p2_y;

		float det = topleft * bottomright - topright * bottomleft;
		if (abs(det) < STANDARD_EPSILON) {
			return 0; //singular matrix no intersection

		}

		float u = (bottomright*goal1 - topright * goal2) / det;
		float v = (topleft*goal2 - bottomleft * goal1) / det;
		if (u >= 0 && v >= 0 ) {
			i_x = p0_x - u * topleft;
			i_y = p0_y - u * bottomleft;

			return 1;
		}



		return 0;
	}


	mat3 multiplyTwoMat3s(const mat3& a, const mat3& b) {
		return mat3{ a.entry11*b.entry11 + a.entry12*b.entry21 + a.entry13*b.entry31,a.entry11*b.entry12 + a.entry12*b.entry22 + a.entry13*b.entry32,a.entry11*b.entry13 + a.entry12*b.entry23 + a.entry13*b.entry33,a.entry21*b.entry11 + a.entry22*b.entry21 + a.entry23*b.entry31,a.entry21*b.entry12 + a.entry22*b.entry22 + a.entry23*b.entry32,a.entry21*b.entry13 + a.entry22*b.entry23 + a.entry23*b.entry33,a.entry31*b.entry11 + a.entry32*b.entry21 + a.entry33*b.entry31,a.entry31*b.entry12 + a.entry32*b.entry22 + a.entry33*b.entry32,a.entry31*b.entry13 + a.entry32*b.entry23 + a.entry33*b.entry33};
	}

	mat3 makeRotationMatrixForXZAndZYOnly(float angleXZ, float angleZY) {
		return multiplyTwoMat3s( mat3{ 1,0,0,0,cos(-angleZY) ,-1*sin(-angleZY),0,sin(-angleZY),cos(-angleZY)}, mat3{ cos(-angleXZ),0,sin(-angleXZ),0,1,0,-1 * sin(-angleXZ),0,cos(-angleXZ) });
	}


	triangle3 newTriangle3(const vec3& a, const vec3& b, const vec3& c) {
		return triangle3{ a,b,c,crossProduct(subtract(b,a),subtract(c,a)) };
	}
	triangle2 projectTriangle(const triangle3& T,float focalDistance) {
		return triangle2{perspectiveProject(T.a,focalDistance),perspectiveProject(T.b,focalDistance) ,perspectiveProject(T.c,focalDistance) };
	}
	triangle3 createTriangle3WithoutCalculatingNormal(const vec3& a, const vec3& b, const vec3& c) {
		return triangle3{ a,b,c,vec3{0,0,0} };
	}
	vec3 getRelPos(const camera& cam , const vec3& pt) {
		vec3 npt{ pt.x - cam.position.x,pt.y - cam.position.y,pt.z - cam.position.z };
		/*
		float radXZ = sqrt(npt.x*npt.x + npt.z*npt.z);
	
		float aXZ = atan2(npt.z, npt.x);
		
		float tcXZ = M_PI_2 - cam.angleXZ;
		float tcZY = -cam.angleZY;
		aXZ += tcXZ;
	
		vec3 npt2 {cos(aXZ)*radXZ,npt.y,sin(aXZ)*radXZ };
	
		float radZY = sqrt(npt2.z*npt2.z + npt2.y*npt2.y);
		
		return vec3{ npt2.x,sin(aZY)*radZY,cos(aZY)*radZY };*/
	
		float rad = sqrt(npt.x*npt.x +npt.y*npt.y +npt.z*npt.z);
		float aXZ = atan2(npt.z, npt.x);
		float aZY = acos(npt.y/rad);
		float tcXZ = M_PI_2 - cam.angleXZ;
		float tcZY = -cam.angleZY;
		aXZ += tcXZ;
		aZY += tcZY;

		return vec3{ rad*sin(aZY)*cos(aXZ),rad*cos(aZY),rad*sin(aZY)*sin(aXZ) };
	





	}
	mat3 rotMatrix;
	triangle3 adjustTriangle3ToCamera(const camera& cam, const triangle3& T) {
		
		
		vec3 newA =matrixMultiply(rotMatrix, subtract(T.a,cam.position));
		vec3 newB = matrixMultiply(rotMatrix, subtract(T.b, cam.position));
		vec3 newC = matrixMultiply(rotMatrix, subtract(T.c, cam.position));

		return newTriangle3(newA,newB, newC);
		//return newTriangle3(getRelPos(cam, T.a), getRelPos(cam, T.b), getRelPos(cam, T.c));
	}
	bool triangle3CullAndClipBehindNearPlane(triangle3 T,triangle3& result1, triangle3& result2, int& oneOrTwoTriangles) {
		int aIsIn=1;
		int bIsIn = 1;
		int cIsIn = 1;
		if (T.a.z < NEAR_PLANE) {
			aIsIn = 0;
		}
		if (T.b.z < NEAR_PLANE) {
			bIsIn = 0;
		}

		if (T.c.z < NEAR_PLANE) {
			cIsIn = 0;
		}
		int sum = aIsIn + bIsIn + cIsIn;
		if (sum == 0)
			return false;
		if (T.a.z > FAR_PLANE&&T.b.z > FAR_PLANE&&T.c.z > FAR_PLANE) 
			return 0;
		
		if (sum == 3) {
			result1 = T;
			oneOrTwoTriangles = 1;
			return true;
		}

	
		if (sum==2) {
			if (!bIsIn) {
				T =createTriangle3WithoutCalculatingNormal(T.b, T.a, T.c);
			}
			if (!cIsIn) {
				T = createTriangle3WithoutCalculatingNormal(T.c, T.a, T.b);
			}
			//vec3 d1 = subtract(T.a, T.b);
			float d1x = T.a.x - T.b.x;
			float d1y = T.a.y - T.b.y;
			float d1z = T.a.z - T.b.z;
			//vec3 d2 = subtract(T.a, T.c);
			float d2x = T.a.x - T.c.x;
			float d2y = T.a.y - T.c.y;
			float d2z = T.a.z - T.c.z;

			float t1 = (NEAR_PLANE - T.b.z) / d1z;
			float t2 = (NEAR_PLANE - T.c.z) / d2z;
			//vec3 I1 = add(T.b, scale(t1, d1));
			//vec3 I2 = add(T.c, scale(t2, d2));
			float I1x = T.b.x + t1 * d1x;
			float I1y = T.b.y + t1 * d1y;
			float I1z = T.b.z + t1 * d1z;
			float I2x = T.c.x + t2 * d2x;
			float I2y = T.c.y + t2 * d2y;
			float I2z = T.c.z + t2 * d2z;
			//tP.T1 = newTriangle(T.b, I1, I2, T.r, T.g, T.bl);
			//tP.T2 = newTriangle(T.b, T.c, I2, T.r, T.g, T.bl);
			result1 = newTriangle3(T.b, vec3{ I1x,I1y,I1z }, vec3{ I2x,I2y,I2z });
			result2 = newTriangle3(T.b, T.c, vec3{ I2x,I2y,I2z });
			oneOrTwoTriangles = 2;
			return true;
		}

		if (sum == 1) {

			if (bIsIn) {
				T = createTriangle3WithoutCalculatingNormal(T.b, T.a, T.c);
			}
			if (cIsIn) {
				T = createTriangle3WithoutCalculatingNormal(T.c, T.a, T.b);
			}
			//	vec3 d1 = subtract(T.b, T.a);
			float d1x = T.b.x - T.a.x;
			float d1y = T.b.y - T.a.y;
			float d1z = T.b.z - T.a.z;
			//vec3 d2 = subtract(T.c,T.a);
			float d2x = T.c.x - T.a.x;
			float d2y = T.c.y - T.a.y;
			float d2z = T.c.z - T.a.z;
			float t1 = (NEAR_PLANE - T.a.z) / d1z;
			float t2 = (NEAR_PLANE - T.a.z) / d2z;
			//vec3 I1 = add(T.a, scale(t1, d1));
			//vec3 I2 = add(T.a, scale(t2, d2));
			float I1x = T.a.x + t1 * d1x;
			float I1y = T.a.y + t1 * d1y;
			float I1z = T.a.z + t1 * d1z;
			float I2x = T.a.x + t2 * d2x;
			float I2y = T.a.y + t2 * d2y;
			float I2z = T.a.z + t2 * d2z;
			//tP.T1 = newTriangle(I1, I2, T.a, T.r, T.g, T.bl);
			result1 = newTriangle3(vec3{ I1x,I1y,I1z }, vec3{ I2x,I2y,I2z }, T.a);
			oneOrTwoTriangles = 1;
			return true;
		}

		


		return false; //return false to fully cull triangle that is fully behind near plane or beyond far plane
	}
	int isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{
	if (((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3)) == 0)
		return 0;
	float a = (float)((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) /(float) ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	float 	b =(float) ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / (float)((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	float c = 1 - a - b;
	if (a >= 0 && a <= 1 && c >= 0 && c <= 1 && b >= 0 && b <= 1) return 1;
	return 0;
}
	// cull  vector has not points
	//https://stackoverflow.com/a/13852650/5166365
	int cull(float x1, float y1, float x2, float y2, float x3, float y3, float left, float right, float top, float bottom) {
		int toShow = 0;

		if (x1 >= left  && x1 <= right&&y1 <= top && y1 >= bottom) {
			toShow = 1;
			
			
		}
		if (x2 >= left && x2 <= right && y2 <= top && y2 >= bottom) {
			toShow = 1;
			
			
		}
		if (x3 >= left && x3 <= right && y3 <= top && y3 >= bottom) {
			toShow = 1;
			
			
		}



		float ix, iy;




		if (get_line_intersection(left, top, left, bottom, x1, y1, x2, y2, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(right, top, right, bottom, x1, y1, x2, y2, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, bottom, right, bottom, x1, y1, x2, y2, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, top, right, top, x1, y1, x2, y2, ix, iy)) {
			
			
			toShow = 1;
		}




		if (get_line_intersection(left, top, left, bottom, x2, y2, x3, y3, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(right, top, right, bottom, x2, y2, x3, y3, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, bottom, right, bottom, x2, y2, x3, y3, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, top, right, top, x2, y2, x3, y3, ix, iy)) {
			
			
			toShow = 1;
		}




		if (get_line_intersection(left, top, left, bottom, x3, y3, x1, y1, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(right, top, right, bottom, x3, y3, x1, y1, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, bottom, right, bottom, x3, y3, x1, y1, ix, iy)) {
			
			
			toShow = 1;
		}


		if (get_line_intersection(left, top, right, top, x3, y3, x1, y1, ix, iy)) {
			
			
			toShow = 1;
		}









		int toAddBackOriginal = 0;
		if (isInside(x1, y1, x2, y2, x3, y3, left,top ))
		{
			toShow = 1;
			
		}
		if (isInside(x1, y1, x2, y2, x3, y3, right,top))
		{
			toShow = 1;
		
		}
		if (isInside(x1, y1, x2, y2, x3, y3, right,bottom))
		{
			toShow = 1;
		

		}
		if (isInside(x1, y1, x2, y2, x3, y3, left,bottom))
		{
			toShow = 1;
			
		}




		return toShow;
	}
	void triangle2CullAndClipOutsideWindowAndRender(const triangle2& subject,float left, float right, float top, float bottom,const vec3& color,const triangle3& parent,float focalDistance) {


		if (!cull(subject.a.x,subject.a.y, subject.b.x,subject.b.y,subject.c.x,subject.c.y,left,right,top,bottom))
			return;
	//	glUseProgram(ShaderProgram);
		//parent.normal = normalize(parent.normal);
		//glGetUniformLocation(ShaderProgram, "origin");
	
			glUniform3f(shaderuniformlocations.colRGB, color.x,color.y,color.z);
	

	
			glUniform3f(shaderuniformlocations.origin, parent.c.x, parent.c.y, parent.c.z);
		

		
		
			glUniform3f(shaderuniformlocations.normal, parent.normal.x, parent.normal.y,parent.normal.z);
		
		
		
	

	



	glBegin(GL_POLYGON);
		glVertex2f(subject.a.x,subject.a.y);
		glVertex2f(subject.b.x, subject.b.y);
		glVertex2f(subject.c.x, subject.c.y);
		glEnd();
		/*
		glUseProgram(scp);
		GLint loc9 = glGetUniformLocation(scp, "colRGB");
		if (loc9 != -1)
		{
			glUniform3f(loc9,1,1,1);

		}*/
		/*
		vec3 np = add(parent.a, n);
		vec2 np2d = perspectiveProject(np,focalDistance);
		glBegin(GL_LINES);
		glVertex2f(subject.a.x, subject.a.y);
		glVertex2f(np2d.x, np2d.y);
		glEnd();

		glUniform3f(loc9, 0.9, 0.2, 0.8);
		glBegin(GL_LINES);
		vec3 np2 = add(parent.a, vec3{0,0,150});
		vec2 np2d2 = perspectiveProject(np2, focalDistance);
		glVertex2f(subject.a.x, subject.a.y);
		glVertex2f(np2d2.x, np2d2.y);
		glEnd();
		*/

		

	


	}

	const vec2_i dirs[]{ vec2_i{1,0},vec2_i{0,1},vec2_i{-1,0},vec2_i{0,-1} };
	const int opdirs[]{WEST,SOUTH,EAST,NORTH};
	vec2_i compassToVec2_i(int dir) {
	
		return dirs[dir];
	}
	vec2_i compassOppositeVec2_i(int side) {
		return dirs[opdirs[side]];
	}


	vec2 closest(const segment2_flat& wall, const vec2& player);
		int checkCircleSegmentCollision(const segment2_flat& seg,
		float x, float z, float rad)
	{
			if (magnitude(subtract(closest(seg, vec2{x,z}), vec2{ x,z }))<=rad)
				return 1;
			return 0;
	}
	//http://doswa.com/2009/07/13/circle-segment-intersectioncollision.html
	vec2 closest(const segment2_flat& wall, const vec2 &player) {
		vec2 wallvec{ wall.p1_x - wall.p0_x,wall.p1_y-wall.p0_y };
		vec2 changevec{player.x-wall.p0_x,player.y-wall.p0_y};
		float rng = magnitude(wallvec);
		float comp = component(changevec, wallvec);
		if (comp < 0)
			return vec2{ wall.p0_x,wall.p0_y };
		if(comp>rng)
			return vec2{ wall.p1_x,wall.p1_y };
		return add(vec2{ wall.p0_x,wall.p0_y }, scalarMultiply(comp, normalize(wallvec)));


	}

	vec2 vec2_itovec2(const vec2_i& v) {
		return vec2{ (float)v.x,(float)v.y };
	}
	
	vec2 fromFlatSegment2(const segment2_flat&wall, int whichPoint) {
		if(whichPoint)
			return vec2{ wall.p1_x,wall.p1_y };
		return vec2{ wall.p0_x,wall.p0_y };
	}

	vec2 getJunction(const segment2_flat& wall1, const segment2_flat& wall2) {
		//debug here
		vec2 junction;
		vec2 w1p0 = fromFlatSegment2(wall1, 0);

		vec2 w1p1 = fromFlatSegment2(wall1, 1);
		int ct = 0;

		vec2 w2p0 = fromFlatSegment2(wall2, 0);
		vec2 w2p1 = fromFlatSegment2(wall2, 1);
		if (magnitude(subtract(w1p0, w2p0)) < 20)
		{
			junction = w1p0;
		}

		if (magnitude(subtract(w1p0, w2p1)) < 20)
		{
			junction = w1p0; 
		}
		if (magnitude(subtract(w1p1, w2p0)) < 20)
		{
			junction = w2p0; 
		}
		if (magnitude(subtract(w1p1, w2p1)) < 20)
		{
			junction = w1p1; 
		}
		return junction;
	
	}


	int ConcaveOrConvexCorner(const segment2_flat& wall1, const segment2_flat& wall2, const vec2& player) {

		//debug here
		vec2 junction, a, b;
		vec2 w1p0 = fromFlatSegment2(wall1, 0);

		vec2 w1p1= fromFlatSegment2(wall1, 1);
		int ct=0;

		vec2 w2p0 = fromFlatSegment2(wall2, 0);
		vec2 w2p1 = fromFlatSegment2(wall2, 1);
		if (magnitude(subtract(w1p0,w2p0)) < 20)
		{
			junction = w1p0; a = w1p1; b = w2p1;
			ct++;
		}

		if (magnitude(subtract(w1p0, w2p1)) < 20)
		{
			junction = w1p0; a = w1p1; b = w2p0;
			//std::cout << "a" << std::endl;
			ct++;
		}
		if (magnitude(subtract(w1p1, w2p0)) <20)
		{
			junction = w2p0; a = w2p1; b = w1p0;
			//std::cout << "b" << std::endl;
			ct++;
		}
		if (magnitude(subtract(w1p1, w2p1)) < 20)
		{
			junction = w1p1; a = w1p0; b = w2p0;
			ct++;
		}
		
		vec2 ja = subtract(a, junction);
		vec2 jb = subtract(b, junction);
		vec2 jp = subtract(player, junction);

		float angleJA =angle(ja);
		//std::cout << angleJA << std::endl;
		float angleJB = angle(jb);
		//std::cout << angleJB << std::endl;
		float angleJP = angle(jp);
		//std::cout << angleJP << std::endl;


		float max = std::max<float>( angleJA, angleJB );
		float min = std::min<float>(angleJA, angleJB);

	
		if (abs(abs(max - min) - angleBetween(ja, jb)) < STANDARD_EPSILON) {
		
			if (angleJP <= max && angleJP >= min) {
			
				return CONCAVE;
			}
		}
		else {
			if (angleJP >= max ) {
				
				return CONCAVE;
			}
		}
		
	
		return CONVEX;
	
	}


	mys::camera defaultCamera;

}
