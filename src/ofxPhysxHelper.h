#pragma once

#include "ofxPhysXConstants.h"

OFX_PHYSX_BEGIN_NAMESPACE

template <typename T1, typename T2>
const T2& toOF(const T1& o1, T2& o2);

template <typename T1, typename T2>
const T2& toPx(const T1& o1, T2& o2);

// Vec3

inline ofVec3f toOF(const physx::PxVec3& o);

template <>
inline const ofVec3f& toOF(const physx::PxVec3& o1, ofVec3f& o2)
{
	o2.set(o1.x, o1.y, o1.z);
	return o2;
}

inline ofVec3f toOF(const physx::PxVec3& o)
{
	ofVec3f result;
	return toOF(o, result);
}

inline physx::PxVec3 toPx(const ofVec3f& o);

template <>
inline const physx::PxVec3& toPx(const ofVec3f& o1, physx::PxVec3& o2)
{
	o2.x = o1.x;
	o2.y = o1.y;
	o2.z = o1.z;
	return o2;
}

inline physx::PxVec3 toPx(const ofVec3f& o)
{
	physx::PxVec3 result;
	return toPx(o, result);
}


// Quat

inline ofQuaternion toOF(const physx::PxQuat& o);

template <>
inline const ofQuaternion& toOF(const physx::PxQuat& o1, ofQuaternion& o2)
{
	o2.set(o1.x, o1.y, o1.z, o1.w);
	return o2;
}

inline ofQuaternion toOF(const physx::PxQuat& o)
{
	ofQuaternion result;
	return toOF(o, result);
}

inline physx::PxQuat toPx(const ofQuaternion& o);

template <>
inline const physx::PxQuat& toPx(const ofQuaternion& o1, physx::PxQuat& o2)
{
	o2.x = o1.x();
	o2.y = o1.y();
	o2.z = o1.z();
	o2.w = o1.w();
	return o2;
}

inline physx::PxQuat toPx(const ofQuaternion& o)
{
	physx::PxQuat result;
	return toPx(o, result);
}

// Transform

inline ofMatrix4x4 toOF(const physx::PxTransform& o);

template <>
inline const ofMatrix4x4& toOF(const physx::PxTransform& o1, ofMatrix4x4& o2)
{
	o2.setTranslation(o1.p.x, o1.p.y, o1.p.z);
	o2.setRotate(ofQuaternion(o1.q.x, o1.q.y, o1.q.z, o1.q.w));
	return o2;
}

inline ofMatrix4x4 toOF(const physx::PxTransform& o)
{
	ofMatrix4x4 result;
	return toOF(o, result);
}

inline physx::PxTransform toPx(const ofMatrix4x4& o);

template <>
inline const physx::PxTransform& toPx(const ofMatrix4x4& o1, physx::PxTransform& o2)
{
	toPx(o1.getTranslation(), o2.p);
	toPx(o1.getRotate(), o2.q);
	return o2;
}

inline physx::PxTransform toPx(const ofMatrix4x4& o)
{
	physx::PxTransform result;
	return toPx(o, result);
}

// Matrix

inline ofMatrix4x4 toOF(const physx::PxMat44& o);

template <>
inline const ofMatrix4x4& toOF(const physx::PxMat44& o1, ofMatrix4x4& o2)
{
	o2.set(o1.front());
	return o2;
}

inline ofMatrix4x4 toOF(const physx::PxMat44& o)
{
	ofMatrix4x4 result;
	return toOF(o, result);
}

template <>
inline const physx::PxMat44& toPx(const ofMatrix4x4& o1, physx::PxMat44& o2)
{
	const float *src = o1.getPtr();
	
	for (int i = 0; i < 4; i++)
	{
		physx::PxVec4& dst = o2[i];
		
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
		
		src += 4;
	}
	
	return o2;
}

OFX_PHYSX_END_NAMESPACE
