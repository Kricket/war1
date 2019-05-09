void executeMoveCommand(WarContext* context, vec2 targetPoint)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    s32 selEntitiesCount = map->selectedEntities.count;

    // move the selected units to the target point,
    // but keeping the bounding box that the 
    // selected units make, this is an intent to keep the
    // formation of the selected units
    //
    rect* rs = (rect*)xcalloc(selEntitiesCount, sizeof(rect));

    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        rs[i] = rectv(entity->transform.position, getUnitSpriteSize(entity));
    }

    rect bbox = rs[0];

    for(s32 i = 1; i < selEntitiesCount; i++)
    {
        if (rs[i].x < bbox.x)
            bbox.x = rs[i].x;
        if (rs[i].y < bbox.y)
            bbox.y = rs[i].y;
        if (rs[i].x + rs[i].width > bbox.x + bbox.width)
            bbox.width = (rs[i].x + rs[i].width) - bbox.x;
        if (rs[i].y + rs[i].height > bbox.y + bbox.height)
            bbox.height = (rs[i].y + rs[i].height) - bbox.y;
    }

    rect targetbbox = rectf(
        targetPoint.x - halff(bbox.width),
        targetPoint.y - halff(bbox.height),
        bbox.width,
        bbox.height);

    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        vec2 position = vec2f(
            rs[i].x + halff(rs[i].width), 
            rs[i].y + halff(rs[i].height));

        position = vec2MapToTileCoordinates(position);

        rect targetRect = rectf(
            targetbbox.x + (rs[i].x - bbox.x),
            targetbbox.y + (rs[i].y - bbox.y),
            rs[i].width, 
            rs[i].height);

        vec2 target = vec2f(
            targetRect.x + halff(targetRect.width), 
            targetRect.y + halff(targetRect.height));

        target = vec2MapToTileCoordinates(target);

        if (isKeyPressed(input, WAR_KEY_SHIFT))
        {
            if (isPatrolling(entity))
            {
                if(isMoving(entity))
                {
                    WarState* moveState = getMoveState(entity);
                    vec2ListAdd(&moveState->move.positions, target);
                }
                
                WarState* patrolState = getPatrolState(entity);
                vec2ListAdd(&patrolState->patrol.positions, target);
            }
            else if(isMoving(entity) && !isAttacking(entity))
            {
                WarState* moveState = getMoveState(entity);
                vec2ListAdd(&moveState->move.positions, target);
            }
            else
            {
                WarState* moveState = createMoveState(context, entity, 2, arrayArg(vec2, position, target));
                changeNextState(context, entity, moveState, true, true);
            }
        }
        else
        {
            WarState* moveState = createMoveState(context, entity, 2, arrayArg(vec2, position, target));
            changeNextState(context, entity, moveState, true, true);

            // WarState* patrolState = createPatrolState(context, entity, 2, arrayArg(vec2, position, target));
            // changeNextState(context, entity, patrolState, true, true);
        }
    }

    free(rs);
}

void executeFollowCommand(WarContext* context, WarEntity* targetEntity)
{
    WarMap* map = context->map;

    s32 selEntitiesCount = map->selectedEntities.count;
    for (s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        WarState* followState = createFollowState(context, entity, targetEntity->id, 1);
        changeNextState(context, entity, followState, true, true);
    }
}

void executeStopCommand(WarContext* context)
{
    WarMap* map = context->map;

    s32 selEntitiesCount = map->selectedEntities.count;
    for (s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        WarState* idleState = createIdleState(context, entity, true);
        changeNextState(context, entity, idleState, true, true);
    }
}

