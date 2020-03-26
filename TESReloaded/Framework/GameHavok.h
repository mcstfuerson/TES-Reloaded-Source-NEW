#pragma once

class hkStatisticsCollector;
class hkSimulation;
class hkReferencedObject;
class hkSimulationIsland;
class hkRigidBody;
class hkWorldMaintenanceManager;
class hkWorldMemoryAvailableWatchDog;
class hkBroadPhase;
class hkTypedBroadPhaseDispatcher;
class hkPhantomBroadPhaseListener;
class hkEntityEntityBroadPhaseListener;
class hkProcessCollisionInput;
class hkCollisionFilter;
class hkCollisionDispatcher;
class hkWorldOperationQueue;
class hkPhantom;
class hkActionListener;
class hkEntityListener;
class hkPhantomListener;
class hkConstraintListener;
class hkWorldDeletionListener;
class hkIslandActivationListener;
class hkWorldPostSimulationListener;
class hkWorldPostIntegrateListener;
class hkWorldPostCollideListener;
class hkIslandPostIntegrateListener;
class hkIslandPostCollideListener;
class hkCollisionListener;
class hkBroadPhaseListener;
class hkBroadPhaseBorderListener;
class hkDefaultConvexListFilter;
class bhkCharacterController;
class bhkCharacterProxy;

class hkVector4 {
public:
	float	x;
	float	y;
	float	z;
	float	w;
};
assert(sizeof(hkVector4) == 0x010);

template <typename T>
class hkArray {
public:
	enum {
		kFlag_CapacityMask  = 0x3FFFFFFF,
		kFlag_NoDealloc		= 0x80000000,	// m_data not owned by us
		kFlag_Locked		= 0x40000000,	// will never be destroyed
	};

	T*		m_data;				// 00
	UInt32	m_size;				// 04
	UInt32	m_capacityAndFlags;	// 08
};
assert(sizeof(hkArray<void>) == 0x00C);

class hkBaseObject {
public:
	virtual void Destructor(bool FreeThis);
};
assert(sizeof(hkBaseObject) == 0x004);

class hkReferencedObject : public hkBaseObject {
public:
	virtual void	calcStatistics(hkStatisticsCollector* collector);

	UInt16	sizeAndFlags;	// 04
	UInt16	refCount;		// 06
};
assert(sizeof(hkReferencedObject) == 0x008);

#if defined(NEWVEGAS)
class hkWorld : public hkReferencedObject {
public:
	hkSimulation*							simulation;					// 008
	float									flt00C;						// 00C
	hkVector4								gravity;					// 010
	hkSimulationIsland*						fixedIsland;				// 020
	hkRigidBody*							fixedRigidBody;				// 024
	hkArray<hkSimulationIsland*>			activeSimulationIslands;	// 028
	hkArray<hkSimulationIsland*>			inactiveSimulationIslands;	// 034
	hkArray<hkSimulationIsland*>			dirtySimulationIslands;		// 040
	hkWorldMaintenanceManager*				maintenanceMgr;				// 04C
	UInt32									unk050;						// 050
	UInt32									unk054;						// 054
	hkBroadPhase*							broadPhase;					// 058
	UInt32									unk05C;						// 05C
	UInt32									unk060;						// 060
	hkBroadPhaseListener**					broadPhaseListeners;		// 064
	hkPhantomBroadPhaseListener*			phantomBroadPhaseListener;	// 068
	hkEntityEntityBroadPhaseListener*		entityBroadPhaseListener;	// 06C
	hkBroadPhaseBorderListener*				broadPhaseBorderListener;	// 070
	void*									ptr074;						// 074
	void*									ptr078;						// 078
	hkCollisionFilter*						collisionFilter;			// 07C
	hkCollisionDispatcher*					collisionDispatcher;		// 080
	hkDefaultConvexListFilter*				convexListFilter;			// 084
	void*									ptr088;						// 088
	UInt32									unk08C[13];					// 08C
	void*									ptr0C0;						// 0C0
	UInt32									unk0C4[19];					// 0C4
	hkArray<hkPhantom*>						phantoms;						// 110
	hkArray<hkActionListener*>				actionListeners;				// 11C
	hkArray<hkEntityListener*>				entityListeners;				// 128
	hkArray<hkPhantomListener*>				phantomListeners;				// 134
	hkArray<hkConstraintListener*>			constraintListeners;			// 140
	hkArray<hkWorldDeletionListener*>		worldDeletionListeners;			// 14C
	hkArray<hkIslandActivationListener*>	islandActivationListeners;		// 158
	hkArray<hkWorldPostSimulationListener*> worldPostSimulationListeners;	// 164
	hkArray<hkWorldPostIntegrateListener*>	worldPostIntegrateListeners;	// 170
	hkArray<hkWorldPostCollideListener*>	worldPostCollideListeners;		// 17C
	hkArray<hkIslandPostIntegrateListener*> islandPostIntegrateListeners;	// 188
	hkArray<hkIslandPostCollideListener*>	islandPostCollideListeners;		// 194 - unconfirmed
	hkArray<hkCollisionListener*>			collisionListeners;				// 1A0
	UInt32									unk1AC[3];						// 1AC
};
assert(sizeof(hkWorld) == 0x1B8);

