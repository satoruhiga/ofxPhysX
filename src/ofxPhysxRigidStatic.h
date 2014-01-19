#pragma once

#include "ofxPhysxConstants.h"
#include "ofxPhysxHelper.h"
#include "ofxPhysxActor.h"

OFX_PHYSX_BEGIN_NAMESPACE

class RigidStatic : public RigidActor_<physx::PxRigidStatic>
{
public:
	
	RigidStatic() : RigidActor_() {}
	RigidStatic(physx::PxRigidStatic *rigid) : RigidActor_(rigid) {}
	RigidStatic(physx::PxActor *actor) : RigidActor_(NULL)
	{
		if (actor->isRigidStatic())
		{
			this->rigid = (physx::PxRigidStatic*)actor;
		}
		else
			ofLogError("ofxPhysX", "invalid cast");
	}
};

OFX_PHYSX_END_NAMESPACE