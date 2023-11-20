#pragma once

#include "Daybreak/Math/Vector.h"

//#include "Daybreak/Math/CoreMathFunctions.h"

namespace Daybreak
{
	struct Mat4
	{
		Mat4(const Vec4 v1, const Vec4 v2, const Vec4 v3, const Vec4 v4)
		{
			data[0]  = v1.x;  data[1] = v1.y; data[2]  = v1.z; data[3]  = v1.w;
			data[4]  = v2.x;  data[5] = v2.y; data[6]  = v2.z; data[7]  = v2.w;
			data[8]  = v3.x;  data[9] = v3.y; data[10] = v3.z; data[11] = v3.w;
			data[12] = v4.x; data[13] = v4.y; data[14] = v4.z; data[15] = v4.w;
		}

		Mat4(const float _11, const float _12, const float _13, const float _14,
			const float _21, const float _22, const float _23, const float _24,
			const float _31, const float _32, const float _33, const float _34,
			const float _41, const float _42, const float _43, const float _44 )
		{
			data[0] = _11,  data[1] = _12; data[2] = _13; data[3] = _14;
			data[4] = _21;  data[5] = _22; data[6] = _23; data[7] = _24;
			data[8] = _31;  data[9] = _32; data[10] = _33; data[11] = _34;
			data[12] = _41; data[13] = _42; data[14] = _34; data[15] = _44;
		}

		Mat4(const float value)
		{
			data[0] = value;  data[1] = 0.0f; data[2] = 0.0f; data[3] = 0.0f;
			data[4] = 0.0f;  data[5] = value; data[6] = 0.0f; data[7] = 0.0f;
			data[8] = 0.0f;  data[9] = 0.0f; data[10] = value; data[11] = 0.0f;
			data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = value;
		}
		
		Mat4(const float value[16])
		{
			for (int i = 0; i < 16;i++)
			{
				data[i] = value[i];
			}
		}

		Mat4() { Mat4(0.0f); }//temp

		static inline Mat4 Identity() { return Mat4(1.0f); }
		static inline Mat4 One() { return Mat4(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); }
		static inline Mat4 Zero() { return Mat4(0.0f); }
		static inline Mat4 Orthographic(const float left, const float right, const float bottom, const float top, const float near_clip, const float far_clip)
		{
			Mat4 out = Mat4::Identity();

			float lr = 1.0f / (left - right);
			float bt = 1.0f / (bottom - top);
			float nf = 1.0f / (near_clip - far_clip);

			out.data[0] = -2.0f * lr;
			out.data[5] = -2.0f * bt;
			out.data[10] = 2.0f * nf;

			out.data[12] = (left + right) * lr;
			out.data[13] = (top + bottom) * bt;
			out.data[14] = (far_clip + near_clip) * nf;
			return out;
		}
		static inline Mat4 Perspective(const float fov_radians, const float aspect_ratio, const float near_clip, const float far_clip)
		{
			float half_fov_tan = Math::Tan(fov_radians * 0.5f);
			Mat4 out = Mat4::Zero();
			out.data[0] = 1.0f / (aspect_ratio * half_fov_tan);
			out.data[5] = 1.0f / half_fov_tan;
			out.data[10] = -((far_clip + near_clip) / (far_clip - near_clip));
			out.data[11] = -1.0f;
			out.data[14] = -((2.0f * far_clip * near_clip) / (far_clip - near_clip));
			return out;
		}
		static inline Mat4 LookAt(Vec3 position, Vec3 target, Vec3 up);
		//{
			//https://www.youtube.com/watch?v=nk3KNPrSRqc at 15:58
			//Mat4 out = Mat4::Zero();
			//Vec3 z_axis(target - position);
			//z_axis.Normalize();
		//}
		
		// Watch https://www.youtube.com/watch?v=nk3KNPrSRqc for translation, scale, rotation
		static inline Mat4 Translation(Vec3 position)
		{
			Mat4 out(1.0f);
			//out.data[3] = position.x;
			//out.data[7] = position.y;
			//out.data[11] = position.z;
			out.data[12] = position.x;
			out.data[13] = position.y;
			out.data[14] = position.z;
			return out;
		}

		static inline Mat4 Translation(Mat4 mat, Vec3 position)
		{
			//mat.data[3] = position.x;
			//mat.data[7] = position.y;
			//mat.data[11] = position.z;
			mat.data[12] = position.x;
			mat.data[13] = position.y;
			mat.data[14] = position.z;
			return mat;
		}

