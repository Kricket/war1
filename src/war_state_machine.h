#pragma once

typedef struct
{
    WarStateType type;
    void (*enterStateFunc)(WarContext* context, WarEntity* entity, WarState* state);
    void (*leaveStateFunc)(WarContext* context, WarEntity* entity, WarState* state);
    void (*updateStateFunc)(WarContext* context, WarEntity* entity, WarState* state);
    void (*freeStateFunc)(WarState* state);
} WarStateDescriptor;

WarState* createState(WarContext* context, WarEntity* entity, WarStateType type);
WarState* createIdleState(WarContext* context, WarEntity* entity, bool lookAround);
WarState* createMoveState(WarContext* context, WarEntity* entity, s32 positionCount, vec2 positions[]);
WarState* createPatrolState(WarContext* context, WarEntity* entity, s32 positionCount, vec2 positions[]);
WarState* createFollowState(WarContext* context, WarEntity* entity, WarEntityId targetEntityId, vec2 targetTile, s32 distance);
WarState* createAttackState(WarContext* context, WarEntity* entity, WarEntityId targetEntityId, vec2 targetTile);
WarState* createDeathState(WarContext* context, WarEntity* entity);
WarState* createDamagedState(WarContext* context, WarEntity* entity);
WarState* createCollapseState(WarContext* context, WarEntity* entity);
WarState* createWaitState(WarContext* context, WarEntity* entity, f32 waitTime);
WarState* createGatherGoldState(WarContext* context, WarEntity* entity, WarEntityId goldmineId);
WarState* createMiningState(WarContext* context, WarEntity* entity, WarEntityId goldmineId);
WarState* createGatherWoodState(WarContext* context, WarEntity* entity, WarEntityId forestId, vec2 position);
WarState* createChoppingState(WarContext* context, WarEntity* entity, WarEntityId forestId, vec2 position);
WarState* createDeliverState(WarContext* context, WarEntity* entity, WarEntityId townHallId);
WarState* createTrainState(WarContext* context, WarEntity* entity, WarUnitType unitToBuild, f32 buildTime);
WarState* createUpgradeState(WarContext* context, WarEntity* entity, WarUpgradeType upgradeToBuild, f32 buildTime);
WarState* createBuildState(WarContext* context, WarEntity* entity, f32 buildTime);
WarState* createRepairState(WarContext* context, WarEntity* entity, WarEntityId buildingId);
WarState* createRepairingState(WarContext* context, WarEntity* entity, WarEntityId buildingId);
WarState* createCastState(WarContext* context, WarEntity* entity, WarSpellType spellType, WarEntityId targetEntityId, vec2 targetTile);

void changeNextState(WarContext* context, WarEntity* entity, WarState* state, bool leaveState, bool enterState);
bool changeStateNextState(WarContext* context, WarEntity* entity, WarState* state);

WarState* getState(WarEntity* entity, WarStateType type);
WarState* getDirectState(WarEntity* entity, WarStateType type);
WarState* getNextState(WarEntity* entity, WarStateType type);

#define getIdleState(entity) getDirectState(entity, WAR_STATE_IDLE)
#define getMoveState(entity) getState(entity, WAR_STATE_MOVE)
#define getPatrolState(entity) getState(entity, WAR_STATE_PATROL)
#define getFollowState(entity) getState(entity, WAR_STATE_FOLLOW)
#define getAttackState(entity) getState(entity, WAR_STATE_ATTACK)
#define getDeathState(entity) getState(entity, WAR_STATE_DEATH)
#define getCollapseState(entity) getState(entity, WAR_STATE_COLLAPSE)
#define getGatherGoldState(entity) getState(entity, WAR_STATE_GOLD)
#define getMiningState(entity) getState(entity, WAR_STATE_MINING)
#define getGatherWoodState(entity) getState(entity, WAR_STATE_WOOD)
#define getChoppingState(entity) getState(entity, WAR_STATE_CHOP)
#define getDeliverState(entity) getState(entity, WAR_STATE_DELIVER)
#define getTrainState(entity) getState(entity, WAR_STATE_TRAIN)
#define getUpgradeState(entity) getState(entity, WAR_STATE_UPGRADE)
#define getBuildState(entity) getState(entity, WAR_STATE_BUILD)
#define getRepairState(entity) getState(entity, WAR_STATE_REPAIR)
#define getRepairingState(entity) getState(entity, WAR_STATE_REPAIRING)
#define getCastState(entity) getState(entity, WAR_STATE_CAST)

bool hasState(WarEntity* entity, WarStateType type);
bool hasDirectState(WarEntity* entity, WarStateType type);
bool hasNextState(WarEntity* entity, WarStateType type);

