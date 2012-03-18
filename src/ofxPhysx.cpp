#include "ofxPhysX.h"


namespace ofxPhysX {
	
using namespace physx;
	
static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
static PxSimulationFilterShader gDefaultFilterShader=PxDefaultSimulationFilterShader;

World::World() : inited(false)
{
}

World::~World()
{
	clear();
}

void World::setup(ofVec3f gravity)
{
	inited = true;
	
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback, PxTolerancesScale());
	
	PxInitExtensions(*physics);
	
	PxSceneDesc sceneDesc(physics-> getTolerancesScale());
	sceneDesc.gravity = toPx(gravity);
	
	if(!sceneDesc.cpuDispatcher)
	{
		int num_threads = 4;
		cpuDispatcher = PxDefaultCpuDispatcherCreate(num_threads);
		assert(cpuDispatcher);
		
		sceneDesc.cpuDispatcher	= cpuDispatcher;
	}
	
	if(!sceneDesc.filterShader)
		sceneDesc.filterShader	= gDefaultFilterShader;
	
	scene = physics->createScene(sceneDesc);
	assert(scene);
	
	scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 3.0f);

	defaultMaterial = physics->createMaterial(0.5, 0.5, 0.5);
}
	
void World::clear()
{
	PxActorTypeSelectionFlags t;
	t |= PxActorTypeSelectionFlag::eRIGID_STATIC;
	t |= PxActorTypeSelectionFlag::eRIGID_DYNAMIC;
	t |= PxActorTypeSelectionFlag::ePARTICLE_FLUID;
	t |= PxActorTypeSelectionFlag::ePARTICLE_SYSTEM;
	t |= PxActorTypeSelectionFlag::eDEFORMABLE;
	t |= PxActorTypeSelectionFlag::eCLOTH;
	
	int n = scene->getNbActors(t);
	vector<PxActor*> buffer(n);
	scene->getActors(t, buffer.data(), n);
	
	for (int i = 0; i < buffer.size(); i++)
	{
		scene->removeActor(*buffer[i]);
		buffer[i]->release();
	}
}

void World::update()
{
	float t = ofGetLastFrameTime();
	if (t <= 0) t = 1. / 60.;
	
	scene->simulate(t);
	scene->fetchResults(true);
}

void World::draw()
{
	assert(inited);

	glPushMatrix();
	
	const PxRenderBuffer& debugRenderable = scene->getRenderBuffer();
	const PxU32 numPoints = debugRenderable.getNbPoints();
	
	const PxU32 numLines = debugRenderable.getNbLines();
	if (numLines)
	{
		const PxDebugLine* PX_RESTRICT lines = debugRenderable.getLines();
		
		glBegin(GL_LINES);
		for(PxU32 i=0; i<numLines; i++)
		{
			const PxDebugLine& line = lines[i];
			ofSetHexColor(line.color0);
			glVertex3fv(&line.pos0[0]);
			glVertex3fv(&line.pos1[0]);
		}
		glEnd();
	}
	
	const PxU32 numTriangles = debugRenderable.getNbTriangles();
	if(numTriangles)
	{
		const PxDebugTriangle* PX_RESTRICT triangles = debugRenderable.getTriangles();
		
		glBegin(GL_TRIANGLES);
		for(PxU32 i=0; i < numTriangles; i++)
		{
			const PxDebugTriangle& triangle = triangles[i];
			ofSetHexColor(triangle.color0);
			glVertex3fv(&triangle.pos0[0]);
			glVertex3fv(&triangle.pos1[0]);
			glVertex3fv(&triangle.pos2[0]);
		}
		glEnd();
	}
	
	glPopMatrix();
}
 
PxActor* World::addBox(const ofVec3f& size, const ofVec3f& pos, const ofQuaternion& rot, float mass)
{
	assert(inited);
	return createRigidBody(PxBoxGeometry(toPx(size)), mass, pos, rot);
}

PxActor* World::addSphere(const float size, const ofVec3f& pos, const ofQuaternion& rot, float mass)
{
	assert(inited);
	return createRigidBody(PxSphereGeometry(size), mass, pos, rot);
}

PxActor* World::addCapsule(const float radius, const float height, const ofVec3f& pos, const ofQuaternion& rot, float mass)
{
	assert(inited);
	return createRigidBody(PxCapsuleGeometry(radius, height), mass, pos, rot);
}

PxActor* World::addPlane(const ofVec3f& pos, const ofQuaternion& rot, float mass)
{
	assert(inited);
	return createRigidBody(PxPlaneGeometry(), mass, pos, rot);
}

vector<PxActor*> World::addWorldBox(const ofVec3f &leftTopFar, const ofVec3f& rightBottomNear)
{
	vector<PxActor*> result;

	result.push_back(addPlane(ofVec3f(leftTopFar.x, 0, 0)));
	result.push_back(addPlane(ofVec3f(rightBottomNear.x, 0, 0), ofQuaternion(180, ofVec3f(0, 1, 0))));

	result.push_back(addPlane(ofVec3f(0, rightBottomNear.y, 0), ofQuaternion(90, ofVec3f(0, 0, -1))));
	result.push_back(addPlane(ofVec3f(0, leftTopFar.y, 0), ofQuaternion(90, ofVec3f(0, 0, 1))));

	result.push_back(addPlane(ofVec3f(0, 0, rightBottomNear.z), ofQuaternion(90, ofVec3f(0, 1, 0))));
	result.push_back(addPlane(ofVec3f(0, 0, leftTopFar.z), ofQuaternion(90, ofVec3f(0, -1, 0))));

	return result;
}
	
void World::removeActor(PxActor *actor)
{
	disposeActor(actor);
}
	
void World::setGravity(ofVec3f gravity)
{
	assert(inited);
	scene->setGravity(toPx(gravity));
}
	
PxActor* World::createRigidBody(const PxGeometry& geometry, float mass, const ofVec3f& pos, const ofQuaternion& rot, float density)
{
	assert(inited);
	
	PxTransform transform;
	toPx(pos, transform.p);
	toPx(rot, transform.q);
	
	PxActor *actor;
	
	if (mass > 0)
	{
		PxRigidDynamic* rigid = PxCreateDynamic(*physics, transform, geometry, *defaultMaterial, density);
		rigid->setMass(mass);
		rigid->setLinearDamping(0.25);
		rigid->setAngularDamping(0.25);
		
		actor = rigid;
	}
	else
	{
		PxRigidStatic *rigid = PxCreateStatic(*physics, transform, geometry, *defaultMaterial);
		actor = rigid;
	}
	
	scene->addActor(*actor);
	
	return actor;
}

void World::disposeActor(PxActor* body)
{
	if (!body) return;
	
	scene->removeActor(*body);
	body->release();
}
	
}