void executeHarvestCommand(WarContext* context, WarEntity* targetEntity, vec2 targetTile)
{
    WarMap* map = context->map;

    assert(isUnitOfType(targetEntity, WAR_UNIT_GOLDMINE) ||
           targetEntity->type == WAR_ENTITY_TYPE_FOREST);

    s32 selEntitiesCount = map->selectedEntities.count;
    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        if (isWorkerUnit(entity))
        {
            if (isCarryingResources(entity))
            {
                // find the closest town hall to deliver the gold
                WarRace race = getUnitRace(entity);
                WarUnitType townHallType = getTownHallOfRace(race);
                WarEntity* townHall = findClosestUnitOfType(context, entity, townHallType);

                // if the town hall doesn't exists (it could be under attack and get destroyed), go idle
                if (townHall)
                {
                    WarState* deliverState = createDeliverState(context, entity, townHall->id);

                    if (targetEntity->type == WAR_ENTITY_TYPE_FOREST)
                    {
                        deliverState->nextState = createGatherWoodState(context, entity, targetEntity->id, targetTile);
                    }
                    else
                    {
                        deliverState->nextState = createGatherGoldState(context, entity, targetEntity->id);
                    }

                    changeNextState(context, entity, deliverState, true, true);
                }
            }
            else
            {
                if (targetEntity->type == WAR_ENTITY_TYPE_FOREST)
                {
                    WarState* gatherWoodState = createGatherWoodState(context, entity, targetEntity->id, targetTile);
                    changeNextState(context, entity, gatherWoodState, true, true);
                }
                else
                {
                    WarState* gatherGoldState = createGatherGoldState(context, entity, targetEntity->id);
                    changeNextState(context, entity, gatherGoldState, true, true);
                }
            }
        }
        else if (isDudeUnit(entity))
        {
            WarState* followState = createFollowState(context, entity, goldmine->id, 1);
            changeNextState(context, entity, followState, true, true);
        }
    }
}

void executeDeliverCommand(WarContext* context, WarEntity* targetEntity)
{
    WarMap* map = context->map;

    s32 selEntitiesCount = map->selectedEntities.count;
    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        WarEntity* townHall = targetEntity;
        if (!townHall)
        {
            WarRace race = getUnitRace(entity);
            WarUnitType townHallType = getTownHallOfRace(race);
            townHall = findClosestUnitOfType(context, entity, townHallType);
            assert(townHall);
        }

        if (isWorkerUnit(entity) && isCarryingResources(entity))
        {
            WarState* deliverState = createDeliverState(context, entity, townHall->id);
            changeNextState(context, entity, deliverState, true, true);
        }
        else if (isDudeUnit(entity))
        {
            WarState* followState = createFollowState(context, entity, townHall->id, 1);
            changeNextState(context, entity, followState, true, true);
        }
    }
}

void executeRepairCommand(WarContext* context, WarEntity* targetEntity)
{
    WarMap* map = context->map;

    s32 selEntitiesCount = map->selectedEntities.count;
    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        // the unit can't repair itself
        if (entity->id == targetEntity->id)
        {
            continue;
        }

        if (isWorkerUnit(entity))
        {
            WarState* repairState = createRepairState(context, entity, targetEntity->id);
            changeNextState(context, entity, repairState, true, true);
        }
    }
}

void executeAttackCommand(WarContext* context, WarEntity* targetEntity, vec2 targetTile)
{
    WarMap* map = context->map;

    s32 selEntitiesCount = map->selectedEntities.count;
    for(s32 i = 0; i < selEntitiesCount; i++)
    {
        WarEntityId entityId = map->selectedEntities.items[i];
        WarEntity* entity = findEntity(context, entityId);
        assert(entity);

        // the unit can't attack itself
        if (entity->id != targetEntity->id)
        {
            if (canAttack(context, entity, targetEntity))
            {
                WarState* attackState = createAttackState(context, entity, targetEntity->id, targetTile);
                changeNextState(context, entity, attackState, true, true);
            }
        }
    }
}

