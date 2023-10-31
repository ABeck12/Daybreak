#include "dbpch.h"

#include "Daybreak/Math/Mat4x4.h"

namespace Daybreak
{
	//Mat4::Mat4(Vec4 C1, Vec4 C2, Vec4 C3, Vec4 C4)
	//{
	//	ColData[0] = C1;
	//	ColData[1] = C2;
	//	ColData[2] = C3;
	//	ColData[3] = C4;
	//}

	Mat4::Mat4(
		float const x1, float const y1, float const z1, float const w1,
		float const x2, float const y2, float const z2, float const w2,
		float const x3, float const y3, float const z3, float const w3,
		float const x4, float const y4, float const z4, float const w4
	)
	{
		ColData[0] = Vec4(x1, x2, x3, x4);
		ColData[1] = Vec4(y1, y2, x3, x4);
		ColData[2] = Vec4(z1, z2, z3, z4);
		ColData[3] = Vec4(w1, w2, w3, w4);
	}

}