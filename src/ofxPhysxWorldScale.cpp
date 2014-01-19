#include "ofxPhysXWorldScale.h"

OFX_PHYSX_BEGIN_NAMESPACE

float WorldScale::world_scale = 100;
float WorldScale::inv_world_scale = 1. / WorldScale::world_scale;
float WorldScale::torque_scale = world_scale * world_scale;
float WorldScale::inv_torque_scale = 1. / torque_scale;
float WorldScale::density_scale = world_scale * world_scale * world_scale;
float WorldScale::inv_density_scale = 1. / density_scale;

void WorldScale::setWorldScale(float scale)
{
	world_scale = scale;
	inv_world_scale = 1. / world_scale;
	
	torque_scale = pow(world_scale, 2);
	inv_torque_scale = 1. / torque_scale;
	
	density_scale = pow(world_scale, 3);
	inv_density_scale = 1. / density_scale;
}

OFX_PHYSX_END_NAMESPACE