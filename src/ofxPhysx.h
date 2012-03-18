#pragma once

#include "ofMain.h"

#define NDEBUG
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "common/PxRenderBuffer.h"

#include "ofxPhysXHelper.h"

namespace ofxPhysX {

using namespace physx;
	
#pragma mark -	

class Static
{
public:
	
	Static() : body(NULL) {}
	Static(PxRigidStatic *body) : body(body) {}
	
	Static(PxActor *actor) : body(NULL)
	{
		if (actor->isRigidStatic())
			body = (PxRigidStatic*)actor;
		else
			ofLogError("ofxPhysX", "type error");
	}
	
	inline Static(const Static& copy)
	{
		body = copy.body;
	}
	
	inline Static& operator=(const Static& copy)
	{
		body = copy.body;
		return *this;
	}

	inline void clear() { body = NULL; }
	inline void release()
	{
		if (body)
		{
			body->release();
			body = NULL;
		}
	}
	
	inline operator bool() const { return body != NULL; }
	
	inline PxRigidStatic* operator *() const { return body; }
	inline PxRigidStatic* get() const { return body; }
	
	ofVec3f getSize() const
	{
		assert(body);
		assert(body->getNbShapes() == 1);
		
		PxShape *shape;
		body->getShapes(&shape, 1);
		
		PxGeometryType::Enum t = shape->getGeometryType();
		if (t == PxGeometryType::eSPHERE)
		{
			PxSphereGeometry g;
			shape->getSphereGeometry(g);
			return ofVec3f(g.radius);
		}
		else if (t == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry g;
			shape->getCapsuleGeometry(g);
			return ofVec3f(g.radius, g.halfHeight);
		}
		else if (t == PxGeometryType::eBOX)
		{
			PxBoxGeometry g;
			shape->getBoxGeometry(g);
			return toOF(g.halfExtents);
		}
		else
		{
			ofLogWarning("ofxPhysX::setSize", "unimplemented shape type");
		}
		
		return ofVec3f(0, 0, 0);
	}
	
	void setSize(const ofVec3f& size)
	{
		assert(body);
		assert(body->getNbShapes() == 1);
		
		PxShape *shape;
		body->getShapes(&shape, 1);
		
		PxGeometryType::Enum t = shape->getGeometryType();
		if (t == PxGeometryType::eSPHERE)
		{
			PxSphereGeometry g(size.x);
			shape->setGeometry(g);
		}
		else if (t == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry g(size.x, size.y);
			shape->setGeometry(g);
		}
		else if (t == PxGeometryType::eBOX)
		{
			PxBoxGeometry g(toPx(size));
			shape->setGeometry(g);
		}
		else
		{
			ofLogWarning("ofxPhysX::setSize", "unimplemented shape type");
		}
	}
	
protected:
	
	PxRigidStatic *body;
};
	
class Rigid
{
public:
	
	Rigid() : body(NULL) {}
	Rigid(PxRigidDynamic *body) : body(body) {}
	Rigid(PxActor *actor) : body(NULL)
	{
		if (actor->isRigidBody())
			body = (PxRigidDynamic*)actor;
		else
			ofLogError("ofxPhysX", "type error");
	}

	~Rigid() { body = NULL; }
	
	inline Rigid(const Rigid& copy)
	{
		body = copy.body;
	}
	
	inline Rigid& operator=(const Rigid& copy)
	{
		body = copy.body;
		return *this;
	}
	
	inline void clear() { body = NULL; }
	
	inline void dispose()
	{
		if (body)
		{
			body->getScene()->removeActor(*body);
			body->release();
			body = NULL;
		}
	}
	
	inline operator bool() const { return body != NULL; }
	
	inline PxRigidDynamic* operator *() const { return body; }
	inline PxRigidDynamic* get() const { return body; }

	inline Rigid& setDamping(float lin_damping, float ang_damping)
	{
		body->setLinearDamping(lin_damping);
		body->setAngularDamping(ang_damping);
		return *this;
	}
	
	inline ofMatrix4x4 getTransform() const
	{
		return toOF(body->getGlobalPose());
	}
	
	inline ofVec3f getPosition() const
	{
		return toOF(body->getGlobalPose().p);
	}
	
	inline ofQuaternion getRotate() const
	{
		return toOF(body->getGlobalPose().q);
	}
	