		static inline Mat4 Scale(Vec3 scale)
		{
			Mat4 out(1.0f);
			out.data[0] *= scale.x;
			out.data[5] *= scale.y;
			out.data[10] *= scale.z;
			return out;
		}

		static inline Mat4 Scale(Mat4 mat, Vec3 scale)
		{
			mat.data[0] *= scale.x;
			mat.data[5] *= scale.y;
			mat.data[10] *= scale.z;
			return mat;
		}

		//Optimize this function
		static inline Mat4 Rotation(Mat4 mat, const float angle, Vec3 vec)
		{
			const float s = Math::Sin(angle);
			const float c = Math::Cos(angle);
			 
			Vec3 u(vec.x, vec.y, vec.z);
			u.Normalize();

			float term = (1.0f - c);

			Mat4 w(0.0f);

			//--------------------wrong-----------
			//w.data[1] = -u.z;
			//w.data[2] = u.y;
			//w.data[4] = u.z;
			//w.data[6] = -u.x;
			//w.data[8] = -u.y;
			//w.data[9] = u.x;
			//Mat4 cross(0.0f);
			//cross.data[0] = u.x * u.x;
			//cross.data[1] = u.x * u.y;
			//cross.data[2] = u.x * u.z;
			//cross.data[4] = u.x * u.y;
			//cross.data[5] = u.y * u.y;
			//cross.data[6] = u.z * u.y;
			//cross.data[8] = u.x * u.z;
			//cross.data[9] = u.y * u.z;
			//cross.data[10] = u.z * u.z;
			//Mat4 I(c);
			//I.data[15] = 1.0f;
			//
			////return I + (w * s) + ((w * w) * term);
			//return Mat4::Transpose((I + ((w * s) +(cross * term))) * mat);
			//--------------------------------

			w.data[4] = -u.z;
			w.data[8] = u.y;
			w.data[1] = u.z;
			w.data[9] = -u.x;
			w.data[3] = -u.y;
			w.data[6] = u.x;

			Mat4 cross(0.0f);
			cross.data[0] = u.x * u.x;
			cross.data[4] = u.x * u.y;
			cross.data[8] = u.x * u.z;
			cross.data[1] = u.x * u.y;
			cross.data[5] = u.y * u.y;
			cross.data[9] = u.z * u.y;
			cross.data[2] = u.x * u.z;
			cross.data[6] = u.y * u.z;
			cross.data[10] = u.z * u.z;

			Mat4 I = Mat4::Scale(c);
			
			return (I + ((w * s) +(cross * term))) * mat;
			
		}


#pragma region NOT IMPLEMENTED YET
		// Watch https://www.youtube.com/watch?v=nk3KNPrSRqc for euler angle matrices
		//static inline Mat4 EulerX(const float angle_radians);
		//static inline Mat4 EulerY(const float angle_radians);
		//static inline Mat4 EulerZ(const float angle_radians);
		//static inline Mat4 EulerXYZ(const float x_angle_radians, const float y_angle_radians, const float z_angle_radians) //double check the order of matrix math here once EulerX,Y,Z are implemented
		//{
		//	Mat4 rx = Mat4::EulerX(x_angle_radians);
		//	Mat4 ry = Mat4::EulerY(y_angle_radians);
		//	Mat4 rz = Mat4::EulerZ(z_angle_radians);

		//	return rz * ry * rz; //double check the order of matrix math here once EulerX,Y,Z are implemented
		//}


		//inline void Transpose(); // to be use like mat.Transpose();
		static inline Mat4 Transpose(Mat4 matrix) // needs to be tested, to be used like Mat4::Transpose(mat);
		{
			Mat4 out_matrix = Mat4::Identity();
			out_matrix.data[0] = matrix.data[0];
			out_matrix.data[1] = matrix.data[4];
			out_matrix.data[2] = matrix.data[8];
			out_matrix.data[3] = matrix.data[12];
			out_matrix.data[4] = matrix.data[1];
			out_matrix.data[5] = matrix.data[5];
			out_matrix.data[6] = matrix.data[9];
			out_matrix.data[7] = matrix.data[13];
			out_matrix.data[8] = matrix.data[2];
			out_matrix.data[9] = matrix.data[6];
			out_matrix.data[10] = matrix.data[10];
			out_matrix.data[11] = matrix.data[14];
			out_matrix.data[12] = matrix.data[3];
			out_matrix.data[13] = matrix.data[7];
			out_matrix.data[14] = matrix.data[11];
			out_matrix.data[15] = matrix.data[15];
			return out_matrix;
		}

