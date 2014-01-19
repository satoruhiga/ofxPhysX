#pragma once

#include "ofxPhysXConstants.h"
#include "ofxPhysXHelper.h"

OFX_PHYSX_BEGIN_NAMESPACE

class Actor
{
public:
	
	typedef Ref_<Actor> Ref;
	
	Actor() {}
	virtual ~Actor() {}
	
};

template <typename T>
class RigidActor_ : public Actor
{
public:
	
	RigidActor_() : actor(NULL) {}
	RigidActor_(const RigidActor_& copy) : actor(copy.actor) {}
	RigidActor_(physx::PxRigidActor *actor) : actor(actor) {}
	~RigidActor_() {}
	
	RigidActor_& operator=(const RigidActor_& copy)
	{
		actor = copy.actor;
		return *this;
	}
	
	virtual void release()
	{
		if (!actor) return;
		actor->getScene()->removeActor(*actor);
		actor->release();
		actor = NULL;
	}
	
	inline ofMatrix4x4 getTransform() const
	{
		return toOF(actor->getGlobalPose());
	}

	inline ofVec3f getPosition() const
	{
		return toOF(actor->getGlobalPose().p);
	}
	
	inline ofQuaternion getRotate() const
	{
		return toOF(actor->getGlobalPose().q);
	}

	ofVec3f getSize() const
	{
		assert(actor);
		assert(actor->getNbShapes() == 1);
		
		physx::PxShape *shape;
		actor->getShapes(&shape, 1);
		
		physx::PxGeometryType::Enum t = shape->getGeometryType();
		if (t == physx::PxGeometryType::eSPHERE)
		{
			physx::PxSphereGeometry g;
			shape->getSphereGeometry(g);
			return ofVec3f(g.radius);
		}
		else if (t == physx::PxGeometryType::eCAPSULE)
		{
			physx::PxCapsuleGeometry g;
			shape->getCapsuleGeometry(g);
			return ofVec3f(g.radius, g.halfHeight);
		}
		else if (t == physx::PxGeometryType::eBOX)
		{
			physx::PxBoxGeometry g;
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
		assert(actor);
		assert(actor->getNbShapes() == 1);
		
		physx::PxShape *shape;
		actor->getShapes(&shape, 1);
		
		physx::PxGeometryType::Enum t = shape->getGeometryType();
		if (t == physx::PxGeometryType::eSPHERE)
		{
			physx::PxSphereGeometry g(size.x);
			shape->setGeometry(g);
		}
		else if (t == physx::PxGeometryType::eCAPSULE)
		{
			physx::PxCapsuleGeometry g(size.x, size.y);
			shape->setGeometry(g);
		}
		else if (t == physx::PxGeometryType::eBOX)
		{
			physx::PxBoxGeometry g(toPx(size));
			shape->setGeometry(g);
		}
		else
		{
			ofLogWarning("ofxPhysX::setSize", "unimplemented shape type");
		}
	}

	inline operator bool() const { return actor != NULL; }
	
	inline T* getRigid() const { return rigid; }

protected:

	union {
		physx::PxRigidActor *actor;
		T *rigid;
	};
};

OFX_PHYSX_END_NAMESPACE