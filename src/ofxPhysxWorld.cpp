#include "ofxPhysXWorld.h"

OFX_PHYSX_BEGIN_NAMESPACE

#define ASSERT(S) if (!S) { assert(S); return false; }

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;

World::World() :
	foundation(NULL),
	profileZoneManager(NULL),
	physics(NULL),
	cpuDispatcher(NULL),
	scene(NULL),
	defaultMaterial(NULL),
	cudaContextManager(NULL)
{
}

World::~World()
{
	clear();
}

void World::clear()
{
	if (defaultMaterial)
		defaultMaterial->release();
	defaultMaterial = NULL;
	
	if (scene)
	{
		physx::PxActorTypeSelectionFlags t;
		t |= physx::PxActorTypeSelectionFlag::eRIGID_STATIC;
		t |= physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC;
		t |= physx::PxActorTypeSelectionFlag::ePARTICLE_FLUID;
		t |= physx::PxActorTypeSelectionFlag::ePARTICLE_SYSTEM;
		t |= physx::PxActorTypeSelectionFlag::eCLOTH;
		
		int n = scene->getNbActors(t);
		vector<physx::PxActor*> buffer(n);
		scene->getActors(t, buffer.data(), n);
		
		for (int i = 0; i < buffer.size(); i++)
		{
			scene->removeActor(*buffer[i]);
			buffer[i]->release();
		}

		scene->release();
	}
	scene = NULL;
	
	if (cpuDispatcher)
		cpuDispatcher->release();
	cpuDispatcher = NULL;
	
	if (cudaContextManager)
		cudaContextManager->release();
	cudaContextManager = NULL;
	
	if (physics)
		physics->release();
	physics = NULL;
}

bool World::setup(const ofVec3f& gravity, float world_scale)
{
	clear();
	
	this->world_scale = world_scale;
	
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	ASSERT(foundation);
	
	profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(foundation);
	ASSERT(profileZoneManager);
	
	if (1)
	{
		physx::PxCudaContextManagerDesc cudaContextManagerDesc;
		cudaContextManager = physx::PxCreateCudaContextManager(*foundation, cudaContextManagerDesc, profileZoneManager);
		
		if (cudaContextManager && !cudaContextManager->contextIsValid())
		{
			cudaContextManager->release();
			cudaContextManager = NULL;
		}
	}
	
	physx::PxTolerancesScale scale;
	scale.length = world_scale;
	scale.mass = 1000;
	scale.speed = scale.length * 10;
	
	bool trackOutstandingAllocations = true;
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, trackOutstandingAllocations, profileZoneManager);
	ASSERT(physics);
	
	ASSERT(PxInitExtensions(*physics));
	
	// default material
	defaultMaterial = physics->createMaterial(0.5, 0.5, 0.5);
	ASSERT(defaultMaterial);
	
	// scene
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
	
	if (!sceneDesc.cpuDispatcher)
	{
		cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
		ASSERT(cpuDispatcher);
		sceneDesc.cpuDispatcher	= cpuDispatcher;
	}
	
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader	= gDefaultFilterShader;
	
	if (!sceneDesc.gpuDispatcher && cudaContextManager)
		sceneDesc.gpuDispatcher = cudaContextManager->getGpuDispatcher();

	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	
	scene = physics->createScene(sceneDesc);
	ASSERT(scene);
	
	{
		physx::PxSceneWriteLock scopedLock(*scene);
		
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, world_scale * 0.1f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_NORMAL, 1.0f);
		
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_LIN_VELOCITY, 0.01f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_ANG_VELOCITY, 0.01f);

		scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 3.0f);
	}

	return true;
}

void World::update()
{
	if (!physics)
	{
		ofLogError("ofxPhysX::World") << "call setup first";
		return;
	}
	
	float t = ofGetLastFrameTime();
	if (t <= 0) t = 1. / 60.;
	
	scene->simulate(t);
	scene->fetchResults(true);
}

