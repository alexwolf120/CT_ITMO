#pragma once
#include <iostream>
#include <math.h>

template< typename T >
struct matrix_t
{
	T data[16];
};

template< typename T >
struct vector3_t
{
	T x, y, z;
};

template< typename T >
class Quat
{
  public:
	Quat() : m_value{ 0, 0, 0, 0 } {}
	Quat(T a, T b, T c, T d) : m_value{ b, c, d, a } {}
	Quat(T angle, bool isRadians, const vector3_t< T >& vec)
	{
		if (!isRadians)
		{
			angle = angle * static_cast< T >(M_PI) / static_cast< T >(180);
		}
		T normal = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		m_value[3] = std::cos(angle / 2);
		m_value[0] = vec.x / normal * std::sin(angle / 2);
		m_value[1] = vec.y / normal * std::sin(angle / 2);
		m_value[2] = vec.z / normal * std::sin(angle / 2);
	}
	Quat operator+(const Quat& other) const
	{
		Quat result(*this);
		return result += other;
	}

	Quat& operator+=(const Quat& other)
	{
		m_value[0] += other.m_value[0];
		m_value[1] += other.m_value[1];
		m_value[2] += other.m_value[2];
		m_value[3] += other.m_value[3];
		return *this;
	}

	Quat operator-(const Quat& other) const
	{
		Quat result(*this);
		return result -= other;
	}

	Quat& operator-=(const Quat& other)
	{
		m_value[0] -= other.m_value[0];
		m_value[1] -= other.m_value[1];
		m_value[2] -= other.m_value[2];
		m_value[3] -= other.m_value[3];
		return *this;
	}

	Quat operator*(const Quat& other) const
	{
		return Quat(
			m_value[3] * other.m_value[3] - m_value[0] * other.m_value[0] - m_value[1] * other.m_value[1] -
				m_value[2] * other.m_value[2],
			m_value[3] * other.m_value[0] + m_value[0] * other.m_value[3] + m_value[1] * other.m_value[2] -
				m_value[2] * other.m_value[1],
			m_value[3] * other.m_value[1] - m_value[0] * other.m_value[2] + m_value[1] * other.m_value[3] +
				m_value[2] * other.m_value[0],
			m_value[3] * other.m_value[2] + m_value[0] * other.m_value[1] - m_value[1] * other.m_value[0] +
				m_value[2] * other.m_value[3]);
	}

	Quat operator~() const { return Quat(m_value[3], -m_value[0], -m_value[1], -m_value[2]); }

	bool operator==(const Quat& other) const
	{
		return m_value[0] == other.m_value[0] && m_value[1] == other.m_value[1] && m_value[2] == other.m_value[2] &&
			   m_value[3] == other.m_value[3];
	}

	bool operator!=(const Quat& other) const { return !(*this == other); }

	explicit operator T() const
	{
		return std::sqrt(m_value[0] * m_value[0] + m_value[1] * m_value[1] + m_value[2] * m_value[2] + m_value[3] * m_value[3]);
	}

	Quat operator*(T scalar) const
	{
		return Quat(m_value[3] * scalar, m_value[0] * scalar, m_value[1] * scalar, m_value[2] * scalar);
	}
	Quat operator*(const vector3_t< T >& vec) const
	{
		Quat quat(0, vec.x, vec.y, vec.z);
		return *this * quat;
	}
	T angle(bool isRadians = true) const
	{
		T angle = 2 * std::acos(m_value[3]);
		if (!isRadians)
		{
			angle = angle * static_cast< T >(180) / static_cast< T >(M_PI);
		}

		return angle;
	}
	vector3_t< T > apply(const vector3_t< T >& vec) const
	{
		Quat< T > normal_quat = *this;
		T abs = static_cast< T >(normal_quat);
		abs *= abs;
		Quat< T > div = normal_quat;
		if (abs != 0)
		{
			for (size_t i = 0; i < 4; i++)
				div.m_value[i] /= abs;
		}
		Quat< T > result = normal_quat * vec * ~div;
		return { result.m_value[0], result.m_value[1], result.m_value[2] };
	}
	matrix_t< T > matrix() const
	{
		return matrix_t< T >{
			m_value[3], -m_value[0], -m_value[1], -m_value[2], m_value[0], m_value[3],	-m_value[2], m_value[1],
			m_value[1], m_value[2],	 m_value[3],  -m_value[0], m_value[2], -m_value[1], m_value[0],	 m_value[3]
		};
	};
	matrix_t< T > rotation_matrix() const
	{
		vector3_t< T > vector1 = apply({ 1, 0, 0 });
		vector3_t< T > vector2 = apply({ 0, 1, 0 });
		vector3_t< T > vector3 = apply({ 0, 0, 1 });
		return matrix_t< T >{ vector1.x, vector1.y, vector1.z, 0, vector2.x, vector2.y, vector2.z, 0,
							  vector3.x, vector3.y, vector3.z, 0, 0,		 0,			0,		   1 };
	};

	const T* data() const { return m_value; }

  private:
	T m_value[4];
};
