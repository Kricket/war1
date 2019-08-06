void addEntityToSelection(WarContext* context, WarEntityId id)
{
    WarMap* map = context->map;

    // subtitute this with a set data structure that doesn't allow duplicates
    if (!WarEntityIdListContains(&map->selectedEntities, id))
        WarEntityIdListAdd(&map->selectedEntities, id);
}

void removeEntityFromSelection(WarContext* context, WarEntityId id)
{
    WarMap* map = context->map;
    WarEntityIdListRemove(&map->selectedEntities, id);
}

void clearSelection(WarContext* context)
{
    WarMap* map = context->map;
    WarEntityIdListClear(&map->selectedEntities);
}

WarEntityList* getEntities(WarMap* map)
{
    return &map->entities;
}

WarEntityList* getEntitiesOfType(WarMap* map, WarEntityType type)
{
    return WarEntityMapGet(&map->entitiesByType, type);
}

WarEntityList* getUnitsOfType(WarMap* map, WarUnitType type)
{
    return WarUnitMapGet(&map->unitsByType, type);
}

WarEntityList* getUIEntities(WarMap* map)
{
    return &map->uiEntities;
}

vec2 vec2ScreenToMapCoordinates(WarContext* context, vec2 v)
{
    WarMap* map = context->map;

    rect mapPanel = map->mapPanel;
    rect viewport = map->viewport;

    v = vec2Translatef(v, -mapPanel.x, -mapPanel.y);
    v = vec2Translatef(v, viewport.x, viewport.y);
    return v;
}

vec2 vec2ScreenToMinimapCoordinates(WarContext* context, vec2 v)
{
    WarMap* map = context->map;

    rect minimapPanel = map->minimapPanel;
    
    v = vec2Translatef(v, -minimapPanel.x, -minimapPanel.y);
    return v;
}

rect rectScreenToMapCoordinates(WarContext* context, rect r)
{
    WarMap* map = context->map;

    rect mapPanel = map->mapPanel;
    rect viewport = map->viewport;

    r = rectTranslatef(r, -mapPanel.x, -mapPanel.y);
    r = rectTranslatef(r, viewport.x, viewport.y);
    return r;
}

vec2 vec2MapToScreenCoordinates(WarContext* context, vec2 v)
{
    WarMap* map = context->map;

    v = vec2Translatef(v, -map->viewport.x, -map->viewport.y);
    v = vec2Translatef(v, map->mapPanel.x, map->mapPanel.y);
    return v;
}

rect rectMapToScreenCoordinates(WarContext* context, rect r)
{
    WarMap* map = context->map;

    r = rectTranslatef(r, -map->viewport.x, -map->viewport.y);
    r = rectTranslatef(r, map->mapPanel.x, map->mapPanel.y);
    return r;
}

vec2 vec2MapToTileCoordinates(vec2 v)
{
    v.x = floorf(v.x / MEGA_TILE_WIDTH);
    v.y = floorf(v.y / MEGA_TILE_HEIGHT);
    return v;
}

vec2 vec2TileToMapCoordinates(vec2 v, bool centeredInTile)
{
    v.x *= MEGA_TILE_WIDTH;
    v.y *= MEGA_TILE_HEIGHT;

    if (centeredInTile)
    {
        v.x += halfi(MEGA_TILE_WIDTH);
        v.y += halfi(MEGA_TILE_HEIGHT);
    }

    return v;
}

vec2 vec2MinimapToViewportCoordinates(WarContext* context, vec2 v)
{
    WarMap* map = context->map;

    rect minimapPanel = map->minimapPanel;
    vec2 minimapPanelSize = vec2f(minimapPanel.width, minimapPanel.height);

    vec2 minimapViewportSize = vec2f(MINIMAP_VIEWPORT_WIDTH, MINIMAP_VIEWPORT_HEIGHT);

    v = vec2Translatef(v, -minimapViewportSize.x / 2, -minimapViewportSize.y / 2);
    v = vec2Clampv(v, VEC2_ZERO, vec2Subv(minimapPanelSize, minimapViewportSize));
    return v;
}

WarMapTile* getMapTileState(WarMap* map, s32 x, s32 y)
{
    return &map->tiles[y * MAP_TILES_WIDTH + x];
}

void setMapTileState(WarMap* map, s32 startX, s32 startY, s32 width, s32 height, WarMapTileState tileState)
{
    if (!inRange(startX, 0, MAP_TILES_WIDTH) || !inRange(startY, 0, MAP_TILES_HEIGHT))
        return;

    if (startX + width >= MAP_TILES_WIDTH)
        width = MAP_TILES_WIDTH - startX;

    if (startY + height >= MAP_TILES_HEIGHT)
        height = MAP_TILES_HEIGHT - startY;

    s32 endX = startX + width;
    s32 endY = startY + height;

    for(s32 y = startY; y < endY; y++)
    {
        for(s32 x = startX; x < endX; x++)
        {
            // exclude the corners of the area to get a more "rounded" shape
            if ((y == startY || y == endY - 1) && (x == startX || x == endX - 1))
                continue;
            
            map->tiles[y * MAP_TILES_WIDTH + x].state = tileState;
        }
    }
}

void setUnitMapTileState(WarMap* map, WarEntity* entity, WarMapTileState tileState)
{
    assert(isUnit(entity));

    vec2 position = getUnitPosition(entity, true);
    vec2 unitSize = getUnitSize(entity);
    rect unitRect = rectv(position, unitSize);

    if (isBuildingUnit(entity))
    {
        WarBuildingStats stats = getBuildingStats(entity->unit.type);
        unitRect = rectExpand(unitRect, stats.sight, stats.sight);
    }
    else
    {
        WarUnitStats stats = getUnitStats(entity->unit.type);
        unitRect = rectExpand(unitRect, stats.sight, stats.sight);
    }

    setMapTileState(map, unitRect.x, unitRect.y, unitRect.width, unitRect.height, tileState);
}

bool checkMapTiles(WarMap* map, s32 startX, s32 startY, s32 width, s32 height, s32 states)
{
    if (!inRange(startX, 0, MAP_TILES_WIDTH) || !inRange(startY, 0, MAP_TILES_HEIGHT))
        return false;

    if (startX + width >= MAP_TILES_WIDTH)
        width = MAP_TILES_WIDTH - startX;

    if (startY + height >= MAP_TILES_HEIGHT)
        height = MAP_TILES_HEIGHT - startY;

    s32 endX = startX + width;
    s32 endY = startY + height;

    for(s32 y = startY; y < endY; y++)
    {
        for(s32 x = startX; x < endX; x++)
        {
            s32 index = y * MAP_TILES_WIDTH + x;
            if (map->tiles[index].state & states)
            {
                return true;
            }
        }
    }

    return false;
}

bool checkUnitTiles(WarMap* map, WarEntity* entity, s32 states)
{
    assert(isUnit(entity));

    WarUnitComponent* unit = &entity->unit;

    vec2 position = getUnitPosition(entity, true);
    return checkMapTiles(map, position.x, position.y, unit->sizex, unit->sizey, states);
}

u8Color getMapTileAverage(WarResource* levelVisual, WarResource* tileset, s32 x, s32 y)
{
    s32 index = y * MAP_TILES_WIDTH + x;
    u16 tileIndex = levelVisual->levelVisual.data[index];

    s32 tilePixelX = (tileIndex % TILESET_TILES_PER_ROW) * MEGA_TILE_WIDTH;
    s32 tilePixelY = ((tileIndex / TILESET_TILES_PER_ROW) * MEGA_TILE_HEIGHT);

    s32 r = 0, g = 0, b = 0;

    for(s32 ty = 0; ty < MEGA_TILE_HEIGHT; ty++)
    {
        for(s32 tx = 0; tx < MEGA_TILE_WIDTH; tx++)
        {
            s32 pixel = (tilePixelY + ty) * TILESET_WIDTH + (tilePixelX + tx);
            r += tileset->tilesetData.data[pixel * 4 + 0];
            g += tileset->tilesetData.data[pixel * 4 + 1];
            b += tileset->tilesetData.data[pixel * 4 + 2];
        }
    }

    r /= 256;
    g /= 256;
    b /= 256;

    u8Color color = {0};
    color.r = (u8)r;
    color.g = (u8)g;
    color.b = (u8)b;
    color.a = 255;
    return color;
}

void updateMinimapTile(WarContext* context, WarResource* levelVisual, WarResource* tileset, s32 x, s32 y)
{
    WarMap* map = context->map;
    WarSpriteFrame* minimapFrame = &map->minimapSprite.frames[1];
    
    s32 index = y * MAP_TILES_WIDTH + x;
    u8Color color = U8COLOR_BLACK;

    WarMapTile* tile = &map->tiles[index];
    if (tile->state == MAP_TILE_STATE_VISIBLE ||
        tile->state == MAP_TILE_STATE_FOG)
    {
        color = getMapTileAverage(levelVisual, tileset, x, y);
    }

    minimapFrame->data[index * 4 + 0] = color.r;
    minimapFrame->data[index * 4 + 1] = color.g;
    minimapFrame->data[index * 4 + 2] = color.b;
    minimapFrame->data[index * 4 + 3] = color.a;
}

s32 getMapTileIndex(WarContext* context, s32 x, s32 y)
{
    WarMap* map = context->map;

    WarResource* levelInfo = getOrCreateResource(context, map->levelInfoIndex);
    assert(levelInfo && levelInfo->type == WAR_RESOURCE_TYPE_LEVEL_INFO);

    WarResource* levelVisual = getOrCreateResource(context, levelInfo->levelInfo.visualIndex);
    assert(levelVisual && levelVisual->type == WAR_RESOURCE_TYPE_LEVEL_VISUAL);

    WarMapTilesetType tilesetType = map->tilesetType;
    assert(tilesetType == MAP_TILESET_FOREST || tilesetType == MAP_TILESET_SWAMP);

    return levelVisual->levelVisual.data[y * MAP_TILES_WIDTH + x];
}

void setMapTileIndex(WarContext* context, s32 x, s32 y, s32 tile)
{
    WarMap* map = context->map;

    WarResource* levelInfo = getOrCreateResource(context, map->levelInfoIndex);
    assert(levelInfo && levelInfo->type == WAR_RESOURCE_TYPE_LEVEL_INFO);

    WarResource* levelVisual = getOrCreateResource(context, levelInfo->levelInfo.visualIndex);
    assert(levelVisual && levelVisual->type == WAR_RESOURCE_TYPE_LEVEL_VISUAL);

    WarResource* tileset = getOrCreateResource(context, levelInfo->levelInfo.tilesetIndex);
    assert(tileset && tileset->type == WAR_RESOURCE_TYPE_TILESET);

    levelVisual->levelVisual.data[y * MAP_TILES_WIDTH + x] = tile;

    updateMinimapTile(context, levelVisual, tileset, x, y);
}

