#pragma once

#include "Daybreak/Math/Vector4.h"

namespace Daybreak
{
	struct Mat4
	{
		Mat4(Vec4 C1, Vec4 C2, Vec4 C3, Vec4 C4);
		Mat4(
			float const x1, float const y1, float const z1, float const w1,
			float const x2, float const y2, float const z2, float const w2,
			float const x3, float const y3, float const z3, float const w3,
			float const x4, float const y4, float const z4, float const w4
		);
		~Mat4();
	public:
		Vec4 ColData[4];
	};

	inline std::ostream& operator<<(std::ostream& os, const Mat4& mat)
	{
		return os << "(" << mat.ColData[0] << "," << mat.ColData[1] << "," << mat.ColData[2] << "," << mat.ColData[3] << ")";
	}
}