bool executeCommand(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;
    WarPlayerInfo* player = &map->players[0];
    WarUnitCommand* command = &map->command;
    
    if (command->type == WAR_COMMAND_NONE)
    {
        return false;
    }

    switch (command->type)
    {
        case WAR_COMMAND_MOVE:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);

                    executeMoveCommand(context, targetPoint);
                    
                    command->type = WAR_COMMAND_NONE;
                    return true;
                }
            }

            return false;
        }

        case WAR_COMMAND_STOP:
        {
            executeStopCommand(context);

            command->type = WAR_COMMAND_NONE;
            return true;
        }

        case WAR_COMMAND_HARVEST:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    WarEntityId targetEntityId = getTileEntityId(map->finder, targetTile.x, targetTile.y);
                    WarEntity* targetEntity = findEntity(context, targetEntityId);
                    if (targetEntity)
                    {
                        if (isUnitOfType(targetEntity, WAR_UNIT_GOLDMINE) ||
                            targetEntity->type == WAR_ENTITY_TYPE_FOREST)
                        {
                            executeHarvestCommand(context, targetEntity, targetTile);
                        }
                    }

                    command->type = WAR_COMMAND_NONE;
                    return true;
                }
            }
            
            return false;
        }

        case WAR_COMMAND_DELIVER:
        {
            executeDeliverCommand(context, NULL);
            
            command->type = WAR_COMMAND_NONE;
            return true;
        }

        case WAR_COMMAND_REPAIR:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    WarEntityId targetEntityId = getTileEntityId(map->finder, targetTile.x, targetTile.y);
                    WarEntity* targetEntity = findEntity(context, targetEntityId);
                    if (targetEntity && isBuildingUnit(targetEntity))
                    {
                        executeRepairCommand(context, targetEntity);
                    }

                    command->type = WAR_COMMAND_NONE;
                    return true;
                }
            }

            return false;
        }

        case WAR_COMMAND_ATTACK:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    WarEntityId targetEntityId = getTileEntityId(map->finder, targetTile.x, targetTile.y);
                    WarEntity* targetEntity = findEntity(context, targetEntityId);
                    if (targetEntity && (isUnit(targetEntity) || isWall(targetEntity)))
                    {
                        executeAttackCommand(context, targetEntity, targetTile);
                    }

                    command->type = WAR_COMMAND_NONE;
                    return true;
                }
            }
            
            return false;
        }

        case WAR_COMMAND_TRAIN_FOOTMAN:
        case WAR_COMMAND_TRAIN_GRUNT:
        case WAR_COMMAND_TRAIN_PEASANT:
        case WAR_COMMAND_TRAIN_PEON:
        case WAR_COMMAND_TRAIN_CATAPULT_HUMANS:
        case WAR_COMMAND_TRAIN_CATAPULT_ORCS:
        case WAR_COMMAND_TRAIN_KNIGHT:
        case WAR_COMMAND_TRAIN_RAIDER:
        case WAR_COMMAND_TRAIN_ARCHER:
        case WAR_COMMAND_TRAIN_SPEARMAN:
        case WAR_COMMAND_TRAIN_CONJURER:
        case WAR_COMMAND_TRAIN_WARLOCK:
        case WAR_COMMAND_TRAIN_CLERIC:
        case WAR_COMMAND_TRAIN_NECROLYTE:
        {
            WarUnitType unitToTrain = command->train.unitToTrain;
            WarUnitType buildingUnit = command->train.buildingUnit;

            assert(map->selectedEntities.count == 1);

            WarEntity* selectedEntity = findEntity(context, map->selectedEntities.items[0]);
            assert(selectedEntity && isBuildingUnit(selectedEntity));
            assert(selectedEntity->unit.type == buildingUnit);

            WarUnitStats stats = getUnitStats(unitToTrain);
            if (checkFarmFood(context, player) && 
                decreasePlayerResources(context, player, stats.goldCost, stats.woodCost))
            {
                f32 buildTime = getScaledTime(context, stats.buildTime);
                WarState* trainState = createTrainState(context, selectedEntity, unitToTrain, buildTime);
                changeNextState(context, selectedEntity, trainState, true, true);
            }

            command->type = WAR_COMMAND_NONE;
            return true;
        }

        case WAR_COMMAND_UPGRADE_SWORDS:
        case WAR_COMMAND_UPGRADE_AXES:
        case WAR_COMMAND_UPGRADE_SHIELD_HUMANS:
        case WAR_COMMAND_UPGRADE_SHIELD_ORCS:
        case WAR_COMMAND_UPGRADE_ARROWS:
        case WAR_COMMAND_UPGRADE_SPEARS:
        case WAR_COMMAND_UPGRADE_HORSES:
        case WAR_COMMAND_UPGRADE_WOLVES:
        case WAR_COMMAND_UPGRADE_SCORPION:
        case WAR_COMMAND_UPGRADE_SPIDER:
        case WAR_COMMAND_UPGRADE_RAIN_OF_FIRE:
        case WAR_COMMAND_UPGRADE_POISON_CLOUD:
        case WAR_COMMAND_UPGRADE_WATER_ELEMENTAL:
        case WAR_COMMAND_UPGRADE_DAEMON:
        case WAR_COMMAND_UPGRADE_HEALING:
        case WAR_COMMAND_UPGRADE_RAISE_DEAD:
        case WAR_COMMAND_UPGRADE_FAR_SIGHT:
        case WAR_COMMAND_UPGRADE_DARK_VISION:
        case WAR_COMMAND_UPGRADE_INVISIBILITY:
        case WAR_COMMAND_UPGRADE_UNHOLY_ARMOR:
        {
            WarUpgradeType upgradeToBuild = command->upgrade.upgradeToBuild;
            WarUnitType buildingUnit = command->upgrade.buildingUnit;

            assert(map->selectedEntities.count == 1);

            WarEntity* selectedEntity = findEntity(context, map->selectedEntities.items[0]);
            assert(selectedEntity && isBuildingUnit(selectedEntity));
            assert(selectedEntity->unit.type == buildingUnit);

            assert(hasRemainingUpgrade(player, upgradeToBuild));

            WarUpgradeStats stats = getUpgradeStats(upgradeToBuild);
            s32 level = getUpgradeLevel(player, upgradeToBuild);
            if (decreasePlayerResources(context, player, stats.goldCost[level], 0))
            {
                f32 buildTime = getScaledTime(context, stats.buildTime);
                WarState* upgradeState = createUpgradeState(context, selectedEntity, upgradeToBuild, buildTime);
                changeNextState(context, selectedEntity, upgradeState, true, true);
            }

            command->type = WAR_COMMAND_NONE;
            return true;
        }

        case WAR_COMMAND_BUILD_FARM_HUMANS:
        case WAR_COMMAND_BUILD_FARM_ORCS:
        case WAR_COMMAND_BUILD_BARRACKS_HUMANS:
        case WAR_COMMAND_BUILD_BARRACKS_ORCS:
        case WAR_COMMAND_BUILD_CHURCH:
        case WAR_COMMAND_BUILD_TEMPLE:
        case WAR_COMMAND_BUILD_TOWER_HUMANS:
        case WAR_COMMAND_BUILD_TOWER_ORCS:
        case WAR_COMMAND_BUILD_TOWNHALL_HUMANS:
        case WAR_COMMAND_BUILD_TOWNHALL_ORCS:
        case WAR_COMMAND_BUILD_LUMBERMILL_HUMANS:
        case WAR_COMMAND_BUILD_LUMBERMILL_ORCS:
        case WAR_COMMAND_BUILD_STABLE:
        case WAR_COMMAND_BUILD_KENNEL:
        case WAR_COMMAND_BUILD_BLACKSMITH_HUMANS:
        case WAR_COMMAND_BUILD_BLACKSMITH_ORCS:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    assert(map->selectedEntities.count > 0);

                    WarEntityId workerId = map->selectedEntities.items[0];
                    WarEntity* worker = findEntity(context, workerId);
                    assert(worker);

                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    WarUnitType buildingToBuild = command->build.buildingToBuild;

                    WarBuildingStats stats = getBuildingStats(buildingToBuild);
                    if (checkTileToBuild(context, buildingToBuild, targetTile.x, targetTile.y) &&
                        decreasePlayerResources(context, player, stats.goldCost, stats.woodCost))
                    {
                        WarEntity* building = createBuilding(context, buildingToBuild, targetTile.x, targetTile.y, 0, true);
                        WarState* repairState = createRepairState(context, worker, building->id);
                        changeNextState(context, worker, repairState, true, true);

                        command->type = WAR_COMMAND_NONE;
                    }
                    
                    return true;
                }
            }

            return false;
        }

        case WAR_COMMAND_BUILD_WALL:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    assert(map->selectedEntities.count > 0);

                    WarEntityId townHallId = map->selectedEntities.items[0];
                    WarEntity* townHall = findEntity(context, townHallId);

                    WarUnitType townHallType = getTownHallOfRace(player->race);
                    assert(isUnitOfType(townHall, townHallType));

                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    if (checkTileToBuildRoadOrWall(context, targetTile.x, targetTile.y) &&
                        decreasePlayerResources(context, player, WAR_WALL_GOLD_COST, WAR_WALL_WOOD_COST))
                    {
                        WarEntity* wall = map->wall;
                        WarWallPiece* piece = addWallPiece(wall, targetTile.x, targetTile.y, 0);
                        piece->hp = WAR_WALL_MAX_HP;
                        piece->maxhp = WAR_WALL_MAX_HP;

                        determineWallTypes(context, wall);

                        command->type = WAR_COMMAND_NONE;
                    }

                    return true;
                }
            }

            return false;
        }

        case WAR_COMMAND_BUILD_ROAD:
        {
            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    assert(map->selectedEntities.count > 0);

                    WarEntityId townHallId = map->selectedEntities.items[0];
                    WarEntity* townHall = findEntity(context, townHallId);

                    WarUnitType townHallType = getTownHallOfRace(player->race);
                    assert(isUnitOfType(townHall, townHallType));

                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    if (checkTileToBuildRoadOrWall(context, targetTile.x, targetTile.y) &&
                        decreasePlayerResources(context, player, WAR_ROAD_GOLD_COST, WAR_ROAD_WOOD_COST))
                    {
                        WarEntity* road = map->road;
                        addRoadPiece(road, targetTile.x, targetTile.y, 0);

                        determineRoadTypes(context, road);

                        command->type = WAR_COMMAND_NONE;
                    }

                    return true;
                }
            }

            return false;
        }

        case WAR_COMMAND_BUILD_BASIC:
        case WAR_COMMAND_BUILD_ADVANCED:
        case WAR_COMMAND_CANCEL:
        {
            // nothing here
            return false;
        }
    }

    return false;
}