class bhkRefObject : public NiObject {
public:
	virtual void		SetObj(hkReferencedObject* obj);
	virtual void		UpdateRefcount(bool incRef);	// inc/decref depending on arg

	hkReferencedObject* hkObj;	// 008
};
assert(sizeof(bhkRefObject) == 0x00C);

class bhkWorld : public bhkRefObject {
public:
	virtual void		Unk_25(UInt32 arg);
	virtual hkWorld*	GetWorld();
	virtual bool		Unk_27(UInt32 arg);
	virtual bool		Unk_28();
	virtual void		FreeData(bool del);				// free hkData
	virtual UInt32		Unk_2A();
	virtual void		LoadHavokData(NiStream* stream);	// called from bhkSerializable::Load after primary load is done
	virtual void		Unk_2C();							// create object
	virtual void*		CreateHavokData(UInt8* arg);		// create Cinfo, return hkData
	virtual void		Unk_2E();							// destroy object
	virtual void		Unk_2F();
	virtual void		Unk_30();

	void*			hkData;		// 00C - stores hkConstraintData (descriptor used to build hkObj)
	void*			unk010;		// 010 - 0x108 byte object
	NiRefObject*	visDebug;	// 014
	UInt8			unk018;		// 018
	UInt8			enabled;	// 019
	UInt8			unk01A;		// 01A
	UInt8			pad01B;		// 01B
	UInt32			unk01C;		// 01C
	UInt32			unk020;		// 020
	UInt32			unk024;		// 024
	void*			unk028;		// 028 - 0x2EE0 byte buffer
	UInt32			unk02C;		// 02C
	void*			unk030;		// 030 - 0x320 byte buffer
	UInt32			unk034;		// 034
	void*			unk038;		// 038 - 0x190 byte buffer
	UInt32			unk03C;		// 03C
	void*			unk040;		// 040 - 0x320 byte buffer
	UInt32			unk044;		// 044
	void*			unk048;		// 048 - 0x2EE0 byte buffer
	UInt32			unk04C;		// 04C
	NiVector4		unk050;		// 050
	NiRefObject**	unk060;		// 060 - simple array
	UInt32			unk064;		// 064 - num elements in 060
	UInt32			unk068;		// 068
	void*			unk06C;		// 06C
	UInt32			unk070;		// 070
	UInt32			unk074;		// 074
	UInt32			pad078[7];	// 078
};
assert(sizeof(bhkWorld) == 0x094);
#elif defined(OBLIVION)
class hkWorld : public hkReferencedObject {
public:
	hkSimulation*							 simulation;						// 008
	UInt32									 unk00C[(0x020 - 0x00C) >> 2];		// 00C
	hkVector4								 gravity;							// 020
	hkSimulationIsland*						 fixedIsland;						// 030
	hkRigidBody*							 fixedRigidBody;					// 034
	hkArray<hkSimulationIsland*>			 activeSimulationIslands;			// 038
	hkArray<hkSimulationIsland*>			 inactiveSimulationIslands;			// 044
	hkArray<hkSimulationIsland*>			 dirtySimulationIslands;			// 050
	hkWorldMaintenanceManager*				 maintenanceMgr;					// 05C
	hkWorldMemoryAvailableWatchDog*			 memoryWatchDog;					// 060 - unconfirmed
	hkBroadPhase*							 broadPhase;						// 064
	hkTypedBroadPhaseDispatcher*			 broadPhaseDispatcher;				// 068
	hkPhantomBroadPhaseListener*			 phantomBroadPhaseListener;			// 06C
	hkEntityEntityBroadPhaseListener*		 entityEntityBroadPhaseListener;	// 070
	hkProcessCollisionInput*				 collisionInput;					// 074
	hkCollisionFilter*						 collisionFilter;					// 078
	hkCollisionDispatcher*					 collisionDispatcher;				// 07C
	hkWorldOperationQueue*					 pendingOperations;					// 080
	int										 pendingOperationsCount;			// 084 - unconfirmed
	int										 lockCount;							// 088
	int										 lockCountForPhantoms;				// 08C
	UInt8									 blockExecutingPendingOperations;	// 090
	UInt8									 criticalOperationsAllowed;			// 091
	UInt8									 pad092[2];							// 092
	UInt32									 unk094[(0x0A0 - 0x094) >> 2];		// 094
	CRITICAL_SECTION*						 unk0A0;							// 0A0
	UInt8									 unk0A4;							// 0A4
	UInt8									 unk0A5[3];							// 0A5
	UInt32									 unk0A8[(0x0B8 - 0x0A8) >> 2];		// 0A8
	hkArray<hkPhantom*>						 phantoms;							// 0B8
	hkArray<hkActionListener*>				 actionListeners;					// 0C4
	hkArray<hkEntityListener*>				 entityListeners;					// 0D0
	hkArray<hkPhantomListener*>				 phantomListeners;					// 0DC
	hkArray<hkConstraintListener*>			 constraintListeners;				// 0E8
	hkArray<hkWorldDeletionListener*>		 worldDeletionListeners;			// 0F4
	hkArray<hkIslandActivationListener*>	 islandActivationListeners;			// 100
	hkArray<hkWorldPostSimulationListener*>  worldPostSimulationListeners;		// 10C
	hkArray<hkWorldPostIntegrateListener*>	 worldPostIntegrateListeners;		// 118
	hkArray<hkWorldPostCollideListener*>	 worldPostCollideListeners;			// 124
	hkArray<hkIslandPostIntegrateListener*>  islandPostIntegrateListeners;		// 130
	hkArray<hkIslandPostCollideListener*>	 islandPostCollideListeners;		// 13C - unconfirmed
	hkArray<hkCollisionListener*>			 collisionListeners;				// 148
	hkReferencedObject*						 unk154;							// 154
};
assert(sizeof(hkWorld) == 0x158);