void changeCursorType(WarContext* context, WarEntity* entity, WarCursorType type)
{
    assert(entity->type == WAR_ENTITY_TYPE_CURSOR);

    if (entity->cursor.type == type)
    {
        return;
    }

    WarResource* resource = getOrCreateResource(context, type);
    assert(resource->type == WAR_RESOURCE_TYPE_CURSOR);

    removeCursorComponent(context, entity);
    addCursorComponent(context, entity, type, vec2i(resource->cursor.hotx, resource->cursor.hoty));

    removeSpriteComponent(context, entity);
    addSpriteComponentFromResource(context, entity, imageResourceRef(type));
}

void createMap(WarContext *context, s32 levelInfoIndex)
{
    WarResource* levelInfo = getOrCreateResource(context, levelInfoIndex);
    assert(levelInfo && levelInfo->type == WAR_RESOURCE_TYPE_LEVEL_INFO);

    WarResource* levelPassable = getOrCreateResource(context, levelInfo->levelInfo.passableIndex);
    assert(levelPassable && levelPassable->type == WAR_RESOURCE_TYPE_LEVEL_PASSABLE);

    WarMap *map = (WarMap*)xcalloc(1, sizeof(WarMap));
    map->status = MAP_PLAYING;
    map->levelInfoIndex = levelInfoIndex;
    map->objectivesTime = 1;
    map->tilesetType = levelInfoIndex & 1 ? MAP_TILESET_FOREST : MAP_TILESET_SWAMP;
    map->scrollSpeed = 200;
    map->audioEnabled = true;

    map->leftTopPanel = recti(0, 0, 72, 72);
    map->leftBottomPanel = recti(0, 72, 72, 128);
    map->rightPanel = recti(312, 0, 8, 200);
    map->topPanel = recti(72, 0, 240, 12);
    map->bottomPanel = recti(72, 188, 240, 12);
    map->mapPanel = recti(72, 12, MAP_VIEWPORT_WIDTH, MAP_VIEWPORT_HEIGHT);
    map->minimapPanel = recti(3, 6, MINIMAP_WIDTH, MINIMAP_HEIGHT);
    map->menuPanel = recti(84, 32, 152, 136);
    map->messagePanel = recti(17, 76, 286, 48);
    map->saveLoadPanel = recti(48, 27, 223, 146);

    s32 startX = levelInfo->levelInfo.startX * MEGA_TILE_WIDTH;
    s32 startY = levelInfo->levelInfo.startY * MEGA_TILE_HEIGHT;
    map->viewport = recti(startX, startY, MAP_VIEWPORT_WIDTH, MAP_VIEWPORT_HEIGHT);

    // initialize entities list
    WarEntityListInit(&map->entities, WarEntityListDefaultOptions);

    // initialize entity by type map
    WarEntityMapOptions entitiesByTypeOptions = (WarEntityMapOptions){0};
    entitiesByTypeOptions.defaultValue = NULL;
    entitiesByTypeOptions.hashFn = hashEntityType;
    entitiesByTypeOptions.equalsFn = equalsEntityType;
    entitiesByTypeOptions.freeFn = freeEntityList;
    WarEntityMapInit(&map->entitiesByType, entitiesByTypeOptions);
    for (WarEntityType type = WAR_ENTITY_TYPE_IMAGE; type < WAR_ENTITY_TYPE_COUNT; type++)
    {
        WarEntityList* list = (WarEntityList*)xmalloc(sizeof(WarEntityList));
        WarEntityListInit(list, WarEntityListNonFreeOptions);
        WarEntityMapSet(&map->entitiesByType, type, list);
    }

    // initialize unit by type map
    WarUnitMapOptions unitsByTypeOptions = (WarUnitMapOptions){0};
    unitsByTypeOptions.defaultValue = NULL;
    unitsByTypeOptions.hashFn = hashUnitType;
    unitsByTypeOptions.equalsFn = equalsUnitType;
    unitsByTypeOptions.freeFn = freeEntityList;
    WarUnitMapInit(&map->unitsByType, unitsByTypeOptions);
    for (WarUnitType type = WAR_UNIT_FOOTMAN; type < WAR_UNIT_COUNT; type++)
    {
        WarEntityList* list = (WarEntityList*)xmalloc(sizeof(WarEntityList));
        WarEntityListInit(list, WarEntityListNonFreeOptions);
        WarUnitMapSet(&map->unitsByType, type, list);
    }

    // initialize the entities by id map
    WarEntityIdMapOptions entitiesByIdOptions = (WarEntityIdMapOptions){0};
    entitiesByIdOptions.defaultValue = NULL;
    entitiesByIdOptions.hashFn = hashEntityId;
    entitiesByIdOptions.equalsFn = equalsEntityId;
    WarEntityIdMapInit(&map->entitiesById, entitiesByIdOptions);

    // initialize ui entities list
    WarEntityListInit(&map->uiEntities, WarEntityListNonFreeOptions);

    // initialize selected entities list
    WarEntityIdListInit(&map->selectedEntities, WarEntityIdListDefaultOptions);

    // initialize map animations lists
    WarSpriteAnimationListInit(&map->animations, WarSpriteAnimationListDefaultOptions);

    map->finder = initPathFinder(PATH_FINDING_ASTAR, MAP_TILES_WIDTH, MAP_TILES_HEIGHT, levelPassable->levelPassable.data);

    context->map = map;

    // create the black sprite
    {
        u8 data[MEGA_TILE_WIDTH * MEGA_TILE_HEIGHT * 4];
        memset(data, 0, MEGA_TILE_WIDTH * MEGA_TILE_HEIGHT * 4);
        for (s32 i = 0; i < MEGA_TILE_WIDTH * MEGA_TILE_HEIGHT; i++)
            data[4 * i + 3] = 255;

        map->blackSprite = createSprite(context, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT, data);
    }

    // set the initial state for the tiles
    {
        for (s32 i = 0; i < MAP_TILES_WIDTH * MAP_TILES_HEIGHT; i++)
        {
            map->tiles[i].state = MAP_TILE_STATE_UNKOWN;
            map->tiles[i].type = WAR_FOG_PIECE_NONE;
            map->tiles[i].boundary = WAR_FOG_BOUNDARY_NONE;
        }
    }

    // create the map sprite
    {
        WarResource* levelVisual = getOrCreateResource(context, levelInfo->levelInfo.visualIndex);
        assert(levelVisual && levelVisual->type == WAR_RESOURCE_TYPE_LEVEL_VISUAL);

        WarResource* tileset = getOrCreateResource(context, levelInfo->levelInfo.tilesetIndex);
        assert(tileset && tileset->type == WAR_RESOURCE_TYPE_TILESET);

        // DEBUG: 
        // print level visual data to console to see the sprites of the map
        //
        // for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
        // {
        //     for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
        //     {
        //         // index of the tile in the tilesheet
        //         u16 tileIndex = levelVisual->levelVisual.data[y * MAP_TILES_WIDTH + x];
        //         printf("%d ", tileIndex);
        //     }

        //     printf("\n");
        // }

        map->sprite = createSprite(context, TILESET_WIDTH, TILESET_HEIGHT, tileset->tilesetData.data);

        // the minimap sprite will be a 2 frames sprite
        // the first one will be the frame that actually render
        // the second one will be the minimap for the terrain, created at startup time, 
        // that way I only have to memcpy to the first frame and do the work only for the units
        // that way I also don't have to allocate memory for the minimap each frame
        WarSpriteFrame minimapFrames[2];
        
        for(s32 i = 0; i < 2; i++)
        {
            minimapFrames[i].dx = 0;
            minimapFrames[i].dy = 0;
            minimapFrames[i].w = MINIMAP_WIDTH;
            minimapFrames[i].h = MINIMAP_HEIGHT;
            minimapFrames[i].off = 0;
            minimapFrames[i].data = (u8*)xcalloc(MINIMAP_WIDTH * MINIMAP_HEIGHT * 4, sizeof(u8));

            // make the frame black
            for (s32 k = 0; k < MINIMAP_WIDTH * MINIMAP_HEIGHT; k++)
                minimapFrames[i].data[k * 4 + 3] = 255;
        }
        
        for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
        {
            for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
            {
                u8Color color = getMapTileAverage(levelVisual, tileset, x, y);
                s32 index = y * MAP_TILES_WIDTH + x;
                minimapFrames[1].data[index * 4 + 0] = color.r;
                minimapFrames[1].data[index * 4 + 1] = color.g;
                minimapFrames[1].data[index * 4 + 2] = color.b;
                minimapFrames[1].data[index * 4 + 3] = color.a;
            }
        }

        map->minimapSprite = createSpriteFromFrames(context, MINIMAP_WIDTH, MINIMAP_HEIGHT, arrayLength(minimapFrames), minimapFrames);
    }

    // create the forest entities
    {
        bool processed[MAP_TILES_WIDTH * MAP_TILES_HEIGHT];
        for(s32 i = 0; i < MAP_TILES_WIDTH * MAP_TILES_HEIGHT; i++)
            processed[i] = false;

        u16* passableData = levelPassable->levelPassable.data;
        for(s32 i = 0; i < MAP_TILES_WIDTH * MAP_TILES_HEIGHT; i++)
        {
            if (!processed[i] && passableData[i] == 128)
            {
                s32 x = i % MAP_TILES_WIDTH;
                s32 y = i / MAP_TILES_WIDTH;

                WarTreeList trees;
                WarTreeListInit(&trees, WarTreeListDefaultOptions);
                WarTreeListAdd(&trees, createTree(x, y, TREE_MAX_WOOD));
                processed[i] = true;

                for(s32 j = 0; j < trees.count; j++)
                {
                    WarTree tree = trees.items[j];
                    for(s32 d = 0; d < dirC; d++)
                    {
                        s32 xx = tree.tilex + dirX[d];
                        s32 yy = tree.tiley + dirY[d];
                        if (isInside(map->finder, xx, yy))
                        {
                            s32 k = yy * MAP_TILES_WIDTH + xx;
                            if (!processed[k] && passableData[k] == 128)
                            {
                                // mark it processed right away, to not process it later
                                processed[k] = true;

                                WarTree newTree = createTree(xx, yy, TREE_MAX_WOOD);
                                WarTreeListAdd(&trees, newTree);
                            }
                        }
                    }
                }
                
                WarEntity* forest = createEntity(context, WAR_ENTITY_TYPE_FOREST, true);
                addSpriteComponent(context, forest, map->sprite);
                addForestComponent(context, forest, trees);

                for (s32 i = 0; i < trees.count; i++)
                {
                    WarTree* tree = &trees.items[i];
                    setStaticEntity(map->finder, tree->tilex, tree->tiley, 1, 1, forest->id);
                }

                determineTreeTiles(context, forest);
            }
        }

        map->forest = createForest(context);;
    }

    // create the starting roads
    {
        WarEntity* road = createRoad(context);

        for(s32 i = 0; i < levelInfo->levelInfo.startRoadsCount; i++)
        {
            WarLevelConstruct *construct = &levelInfo->levelInfo.startRoads[i];
            if (construct->type == WAR_CONSTRUCT_ROAD)
            {
                addRoadPiecesFromConstruct(road, construct);
            }
        }

        determineRoadTypes(context, road);

        map->road = road;
    }

    // create the starting walls
    {
        WarEntity* wall = createWall(context);

        for(s32 i = 0; i < levelInfo->levelInfo.startWallsCount; i++)
        {
            WarLevelConstruct *construct = &levelInfo->levelInfo.startWalls[i];
            if (construct->type == WAR_CONSTRUCT_WALL)
            {
                addWallPiecesFromConstruct(wall, construct);
            }
        }

        determineWallTypes(context, wall);

        for(s32 i = 0; i < wall->wall.pieces.count; i++)
        {
            WarWallPiece* piece = &wall->wall.pieces.items[i];
            piece->hp = WAR_WALL_MAX_HP;
            piece->maxhp = WAR_WALL_MAX_HP;
        }

        addStateMachineComponent(context, wall);

        WarState* idleState = createIdleState(context, wall, false);
        changeNextState(context, wall, idleState, true, true);

        map->wall = wall;
    }

    // create ruins
    {
        map->ruin = createRuins(context);
    }

    // create players info
    {
        for (s32 i = 0; i < MAX_PLAYERS_COUNT; i++)
        {
            map->players[i].index = 0;
            map->players[i].race = levelInfo->levelInfo.races[i];
            map->players[i].gold = levelInfo->levelInfo.gold[i];
            map->players[i].wood = levelInfo->levelInfo.lumber[i];

            for (s32 j = 0; j < MAX_FEATURES_COUNT; j++)
            {
                map->players[i].features[j] = levelInfo->levelInfo.allowedFeatures[j];

                // REMOVE THIS: This is only for testing
                // if (levelInfoIndex == 117)
                //     map->players[i].features[j] = true;
                // else if (levelInfoIndex == 118)
                //     map->players[i].features[j] = true;
            }

            for (s32 j = 0; j < MAX_UPGRADES_COUNT; j++)
            {
                map->players[i].upgrades[j].allowed = levelInfo->levelInfo.allowedUpgrades[j][i];
                map->players[i].upgrades[j].level = 0;

                // REMOVE THIS: This is only for testing
                // if (levelInfoIndex == 117)
                //     map->players[i].upgrades[j].allowed = 2;
                // else if (levelInfoIndex == 118)
                //     map->players[i].upgrades[j].allowed = 2;
            }
        }
    }

    // create the starting entities
    {
        for(s32 i = 0; i < levelInfo->levelInfo.startEntitiesCount; i++)
        {
            WarLevelUnit startUnit = levelInfo->levelInfo.startEntities[i];

            // REMOVE THIS: This is only for testing
            // if (levelInfoIndex == 117)
            // {
            //     if (startUnit.type == WAR_UNIT_FOOTMAN)
            //         startUnit.type = WAR_UNIT_CLERIC;
            // }
            // else if (levelInfoIndex == 118)
            // {
            //     if (startUnit.type == WAR_UNIT_GRUNT)
            //         startUnit.type = WAR_UNIT_NECROLYTE;
            // }
            
            createUnit(context, startUnit.type, startUnit.x, startUnit.y, startUnit.player, 
                       startUnit.resourceKind, startUnit.amount, true);
        }

        // REMOVE THIS: This is only for testing
        // if (levelInfoIndex == 117)
        // {
        //     createBuilding(context, WAR_UNIT_BARRACKS_HUMANS, 37, 18, 0, false);
        //     createBuilding(context, WAR_UNIT_LUMBERMILL_HUMANS, 36, 22, 0, false);
        //     createBuilding(context, WAR_UNIT_BLACKSMITH_HUMANS, 40, 16, 0, false);
        //     createBuilding(context, WAR_UNIT_CHURCH, 45, 22, 0, false);
        //     createBuilding(context, WAR_UNIT_STABLE, 45, 18, 0, false);
        //     createBuilding(context, WAR_UNIT_TOWER_HUMANS, 34, 16, 0, false);

        //     createBuilding(context, WAR_UNIT_LUMBERMILL_ORCS, 24, 16, 1, false);
        // }
        // else if (levelInfoIndex == 118)
        // {
        //     createBuilding(context, WAR_UNIT_BARRACKS_ORCS, 48, 30, 0, false);
        //     createBuilding(context, WAR_UNIT_LUMBERMILL_ORCS, 47, 27, 0, false);
        //     createBuilding(context, WAR_UNIT_BLACKSMITH_ORCS, 58, 26, 0, false);
        //     createBuilding(context, WAR_UNIT_TEMPLE, 48, 23, 0, false);
        //     createBuilding(context, WAR_UNIT_KENNEL, 57, 22, 0, false);
        //     createBuilding(context, WAR_UNIT_TOWER_ORCS, 51, 21, 0, false);

        //     createBuilding(context, WAR_UNIT_LUMBERMILL_HUMANS, 24, 16, 1, false);
        // }
    }

    // add ui entities
    createMapUI(context);
    createMenu(context);
    createOptionsMenu(context);
    createGameOverMenu(context);
    createQuitMenu(context);
    createUICursor(context, "cursor", WAR_CURSOR_ARROW, VEC2_ZERO);

    createAudio(context, WAR_MUSIC_00, true);
}

