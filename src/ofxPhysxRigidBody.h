#pragma once

#include "ofxPhysxConstants.h"
#include "ofxPhysxHelper.h"
#include "ofxPhysxActor.h"

OFX_PHYSX_BEGIN_NAMESPACE

class RigidBody : public RigidActor_<physx::PxRigidDynamic>
{
public:
	
	RigidBody() : RigidActor_() {}
	RigidBody(physx::PxRigidDynamic *rigid) : RigidActor_(rigid) {}
	RigidBody(physx::PxActor *actor) : RigidActor_(NULL)
	{
		if (actor->isRigidBody())
			this->rigid = (physx::PxRigidDynamic*)actor;
		else
			ofLogError("ofxPhysX", "invalid cast");
	}
	
	inline RigidBody& setDamping(float lin_damping, float ang_damping)
	{
		rigid->setLinearDamping(lin_damping);
		rigid->setAngularDamping(ang_damping);
		return *this;
	}
	
	inline RigidBody& setMass(float mass)
	{
		rigid->setMass(mass);
		return *this;
	}
	
	inline float getMass() const
	{
		return rigid->getMass();
	}
	
	inline RigidBody& activate()
	{
		rigid->wakeUp();
		return *this;
	}
	
	inline RigidBody& applyForce(const ofVec3f& force, bool is_local = false)
	{
		physx::PxVec3 F;
		
		if (is_local)
			F = rigid->getGlobalPose().q.rotate(toPx(force));
		else
			F = toPx(force);
		
		rigid->addForce(F, physx::PxForceMode::eFORCE);
		
		return *this;
	}
	
	inline RigidBody& applyImpulseForce(const ofVec3f& force, bool is_local = false)
	{
		physx::PxVec3 F;
		
		if (is_local)
			F = rigid->getGlobalPose().q.rotate(toPx(force));
		else
			F = toPx(force);
		
		rigid->addForce(F, physx::PxForceMode::eIMPULSE);
		
		return *this;
	}
	
	inline RigidBody& applyTorque(const ofVec3f& torque, bool is_local = false)
	{
		physx::PxVec3 T;
		
		if (is_local)
			T = rigid->getGlobalPose().q.rotate(toPx(torque));
		else
			T = toPx(torque);
		
		rigid->addTorque(T, physx::PxForceMode::eFORCE);
		return *this;
	}
	
	inline RigidBody& applyImpulseTorque(const ofVec3f& torque, bool is_local = false)
	{		physx::PxVec3 T;
		
		if (is_local)
			T = rigid->getGlobalPose().q.rotate(toPx(torque));
		else
			T = toPx(torque);
		
		rigid->addTorque(T, physx::PxForceMode::eIMPULSE);
		return *this;
	}
	
	inline RigidBody& clearForce()
	{
		rigid->clearForce(physx::PxForceMode::eFORCE);
		return *this;
	}
	
	inline RigidBody& clearTorque()
	{
		rigid->clearTorque(physx::PxForceMode::eFORCE);
		return *this;
	}
	
};

OFX_PHYSX_END_NAMESPACE