void World::draw()
{
	if (!physics)
	{
		ofLogError("ofxPhysX::World") << "call setup first";
		return;
	}
	
	glPushMatrix();
	
	const physx::PxRenderBuffer& debugRenderable = scene->getRenderBuffer();
	const physx::PxU32 numPoints = debugRenderable.getNbPoints();
	
	const physx::PxU32 numLines = debugRenderable.getNbLines();
	if (numLines)
	{
		const physx::PxDebugLine* PX_RESTRICT lines = debugRenderable.getLines();
		
		glBegin(GL_LINES);
		for(physx::PxU32 i=0; i<numLines; i++)
		{
			const physx::PxDebugLine& line = lines[i];
			ofSetHexColor(line.color0);
			glVertex3fv(&line.pos0[0]);
			glVertex3fv(&line.pos1[0]);
		}
		glEnd();
	}
	
	const physx::PxU32 numTriangles = debugRenderable.getNbTriangles();
	if(numTriangles)
	{
		const physx::PxDebugTriangle* PX_RESTRICT triangles = debugRenderable.getTriangles();
		
		glBegin(GL_TRIANGLES);
		for(physx::PxU32 i=0; i < numTriangles; i++)
		{
			const physx::PxDebugTriangle& triangle = triangles[i];
			ofSetHexColor(triangle.color0);
			glVertex3fv(&triangle.pos0[0]);
			glVertex3fv(&triangle.pos1[0]);
			glVertex3fv(&triangle.pos2[0]);
		}
		glEnd();
	}
	
	glPopMatrix();
}

//

physx::PxRigidActor* World::createRigid(const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	physx::PxTransform transform;
	toPx(pos, transform.p);
	toPx(rot, transform.q);
	
	physx::PxRigidActor *actor;
	
	if (density > 0)
	{
		physx::PxRigidDynamic* rigid = physics->createRigidDynamic(transform);
		rigid->setLinearDamping(0.25);
		rigid->setAngularDamping(0.25);
		actor = rigid;
	}
	else
	{
		physx::PxRigidStatic *rigid = physics->createRigidStatic(transform);
		actor = rigid;
	}
	
	assert(actor);
	scene->addActor(*actor);
	
	return actor;
}

physx::PxRigidActor* World::updateMassAndInertia(physx::PxRigidActor *rigid, float density)
{
	if (density <= 0) return rigid;
	
	physx::PxRigidBody *rigidbody = rigid->isRigidBody();
	if (rigidbody)
		physx::PxRigidBodyExt::updateMassAndInertia(*rigidbody, density);
	
	return rigid;
}

//

physx::PxActor* World::addBox(const ofVec3f& size, const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	physx::PxRigidActor *rigid = createRigid(pos, rot, density);
	rigid->createShape(physx::PxBoxGeometry(toPx(size)), *defaultMaterial);
	return updateMassAndInertia(rigid, density);
}

physx::PxActor* World::addSphere(const float size, const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	physx::PxRigidActor *rigid = createRigid(pos, rot, density);
	rigid->createShape(physx::PxSphereGeometry(size), *defaultMaterial);
	return updateMassAndInertia(rigid, density);
}

physx::PxActor* World::addCapsule(const float radius, const float height, const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	physx::PxRigidActor *rigid = createRigid(pos, rot, density);
	rigid->createShape(physx::PxCapsuleGeometry(radius, height), *defaultMaterial);
	return updateMassAndInertia(rigid, density);
}

physx::PxActor* World::addPlane(const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	physx::PxRigidActor *rigid = createRigid(pos, rot, density);
	rigid->createShape(physx::PxPlaneGeometry(), *defaultMaterial, physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat(ofDegToRad(90), physx::PxVec3(0, 0, 1))));
	return updateMassAndInertia(rigid, density);
}

physx::PxActor* World::addWorldBox(const ofVec3f &leftBottomFar, const ofVec3f& rightTopNear)
{
	physx::PxRigidActor *rigid = createRigid(ofVec3f(0, 0, 0), ofQuaternion(), 0);
	
	physx::PxPlaneGeometry p;
	p = physx::PxPlaneGeometry();

	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(leftBottomFar.x, 0, 0),
physx::PxQuat(ofDegToRad(0), physx::PxVec3(0, 0, 1))));
	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(rightTopNear.x, 0, 0), physx::PxQuat(ofDegToRad(180), physx::PxVec3(0, 0, -1))));

	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(0, leftBottomFar.y, 0), physx::PxQuat(ofDegToRad(90), physx::PxVec3(0, 0, 1))));
	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(0, rightTopNear.y, 0), physx::PxQuat(ofDegToRad(90), physx::PxVec3(0, 0, -1))));
	
	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(0, 0, leftBottomFar.z), physx::PxQuat(ofDegToRad(90), physx::PxVec3(0, -1, 0))));
	rigid->createShape(p, *defaultMaterial, physx::PxTransform(physx::PxVec3(0, 0, rightTopNear.z), physx::PxQuat(ofDegToRad(90), physx::PxVec3(0, 1, 0))));

	return updateMassAndInertia(rigid, 0);
}

OFX_PHYSX_END_NAMESPACE