void freeMap(WarContext* context)
{
    WarMap* map = context->map;

    map->audioEnabled = false;

    freeSprite(context, map->sprite);
    freeSprite(context, map->minimapSprite);
    freeSprite(context, map->blackSprite);

    WarEntityListFree(&map->entities);
    WarEntityMapFree(&map->entitiesByType);
    WarUnitMapFree(&map->unitsByType);
    WarEntityIdMapFree(&map->entitiesById);
    WarEntityListFree(&map->uiEntities);

    WarEntityIdListFree(&map->selectedEntities);

    // these are already free when the lists and maps are
    // freeEntity(map->forest);
    // freeEntity(map->wall);
    // freeEntity(map->road);
    // freeEntity(map->ruin);

    free(map->finder.data);

    WarSpriteAnimationListFree(&map->animations);

    context->map = NULL;
}

void updateGlobalSpeed(WarContext* context)
{
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && !isKeyPressed(input, WAR_KEY_SHIFT))
    {
        if (wasKeyPressed(input, WAR_KEY_1))
            setGlobalSpeed(context, 1.0f);
        else if (wasKeyPressed(input, WAR_KEY_2))
            setGlobalSpeed(context, 2.0f);
        else if (wasKeyPressed(input, WAR_KEY_3))
            setGlobalSpeed(context, 3.0f);
        else if (wasKeyPressed(input, WAR_KEY_4))
            setGlobalSpeed(context, 4.0f);
    }
}

void updateGlobalScale(WarContext* context)
{
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && isKeyPressed(input, WAR_KEY_SHIFT))
    {
        if (wasKeyPressed(input, WAR_KEY_1))
            setGlobalScale(context, 1.0f);
        else if (wasKeyPressed(input, WAR_KEY_2))
            setGlobalScale(context, 2.0f);
        else if (wasKeyPressed(input, WAR_KEY_3))
            setGlobalScale(context, 3.0f);
        else if (wasKeyPressed(input, WAR_KEY_4))
            setGlobalScale(context, 4.0f);
    }
}

void updateVolume(WarContext* context)
{
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL))
    {
        if (isKeyPressed(input, WAR_KEY_SHIFT))
        {
            if (wasKeyPressed(input, WAR_KEY_UP))
                changeMusicVolume(context, 0.1f);
            else if (wasKeyPressed(input, WAR_KEY_DOWN))
                changeMusicVolume(context, -0.1f);
        }
        else
        {
            if (wasKeyPressed(input, WAR_KEY_UP))
                changeSoundVolume(context, 0.1f);
            else if (wasKeyPressed(input, WAR_KEY_DOWN))
                changeSoundVolume(context, -0.1f);
        }
    }
}

void updateViewport(WarContext *context)
{
    WarMap *map = context->map;
    WarInput *input = &context->input;

    map->wasScrolling = false;

    vec2 dir = VEC2_ZERO;
    bool wasScrolling = map->isScrolling;

    // if there was a click in the minimap, then update the position of the viewport
    if (isButtonPressed(input, WAR_MOUSE_LEFT))
    {
        // check if the click is inside the minimap panel        
        if (rectContainsf(map->minimapPanel, input->pos.x, input->pos.y))
        {
            vec2 minimapSize = vec2i(MINIMAP_WIDTH, MINIMAP_HEIGHT);
            vec2 offset = vec2ScreenToMinimapCoordinates(context, input->pos);
            offset = vec2MinimapToViewportCoordinates(context, offset);

            map->viewport.x = offset.x * MAP_WIDTH / minimapSize.x;
            map->viewport.y = offset.y * MAP_HEIGHT / minimapSize.y;
        }
        // check if it was at the edge of the map to scroll also and update the position of the viewport
        else if(!input->isDragging)
        {
            dir = getDirFromMousePos(context, input);
        }
    }
    // check for the arrows keys and update the position of the viewport
    else
    {
        if (!isKeyPressed(input, WAR_KEY_CTRL) && 
            !isKeyPressed(input, WAR_KEY_SHIFT))
        {
            dir = getDirFromArrowKeys(context, input);
        }
    }

    map->viewport.x += map->scrollSpeed * dir.x * context->deltaTime;
    map->viewport.x = clamp(map->viewport.x, 0.0f, MAP_WIDTH - map->viewport.width);

    map->viewport.y += map->scrollSpeed * dir.y * context->deltaTime;
    map->viewport.y = clamp(map->viewport.y, 0.0f, MAP_HEIGHT - map->viewport.height);

    map->isScrolling = !vec2IsZero(dir);

    if (!map->isScrolling)
    {
        map->wasScrolling = wasScrolling;
    }
}