// train units
void trainUnit(WarContext* context, WarUnitCommandType commandType, WarUnitType unitToTrain, WarUnitType buildingUnit)
{
    WarMap* map = context->map;

    map->command.type = commandType;
    map->command.train.unitToTrain = unitToTrain;
    map->command.train.buildingUnit = buildingUnit;
}

void trainFootman(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_FOOTMAN, WAR_UNIT_FOOTMAN, WAR_UNIT_BARRACKS_HUMANS);
}

void trainGrunt(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_GRUNT, WAR_UNIT_GRUNT, WAR_UNIT_BARRACKS_ORCS);
}

void trainPeasant(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_PEASANT, WAR_UNIT_PEASANT, WAR_UNIT_TOWNHALL_HUMANS);
}

void trainPeon(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_PEON, WAR_UNIT_PEON, WAR_UNIT_TOWNHALL_ORCS);
}

void trainHumanCatapult(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_CATAPULT_HUMANS, WAR_UNIT_CATAPULT_HUMANS, WAR_UNIT_BARRACKS_HUMANS);
}

void trainOrcCatapult(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_CATAPULT_ORCS, WAR_UNIT_CATAPULT_ORCS, WAR_UNIT_BARRACKS_ORCS);
}

void trainKnight(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_KNIGHT, WAR_UNIT_KNIGHT, WAR_UNIT_BARRACKS_HUMANS);
}

