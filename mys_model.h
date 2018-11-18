#pragma once
#include <mys.h>
using namespace mys;
namespace mys_model {
	mat3 eulerX(float angle){
		return mat3{};
	}
	mat3 eulerY(float angle) {
		return mat3{};
	}
	mat3 eulerZ(float angle) {
		return mat3{};
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
	
	//model that can move in sapce but has no moving parts
	class mesh {
		vec3 position;
		axes currentAxes;


	};



}