void updateDragRect(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    input->wasDragging = false;
    input->dragRect = RECT_EMPTY;

    if (map->isScrolling)
    {
        input->isDragging = false;
        input->dragPos = VEC2_ZERO;
        return;    
    }

    if (isButtonPressed(input, WAR_MOUSE_LEFT))
    {
        if(rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
        {
            if (!input->isDragging)
            {
                input->dragPos = input->pos;
                input->isDragging = true;
            }
        }
    }
    else if(wasButtonPressed(input, WAR_MOUSE_LEFT))
    {
        if (input->isDragging)
        {
            input->isDragging = false;
            input->wasDragging = true;
            input->dragRect = rectpf(input->dragPos.x, input->dragPos.y, input->pos.x, input->pos.y);
        }
    }
}

void updateSelection(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if(wasButtonPressed(input, WAR_MOUSE_LEFT))
    {
        // if it was scrolling last frame, don't perform any selection this frame
        if (!map->wasScrolling)
        {
            // check if the click is inside the map panel
            if(input->wasDragging || rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
            {
                WarEntityList newSelectedEntities;
                WarEntityListInit(&newSelectedEntities, WarEntityListNonFreeOptions);
                
                rect pointerRect = rectScreenToMapCoordinates(context, input->dragRect);

                // select the entities inside the dragging rect
                WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
                for(s32 i = 0; i < units->count; i++)
                {
                    WarEntity* entity = units->items[i];
                    if (entity)
                    {
                        WarTransformComponent* transform = &entity->transform;
                        WarUnitComponent* unit = &entity->unit;
                        if (unit->enabled)
                        {
                            // don't select dead units or corpses
                            if (isDead(entity) || isGoingToDie(entity) || isCorpseUnit(entity))
                            {
                                continue;
                            }

                            // don't select collased buildings
                            if (isCollapsing(entity) || isGoingToCollapse(entity))
                            {
                                continue;
                            }

                            s32 tileX = (s32)(transform->position.x / MEGA_TILE_WIDTH);
                            s32 tileY = (s32)(transform->position.y / MEGA_TILE_HEIGHT);

                            // don't select non-visible units
                            if (!checkMapTiles(map, tileX, tileY, unit->sizex, unit->sizey, MAP_TILE_STATE_VISIBLE))
                            {
                                continue;
                            }

                            rect unitRect = rectv(entity->transform.position, getUnitSpriteSize(entity));
                            if (rectIntersects(pointerRect, unitRect))
                            {
                                WarEntityListAdd(&newSelectedEntities, entity);
                            }
                        }
                    }
                }

                // include the already selected entities if the Ctrl key is pressed
                if (isKeyPressed(input, WAR_KEY_CTRL))
                {
                    // the max number of selected entities is 4, so there is no much
                    // throuble looking for the actual entities here, it will also
                    // improve when a hash is make for looking up the entities
                    for (s32 i = 0; i < map->selectedEntities.count; i++)
                    {
                        WarEntity* entity = findEntity(context, map->selectedEntities.items[i]);
                        if (entity)
                            WarEntityListAdd(&newSelectedEntities, entity);
                    }
                }

                bool areDudesSelected = false;
                bool areBuildingSelected = false;

                // calculate the number of dudes and building entities in the selection
                for (s32 i = 0; i < newSelectedEntities.count; i++)
                {
                    WarEntity* entity = newSelectedEntities.items[i];
                    if (isDudeUnit(entity))
                        areDudesSelected = true;
                    else if (isBuildingUnit(entity))
                        areBuildingSelected = true;
                }

                if (areDudesSelected)
                {
                    // remove all new selected buildings
                    for (s32 i = newSelectedEntities.count - 1; i >= 0; i--)
                    {
                        WarEntity* entity = newSelectedEntities.items[i];
                        if (isBuildingUnit(entity))
                            WarEntityListRemoveAt(&newSelectedEntities, i);
                    }
                }
                else if (areBuildingSelected)
                {
                    // remove all other new selected buildings
                    WarEntityListRemoveAtRange(&newSelectedEntities, 1, newSelectedEntities.count - 1);
                }

                if (areDudesSelected)
                {
                    if (newSelectedEntities.count == 1)
                    {
                        WarEntity* newSelectedEntity = newSelectedEntities.items[0];
                        if (isFriendlyUnit(context, newSelectedEntity))
                        {
                            playDudeSelectionSound(context, newSelectedEntity);
                        }
                        else
                        {
                            createAudio(context, WAR_UI_CLICK, false);
                        }
                    }
                }
                else if (areBuildingSelected)
                {
                    WarEntity* newSelectedEntity = newSelectedEntities.items[0];
                    if (isFriendlyUnit(context, newSelectedEntity))
                    {
                        playBuildingSelectionSound(context, newSelectedEntity);
                    }
                }

                // clear the current selection
                clearSelection(context);

                // and add the new selection
                s32 selectedEntitiesCount = min(newSelectedEntities.count, 4);
                for (s32 i = 0; i < selectedEntitiesCount; i++)
                {
                    WarEntity* entity = newSelectedEntities.items[i];
                    addEntityToSelection(context, entity->id);
                }

                WarEntityListFree(&newSelectedEntities);
            }
        }
    }
}

void updateTreesEdit(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && 
        wasKeyPressed(input, WAR_KEY_T))
    {
        map->editingTrees = !map->editingTrees;
    }

    if (map->editingTrees)
    {
        if (wasButtonPressed(input, WAR_MOUSE_LEFT))
        {
            if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
            {
                vec2 pointerPos = vec2ScreenToMapCoordinates(context, input->pos);
                pointerPos =  vec2MapToTileCoordinates(pointerPos);

                s32 x = (s32)pointerPos.x;
                s32 y = (s32)pointerPos.y;

                WarEntityId entityId = getTileEntityId(map->finder, x, y);
                WarEntity* entity = findEntity(context, entityId);
                if (!entity)
                {
                    entity = map->forest;

                    plantTree(context, entity, x, y);
                    determineAllTreeTiles(context);
                }
                else if (entity->type == WAR_ENTITY_TYPE_FOREST)
                {
                    WarTree* tree = getTreeAtPosition(entity, x, y);
                    if (tree)
                    {
                        chopTree(context, entity, tree, TREE_MAX_WOOD);
                        determineAllTreeTiles(context);
                    }
                }
            }
        }
    }
}

void updateRoadsEdit(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && 
        wasKeyPressed(input, WAR_KEY_R))
    {
        map->editingRoads = !map->editingRoads;
    }

    if (map->editingRoads)
    {
        if (wasButtonPressed(input, WAR_MOUSE_LEFT))
        {
            if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
            {
                vec2 pointerPos = vec2ScreenToMapCoordinates(context, input->pos);
                pointerPos =  vec2MapToTileCoordinates(pointerPos);

                s32 x = (s32)pointerPos.x;
                s32 y = (s32)pointerPos.y;

                WarEntity* road = map->road;

                WarRoadPiece* piece = getRoadPieceAtPosition(road, x, y);
                if (!piece)
                {
                    addRoadPiece(road, x, y, 0);
                    determineRoadTypes(context, road);
                }
                else
                {
                    removeRoadPiece(road, piece);
                    determineRoadTypes(context, road);
                }
            }
        }
    }
}

void updateWallsEdit(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && 
        wasKeyPressed(input, WAR_KEY_W))
    {
        map->editingWalls = !map->editingWalls;
    }

    if (map->editingWalls)
    {
        if (wasButtonPressed(input, WAR_MOUSE_LEFT))
        {
            if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
            {
                vec2 pointerPos = vec2ScreenToMapCoordinates(context, input->pos);
                pointerPos =  vec2MapToTileCoordinates(pointerPos);

                s32 x = (s32)pointerPos.x;
                s32 y = (s32)pointerPos.y;

                WarEntity* wall = map->wall;

                WarWallPiece* piece = getWallPieceAtPosition(wall, x, y);
                if (!piece)
                {
                    WarWallPiece* piece = addWallPiece(wall, x, y, 0);
                    piece->hp = WAR_WALL_MAX_HP;
                    piece->maxhp = WAR_WALL_MAX_HP;
                    
                    determineWallTypes(context, wall);
                }
                else
                {
                    removeWallPiece(wall, piece);
                    determineWallTypes(context, wall);
                }
            }
        }
    }
}

void updateRuinsEdit(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && 
        wasKeyPressed(input, WAR_KEY_U))
    {
        map->editingRuins = !map->editingRuins;
    }

    if (map->editingRuins)
    {
        if (wasButtonPressed(input, WAR_MOUSE_LEFT))
        {
            if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
            {
                vec2 pointerPos = vec2ScreenToMapCoordinates(context, input->pos);
                pointerPos =  vec2MapToTileCoordinates(pointerPos);

                s32 x = (s32)pointerPos.x;
                s32 y = (s32)pointerPos.y;

                WarEntity* ruin = map->ruin;

                WarRuinPiece* piece = getRuinPieceAtPosition(ruin, x, y);
                if (!piece)
                {
                    addRuinsPieces(context, ruin, x, y, 2);
                    determineRuinTypes(context, ruin);
                }
                else
                {
                    removeRuinPiece(ruin, piece);
                    determineRuinTypes(context, ruin);
                }
            }
        }
    }
}

void updateRainOfFireEdit(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    if (isKeyPressed(input, WAR_KEY_CTRL) && 
        wasKeyPressed(input, WAR_KEY_K))
    {
        map->editingRainOfFire = !map->editingRainOfFire;
    }

    if (map->editingRainOfFire)
    {
        if (wasKeyPressed(input, WAR_KEY_L) && map->selectedEntities.count > 0)
        {
            WarEntityId selectedEntityId = map->selectedEntities.items[0];

            rect viewport = map->viewport;
            f32 padding = MEGA_TILE_WIDTH * 3;

            f32 offsetx = randomf(viewport.x + padding, viewport.x + viewport.width - padding);
            f32 offsety = randomf(viewport.y + padding, viewport.y + viewport.height - padding);
            vec2 target = vec2f(offsetx, offsety);
            vec2 origin = vec2f(target.x, viewport.y);

            createProjectile(context, WAR_PROJECTILE_RAIN_OF_FIRE, selectedEntityId, 0, origin, target);
        }
    }
}

void updateCommands(WarContext* context)
{
    WarMap* map = context->map;

    WarEntity* commandButtons[6] = 
    {
        findUIEntity(context, "btnCommand0"),
        findUIEntity(context, "btnCommand1"),
        findUIEntity(context, "btnCommand2"),
        findUIEntity(context, "btnCommand3"),
        findUIEntity(context, "btnCommand4"),
        findUIEntity(context, "btnCommand5")
    };

    WarEntity* commandTexts[4] =
    {
        findUIEntity(context, "txtCommand0"),
        findUIEntity(context, "txtCommand1"),
        findUIEntity(context, "txtCommand2"),
        findUIEntity(context, "txtCommand3")
    };

    for (s32 i = 0; i < arrayLength(commandButtons); i++)
        commandButtons[i]->button.enabled = false;

    for (s32 i = 0; i < arrayLength(commandTexts); i++)
        clearUIText(commandTexts[i]);

    s32 selectedEntitiesCount = map->selectedEntities.count;
    if (selectedEntitiesCount == 0)
        return;

    WarEntity* entity = findEntity(context, map->selectedEntities.items[0]);
    assert(entity && isUnit(entity));
    
    // if the selected unit is a farm, 
    // just show the text about the food consumtion
    if (entity->unit.type == WAR_UNIT_FARM_HUMANS ||
        entity->unit.type == WAR_UNIT_FARM_ORCS)
    {
        if (!entity->unit.building)
        {
            s32 farmsCount = getNumberOfBuildingsOfType(context, 0, entity->unit.type, true);
            s32 dudesCount = getTotalNumberOfDudes(context, 0);

            setUIText(commandTexts[0], NO_HIGHLIGHT, "FOOD USAGE:");
            setUITextFormat(commandTexts[1], NO_HIGHLIGHT, "GROWN %d", farmsCount * 4 + 1);
            setUITextFormat(commandTexts[2], NO_HIGHLIGHT, " USED %d", dudesCount);

            return;
        }
    }
    
    // if the selected unit is a goldmine,
    // just add the text with the remaining gold
    if (entity->unit.type == WAR_UNIT_GOLDMINE)
    {
        s32 gold = entity->unit.amount;

        setUIText(commandTexts[0], NO_HIGHLIGHT, "GOLD LEFT");
        setUITextFormat(commandTexts[3], NO_HIGHLIGHT, "%d", gold);
        return;
    }

    // determine the commands for the selected unit(s)
    WarUnitCommandType commands[6] = {0};
    getUnitCommands(context, entity, commands);

    if (selectedEntitiesCount > 1)
    {
        WarUnitCommandType selectedCommands[6] = {0};
        for (s32 i = 1; i < selectedEntitiesCount; i++)
        {
            WarEntity* selectedEntity = findEntity(context, map->selectedEntities.items[i]);
            assert(selectedEntity && isUnit(selectedEntity));

            memset(selectedCommands, 0, sizeof(selectedCommands));
            getUnitCommands(context, selectedEntity, selectedCommands);

            for (s32 j = 0; j < arrayLength(commands); j++)
            {
                if (commands[j] != selectedCommands[j])
                {
                    commands[j] = WAR_COMMAND_NONE;
                }
            }
        }
    }

    for (s32 i = 0; i < arrayLength(commands); i++)
    {
        if (commands[i] != WAR_COMMAND_NONE)
        {
            WarUnitCommandData commandData = getUnitCommandData(context, entity, commands[i]);
            setUIImage(commandButtons[i], commandData.frameIndex);
            setUITooltip(commandButtons[i], commandData.highlightIndex, commandData.tooltip);
            commandButtons[i]->button.enabled = true;
            commandButtons[i]->button.gold = commandData.gold;
            commandButtons[i]->button.wood = commandData.wood;
            commandButtons[i]->button.hotKey = commandData.hotKey;
            commandButtons[i]->button.clickHandler = commandData.clickHandler;
        }
    }
}