#define isIdle(entity) hasDirectState(entity, WAR_STATE_IDLE)
#define isMoving(entity) hasState(entity, WAR_STATE_MOVE)
#define isPatrolling(entity) hasState(entity, WAR_STATE_PATROL)
#define isFollowing(entity) hasState(entity, WAR_STATE_FOLLOW)
#define isAttacking(entity) hasState(entity, WAR_STATE_ATTACK)
#define isDead(entity) hasState(entity, WAR_STATE_DEATH)
#define isCollapsing(entity) hasState(entity, WAR_STATE_COLLAPSE)
#define isGatheringGold(entity) hasState(entity, WAR_STATE_GOLD)
#define isMining(entity) hasState(entity, WAR_STATE_MINING)
#define isGatheringWood(entity) hasState(entity, WAR_STATE_WOOD)
#define isChopping(entity) hasState(entity, WAR_STATE_CHOPPING)
#define isDelivering(entity) hasState(entity, WAR_STATE_DELIVER)
#define isTraining(entity) hasState(entity, WAR_STATE_TRAIN)
#define isUpgrading(entity) hasState(entity, WAR_STATE_UPGRADE)
#define isBuilding(entity) hasState(entity, WAR_STATE_BUILD)
#define isRepairing(entity) hasState(entity, WAR_STATE_REPAIR)
#define isRepairing2(entity) hasState(entity, WAR_STATE_REPAIRING)
#define isCasting(entity) hasState(entity, WAR_STATE_CAST)

#define isGoingToIdle(entity) hasNextState(entity, WAR_STATE_IDLE)
#define isGoingToMove(entity) hasNextState(entity, WAR_STATE_MOVE)
#define isGoingToPatrol(entity) hasNextState(entity, WAR_STATE_PATROL)
#define isGoingToFollow(entity) hasNextState(entity, WAR_STATE_FOLLOW)
#define isGoingToAttack(entity) hasNextState(entity, WAR_STATE_ATTACK)
#define isGoingToDie(entity) hasNextState(entity, WAR_STATE_DEATH)
#define isGoingToCollapse(entity) hasNextState(entity, WAR_STATE_COLLAPSE)
#define isGoingToGatherGold(entity) hasNextState(entity, WAR_STATE_GOLD)
#define isGoingToMine(entity) hasNextState(entity, WAR_STATE_MINING)
#define isGoingToGatherWood(entity) hasNextState(entity, WAR_STATE_WOOD)
#define isGoingToChop(entity) hasNextState(entity, WAR_STATE_CHOPPING)
#define isGoingToDeliver(entity) hasNextState(entity, WAR_STATE_DELIVER)
#define isGoingToTrain(entity) hasNextState(entity, WAR_STATE_TRAIN)
#define isGoingToUpgrade(entity) hasNextState(entity, WAR_STATE_UPGRADE)
#define isGoingToBuild(entity) hasNextState(entity, WAR_STATE_BUILD)
#define isGoingToRepair(entity) hasNextState(entity, WAR_STATE_REPAIR)
#define isGoingToRepair2(entity) hasNextState(entity, WAR_STATE_REPAIRING)
#define isGoingToCast(entity) hasNextState(entity, WAR_STATE_CAST)

#define setDelay(state, seconds) ((state)->delay = (seconds))

void sendToIdleState(WarContext* context, WarEntity* entity, bool lookAround);
void sendToMoveState(WarContext* context, WarEntity* entity, s32 positionCount, vec2 positions[]);
void sendToPatrolState(WarContext* context, WarEntity* entity, s32 positionCount, vec2 positions[]);
void sendToFollowState(WarContext* context, WarEntity* entity, WarEntityId targetEntityId, vec2 targetTile, s32 distance);
void sendToAttackState(WarContext* context, WarEntity* entity, WarEntityId targetEntityId, vec2 targetTile);
void sendToDeathState(WarContext* context, WarEntity* entity);
void sendToCollapseState(WarContext* context, WarEntity* entity);
void sendToWaitState(WarContext* context, WarEntity* entity, f32 waitTime, WarState* nextState);
void sendToGatherGoldState(WarContext* context, WarEntity* entity, WarEntityId goldmineId);
void sendToMiningState(WarContext* context, WarEntity* entity, WarEntityId goldmineId);
void sendToGatherWoodState(WarContext* context, WarEntity* entity, WarEntityId forestId, vec2 position);
void sendToChoppingState(WarContext* context, WarEntity* entity, WarEntityId forestId, vec2 position);
void sendToDeliverState(WarContext* context, WarEntity* entity, WarEntityId townHallId, WarState* nextState);
void sendToTrainState(WarContext* context, WarEntity* entity, WarUnitType unitToBuild, f32 buildTime);
void sendToUpgradeState(WarContext* context, WarEntity* entity, WarUpgradeType upgradeToBuild, f32 buildTime);
void sendToBuildState(WarContext* context, WarEntity* entity, f32 buildTime);
void sendToRepairState(WarContext* context, WarEntity* entity, WarEntityId buildingId);
void sendToRepairingState(WarContext* context, WarEntity* entity, WarEntityId buildingId);
// void sendToCastState(WarContext* context, WarEntity* entity, WarSpellType spellType, WarEntityId targetEntityId, vec2 targetTile);

void enterState(WarContext* context, WarEntity* entity, WarState* state);
void leaveState(WarContext* context, WarEntity* entity, WarState* state);
void updateStateMachine(WarContext* context, WarEntity* entity);
void freeState(WarState* state);
