#ifndef __PLAYER_H__
#define __PLAYER_H__
Entity *newPlayer(Vec3D position,const char *name);
void playerThink(Entity *self);
Entity *newCrosshair(Vec3D position,const char *name);
void crossHairThink(Entity *self);
Entity *newBullet(Vec3D position,const char *name);
void bulletThink(Entity *self);
#endif