void updateButtons(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;

    WarEntityList* buttons = getEntitiesOfType(map, WAR_ENTITY_TYPE_BUTTON);
    for(s32 i = 0; i < buttons->count; i++)
    {
        WarEntity* entity = buttons->items[i];
        if (entity)
        {
            WarTransformComponent* transform = &entity->transform;
            WarUIComponent* ui = &entity->ui;
            WarButtonComponent* button = &entity->button;
            
            if (!ui->enabled || !button->enabled || !button->interactive)
            {
                button->hot = false;
                button->active = false;
                continue;
            }

            if (wasKeyPressed(input, button->hotKey))
            {
                if (button->clickHandler)
                {
                    button->hot = false;
                    button->active = false;

                    button->clickHandler(context, entity);

                    // in this case break to not allow pressing multiple keys
                    // and executing all of the command for those keys
                    break;
                }
            }

            vec2 backgroundSize = vec2i(button->normalSprite.frameWidth, button->normalSprite.frameHeight);
            rect buttonRect = rectv(transform->position, backgroundSize);
            bool pointerInside = rectContainsf(buttonRect, input->pos.x, input->pos.y);

            if (wasButtonPressed(input, WAR_MOUSE_LEFT))
            {
                button->hot = pointerInside;

                if (button->hot)
                {
                    if (button->clickHandler)
                        button->clickHandler(context, entity);

                    createAudio(context, WAR_UI_CLICK, false);
                }

                button->active = false;
            }
            else if (isButtonPressed(input, WAR_MOUSE_LEFT))
            {
                if (button->hot)
                {
                    button->active = true;
                }
            }
            else if (pointerInside)
            {
                for(s32 j = 0; j < buttons->count; j++)
                {
                    WarEntity* otherButton = buttons->items[i];
                    if (otherButton)
                    {
                        otherButton->button.hot = false;
                        otherButton->button.active = false;
                    }
                }

                button->hot = true;
            }
            else
            {
                button->hot = false;
            }
        }
    }
}

void updateCommandFromRightClick(WarContext* context)
{
    WarMap* map = context->map;
    WarUnitCommand* command = &map->command;
    WarInput* input = &context->input;

    if (wasButtonPressed(input, WAR_MOUSE_RIGHT))
    {
        if (command->type == WAR_COMMAND_NONE)
        {
            s32 selEntitiesCount = map->selectedEntities.count;
            if (selEntitiesCount > 0)
            {
                // if the right click was on the map
                if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
                {
                    vec2 targetPoint = vec2ScreenToMapCoordinates(context, input->pos);
                    vec2 targetTile = vec2MapToTileCoordinates(targetPoint);

                    WarEntityId targetEntityId = getTileEntityId(map->finder, targetTile.x, targetTile.y);
                    WarEntity* targetEntity = findEntity(context, targetEntityId);
                    if (targetEntity)
                    {
                        if (isUnitOfType(targetEntity, WAR_UNIT_GOLDMINE))
                        {
                            if (checkUnitTiles(map, targetEntity, MAP_TILE_STATE_VISIBLE))
                                executeHarvestCommand(context, targetEntity, targetTile);
                            else
                                executeMoveCommand(context, targetPoint);
                        }
                        else if (isEntityOfType(targetEntity, WAR_ENTITY_TYPE_FOREST))
                        {
                            if (isTileVisible(map, (s32)targetTile.x, (s32)targetTile.y))
                            {
                                executeHarvestCommand(context, targetEntity, targetTile);
                            }
                            else
                            {
                                WarTree* tree = findAccesibleTree(context, targetEntity, targetTile);
                                if (tree)
                                {
                                    targetTile = vec2i(tree->tilex, tree->tiley);
                                    executeHarvestCommand(context, targetEntity, targetTile);
                                }
                                else
                                {
                                    executeMoveCommand(context, targetPoint);
                                }
                            }
                        }
                        else if (isUnitOfType(targetEntity, WAR_UNIT_TOWNHALL_HUMANS) || 
                                 isUnitOfType(targetEntity, WAR_UNIT_TOWNHALL_ORCS))
                        {
                            if (checkUnitTiles(map, targetEntity, MAP_TILE_STATE_VISIBLE))
                            {
                                if (isEnemyUnit(context, targetEntity))
                                {
                                    executeAttackCommand(context, targetEntity, targetTile);
                                }
                                else
                                {
                                    executeDeliverCommand(context, targetEntity);
                                }
                            }
                            else
                            {
                                executeMoveCommand(context, targetPoint);
                            }
                        }
                        else if (isWall(targetEntity))
                        {
                            // it doesn't matter if the wall piece is visible or not,
                            // the unit will walk to it
                            executeMoveCommand(context, targetPoint);
                        }
                        else
                        {
                            if (isEnemyUnit(context, targetEntity))
                            {
                                executeAttackCommand(context, targetEntity, targetTile);
                            }
                            else
                            {
                                executeFollowCommand(context, targetEntity);
                            }
                        }
                    }
                    else
                    {
                        executeMoveCommand(context, targetPoint);
                    }
                }
                // if the right click was on the minimap
                else if (rectContainsf(map->minimapPanel, input->pos.x, input->pos.y))
                {
                    vec2 offset = vec2ScreenToMinimapCoordinates(context, input->pos);
                    vec2 targetPoint = vec2TileToMapCoordinates(offset, true);

                    executeMoveCommand(context, targetPoint);
                }
            }
        }
        else
        {
            cancel(context, NULL);
        }
    }
}

void updateStatus(WarContext* context)
{
    WarMap* map = context->map;
    WarFlashStatus* flashStatus = &map->flashStatus;

    if (flashStatus->enabled)
    {
        if (flashStatus->startTime + flashStatus->duration >= context->time)
        {
            setStatus(context, NO_HIGHLIGHT, 0, 0, flashStatus->text);
            return;
        }
        
        // if the time for the flash status is over, just disabled it
        flashStatus->enabled = false;
    }

    char statusText[50] = {0};
    s32 highlightIndex = NO_HIGHLIGHT;
    s32 goldCost = 0;
    s32 woodCost = 0;

    if (map->selectedEntities.count > 0)
    {
        for (s32 i = 0; i < map->selectedEntities.count; i++)
        {
            WarEntityId selectedEntityId = map->selectedEntities.items[i];
            WarEntity* selectedEntity = findEntity(context, selectedEntityId);
            assert(selectedEntity);

            if (isBuildingUnit(selectedEntity))
            {
                if (isTraining(selectedEntity) || isGoingToTrain(selectedEntity))
                {
                    WarState* trainState = getTrainState(selectedEntity);
                    WarUnitType unitToBuild = trainState->train.unitToBuild;
                    WarUnitCommandMapping commandMapping = getCommandMappingFromUnitType(unitToBuild);
                    WarUnitCommandBaseData commandData = getCommandBaseData(commandMapping.type);

                    strcpy(statusText, commandData.tooltip2);
                    highlightIndex = NO_HIGHLIGHT;
                    goldCost = 0;
                    woodCost = 0;
                }
                else if (isUpgrading(selectedEntity) || isGoingToUpgrade(selectedEntity))
                {
                    WarState* upgradeState = getUpgradeState(selectedEntity);
                    WarUpgradeType upgradeToBuild = upgradeState->upgrade.upgradeToBuild;
                    WarUnitCommandMapping commandMapping = getCommandMappingFromUpgradeType(upgradeToBuild);
                    WarUnitCommandBaseData commandData = getCommandBaseData(commandMapping.type);
                    
                    strcpy(statusText, commandData.tooltip2);
                    highlightIndex = NO_HIGHLIGHT;
                    goldCost = 0;
                    woodCost = 0;
                }
                else
                {
                    s32 hp = selectedEntity->unit.hp;
                    s32 maxhp = selectedEntity->unit.maxhp;
                    if (hp < maxhp)
                    {
                        // to calculate the amount of wood and gold needed to repair a 
                        // building I'm taking the 12% of the damage of the building,
                        // so for the a FARM if it has a damage of 200, the amount of 
                        // wood and gold would be 200 * 0.12 = 24.
                        //
                        s32 repairCost = (s32)ceil((maxhp - hp) * 0.12f);
                        sprintf(statusText, "FULL REPAIRS WILL COST %d GOLD & LUMBER", repairCost);
                        highlightIndex = NO_HIGHLIGHT;
                        goldCost = 0;
                        woodCost = 0;
                    }
                }
            }
            else if (isWorkerUnit(selectedEntity))
            {
                if (isCarryingResources(selectedEntity))
                {
                    if (selectedEntity->unit.resourceKind == WAR_RESOURCE_GOLD)
                    {
                        strcpy(statusText, "CARRYING GOLD");
                    }
                    else if (selectedEntity->unit.resourceKind == WAR_RESOURCE_WOOD)
                    {
                        strcpy(statusText, "CARRYING LUMBER");
                    }

                    highlightIndex = NO_HIGHLIGHT;
                    goldCost = 0;
                    woodCost = 0;
                }
            }
        }
    }

    WarEntityList* buttons = getEntitiesOfType(map, WAR_ENTITY_TYPE_BUTTON);
    for(s32 i = 0; i < buttons->count; i++)
    {
        WarEntity* entity = buttons->items[i];
        if (entity)
        {
            WarButtonComponent* button = &entity->button;
            if (button->hot)
            {
                strcpy(statusText, button->tooltip);
                goldCost = button->gold;
                woodCost = button->wood;
                highlightIndex = button->highlightIndex;
                break;
            }
        }
    }

    setStatus(context, highlightIndex, goldCost, woodCost, statusText);
}

