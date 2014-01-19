#pragma once

#include "ofxPhysXConstants.h"

OFX_PHYSX_BEGIN_NAMESPACE

struct WorldScale
{
	
	static void setWorldScale(float scale);
	
	inline static float getWorldScale() { return world_scale; }
	inline static float getInvWorldScale() { return inv_world_scale; }
	
	inline static float getTorqueScale() { return torque_scale; }
	inline static float getInvTorqueScale() { return inv_torque_scale; }
	
	inline static float getDensityScale() { return density_scale; }
	inline static float getInvDensityScale() { return inv_density_scale; }
	
private:
	
	static float world_scale, inv_world_scale;
	static float torque_scale, inv_torque_scale;
	static float density_scale, inv_density_scale;

};

OFX_PHYSX_END_NAMESPACE