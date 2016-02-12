#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

#include <cmath>

namespace approx{

	template <class T> struct Vector2{
		T x, y;
		Vector2(T _x=0, T _y=0) : x(_x), y(_y){}
		Vector2& operator =(const Vector2&) = default;

		Vector2& operator +=(const Vector2& other){
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2& operator -=(const Vector2& other){
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2& operator *=(T num){
			x *= num;
			y *= num;
			return *this;
		}
		Vector2& operator /=(T num){
			x /= num;
			y /= num;
			return *this;
		}

		Vector2 operator + (const Vector2& other) const{ return Vector2(x + other.x, y + other.y); }
		Vector2 operator - (const Vector2& other) const{ return Vector2(x - other.x, y - other.y); }
		Vector2 operator * (T num) const{ return Vector2(x*num, y*num); }
		Vector2 operator / (T num) const{ return Vector2(x/num, y/num); }
		T length() const { return sqrt(x*x+y*y); }
		Vector2 normalized() const { T len = length(); return Vector2(x / len, y / len); }

		void normalize(){ *this /= length(); }
	};

	template <class T> struct Vector3{
		T x, y, z;

		Vector3(T _x=0, T _y=0, T _z=0) :x(_x), y(_y), z(_z){}
		Vector3(const Vector3& a) :x(a.x), y(a.y), z(a.z){}

		Vector3& operator = (const Vector3&) = default;
		Vector3& operator +=(const Vector3& other){
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3& operator -=(const Vector3& other){
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3& operator *=(T number){
			x *=f;
			y *=f;
			z *=f;
			return *this;
		}
		Vector3& operator /=(T number){
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}

		Vector3 operator + (const Vector3& other) const {
			return Vector3(x+other.x, y+other.y,z+other.z);
		}
		Vector3 operator - (const Vector3& other) const {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}
		Vector3 operator * (T num) const {
			return Vector3(x*num, y*num, z*num);
		}
		Vector3 operator / (T num) const {
			return Vector3(x/num, y/num, z/num);
		}

		T length() const { return sqrt(x*x+y*y+z*z); }

		void normalize(){
			*this /= length();
		}

		Vector3 normalized() const {
			T len = length();
			return Vector3(x / len, y / len, z / len);
		}
	};

	template <class T> Vector2<T> operator* (T num, const Vector2<T>& vec){
		return vec*num;
	}

	template <class T> Vector3<T> operator* (T num,const Vector3<T>& vec){
		return vec*num;
	}
	
	template <class T> T dot(const Vector3<T>& a,const Vector3<T>& b){
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	template <class T> T dot(const Vector2<T>& a, const Vector2<T>& b){
		return a.x*b.x + a.y*b.y;
	}

	template <class T> Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v){
		return Vector3<T>(u.y*v.z-u.z*v.y,
			              u.z*v.x-u.x*v.z,
						  u.x*v.y-u.y*v.x);
	}


}

#endif