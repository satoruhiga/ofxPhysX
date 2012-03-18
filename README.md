##Setup notes

* get [PhysX SDK](http://developer.nvidia.com/physx-downloads)
* add OTHER_CPPFLAGS '-malign-double'
* change Project.xcconfig

		PHYSX_SDK_PATH = "/PATH_TO/PhysX-3.1.2_OSX_SDK_Core/SDKs"
		
		PHYSX_LIB_DIR = $(PHYSX_SDK_PATH)/lib/osx32
		PHYSX_LIB_FLAGS = -L$(PHYSX_LIB_DIR) -lFoundation -lPhysX3Vehicle -lGeomUtils -lPhysXProfileSDK -lLowLevel -lPvdRuntime -lLowLevelCloth -lPxTask -lPhysX3 -lRepX3 -lPhysX3CharacterKinematic -lRepXUpgrader3 -lPhysX3Common -lSceneQuery -lPhysX3Cooking -lSimulationController -lPhysX3Extensions
		PHYSX_INC_DIR = $(PHYSX_SDK_PATH)/PhysXAPI $(PHYSX_SDK_PATH)/PxFoundation $(PHYSX_SDK_PATH)/PxTask/include
		
		OTHER_LDFLAGS = $(OF_CORE_LIBS) $(PHYSX_LIB_FLAGS)
		HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) $(PHYSX_INC_DIR)