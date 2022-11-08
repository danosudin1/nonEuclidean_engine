#pragma once

#include <cmath>
#include "pch.h"

namespace engine
{
	// 2D Polar Vector class with r - distance from the origin and theta - angle of rotation
	class polar_vector2
	{
		enum { D = 2 };
	public:

		/**Create uninitialised*/
		polar_vector2()
		{ }

		/**Create initialised to a single value*/
		polar_vector2(float v)
		{
			r = theta = v;
		}

		/**Create initialised to different values*/
		polar_vector2(float v1, float v2)
		{
			r = v1; theta = v2;
		}

		/**Copy*/
		polar_vector2(const polar_vector2& pv)
		{
			r = pv.r; theta = pv.theta;
		}

		/**Destructor*/
		~polar_vector2() {}

		/**Setter*/
		inline void set(const float& v1, const float& v2)
		{
			r = v1; theta = v2;
		}

		/**Getters*/

		inline float length() const
		{
			return r;
		}

		inline float angle() const
		{
			return theta;
		}

		const polar_vector2 unit() const
		{
			return polar_vector2(1.0f, theta);
		}

		/**Casting to a pointer*/
		inline operator float* (void)
		{
			return val;
		}

		/**Casting to a constant pointer*/
		inline operator const float* (void) const
		{
			return val;
		}

		/**GL interface functions*/
		inline void glScale() const;
		inline void glRotate(float ang) const;
		inline void glTranslate() const;
		inline void glVertex() const;
		inline void glNormal() const;

		/**Summing vectors*/
		inline const polar_vector2 operator + (const polar_vector2 pv) const
		{
			float dTheta = pv.theta - theta;
			float cosDTheta = cos(dTheta);
			float newR = sqrt(r * r + pv.r * pv.r + 2 * r * pv.r * cosDTheta);
			float newTheta = theta + atan2(pv.r * sin(dTheta), r + pv.r * cosDTheta);


			return polar_vector2(newR, newTheta);
		}

		/**Subtracting vectors*/
		inline const polar_vector2 operator - (const polar_vector2 pv) const
		{
			polar_vector2 negatedPv = -pv;
			return polar_vector2(r, theta) + negatedPv;
		}

		/**Multiplying two vectors*/
		inline const polar_vector2 operator * (const polar_vector2 pv) const
		{
			return polar_vector2(r * pv.r, theta + pv.theta);
		}

		/**Dividing two vectors*/
		inline const polar_vector2 operator / (const polar_vector2 pv) const
		{
			return polar_vector2(r / pv.r, theta - pv.theta);
		}

		/**Multiplying the vector by a scalar*/
		inline const polar_vector2 operator * (const float& v) const
		{
			return polar_vector2(r * v, theta);
		}

		/**Dividing the vector by a scalar*/
		inline const polar_vector2 operator / (const float& v) const
		{
			return polar_vector2(r / v, theta);
		}

		/**Add to the existing vector*/
		polar_vector2& operator += (const polar_vector2 pv)
		{
			polar_vector2 newPolarVector = polar_vector2(r, theta) + pv;
			r = newPolarVector.r;
			theta = newPolarVector.theta;

			return *this;
		}

		/**Subtract from the existing vector*/
		polar_vector2& operator -= (const polar_vector2 pv)
		{
			polar_vector2 newPolarVector = polar_vector2(r, theta) - pv;
			r = newPolarVector.r;
			theta = newPolarVector.theta;

			return *this;
		}

		/**Multiply this by a scalar*/
		polar_vector2& operator *= (const float v)
		{
			r *= v;
			return *this;
		}

		/**Divide this by a scalar*/
		polar_vector2& operator /= (const float v)
		{
			r /= v;
			return *this;
		}

		/**Equals operator*/
		polar_vector2& operator = (const polar_vector2& pv)
		{
			r = pv.r;
			theta = pv.theta;
			return *this;
		}

		/**Comparing vectors*/
		bool operator == (const polar_vector2& pv)
		{
			if (r == pv.r) {
				float newTheta1 = constrain_angle_rad(theta);
				float newTheta2 = constrain_angle_rad(pv.theta);
				
				if (newTheta1 == newTheta2) return true;
			}
			return false;
		}

		/**Negation operator*/
		inline const polar_vector2 operator - () const
		{
			return polar_vector2(-r, theta);
		}

		/**Dot product*/
		inline const float dot(const polar_vector2& pv) const
		{
			return r * pv.r * cos(pv.theta - theta);
		}

		/**Normalise the vector*/
		polar_vector2& normalise()
		{
			r = 1.0f;
			return *this;
		}

		glm::vec3 ToCartesian() {
			return glm::vec3(r * cos(theta), r * sin(theta), -0.1f);
		}

		static polar_vector2 ToPolar(float x, float y) {
			return polar_vector2(sqrt(x*x + y*y), atan2f(y, x));

		}

		inline static float constrain_angle_rad(float angle)
		{
			angle = fmod(angle, 2 * m_pi);
			if (angle < 0)
				angle += 2 * m_pi;
			return angle;
		}

#ifdef GLVECTOR_IOSTREAM
		//!iostream interface for input from char stream, enable by defining GLVECTOR_IOSTREAM
		friend std::basic_istream<char, std::char_traits<char> >& operator >>
			(std::basic_istream<char, std::char_traits<char> >& in, polar_vector2& vec)
		{
			in >> vec.val[0] >> vec.val[1]; return in;
		};

		//!iostream interface for output to char stream, enable by defining GLVECTOR_IOSTREAM
		friend std::basic_ostream<char, std::char_traits<char> >& operator <<
			(std::basic_ostream<char, std::char_traits<char> >& out, polar_vector2& vec)
		{
			out << vec.val[0] << " " << vec.val[1]; return out;
		};
#endif

		//!Value array
		//!Anonymous union and struct by sebastien bloc (sebastien.bloc@free.fr)
		union
		{
			float val[D];
			struct
			{
				float r, theta;
			};
		};
	};
}
