#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "entity.h"

Entity *newCube(Vec3D position,const char *name);
Entity *newSwordsmen(Vec3D position,const char *name);
Entity *newFlying(Vec3D position,const char *name);

void touch_callback(void *data, void *context);
void boxThink(Entity *self);
void swordThink(Entity *self);
void sword_callback(void *data, void *context);
void flyingThink(Entity *self);
void flyingCallback(void *data, void *context);
#endif