#include "entity.h"
#include "simple_logger.h"
#include "player.h"
#include "space.h"
#include "graphics3d.h"
extern Space *space;
extern int horizontalDir;
extern int verticalDir;
extern int attackDir;
extern int leftMouseInput;
extern int rightMouseInput;
extern Entity* Player;
extern int then;
extern int now;
extern int jump;
int horizontalDir;
int verticalDir;
 int leftMouseInput;
 int rightMouseInput;
int attackDir;
int timeTillNextBullet;
int jump;

Entity* Player;
Entity *newPlayer(Vec3D position,const char *name)
{
    Entity * ent;
    char buffer[255];
    int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
	
	ent->type=PLAYER;
	ent->health=100;

	vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 90;
    sprintf(ent->name,"%s",name);
    ent->think = playerThink;
    ent->state = 0;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->body.touch.data=ent;
	space_add_body(space,&ent->body);
	Player=ent;
    return ent;
}

void playerThink(Entity *self)
{
	float speed;
	speed=0.3;
	if(self->body.position.x<-3&&horizontalDir==-1){
		speed=0;
	}else if(self->body.position.x>3&&horizontalDir==1){
		speed=0;
	}
	if(horizontalDir!=0){
		attackDir=horizontalDir;
	}
	self->body.velocity.x=horizontalDir*speed;
	self->body.velocity.y=verticalDir*speed;
	
}

Entity *newCrosshair(Vec3D position,const char *name){
	
    Entity * ent;
    char buffer[255];
    int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
	vec3d_cpy(ent->scale,vec3d(.05,.05,.05));
	ent->type=CROSSHAIR;
	

	vec3d_cpy(ent->body.position,position);
    //cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 90;
    sprintf(ent->name,"%s",name);
   ent->think = crossHairThink;
    ent->state = 0;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->body.touch.data=ent;
	space_add_body(space,&ent->body);
    return ent;
}
void crossHairThink(Entity *self){
	int x,z;
	float speed = .005;
	SDL_GetRelativeMouseState(&x,&z);
//	slog("%f , %f",self->body.position.x,self->body.position.z);

	timeTillNextBullet -= 33;
	self->body.velocity.x=speed *x;
	self->body.velocity.z=-speed *z;
	if(((self->body.position.x>3)&&(self->body.velocity.x>0))||
		((self->body.position.x<-3)&&(self->body.velocity.x<0))){
		self->body.velocity.x=0;
	}
	if(((self->body.position.z>5)&&(self->body.velocity.z>0))||
		((self->body.position.z<1.5)&&(self->body.velocity.z<0))){
		self->body.velocity.z=0;
	}
	if(timeTillNextBullet<0&&rightMouseInput==1){
		newBullet(vec3d(self->body.position.x,self->body.position.y+.5,self->body.position.z),"Bullet");
		timeTillNextBullet=150;
	}
	
}

Entity *newBullet(Vec3D position,const char *name){
	
    Entity * ent;
    char buffer[255];
    int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
	vec3d_cpy(ent->scale,vec3d(.05,.05,.05));
	ent->type=BULLET;
	

	vec3d_cpy(ent->body.position,position);
    //cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 90;
    sprintf(ent->name,"%s",name);
   ent->think = bulletThink;
    ent->state = 0;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->body.touch.data=ent;
	ent->body.velocity.y=+.5;
	space_add_body(space,&ent->body);
    return ent;
}
void bulletThink(Entity *self){
	if(self->body.position.y>30){
		space_remove_body(space,&self->body);
		entity_free(self);
	}
}