void updateCursor(WarContext* context)
{
    WarMap* map = context->map;
    WarInput* input = &context->input;
    
    WarEntity* entity = findUIEntity(context, "cursor");
    if (entity)
    {
        entity->transform.position = vec2Subv(input->pos, entity->cursor.hot);

        if (map->status != MAP_PLAYING)
        {
            changeCursorType(context, entity, WAR_CURSOR_ARROW);
            return;
        }

        if (input->isDragging)
        {
            changeCursorType(context, entity, WAR_CURSOR_GREEN_CROSSHAIR);
        }
        else if (rectContainsf(map->mapPanel, input->pos.x, input->pos.y))
        {
            WarUnitCommand* command = &map->command;
            switch (command->type)
            {
                case WAR_COMMAND_ATTACK:
                case WAR_COMMAND_SPELL_RAIN_OF_FIRE:
                case WAR_COMMAND_SPELL_POISON_CLOUD:
                {
                    changeCursorType(context, entity, WAR_CURSOR_RED_CROSSHAIR);
                    break;
                }

                case WAR_COMMAND_MOVE:
                case WAR_COMMAND_STOP:
                case WAR_COMMAND_HARVEST:
                case WAR_COMMAND_DELIVER:
                case WAR_COMMAND_REPAIR:
                case WAR_COMMAND_SPELL_HEALING:
                case WAR_COMMAND_SPELL_FAR_SIGHT:
                case WAR_COMMAND_SPELL_INVISIBILITY:
                case WAR_COMMAND_SPELL_RAISE_DEAD:
                case WAR_COMMAND_SPELL_DARK_VISION:
                case WAR_COMMAND_SPELL_UNHOLY_ARMOR:
                {
                    changeCursorType(context, entity, WAR_CURSOR_YELLOW_CROSSHAIR);
                    break;
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
                case WAR_COMMAND_BUILD_ROAD:
                case WAR_COMMAND_BUILD_WALL:
                {
                    changeCursorType(context, entity, WAR_CURSOR_ARROW);
                    break;
                }

                default:
                {
                    bool entityUnderCursor = false;

                    vec2 pointerRect = vec2ScreenToMapCoordinates(context, input->pos);

                    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
                    for(s32 i = 0; i < units->count; i++)
                    {
                        WarEntity* entity = units->items[i];
                        if (entity)
                        {
                            WarTransformComponent* transform = &entity->transform;
                            WarUnitComponent* unit = &entity->unit;
                            if (unit->enabled)
                            {
                                // don't change the cursor for dead units or corpses
                                if (isDead(entity) || isGoingToDie(entity) || isCorpseUnit(entity))
                                {
                                    continue;
                                }

                                // don't change the cursor for collased buildings
                                if (isCollapsing(entity) || isGoingToCollapse(entity))
                                {
                                    continue;
                                }

                                s32 tileX = (s32)(transform->position.x / MEGA_TILE_WIDTH);
                                s32 tileY = (s32)(transform->position.y / MEGA_TILE_HEIGHT);

                                // don't change the cursor for non-visible units
                                if (!checkMapTiles(map, tileX, tileY, unit->sizex, unit->sizey, MAP_TILE_STATE_VISIBLE))
                                {
                                    continue;
                                }
                            }
                            
                            rect unitRect = rectv(entity->transform.position, getUnitSpriteSize(entity));
                            if (rectContainsf(unitRect, pointerRect.x, pointerRect.y))
                            {
                                entityUnderCursor = true;
                                break;
                            }
                        }
                    }

                    if (entityUnderCursor)
                    {
                        changeCursorType(context, entity, WAR_CURSOR_MAGNIFYING_GLASS);                        
                    }
                    else
                    {
                        changeCursorType(context, entity, WAR_CURSOR_ARROW);
                    }
                    break;
                }
            }
        }
        else if (rectContainsf(map->minimapPanel, input->pos.x, input->pos.y))
        {
            WarUnitCommand* command = &map->command;
            switch (command->type)
            {
                case WAR_COMMAND_ATTACK:
                case WAR_COMMAND_SPELL_RAIN_OF_FIRE:
                case WAR_COMMAND_SPELL_POISON_CLOUD:
                {
                    changeCursorType(context, entity, WAR_CURSOR_RED_CROSSHAIR);
                    break;
                }

                case WAR_COMMAND_MOVE:
                case WAR_COMMAND_SPELL_FAR_SIGHT:
                case WAR_COMMAND_SPELL_DARK_VISION:
                {
                    changeCursorType(context, entity, WAR_CURSOR_YELLOW_CROSSHAIR);
                    break;
                }

                default:
                {
                    changeCursorType(context, entity, WAR_CURSOR_ARROW);
                    break;
                }
            }
        }
        else
        {
            vec2 dir = getDirFromMousePos(context, input);
            if (dir.x < 0 && dir.y < 0)         // -1, -1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_UP_LEFT);
            else if (dir.x < 0 && dir.y > 0)    // -1,  1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_BOTTOM_LEFT);
            else if (dir.x > 0 && dir.y < 0)    //  1, -1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_UP_RIGHT);
            else if (dir.x > 0 && dir.y > 0)    //  1,  1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_BOTTOM_RIGHT);
            else if (dir.x < 0)                 // -1,  0
                changeCursorType(context, entity, WAR_CURSOR_ARROW_LEFT);
            else if (dir.x > 0)                 //  1,  0
                changeCursorType(context, entity, WAR_CURSOR_ARROW_RIGHT);
            else if (dir.y < 0)                 //  0, -1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_UP);
            else if (dir.y > 0)                 //  0,  1
                changeCursorType(context, entity, WAR_CURSOR_ARROW_BOTTOM);
            else                                //  0,  0
                changeCursorType(context, entity, WAR_CURSOR_ARROW);                
        }
    }
}

void updateStateMachines(WarContext* context)
{
    WarMap* map = context->map;

    WarEntityList* entities = getEntities(map);
    for(s32 i = 0; i < entities->count; i++)
    {
        WarEntity* entity = entities->items[i];
        if (entity)
        {
            updateStateMachine(context, entity);
        }
    }
}

void updateActions(WarContext* context)
{
    WarMap* map = context->map;

    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
    for(s32 i = 0; i < units->count; i++)
    {
        WarEntity* entity = units->items[i];
        if (entity)
        {
            updateAction(context, entity);
        }
    }
}

void updateAnimations(WarContext* context)
{
    WarMap* map = context->map;

    // update all animations of entities
    WarEntityList* entities = getEntities(map);
    for(s32 i = 0; i < entities->count; i++)
    {
        WarEntity* entity = entities->items[i];
        if (entity)
        {
            WarAnimationsComponent* animations = &entity->animations;
            if (animations->enabled)
            {
                for(s32 i = 0; i < animations->animations.count; i++)
                {
                    WarSpriteAnimation* anim = animations->animations.items[i];
                    updateAnimation(context, entity, anim);
                }
            }
        }
    }

    // update all animations of the map
    for(s32 i = 0; i < map->animations.count; i++)
    {
        WarSpriteAnimation* anim = map->animations.items[i];
        updateAnimation(context, NULL, anim);
    }
}

void updateProjectiles(WarContext* context)
{
    WarMap* map = context->map;

    WarEntityList* projectiles = getEntitiesOfType(map, WAR_ENTITY_TYPE_PROJECTILE);
    for (s32 i = 0; i < projectiles->count; i++)
    {
        WarEntity* entity = projectiles->items[i];
        if (entity)
        {
            updateProjectile(context, entity);
        }
    }
}

void updateMagic(WarContext* context)
{
    WarMap* map = context->map;

    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
    for (s32 i = 0; i < units->count; i++)
    {
        WarEntity* entity = units->items[i];
        if (entity && isMagicUnit(entity))
        {
            if (isDead(entity) || isGoingToDie(entity))
                continue;

            WarUnitComponent* unit = &entity->unit;

            if (unit->manaTime <= 0)
            {
                if (isSummonUnit(entity))
                {
                    unit->mana = max(unit->mana - 1, 0);

                    // when the mana runs out the summoned units will die
                    if (unit->mana == 0)
                    {
                        WarState* deathState = createDeathState(context, entity);
                        changeNextState(context, entity, deathState, true, true);

                        if (entity->unit.type == WAR_UNIT_SCORPION ||
                            entity->unit.type == WAR_UNIT_SPIDER)
                        {
                            createAudio(context, WAR_DEAD_SPIDER_SCORPION, false);
                        }
                    }
                }
                else
                {
                    // the magic units have a mana regeneration rate of roughly 1 point/sec
                    // so a magic unit will spend almost 4 minutes to fill its mana when its rans out
                    unit->mana = min(unit->mana + 1, unit->maxMana);
                }
                
                unit->manaTime = getScaledTime(context, 1);
            }
            else
            {
                unit->manaTime -= context->deltaTime;
            }
        }
    }
}

bool updatePoisonCloud(WarContext* context, WarEntity* entity)
{
    WarPoisonCloudComponent* poisonCloud = &entity->poisonCloud;

    poisonCloud->time -= context->deltaTime;
    poisonCloud->damageTime -= context->deltaTime;

    if (poisonCloud->damageTime <= 0)
    {
        WarEntityList* nearUnits = getNearUnits(context, poisonCloud->position, 2);
        for (s32 i = 0; i < nearUnits->count; i++)
        {
            WarEntity* targetEntity = nearUnits->items[i];
            if (targetEntity && 
                !isDead(targetEntity) && !isGoingToDie(targetEntity) && 
                !isCollapsing(targetEntity) && !isGoingToCollapse(targetEntity))
            {
                takeDamage(context, targetEntity, 0, POISON_CLOUD_DAMAGE);
            }
        }
        WarEntityListFree(nearUnits);

        poisonCloud->damageTime = getScaledTime(context, 1);
    }

    return poisonCloud->time <= 0;
}

bool updateSight(WarContext* context, WarEntity* entity)
{
    WarSightComponent* sight = &entity->sight;
    sight->time -= context->deltaTime;
    return sight->time <= 0;
}

void updateSpells(WarContext* context)
{
    WarMap* map = context->map;

    WarEntityIdList spellsToRemove;
    WarEntityIdListInit(&spellsToRemove, WarEntityIdListDefaultOptions);

    WarEntityList* poisonCloudSpells = getEntitiesOfType(map, WAR_ENTITY_TYPE_POISON_CLOUD);
    for (s32 i = 0; i < poisonCloudSpells->count; i++)
    {
        WarEntity* entity = poisonCloudSpells->items[i];
        if (entity)
        {
            if (updatePoisonCloud(context, entity))
            {
                WarEntityIdListAdd(&spellsToRemove, entity->id);
                removeAnimation(context, NULL, entity->poisonCloud.animName);
            }
        }
    }

    WarEntityList* sightSpells = getEntitiesOfType(map, WAR_ENTITY_TYPE_SIGHT);
    for (s32 i = 0; i < sightSpells->count; i++)
    {
        WarEntity* entity = sightSpells->items[i];
        if (entity)
        {
            if (updateSight(context, entity))
            {
                WarEntityIdListAdd(&spellsToRemove, entity->id);
            }
        }
    }

    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
    for (s32 i = 0; i < units->count; i++)
    {
        WarEntity* entity = units->items[i];
        if (entity)
        {
            WarUnitComponent* unit = &entity->unit;
            
            if (unit->invisible)
            {
                unit->invisibilityTime -= context->deltaTime;
                if (unit->invisibilityTime <= 0)
                {
                    unit->invisible = false;
                    unit->invisibilityTime = 0;
                }
            }

            if (unit->invulnerable)
            {
                unit->invulnerabilityTime -= context->deltaTime;
                if (unit->invulnerabilityTime <= 0)
                {
                    unit->invulnerable = false;
                    unit->invulnerabilityTime = 0;
                }
            }
        }
    }

    for (s32 i = 0; i < spellsToRemove.count; i++)
    {
        removeEntityById(context, spellsToRemove.items[i]);
    }

    WarEntityIdListFree(&spellsToRemove);
}

void updateFoW(WarContext* context)
{
    WarMap* map = context->map;

    for (s32 i = 0; i < MAP_TILES_WIDTH * MAP_TILES_HEIGHT; i++)
    {
        map->tiles[i].type = WAR_FOG_PIECE_NONE;
        map->tiles[i].boundary = WAR_FOG_BOUNDARY_NONE;
        if (map->tiles[i].state == MAP_TILE_STATE_VISIBLE)
            map->tiles[i].state = MAP_TILE_STATE_FOG;
    }

    // the Holy Sight and Dark Vision spells are the first entities that change FoW
    WarEntityList* sightSpells = getEntitiesOfType(map, WAR_ENTITY_TYPE_SIGHT);
    for (s32 i = 0; i < sightSpells->count; i++)
    {
        WarEntity* entity = sightSpells->items[i];
        if (entity)
        {
            WarSightComponent* sight = &entity->sight;

            rect r = rectExpand(rectv(sight->position, VEC2_ONE), 3, 3);
            setMapTileState(map, r.x, r.y, r.width, r.height, MAP_TILE_STATE_VISIBLE);
        }
    }

    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);

    // do the update of the FoW for friendly units first
    for (s32 i = 0; i < units->count; i++)
    {
        WarEntity* entity = units->items[i];
        if (entity)
        {
            if (isFriendlyUnit(context, entity))
            {
                if (isBuildingUnit(entity))
                {
                    // the friendly buildings are always seen by the player
                    entity->unit.hasBeenSeen = true;
                }

                // mark the tiles of the unit as visible
                setUnitMapTileState(map, entity, MAP_TILE_STATE_VISIBLE);

                // reveal the attack target of the unit
                WarEntity* targetEntity = getAttackTarget(context, entity);
                if (targetEntity)
                {
                    if (isUnit(targetEntity))
                    {
                        setUnitMapTileState(map, targetEntity, MAP_TILE_STATE_VISIBLE);
                    }
                    else if (isWall(targetEntity))
                    {
                        WarState* attackState = getAttackState(entity);
                        vec2 targetTile = attackState->attack.targetTile;
                        WarWallPiece* piece = getWallPieceAtPosition(targetEntity, targetTile.x, targetTile.y);
                        if (piece)
                        {
                            setMapTileState(map, targetTile.x, targetTile.y, 1, 1, MAP_TILE_STATE_VISIBLE);
                        }
                    }
                }

                // reveal the attacker
                WarEntity* attacker = getAttacker(context, entity);
                if (attacker)
                {
                    // if the attacker is the same the unit is attacking to
                    // don't change tile state because already happened above
                    if (!targetEntity || attacker->id != targetEntity->id)
                    {
                        setUnitMapTileState(map, attacker, MAP_TILE_STATE_VISIBLE);
                    }
                }
            }
        }
    }

    // and then do the update of the FoW for enemies and neutrals units
    for (s32 i = 0; i < units->count; i++)
    {
        WarEntity* entity = units->items[i];
        if (entity)
        {
            if (!isFriendlyUnit(context, entity))
            {
                if (isBuildingUnit(entity) && !entity->unit.hasBeenSeen)
                {
                    // mark the enemy's buildings as seen if they are currently in sight
                    vec2 tilePosition = getUnitPosition(entity, true);
                    vec2 unitSize = getUnitSize(entity);
                    entity->unit.hasBeenSeen = checkMapTiles(map, tilePosition.x, tilePosition.y, 
                                                             unitSize.x, unitSize.y, MAP_TILE_STATE_VISIBLE);
                }
            }
        }
    }
}

