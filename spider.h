#pragma once
#include "mys.h"
#include <boost/scoped_array.hpp>
using namespace mys;
namespace spider {
	class spider {
		const int numPoints = 1 + 6 + 6 + 6 + 1;
		const float backLength = 50;
		const float backRadius = 20;
		const float frontLength = 25;
		const float frontRadius = 12;
		vec2 center, direction;
		boost::scoped_array<vec3> mesh;
		void Spider(vec2 center, vec2 direction) {
			center = center;
			direction = normalize(direction);

		}
		void build() {
			mesh.reset(new vec3[numPoints]);
			int ct = 0;
		//s	mesh[ct++]=add(center,scalarMultiply());
			
			
			

		}


	};

}