void trainRaider(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_RAIDER, WAR_UNIT_KNIGHT, WAR_UNIT_BARRACKS_ORCS);
}

void trainArcher(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_ARCHER, WAR_UNIT_ARCHER, WAR_UNIT_BARRACKS_HUMANS);
}

void trainSpearman(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_SPEARMAN, WAR_UNIT_SPEARMAN, WAR_UNIT_BARRACKS_ORCS);
}

void trainConjurer(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_CONJURER, WAR_UNIT_CONJURER, WAR_UNIT_TOWER_HUMANS);
}

void trainWarlock(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_WARLOCK, WAR_UNIT_WARLOCK, WAR_UNIT_TOWER_ORCS);
}

void trainCleric(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_CLERIC, WAR_UNIT_CLERIC, WAR_UNIT_CHURCH);
}

void trainNecrolyte(WarContext* context, WarEntity* entity)
{
    trainUnit(context, WAR_COMMAND_TRAIN_NECROLYTE, WAR_UNIT_NECROLYTE, WAR_UNIT_TEMPLE);
}

// upgrades
void upgradeUpgrade(WarContext* context, WarUnitCommandType commandType, WarUpgradeType upgradeToBuild, WarUnitType buildingUnit)
{
    WarMap* map = context->map;

    map->command.type = commandType;
    map->command.upgrade.upgradeToBuild = upgradeToBuild;
    map->command.upgrade.buildingUnit = buildingUnit;
}