void determineFoWTypes(WarContext* context)
{
    WarMap* map = context->map;

    const s32 dirC = 8;
    const s32 dirX[] = { -1,  0,  1, 1, 1, 0, -1, -1 };
    const s32 dirY[] = { -1, -1, -1, 0, 1, 1,  1,  0 };

    for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
    {
        for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
        {
            WarMapTile* tile = getMapTileState(map, x, y);
            if (tile->state == MAP_TILE_STATE_VISIBLE)
            {
                s32 index = 0;
                s32 unkownCount = 0;
                s32 fogCount = 0;

                for (s32 d = 0; d < dirC; d++)
                {
                    s32 xx = x + dirX[d];
                    s32 yy = y + dirY[d];
                    
                    if (inRange(xx, 0, MAP_TILES_WIDTH) && 
                        inRange(yy, 0, MAP_TILES_HEIGHT))
                    {
                        WarMapTile* neighborTile = getMapTileState(map, xx, yy);
                        if (neighborTile->state == MAP_TILE_STATE_VISIBLE)
                            index = index | (1 << d);
                        else if (neighborTile->state == MAP_TILE_STATE_FOG)
                            fogCount++;
                        else
                            unkownCount++;
                    }
                }

                if (index != 0xFF)
                {
                    tile->type = fogTileTypeMap[index];
                }

                if (fogCount > 0)
                    tile->boundary = WAR_FOG_BOUNDARY_FOG;
                else if (unkownCount > 0)
                    tile->boundary = WAR_FOG_BOUNDARY_UNKOWN;
            }
            else if (tile->state == MAP_TILE_STATE_FOG)
            {
                s32 index = 0;
                s32 unkownCount = 0;
 
                for (s32 d = 0; d < dirC; d++)
                {
                    s32 xx = x + dirX[d];
                    s32 yy = y + dirY[d];
                    
                    if (inRange(xx, 0, MAP_TILES_WIDTH) && 
                        inRange(yy, 0, MAP_TILES_HEIGHT))
                    {
                        WarMapTile* neighborTile = getMapTileState(map, xx, yy);
                        if (neighborTile->state == MAP_TILE_STATE_VISIBLE || 
                            neighborTile->state == MAP_TILE_STATE_FOG)
                        {
                            index = index | (1 << d);
                        }
                        else
                        {
                            unkownCount++;
                        }
                    }
                }

                if (index != 0xFF)
                {
                    tile->type = fogTileTypeMap[index];
                }

                if (unkownCount > 0)
                {
                    tile->boundary = WAR_FOG_BOUNDARY_UNKOWN;
                }
            }
            else
            {
                s32 index = 0;

                for (s32 d = 0; d < dirC; d++)
                {
                    s32 xx = x + dirX[d];
                    s32 yy = y + dirY[d];
                    
                    if (inRange(xx, 0, MAP_TILES_WIDTH) && 
                        inRange(yy, 0, MAP_TILES_HEIGHT))
                    {
                        WarMapTile* neighborTile = getMapTileState(map, xx, yy);
                        if (neighborTile->state == MAP_TILE_STATE_VISIBLE ||
                            neighborTile->state == MAP_TILE_STATE_FOG)
                        {
                            index = index | (1 << d);
                        }
                    }
                }

                if (index == 0xFF)
                {
                    tile->type = fogTileTypeMap[index];
                }
            }
        }
    }
}

bool checkObjectives(WarContext* context)
{
    WarMap* map = context->map;

    map->objectivesTime -= context->deltaTime;

    if (map->objectivesTime <= 0)
    {
        WarCampaignMapData data = getCampaignData((WarCampaignMapType)map->levelInfoIndex);
        if (data.checkObjectivesFunc)
        {
            if (data.checkObjectivesFunc(context))
                return true;
        }

        map->objectivesTime = 1;
    }

    return false;
}

void updateMapPlaying(WarContext* context)
{
    WarMap* map = context->map;

    updateViewport(context);
    updateDragRect(context);

    if (!executeCommand(context))
    {
        // only update the selection if the current command doesn't get
        // executed or there is no command at all.
        // the reason is because some commands are executed by the left click
        // as well as the selection, and if a command get executed the current 
        // selection shouldn't be lost
        updateSelection(context);
    }

    updateStateMachines(context);
    updateActions(context);
    updateAnimations(context);
    updateProjectiles(context);
    updateMagic(context);
    updateSpells(context);

    updateFoW(context);
    determineFoWTypes(context);

    updateGoldText(context);
    updateWoodText(context);
    updateSelectedUnitsInfo(context);
    updateCommands(context);
    updateButtons(context);
    updateCommandFromRightClick(context);
    updateStatus(context);
    updateCursor(context);

    updateTreesEdit(context);
    updateRoadsEdit(context);
    updateWallsEdit(context);
    updateRuinsEdit(context);
    updateRainOfFireEdit(context);

    if (checkObjectives(context))
    {
        showOrHideGameOverMenu(context, true);

        map->status = MAP_GAME_OVER;
    }
}

void updateMapMenu(WarContext* context)
{
    updateButtons(context);
    updateCursor(context);
}

void updateMap(WarContext* context)
{
    WarMap* map = context->map;

    updateGlobalSpeed(context);
    updateGlobalScale(context);
    updateVolume(context);

    switch (map->status)
    {
        case MAP_PLAYING:
        {
            updateMapPlaying(context);
            break;
        }
        case MAP_MENU:
        case MAP_OPTIONS:
        case MAP_QUIT:
        case MAP_GAME_OVER:
        {
            updateMapMenu(context);
            break;
        }
        case MAP_SAVE_GAME:
        {
            break;
        }
        case MAP_LOAD_GAME:
        {
            break;
        }
        default:
        {
            logError("Unkown map status: %d\n", map->status);
            break;
        }
    }
}

