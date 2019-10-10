
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

	Vector3 &operator=(Vector3 &vector) {
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	Vector3 &operator-(Vector3 &vector) {
		Vector3 result;
		result.x = x - vector.x;
		result.y = y - vector.y;
		result.z = z - vector.z;
		return *this;
	}

	void scal(float value) {
		x *= value;
		y *= value;
		z *= value;
	}

	
};

class Triangle{
public:
	Vector3 a;
	Vector3 b;
	Vector3 c;
public:
	Triangle() {
	}

	Triangle(Vector3 &vertexA, Vector3 &vertexB, Vector3 &vertexC) {
		a = vertexA;
		b = vertexB;
		c = vertexC;
	}
	
	bool checkLengths(float maxSide){
		float lengthAB = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
		float lengthAC = (c.x - a.x) * (c.x - a.x) + (c.y - a.y) * (c.y - a.y) + (c.z - a.z) * (c.z - a.z);
		float lengthBC = (c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y) + (c.z - b.z) * (c.z - b.z);
		float squareSide = maxSide * maxSide;
		if( lengthAB > squareSide || lengthAC > squareSide || lengthBC > squareSide){
			return false;
		}
		return true;
	}

	void split(Triangle& triangle) {
		Vector3 middle;
		float lengthAB = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
		float lengthAC = (c.x - a.x) * (c.x - a.x) + (c.y - a.y) * (c.y - a.y) + (c.z - a.z) * (c.z - a.z);
		float lengthBC = (c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y) + (c.z - b.z) * (c.z - b.z);
		if (lengthAB > lengthAC && lengthAB > lengthBC) {
			middle.x = a.x + (b.x - a.x) / 2;
			middle.y = a.y + (b.y - a.y) / 2;
			middle.z = a.z + (b.z - a.z) / 2;
			triangle.a = middle;
			triangle.b = b;
			triangle.c = c;			
			b = middle;
		}else if (lengthAC > lengthAB && lengthAC > lengthBC) {
			middle.x = a.x + (c.x - a.x) / 2;
			middle.y = a.y + (c.y - a.y) / 2;
			middle.z = a.z + (c.z - a.z) / 2;
			triangle.a = middle;
			triangle.b = b;
			triangle.c = c;			
			c = middle;
		}else{
			middle.x = b.x + (c.x - b.x) / 2;
			middle.y = b.y + (c.y - b.y) / 2;
			middle.z = b.z + (c.z - b.z) / 2;
			triangle.a = a;
			triangle.b = middle;
			triangle.c = c;
			c = middle;
		}
	}
};