void upgradeSwords(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SWORDS, WAR_UPGRADE_SWORDS, WAR_UNIT_BLACKSMITH_HUMANS);
}

void upgradeAxes(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_AXES, WAR_UPGRADE_AXES, WAR_UNIT_BLACKSMITH_ORCS);
}

void upgradeHumanShields(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SHIELD_HUMANS, WAR_UPGRADE_SHIELD, WAR_UNIT_BLACKSMITH_HUMANS);
}

void upgradeOrcsShields(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SHIELD_ORCS, WAR_UPGRADE_SHIELD, WAR_UNIT_BLACKSMITH_ORCS);
}

void upgradeArrows(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_ARROWS, WAR_UPGRADE_ARROWS, WAR_UNIT_LUMBERMILL_HUMANS);
}

void upgradeSpears(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SPEARS, WAR_UPGRADE_SPEARS, WAR_UNIT_LUMBERMILL_ORCS);
}

void upgradeHorses(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_HORSES, WAR_UPGRADE_HORSES, WAR_UNIT_STABLE);
}

void upgradeWolves(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_WOLVES, WAR_UPGRADE_WOLVES, WAR_UNIT_KENNEL);
}

void upgradeScorpions(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SCORPION, WAR_UPGRADE_SCORPIONS, WAR_UNIT_TOWER_HUMANS);
}

void upgradeSpiders(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_SPIDER, WAR_UPGRADE_SPIDERS, WAR_UNIT_TOWER_ORCS);
}

void upgradeRainOfFire(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_RAIN_OF_FIRE, WAR_UPGRADE_RAIN_OF_FIRE, WAR_UNIT_TOWER_HUMANS);
}

void upgradePoisonCloud(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_POISON_CLOUD, WAR_UPGRADE_POISON_CLOUD, WAR_UNIT_TOWER_ORCS);
}

void upgradeWaterElemental(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_WATER_ELEMENTAL, WAR_UPGRADE_WATER_ELEMENTAL, WAR_UNIT_TOWER_HUMANS);
}

void upgradeDaemon(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_DAEMON, WAR_UPGRADE_DAEMON, WAR_UNIT_TOWER_ORCS);
}

void upgradeHealing(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_HEALING, WAR_UPGRADE_HEALING, WAR_UNIT_CHURCH);
}

void upgradeRaiseDead(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_RAISE_DEAD, WAR_UPGRADE_RAISE_DEAD, WAR_UNIT_TEMPLE);
}

void upgradeFarSight(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_FAR_SIGHT, WAR_UPGRADE_FAR_SIGHT, WAR_UNIT_CHURCH);
}

void upgradeDarkVision(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_DARK_VISION, WAR_UPGRADE_DARK_VISION, WAR_UNIT_TEMPLE);
}

void upgradeInvisibility(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_INVISIBILITY, WAR_UPGRADE_INVISIBILITY, WAR_UNIT_CHURCH);
}

void upgradeUnholyArmor(WarContext* context, WarEntity* entity)
{
    upgradeUpgrade(context, WAR_COMMAND_UPGRADE_UNHOLY_ARMOR, WAR_UPGRADE_UNHOLY_ARMOR, WAR_UNIT_TEMPLE);
}