void renderTerrain(WarContext* context)
{
    WarMap *map = context->map;

    NVGcontext* gfx = context->gfx;

    WarResource* levelInfo = getOrCreateResource(context, map->levelInfoIndex);
    assert(levelInfo && levelInfo->type == WAR_RESOURCE_TYPE_LEVEL_INFO);

    WarResource* levelVisual = getOrCreateResource(context, levelInfo->levelInfo.visualIndex);
    assert(levelVisual && levelVisual->type == WAR_RESOURCE_TYPE_LEVEL_VISUAL);

    NVGimageBatch* batch = nvgBeginImageBatch(gfx, map->sprite.image, MAP_TILES_WIDTH * MAP_TILES_HEIGHT);

    for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
    {
        for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
        {
            WarMapTile* tile = getMapTileState(map, x, y);
            if (tile->state == MAP_TILE_STATE_VISIBLE ||
                tile->state == MAP_TILE_STATE_FOG)
            {
                // index of the tile in the tilesheet
                u16 tileIndex = levelVisual->levelVisual.data[y * MAP_TILES_WIDTH + x];
                
                // coordinates in pixels of the terrain tile
                s32 tilePixelX = (tileIndex % TILESET_TILES_PER_ROW) * MEGA_TILE_WIDTH;
                s32 tilePixelY = ((tileIndex / TILESET_TILES_PER_ROW) * MEGA_TILE_HEIGHT);

                nvgSave(gfx);
                nvgTranslate(gfx, x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT);

                rect rs = recti(tilePixelX, tilePixelY, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                rect rd = recti(0, 0, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                nvgRenderBatchImage(gfx, batch, rs, rd, VEC2_ONE);

                nvgRestore(gfx);
            }
        }
    }

    nvgEndImageBatch(gfx, batch);
}

void renderFoW(WarContext* context)
{
    WarMap* map = context->map;

    NVGcontext* gfx = context->gfx;

    NVGimageBatch* unkownBatch = nvgBeginImageBatch(gfx, map->blackSprite.image, MAP_TILES_WIDTH * MAP_TILES_HEIGHT);
    NVGimageBatch* fogBatch = nvgBeginImageBatch(gfx, map->blackSprite.image, MAP_TILES_WIDTH * MAP_TILES_HEIGHT);
    NVGimageBatch* unkownBoundaryBatch = nvgBeginImageBatch(gfx, map->sprite.image, MAP_TILES_WIDTH * MAP_TILES_HEIGHT);
    NVGimageBatch* fogBoundaryBatch = nvgBeginImageBatch(gfx, map->sprite.image, MAP_TILES_WIDTH * MAP_TILES_HEIGHT);

    for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
    {
        for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
        {
            WarMapTile* tile = getMapTileState(map, x, y);
            if (tile->type != WAR_FOG_PIECE_NONE)
            {
                s32 tileIndex = (s32)tile->type;

                // coordinates in pixels of the terrain tile
                s32 tilePixelX = (tileIndex % TILESET_TILES_PER_ROW) * MEGA_TILE_WIDTH;
                s32 tilePixelY = ((tileIndex / TILESET_TILES_PER_ROW) * MEGA_TILE_HEIGHT);

                rect rs = recti(tilePixelX, tilePixelY, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                rect rd = recti(x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);

                if (tile->state == MAP_TILE_STATE_VISIBLE && tile->boundary == WAR_FOG_BOUNDARY_FOG)
                {
                    nvgRenderBatchImage(gfx, fogBoundaryBatch, rs, rd, VEC2_ONE);                        
                }
                else if (tile->boundary == WAR_FOG_BOUNDARY_UNKOWN)
                {
                    nvgRenderBatchImage(gfx, unkownBoundaryBatch, rs, rd, VEC2_ONE);
                }
            }

            if (tile->state == MAP_TILE_STATE_UNKOWN)
            {
                rect rs = recti(0, 0, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                rect rd = recti(x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                nvgRenderBatchImage(gfx, unkownBatch, rs, rd, VEC2_ONE);
            }
            else if (tile->state == MAP_TILE_STATE_FOG)
            {
                rect rs = recti(0, 0, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                rect rd = recti(x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT, MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                nvgRenderBatchImage(gfx, fogBatch, rs, rd, VEC2_ONE);
            }
        }
    }

    nvgEndImageBatch(gfx, unkownBoundaryBatch);
    nvgEndImageBatch(gfx, unkownBatch);

    nvgSave(gfx);
    nvgGlobalAlpha(gfx, 0.5f);
    nvgEndImageBatch(gfx, fogBoundaryBatch);
    nvgEndImageBatch(gfx, fogBatch);
    nvgRestore(gfx);
}

void renderAnimations(WarContext* context)
{
    WarMap *map = context->map;

    NVGcontext* gfx = context->gfx;

    for(s32 i = 0; i < map->animations.count; i++)
    {
        WarSpriteAnimation* anim = map->animations.items[i];
        if (anim->status == WAR_ANIM_STATUS_RUNNING)
        {
            s32 animFrameIndex = (s32)(anim->animTime * anim->frames.count);
            animFrameIndex = clamp(animFrameIndex, 0, anim->frames.count - 1);

            s32 spriteFrameIndex = anim->frames.items[animFrameIndex];
            assert(spriteFrameIndex >= 0 && spriteFrameIndex < anim->sprite.framesCount);

            nvgSave(gfx);

            nvgTranslate(gfx, anim->offset.x, anim->offset.y);
            nvgScale(gfx, anim->scale.x, anim->scale.y);

#ifdef DEBUG_RENDER_MAP_ANIMATIONS
            // size of the original sprite
            vec2 animFrameSize = vec2i(anim->sprite.frameWidth, anim->sprite.frameHeight);

            nvgFillRect(gfx, rectv(VEC2_ZERO, animFrameSize), NVG_GRAY_TRANSPARENT);
#endif

            WarSpriteFrame frame = anim->sprite.frames[spriteFrameIndex];
            updateSpriteImage(context, anim->sprite, frame.data);
            renderSprite(context, anim->sprite, VEC2_ZERO, VEC2_ONE);

            nvgRestore(gfx);
        }
    }
}

void renderUnitPaths(WarContext* context)
{
    WarMap *map = context->map;

    NVGcontext* gfx = context->gfx;

    WarEntityList* units = getEntitiesOfType(map, WAR_ENTITY_TYPE_UNIT);
    for(s32 i = 0; i < units->count; i++)
    {
        WarEntity *entity = units->items[i];
        if (entity)
        {
            WarState* moveState = getDirectState(entity, WAR_STATE_MOVE);
            if (moveState)
            {
                vec2List positions = moveState->move.positions;
                for(s32 k = moveState->move.positionIndex; k < positions.count; k++)
                {
                    vec2 pos = vec2TileToMapCoordinates(positions.items[k], true);
                    pos = vec2Subv(pos, vec2i(2, 2));
                    nvgFillRect(gfx, rectv(pos, vec2i(4, 4)), getColorFromList(entity->id));
                }
                
                s32 index = moveState->move.pathNodeIndex;
                WarMapPath path = moveState->move.path;
                
                if (index >= 0)
                {
                    vec2 prevPos;
                    for(s32 k = 0; k < path.nodes.count; k++)
                    {
                        vec2 pos = vec2TileToMapCoordinates(path.nodes.items[k], true);

                        if (k > 0)
                            nvgStrokeLine(gfx, prevPos, pos, getColorFromList(entity->id), 0.5f);

                        nvgFillRect(gfx, rectv(pos, VEC2_ONE), k == index ? nvgRGB(255, 0, 255) : nvgRGB(255, 255, 0));

                        prevPos = pos;
                    }
                }
            }
        }
    }
}

void renderPassableInfo(WarContext* context)
{
    WarMap *map = context->map;

    NVGcontext* gfx = context->gfx;

    for(s32 y = 0; y < MAP_TILES_HEIGHT; y++)
    {
        for(s32 x = 0; x < MAP_TILES_WIDTH; x++)
        {
            if (isStatic(map->finder, x, y))
            {
                vec2 pos = vec2i(x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT);
                vec2 size = vec2i(MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                nvgFillRect(gfx, rectv(pos, size), nvgRGBA(255, 0, 0, 100));
            }
            else if(isDynamic(map->finder, x, y))
            {
                vec2 pos = vec2i(x * MEGA_TILE_WIDTH, y * MEGA_TILE_HEIGHT);
                vec2 size = vec2i(MEGA_TILE_WIDTH, MEGA_TILE_HEIGHT);
                nvgFillRect(gfx, rectv(pos, size), nvgRGBA(255, 150, 100, 100));
            }
        }
    }
}

void renderMapGrid(WarContext* context)
{
    NVGcontext* gfx = context->gfx;

    for(s32 x = 1; x < MAP_TILES_WIDTH; x++)
    {
        vec2 p1 = vec2i(x * MEGA_TILE_WIDTH, 0);
        vec2 p2 = vec2i(x * MEGA_TILE_WIDTH, MAP_TILES_HEIGHT * MEGA_TILE_HEIGHT);
        nvgStrokeLine(gfx, p1, p2, NVG_WHITE, 0.25f);
    }

    for(s32 y = 1; y < MAP_TILES_HEIGHT; y++)
    {
        vec2 p1 = vec2i(0, y * MEGA_TILE_HEIGHT);
        vec2 p2 = vec2i(MAP_TILES_WIDTH * MAP_TILES_WIDTH, y * MEGA_TILE_HEIGHT);
        nvgStrokeLine(gfx, p1, p2, NVG_WHITE, 0.25f);
    }
}

void renderMapPanel(WarContext *context)
{
    WarMap *map = context->map;

    NVGcontext* gfx = context->gfx;

    nvgSave(gfx);

    nvgTranslate(gfx, map->mapPanel.x, map->mapPanel.y);
    nvgTranslate(gfx, -map->viewport.x, -map->viewport.y);
    
    renderTerrain(context);
    renderEntitiesOfType(context, WAR_ENTITY_TYPE_ROAD);
    renderEntitiesOfType(context, WAR_ENTITY_TYPE_WALL);
    renderEntitiesOfType(context, WAR_ENTITY_TYPE_RUIN);
    renderEntitiesOfType(context, WAR_ENTITY_TYPE_FOREST);
    
#ifdef DEBUG_RENDER_UNIT_PATHS
    renderUnitPaths(context);
#endif

#ifdef DEBUG_RENDER_PASSABLE_INFO
    renderPassableInfo(context);
#endif

#ifdef DEBUG_RENDER_MAP_GRID
    renderMapGrid(context);
#endif

    renderEntitiesOfType(context, WAR_ENTITY_TYPE_UNIT);
    renderUnitSelection(context);
    renderEntitiesOfType(context, WAR_ENTITY_TYPE_PROJECTILE);    
    renderAnimations(context);
    renderFoW(context);
    
    nvgRestore(gfx);
}

void renderMap(WarContext *context)
{
    NVGcontext* gfx = context->gfx;

    nvgSave(gfx);
    nvgScale(gfx, context->globalScale, context->globalScale);

    // render map
    renderMapPanel(context);

    // render ui
    renderMapUI(context);

    nvgRestore(gfx);
}