#pragma once

#include "ofMain.h"

#define NDEBUG
#include "PxPhysicsAPI.h"

#define OFX_PHYSX_BEGIN_NAMESPACE namespace ofx { namespace PhysX {
#define OFX_PHYSX_END_NAMESPACE } }

OFX_PHYSX_BEGIN_NAMESPACE

template <typename T>
struct Ref_ : public ofPtr<T>
{
    Ref_() : ofPtr<T>(NULL) {}
	Ref_(T *t) : ofPtr<T>(t) {}
	Ref_(const ofPtr<T>& o) : ofPtr<T>(o) {}
	
	template <typename TT>
	Ref_<TT> cast() const { return dynamic_pointer_cast<TT>(*this); }
};

OFX_PHYSX_END_NAMESPACE

namespace ofxPhysX = ofx::PhysX;