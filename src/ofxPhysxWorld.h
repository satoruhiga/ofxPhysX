#pragma once

#include "ofxPhysXConstants.h"
#include "ofxPhysXHelper.h"
#include "ofxPhysXWorldScale.h"

#define NDEBUG
#include "PxPhysicsAPI.h"

OFX_PHYSX_BEGIN_NAMESPACE

class World
{
public:
	
	World();
	virtual ~World();
	
	bool setup(const ofVec3f& gravity = ofVec3f(0, -980, 0));
	void update();
	void draw();
	
	physx::PxActor* addBox(const ofVec3f& size, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float density = 1);
	physx::PxActor* addSphere(const float size, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float density = 1);
	physx::PxActor* addCapsule(const float radius, const float height, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float density = 1);
	physx::PxActor* addPlane(const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float density = 0);
	physx::PxActor* addWorldBox(const ofVec3f &leftBottomFar, const ofVec3f& rightTopNear);
	
	void removeActor(physx::PxActor *actor);
	
	void setGravity(ofVec3f gravity);
	
	void clear();
	
protected:
	
	physx::PxRigidActor* createRigid(const ofVec3f& pos, const ofQuaternion& rot, float density);
	physx::PxRigidActor* updateMassAndInertia(physx::PxRigidActor *rigid, float density);
	
protected:
	
	physx::PxFoundation *foundation;
	physx::PxProfileZoneManager *profileZoneManager;
	physx::PxPhysics *physics;
	physx::PxDefaultCpuDispatcher *cpuDispatcher;
	
	physx::PxScene *scene;
	physx::PxMaterial *defaultMaterial;
	
	physx::PxCudaContextManager* cudaContextManager;
};

OFX_PHYSX_END_NAMESPACE