class bhkRefObject : public NiObject {
public:
	virtual void		SetObj(hkReferencedObject* obj);
	virtual void		UpdateRefcount(bool incRef);	// inc/decref depending on arg

	hkReferencedObject* hkObj;	// 008
};
assert(sizeof(bhkRefObject) == 0x00C);

class bhkWorld : public bhkRefObject {
public:
	virtual void		Unk_15(UInt32 arg);
	virtual hkWorld*	GetWorld();
	virtual bool		Unk_17(UInt32 arg);
	virtual bool		Unk_18();
	virtual void		FreeData(bool del);				// free hkData
	virtual UInt32		Unk_1A();
	virtual void		LoadHavokData(NiStream* stream);	// called from bhkSerializable::Load after primary load is done
	virtual void		Unk_1C();							// create object
	virtual void*		CreateHavokData(UInt8* arg);		// create Cinfo, return hkData
	virtual void		Unk_1E();							// destroy object
	virtual void		Unk_1F();

	void*			hkData;		// 00C - stores hkConstraintData (descriptor used to build hkObj)
	void*			unk010;		// 010 - 0x108 byte object
	NiRefObject*	visDebug;	// 014
	UInt8			unk018;		// 018
	UInt8			enabled;	// 019
	UInt8			unk01A;		// 01A
	UInt8			pad01B;		// 01B
	UInt32			unk01C;		// 01C
	UInt32			unk020;		// 020
	UInt32			unk024;		// 024
	void*			unk028;		// 028 - 0x2EE0 byte buffer
	UInt32			unk02C;		// 02C
	void*			unk030;		// 030 - 0x320 byte buffer
	UInt32			unk034;		// 034
	void*			unk038;		// 038 - 0x190 byte buffer
	UInt32			unk03C;		// 03C
	void*			unk040;		// 040 - 0x320 byte buffer
	UInt32			unk044;		// 044
	void*			unk048;		// 048 - 0x2EE0 byte buffer
	UInt32			unk04C;		// 04C
	NiVector4		unk050;		// 050
	NiRefObject**	unk060;		// 060 - simple array
	UInt32			unk064;		// 064 - num elements in 060
	UInt32			unk068;		// 068
	void*			unk06C;		// 06C
	UInt32			unk070;		// 070
	UInt32			unk074;		// 074
	UInt32			pad078[6];	// 078
};
assert(sizeof(bhkWorld) == 0x090);
#elif defined(SKYRIM)
class hkWorld : public hkReferencedObject {
public:
	hkSimulation*	simulation;						// 008
	UInt32			unk00C[(0x020 - 0x00C) >> 2];	// 00C
	hkVector4		gravity;						// 020
	//More
};
assert(sizeof(hkWorld) == 0x30);

class bhkRefObject : public NiObject {
public:
	virtual void		SetObj(hkReferencedObject* obj);
	virtual void		UpdateRefcount(bool incRef);	// inc/decref depending on arg

	hkReferencedObject* hkObj;	// 008
};
assert(sizeof(bhkRefObject) == 0x00C);

class bhkWorld : public bhkRefObject {
public:
	virtual void		Unk_23(UInt32 arg);
	virtual hkWorld*	GetWorld();

	UInt32			unk00C[0x18C5];	// 00C
};
assert(sizeof(bhkWorld) == 0x6320);
#endif