	inline ofVec3f getRotateEuler() const
	{
		return getRotate().getEuler();
	}
	
	inline Rigid& setMass(float mass)
	{
		body->setMass(mass);
		return *this;
	}
	
	inline float getMass() const
	{
		return body->getMass();
	}
	
	ofVec3f getSize() const
	{
		assert(body);
		assert(body->getNbShapes() == 1);
		
		PxShape *shape;
		body->getShapes(&shape, 1);
		
		PxGeometryType::Enum t = shape->getGeometryType();
		if (t == PxGeometryType::eSPHERE)
		{
			PxSphereGeometry g;
			shape->getSphereGeometry(g);
			return ofVec3f(g.radius);
		}
		else if (t == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry g;
			shape->getCapsuleGeometry(g);
			return ofVec3f(g.radius, g.halfHeight);
		}
		else if (t == PxGeometryType::eBOX)
		{
			PxBoxGeometry g;
			shape->getBoxGeometry(g);
			return toOF(g.halfExtents);
		}
		else
		{
			ofLogWarning("ofxPhysX::setSize", "unimplemented shape type");
		}

		return ofVec3f(0, 0, 0);
	}

	void setSize(const ofVec3f& size)
	{
		assert(body);
		assert(body->getNbShapes() == 1);
		
		PxShape *shape;
		body->getShapes(&shape, 1);
		
		PxGeometryType::Enum t = shape->getGeometryType();
		if (t == PxGeometryType::eSPHERE)
		{
			PxSphereGeometry g(size.x);
			shape->setGeometry(g);
		}
		else if (t == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry g(size.x, size.y);
			shape->setGeometry(g);
		}
		else if (t == PxGeometryType::eBOX)
		{
			PxBoxGeometry g(toPx(size));
			shape->setGeometry(g);
		}
		else
		{
			ofLogWarning("ofxPhysX::setSize", "unimplemented shape type");
		}
	}
	
	inline Rigid& activate()
	{
		body->wakeUp();
		return *this;
	}
	
	
	inline Rigid& applyForce(const ofVec3f& force)
	{
		body->addForce(toPx(force), PxForceMode::eFORCE);
		return *this;
	}
	
	inline Rigid& applyImpulseForce(const ofVec3f& force)
	{
		body->addForce(toPx(force), PxForceMode::eIMPULSE);
		return *this;
	}
	
	inline Rigid& applyTorque(const ofVec3f& force)
	{
		body->addTorque(toPx(force), PxForceMode::eFORCE);
		return *this;
	}

	inline Rigid& applyImpulseTorque(const ofVec3f& force)
	{
		body->addTorque(toPx(force), PxForceMode::eIMPULSE);
		return *this;
	}

	inline Rigid& clearForce(const ofVec3f& force)
	{
		body->clearForce(PxForceMode::eFORCE);
		return *this;
	}

	inline Rigid& clearTorque(const ofVec3f& force)
	{
		body->clearTorque(PxForceMode::eFORCE);
		return *this;
	}

	/*
	inline Rigid& setKinematic()
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	
	inline bool isKinematic() const
	{
		return (body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	 */
	
protected:
	
	PxRigidDynamic *body;
};

#pragma mark -

class World
{
public:
	 
	World();
	~World();

	void clear();

	void setup(ofVec3f gravity = ofVec3f(0, -9.8, 0));
	void update();
	void draw();
	
	PxActor* addBox(const ofVec3f& size, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float mass = 1);
	PxActor* addSphere(const float size, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float mass = 1);
	PxActor* addCapsule(const float radius, const float height, const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float mass = 1);
	PxActor* addPlane(const ofVec3f& pos, const ofQuaternion& rot = ofQuaternion(), float mass = 0);
	vector<PxActor*> addWorldBox(const ofVec3f &leftTopFar, const ofVec3f& rightBottomNear);
	
	void removeActor(PxActor *actor);
	
	void setGravity(ofVec3f gravity);

protected:
	
	PxActor* createRigidBody(const PxGeometry& geometry, float mass, const ofVec3f& pos, const ofQuaternion& rot, float density = 1);
	void disposeActor(PxActor* body);
	
	PxPhysics *physics;
	PxDefaultCpuDispatcher *cpuDispatcher;
	
	PxScene *scene;
	PxMaterial *defaultMaterial;
	
	bool inited;
	
};
	
}
