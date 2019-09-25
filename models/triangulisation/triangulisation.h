
#include <string>
using namespace std;

class Vector3{
public:
	float x;
	float y;
	float z;

	Vector3(float pointX, float pointY, float pointZ) {
		set(pointX, pointY, pointZ);
	}

	Vector3(float pointX, float pointY) {
		set(pointX, pointY, 0);
	}

	Vector3() {
		set(0, 0, 0);
	}

	void set(float pointX, float pointY, float pointZ) {
		x = pointX;
		y = pointY;
		z = pointZ;
	}

	void set(float pointX, float pointY) {
		x = pointX;
		y = pointY;
	}
	
	operator float() { 
		return sqrt(x*x + y*y + z*z);
	}
	
	Vector3 operator+(Vector3 vector) {
		Vector3 result;
		result.x = x + vector.x;
		result.y = y + vector.y;
		result.z = z + vector.z;
		return result;
	}

	Vector3 operator+(float v) {
		Vector3 result;
		result.x = x + v;
		result.y = y + v;
		result.z = z + v;
		return result;
	}
	
	Vector3 operator-(Vector3 vector) {
		Vector3 result;
		result.x = x - vector.x;
		result.y = y - vector.y;
		result.z = z - vector.z;
		return result;
	}

	Vector3 operator-(float v) {
		Vector3 result;
		result.x = x - v;
		result.y = y - v;
		result.z = z - v;
		return result;
	}
	
	Vector3 operator*(Vector3 vector) {
		Vector3 result;
		result.x = x * vector.x;
		result.y = y * vector.y;
		result.z = z * vector.z;
		return result;
	}

	Vector3 operator*(float v) {
		Vector3 result;
		result.x = x * v;
		result.y = y * v;
		result.z = z * v;
		return result;
	}
	
	Vector3 operator/(Vector3 vector) {
		Vector3 result;
		result.x = x / vector.x;
		result.y = y / vector.y;
		result.z = z / vector.z;
		return result;
	}

	Vector3 operator/(float v) {
		Vector3 result;
		result.x = x / v;
		result.y = y / v;
		result.z = z / v;
		return result;
	}

	Vector3 operator=(Vector3 vector) {
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	Vector3 operator=(float v) {
		x = v;
		y = v;
		z = v;
		return *this;
	}
	
	
};

class Triangle{
public:
	Vector3 a;
	Vector3 b;
	Vector3 c;

	Triangle() {
	}

	Triangle(Vector3 vertexA, Vector3 vertexB, Vector3 vertexC) {
		a = vertexA;
		b = vertexB;
		c = vertexC;
	}
	
	bool isFitIntoRectangle(int width, int height){
		Vector3 AC = a - c;
		Vector3 AB = a - b;
		Vector3 BC = b - c;
		if( abs(AC.x) > width  || abs(AB.x) > width  || abs(BC.x) > width ||
			abs(AC.y) > height || abs(AB.y) > height || abs(BC.y) > height){
			return false;
		}
		return true;
	}
};