// cancel
void cancel(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;
    WarPlayerInfo* player = &map->players[0];

    map->command.type = WAR_COMMAND_NONE;

    for (s32 i = 0; i < map->selectedEntities.count; i++)
    {
        WarEntityId selectedEntityId = map->selectedEntities.items[i];
        WarEntity* selectedEntity = findEntity(context, selectedEntityId);

        if (isBuildingUnit(selectedEntity))
        {
            if (isBuilding(selectedEntity) || isGoingToBuild(selectedEntity))
            {
                WarBuildingStats stats = getBuildingStats(selectedEntity->unit.type);
                increasePlayerResources(context, player, stats.goldCost, stats.woodCost);

                WarState* collapseState = createCollapseState(context, selectedEntity);
                changeNextState(context, selectedEntity, collapseState, true, true);
            }
            else if (selectedEntity->unit.building)
            {
                if (isTraining(selectedEntity) || isGoingToTrain(selectedEntity))
                {
                    WarState* trainState = getTrainState(selectedEntity);
                    WarUnitType unitToBuild = trainState->train.unitToBuild;

                    WarUnitStats stats = getUnitStats(unitToBuild);
                    increasePlayerResources(context, player, stats.goldCost, stats.woodCost);
                }
                else if (isUpgrading(selectedEntity) || isGoingToUpgrade(selectedEntity))
                {
                    WarState* upgradeState = getUpgradeState(selectedEntity);
                    WarUpgradeType upgradeToBuild = upgradeState->upgrade.upgradeToBuild;
                    assert(hasRemainingUpgrade(player, upgradeToBuild));

                    s32 upgradeLevel = getUpgradeLevel(player, upgradeToBuild);
                    WarUpgradeStats stats = getUpgradeStats(upgradeToBuild);
                    increasePlayerResources(context, player, stats.goldCost[upgradeLevel], 0);
                }

                WarState* idleState = createIdleState(context, entity, false);
                changeNextState(context, selectedEntity, idleState, true, true);
            }
        }
    }
}

// basic
void move(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_MOVE;
}

void stop(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_STOP;
}

void harvest(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_HARVEST;
}

void deliver(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_DELIVER;
}

void repair(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_REPAIR;
}

void attack(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_ATTACK;
}

void buildBasic(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_BUILD_BASIC;
}

void buildAdvanced(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;

    map->command.type = WAR_COMMAND_BUILD_ADVANCED;
}

void buildBuilding(WarContext* context, WarUnitCommandType commandType, WarUnitType buildingToBuild)
{
    WarMap* map = context->map;
    
    map->command.type = commandType;
    map->command.build.buildingToBuild = buildingToBuild;
}

void buildFarmHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_FARM_HUMANS, WAR_UNIT_FARM_HUMANS);
}

void buildFarmOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_FARM_ORCS, WAR_UNIT_FARM_ORCS);
}

void buildBarracksHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_BARRACKS_HUMANS, WAR_UNIT_BARRACKS_HUMANS);
}

void buildBarracksOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_BARRACKS_ORCS, WAR_UNIT_BARRACKS_ORCS);
}

void buildChurch(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_CHURCH, WAR_UNIT_CHURCH);
}

void buildTemple(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_TEMPLE, WAR_UNIT_TEMPLE);
}

void buildTowerHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_TOWER_HUMANS, WAR_UNIT_TOWER_HUMANS);
}

void buildTowerOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_TOWER_ORCS, WAR_UNIT_TOWER_ORCS);
}

void buildTownHallHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_TOWNHALL_HUMANS, WAR_UNIT_TOWNHALL_HUMANS);
}

void buildTownHallOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_TOWNHALL_ORCS, WAR_UNIT_TOWNHALL_ORCS);
}

void buildLumbermillHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_LUMBERMILL_HUMANS, WAR_UNIT_LUMBERMILL_HUMANS);
}

void buildLumbermillOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_LUMBERMILL_ORCS, WAR_UNIT_LUMBERMILL_ORCS);
}

void buildStable(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_STABLE, WAR_UNIT_STABLE);
}

void buildKennel(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_KENNEL, WAR_UNIT_KENNEL);
}

void buildBlacksmithHumans(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_BLACKSMITH_HUMANS, WAR_UNIT_BLACKSMITH_HUMANS);
}

void buildBlacksmithOrcs(WarContext* context, WarEntity* entity)
{
    buildBuilding(context, WAR_COMMAND_BUILD_BLACKSMITH_ORCS, WAR_UNIT_BLACKSMITH_HUMANS);
}

void buildWall(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;
    
    map->command.type = WAR_COMMAND_BUILD_WALL;
}

void buildRoad(WarContext* context, WarEntity* entity)
{
    WarMap* map = context->map;
    
    map->command.type = WAR_COMMAND_BUILD_ROAD;
}
