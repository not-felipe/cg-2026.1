#pragma once

#include "vec.h"

struct Quaternion{
	float a;
	vec3 v;

	Quaternion(float a, float b, float c, float d): a{a}, v{b, c, d}{}
	Quaternion(float a=0, vec3 v={0,0,0}): a{a}, v{v}{}
	Quaternion(vec3 v): a{0}, v{v}{}
};

inline void print(Quaternion Q){
	printf("%10.4f ", Q.a);
	printf("%+10.4fi ", Q.v[0]);
	printf("%+10.4fj ", Q.v[1]);
	printf("%+10.4fk\n", Q.v[2]);
}

inline float norm(Quaternion Q){
	return sqrt(Q.a*Q.a + dot(Q.v,Q.v));
}

inline Quaternion operator+(float a, vec3 v){
	return {a, v};
}

inline Quaternion operator-(float a, vec3 v){
	return {a, -v};
}

inline Quaternion operator+(Quaternion q1, Quaternion q2){
	return {q1.a + q2.a, q1.v + q2.v};
}

inline Quaternion operator*(float alpha, Quaternion q1){
	return {alpha*q1.a, alpha*q1.v};
}

inline Quaternion operator*(Quaternion q1, Quaternion q2){
	float a = q1.a;
	float b = q2.a;
	vec3 u = q1.v;
	vec3 v = q2.v;
	return {a*b - dot(u,v), a*v + b*u + cross(u, v)}; 
}

inline Quaternion exp(vec3 v){
    float theta = norm(v);
	if(theta <= 1e-5)
		return 1;

    vec3 n = normalize(v);
    return {cosf(theta), sinf(theta)*n};
}

inline Quaternion quaternion_rotation(vec3 n, float theta){
    return exp(theta/2*normalize(n));
}

inline vec3 rotate(Quaternion q, vec3 v){
   Quaternion qi = q.a - q.v;
   Quaternion vr = q*v*qi;
   return vr.v;
}

inline Quaternion power(Quaternion Q, float t){
   float theta = acos(Q.a);
   vec3 n = (1/sin(theta))*Q.v;
   return exp(t*theta*n);
}

inline Quaternion slerp(float t, Quaternion q1, Quaternion q2){
	Quaternion q1c = q1.a - q1.v;
   	return q1*power(q1c*q2, t);
}
