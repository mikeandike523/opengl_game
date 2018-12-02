#pragma once
#include <mys.h>

using namespace mys;
namespace mys_model {
	void useMeshSubCoords() {
		glUniform1i(shaderuniformlocations.USE_MESH_COORDS, 1);
	}
	const vec3 MODEL_YELLOW{ 1,1,0.2 };
	constexpr float cullBuffer = 5;
	mat3 eulerX(float angle) {
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


	mat3 yaw_pitch_rollMatrix(const yaw_pitch_roll& params) {
		return multiplyTwoMat3s(multiplyTwoMat3s(eulerY(params.yaw), eulerX(params.pitch)), eulerZ(params.roll));
	}

	struct axes {
		vec3 i, j, k; //screenright, screentop, depth
	};

	axes extractRotatedAxes(const mat3& ypr) {
		return axes{
		vec3{ypr.entry11,ypr.entry21,ypr.entry31},
		vec3{ypr.entry12,ypr.entry22,ypr.entry32},
		vec3{ypr.entry13,ypr.entry23,ypr.entry33}
		};
	}
	axes getRotatedAxes(const yaw_pitch_roll& orientation) {
		return extractRotatedAxes(yaw_pitch_rollMatrix(orientation));
	}
	yaw_pitch_roll add(const yaw_pitch_roll& a, const yaw_pitch_roll& b) {
		return yaw_pitch_roll{ a.yaw + b.yaw,a.pitch + b.pitch,a.roll + b.roll };
	}
	//model that can move in sapce but has no moving parts
	struct triangle3NoNormal {
		vec3 a, b, c;
	};

	class mesh {
	public:
		int toRebuild = 1;
		vec3 position;
		axes currentAxes;
		yaw_pitch_roll orientation;
		int canDraw = 0;
		std::vector<triangle3> composition;
		std::vector<triangle3> presence;
		mesh(vec3 position1, yaw_pitch_roll orientation1) {
			position = position1;
			orientation = orientation1;
			currentAxes = getRotatedAxes(orientation);

		}

		void move(vec3 delta) {
			moveTo(add(position, delta));
		}
		void rotate(yaw_pitch_roll delta) {
			rotateTo(add(orientation, delta));
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
		float cullRadius = 0;
		void determineCullRadius() {
			cullRadius = 0;
			for (int i = 0;i < composition.size();i++) {

				if (magnitude(composition[i].a) > cullRadius)
					cullRadius = magnitude((composition[i].a));
				if (magnitude(composition[i].b) > cullRadius)
					cullRadius = magnitude((composition[i].b));
				if (magnitude(composition[i].c) > cullRadius)
					cullRadius = magnitude((composition[i].c));



			}
		}
		void rebuild(int forcebuild = 0) {

		}
		int cullModel() {
			vec3 rp = getRelPos(defaultCamera, position);

			if (rp.z + cullRadius + cullBuffer < 0)
				return 0;

			if (magnitude(rp) - cullRadius - cullBuffer > FAR_PLANE)
				return 0;
			float rad2d = (cullRadius + cullBuffer)*defaultCamera.focalDistance / rp.z;
			vec2 rp2d = perspectiveProject(rp, defaultCamera.focalDistance);
			if (rp2d.x - rad2d > clipAreaXRight || rp2d.x + rad2d<clipAreaXLeft || rp2d.y - rad2d>clipAreaYTop || rp2d.y + rad2d < clipAreaYBottom)
				return 0;
			return 1;
		}

		int enabled = 1;
		void disable() {
			enabled = 0;
		}
		void enable() {
			enabled = 1;
		}
		int determinedcr = 0;
		void render() {
			if (!determinedcr) {
				determineCullRadius();
				determinedcr = 1;
			}
			glUniform3f(shaderuniformlocations.MESH_ORIG, position.x, position.y, position.z);
			glUniform3f(shaderuniformlocations.MESH_YPR, orientation.yaw, orientation.pitch, orientation.roll);
			if (!enabled)
				return;


			// if (canDraw) {
				//rebuild();
			if (cullModel())
			
					fastRenderTriangle3s(composition, MODEL_YELLOW);
				
			
	



		}







	};

	void copy_composition_from_to(mesh& m1, mesh& m2) {
		m2.composition.clear();
		for (int i = 0;i < m1.composition.size();i++) {
			m2.composition.push_back(m1.composition[i]);
		}

	}

	void makeCoin(mesh& m) {
		for (int i = 0;i < 12;i++) {
			float a = (float)2 * M_PI*(float)i / (float)12;
			float ap1 = (float)2 * M_PI*(float)(i + 1) / (float)12;
			float x1 = 25 * cos(a);
			float y1 = 25 * sin(a);
			float x2 = 25 * cos(ap1);
			float y2 = 25 * sin(ap1);
			vec3 fr{ x1,y1,5 };
			vec3 br{ x1,y1,-5 };
			vec3 fl{ x2,y2,5 };
			vec3 bl{ x2,y2,-5 };
			m.composition.push_back(newTriangle3(fl, fr, br));
			m.composition.push_back(newTriangle3(fl, bl, br));
			m.composition.push_back(newTriangle3(vec3{ 0,0,-5 }, br, bl));
			m.composition.push_back(newTriangle3(vec3{ 0,0,5 }, fr, fl));

		}



	}

}