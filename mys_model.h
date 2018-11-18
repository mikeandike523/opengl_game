#pragma once
#include <mys.h>
#include <parse_stl.h>
using namespace mys;
namespace mys_model {
	mat3 eulerX(float angle){
		return mat3{
			1,0,0,
			0,cos(angle),-sin(angle),
			0,sin(angle),cos(angle)
		};
	}
	mat3 eulerY(float angle) {
		return mat3{
			cos(angle),0,sin(angle),
			0,1,0,
			-sin(angle),0,cos(angle)
		};
	}
	mat3 eulerZ(float angle) {
		return mat3{
		cos(angle),-sin(angle),0,
		sin(angle),cos(angle),0,
		0,0,1
		};
	}
	
	struct yaw_pitch_roll {
		float yaw, pitch, roll;
	};


	mat3 yaw_pitch_rollMatrix(yaw_pitch_roll params) {
		return multiplyTwoMat3s(multiplyTwoMat3s(eulerY(params.yaw),eulerX(params.pitch)),eulerZ(params.roll));
	}

	struct axes {
		vec3 i, j, k; //screenright, screentop, depth
	};
	
	axes extractRotatedAxes(mat3 ypr) {
		return axes{
		vec3{ypr.entry11,ypr.entry21,ypr.entry31},
		vec3{ypr.entry12,ypr.entry22,ypr.entry32},
		vec3{ypr.entry13,ypr.entry23,ypr.entry33}
		};
	}
	axes getRotatedAxes(yaw_pitch_roll orientation) {
		return extractRotatedAxes(yaw_pitch_rollMatrix(orientation));
	}
	yaw_pitch_roll add(yaw_pitch_roll a, yaw_pitch_roll b) {
		return yaw_pitch_roll{ a.yaw + b.yaw,a.pitch + b.pitch,a.roll + b.roll };
	}
	//model that can move in sapce but has no moving parts
	struct triangle3NoNormal {
		vec3 a, b, c;
	};
	triangle3NoNormal translateConvention(stl::triangle T) {
		return triangle3NoNormal{ vec3{T.v1.x,T.v1.y,T.v1.z},vec3{T.v2.x,T.v2.y,T.v2.z},vec3{T.v3.x,T.v3.y,T.v3.z} };
	}
	class mesh {
		int toRebuild=1;
		vec3 position;
		axes currentAxes;
		yaw_pitch_roll orientation;
		int canDraw = 0;
		std::vector<triangle3NoNormal> composition;
		std::vector<triangle3> presence;
		 mesh(vec3 position, yaw_pitch_roll orientation) {
			 position = position;
			 orientation = orientation;
			 currentAxes = getRotatedAxes(orientation);

		}

		 void move(vec3 delta) {
			 moveTo(add(position, delta));
		 }
		 void rotate(yaw_pitch_roll delta) {
			 rotateTo(add(orientation,delta));
		 }
		 void moveTo(vec3 newPosition) {
			 position = newPosition;
			 toRebuild = 1;
		 }
		 void rotateTo(yaw_pitch_roll newOrientation) {
			 orientation = newOrientation;
			 currentAxes = getRotatedAxes(newOrientation);
			 toRebuild = 1;
		 }
		 //credit here: https://github.com/dillonhuff/stl_parser
		 void loadSTL(const char* filepath, float rescale) {
			 composition.clear();
			 auto info = stl::parse_stl(filepath);

			 std::vector<stl::triangle> triangles = info.triangles;
			 for (int i = 0;i < triangles.size();i++) {
				 triangle3NoNormal nt = translateConvention(triangles[i]);
				 composition.push_back(triangle3NoNormal{ scalarMultiply(rescale,nt.a),scalarMultiply(rescale,nt.b) ,scalarMultiply(rescale,nt.c) });
			 }
			 toRebuild = 1;

		 }
		 void rebuild() {
			 if (toRebuild) {
				 //write code here

				 toRebuild = 0;
			 }
			 canDraw = 1;
		 }

		 void render() {
			 if (canDraw) {
				 rebuild();
				 for (int i = 0;i < composition.size();i++) {
				 
				 }

			 
			 }
		 }






	};



}