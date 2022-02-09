#if defined(__cplusplus) && !defined(__EXT_NMTYPE_H__)
#define __EXT_NMTYPE_H__

// nmtype extensinons for c++

#include "nmtype.h"

// v2nm32f
v2nm32f operator+(v2nm32f l, v2nm32f r){
	v2nm32f res;
	res.v0 = l.v0 + r.v0;
	res.v1 = l.v1 + r.v1;
	return res;
}
v2nm32f operator-(v2nm32f l, v2nm32f r){
	v2nm32f res;
	res.v0 = l.v0 - r.v0;
	res.v1 = l.v1 - r.v1;
	return res;
}
v2nm32f operator-(v2nm32f r){
	r.v0 = -r.v0;
	r.v1 = -r.v1;
	return r;
}

bool operator==(v2nm32f l, v2nm32f r){
	bool v0 = l.v0 == r.v0;
	bool v1 = l.v1 == r.v1;
	return v0 && v1;
}

bool operator!=(v2nm32f l, v2nm32f r){
	return !(l == r);
}

// v4nm32f
v4nm32f operator+(v4nm32f l, v4nm32f r){
	v4nm32f res;
	for(int i = 0; i < 4; i++){
		res.vec[i] = l.vec[i] + r.vec[i];
	}
	return res;
}
v4nm32f operator-(v4nm32f l, v4nm32f r){
	v4nm32f res;
	for(int i = 0; i < 4; i++){
		res.vec[i] = l.vec[i] - r.vec[i];
	}
	return res;
}
v4nm32f operator-(v4nm32f r){
	for(int i = 0; i < 4; i++){
		r.vec[i] = -r.vec[i];
	}
	return r;
}

bool operator==(v4nm32f l, v4nm32f r){
	bool res = true;
	for(int i = 0; i < 4; i++){
		res = res && (l.vec[i] == r.vec[i]);
	}
	return res;
}

bool operator!=(v4nm32f l, v4nm32f r){
	return !(l == r);
}


#endif //__EXT_NMTYPE_H__