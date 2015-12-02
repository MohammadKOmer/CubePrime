#include "entity.h"
#include "simple_logger.h"
#include "player.h"
#include "space.h"
#include "graphics3d.h"
#include <math.h>


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
extern int mouseX;
extern int mouseY;
extern    Vec3D cameraPosition;
 extern   Vec3D cameraRotation ;
 Vec3D cameraPosition;
 Vec3D cameraRotation;
 float cameraHorizontalAngle;
 float cameraVerticalAngle;
int horizontalDir;
int verticalDir;
 int leftMouseInput;
 int rightMouseInput;
int attackDir;
int timeTillNextBullet;
int jump;
int mouseX;
int mouseY;
#define PI 3.14159265

Entity* Player;
Entity *newPlayer(Vec3D position,const char *name)
{
    Entity * ent;
    char buffer[255];

    int i;
		  cameraPosition = vec3d(0,-10,2);
     cameraRotation = vec3d(90,0,0);
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
	
	
	cameraHorizontalAngle=0;
	cameraVerticalAngle=0;
    return ent;
}

void playerThink(Entity *self)
{
	float speed,sensitivity,camDist;
	sensitivity = .5;
	speed=0.3;
	camDist=10;
	if(horizontalDir!=0){
		attackDir=horizontalDir;
	}
	


	vec3d_add(self->rotation,self->rotation,vec3d(0,-mouseX*sensitivity,0));
	if(self->rotation.y>360){
		self->rotation.y-=360;
	}else if (self->rotation.y<0){
		self->rotation.y=360-self->rotation.y;
	}
	

	 self->body.velocity.x=horizontalDir*speed*cos(self->rotation.y* PI / 180.0);
	 self->body.velocity.y=horizontalDir*speed*sin(self->rotation.y* PI / 180.0);
	self->body.velocity.x+=verticalDir*speed*cos((self->rotation.y+90)* PI / 180.0);
	self->body.velocity.y+=verticalDir*speed*sin((self->rotation.y+90)* PI / 180.0);
	
	cameraRotation.z=self->rotation.y;
	cameraPosition.x=self->body.position.x+camDist*sin(self->rotation.y* PI / 180.0);//s (x0 + r cos theta, y0 + r sin theta).
	cameraPosition.y=self->body.position.y-camDist*cos(self->rotation.y* PI / 180.0);
	slog("%f, x:%f,  y:%f",self->rotation.y,self->body.velocity.x,self->body.velocity.y);

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

	float speed = .005;
	
//	slog("%f , %f",self->body.position.x,self->body.position.z);

	timeTillNextBullet -= 33;
	self->body.velocity.x=speed *mouseX;
	self->body.velocity.z=-speed *mouseY;
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