		//inline void Inverse(); // to be use like mat.Inverse();
		static inline Mat4 Inverse(Mat4 matrix) // needs to be tested, to be used like Mat4::Inverse(mat);
		{
			const float* m = matrix.data;
			float t0 = m[10] * m[15];
			float t1 = m[14] * m[11];
			float t2 = m[6] * m[15];
			float t3 = m[14] * m[7];
			float t4 = m[6] * m[11];
			float t5 = m[10] * m[7];
			float t6 = m[2] * m[15];
			float t7 = m[14] * m[3];
			float t8 = m[2] * m[11];
			float t9 = m[10] * m[3];
			float t10 = m[2] * m[7];
			float t11 = m[6] * m[3];
			float t12 = m[8] * m[13];
			float t13 = m[12] * m[9];
			float t14 = m[4] * m[13];
			float t15 = m[12] * m[5];
			float t16 = m[4] * m[9];
			float t17 = m[8] * m[5];
			float t18 = m[0] * m[13];
			float t19 = m[12] * m[1];
			float t20 = m[0] * m[9];
			float t21 = m[8] * m[1];
			float t22 = m[0] * m[5];
			float t23 = m[4] * m[1];
			Mat4 out_matrix;
			float* o = out_matrix.data;
		
			o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
			o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
			o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
			o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);
		
			float d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);
		
			o[0] = d * o[0];
			o[1] = d * o[1];
			o[2] = d * o[2];
			o[3] = d * o[3];
			o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
			o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
			o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
			o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
			o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
			o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
			o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
			o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
			o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
			o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
			o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
			o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));
			return out_matrix;
		}



		//static inline Mat4 Forward();
		//static inline Mat4 Backward();
		//static inline Mat4 Up();
		//static inline Mat4 Down();
		//static inline Mat4 Left();
		//static inline Mat4 Right();
#pragma endregion


		float data[16];


		inline Mat4 operator+(const Mat4& other) const
		{
			float newdata[16];
			for (int i = 0; i < 16;i++)
			{
				newdata[i] = data[i] + other.data[i];
			}
			return Mat4(newdata);
		}

		inline Mat4 operator*(const Mat4& other) const
		{
			Mat4 out = Mat4::Identity();
			const float* m1_ptr = data;
			const float* m2_ptr = other.data;
			float* out_ptr = out.data;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4;j++)
				{
					*out_ptr = m1_ptr[0] * m2_ptr[0 + j] +
								m1_ptr[1] * m2_ptr[4 + j] +
								m1_ptr[2] * m2_ptr[8 + j] +
								m1_ptr[3] * m2_ptr[12 + j];
					out_ptr++;
				}
				m1_ptr += 4;
			}
			return out;
		}


		inline Mat4 operator*(const float& value) const
		{
			float newdata[16];
			for (int i = 0; i < 16;i++)
			{
				newdata[i] = data[i] * value;
			}
			return Mat4(newdata);
		}

		//float& operator[](const int i)
		//{
		//	//DB_CORE_ASSERT(i <= 15 && i >=0, "Vec2 index out of range!");
		//	return data[i];
		//}
	};

	inline Mat4 operator*(const float& value, Mat4 mat)
	{
		float newdata[16];
		for (int i = 0; i < 16;i++)
		{
			newdata[i] = mat.data[i] * value;
		}
		return Mat4(newdata);
	}


	inline std::ostream& operator<<(std::ostream& os, const Mat4& mat)
	{
		os << "|" << mat.data[0] << ","  << mat.data[1] <<  "," << mat.data[2] <<  "," << mat.data[3] <<  "|\n";
		os << "|" << mat.data[4] << ","  << mat.data[5] <<  "," << mat.data[6] <<  "," << mat.data[7] <<  "|\n";
		os << "|" << mat.data[8] << ","  << mat.data[9] <<  "," << mat.data[10] << "," << mat.data[11] << "|\n";
		os << "|" << mat.data[12] << "," << mat.data[13] << "," << mat.data[14] << "," << mat.data[15] << "|\n";
		return os;
	}

}