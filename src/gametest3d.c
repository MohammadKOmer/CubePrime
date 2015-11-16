/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"
#include "enemy.h"
#include "player.h"
#include <stdlib.h>
#include <math.h>
void set_camera(Vec3D position, Vec3D rotation);

extern int horizontalDir;
extern int verticalDir;
extern int leftMouseInput;
extern int rightMouseInput;
extern int now;
extern int then;
extern Space *space;
extern int jump;
int obsSpawnTimer;
int flySpawnTimer;
int enemySpawnTimer;
int difficultyMod;

void spawnForThisTick(){
	
	obsSpawnTimer-= 33;
	flySpawnTimer-= 33;
	enemySpawnTimer-= 33;
	if(obsSpawnTimer<0){
		newCube(vec3d((float)rand()/((float)RAND_MAX/6)-3
			,10,0),"Box");
		obsSpawnTimer=(rand()%40)*difficultyMod;
		difficultyMod--;
	}
	if(enemySpawnTimer<0){
		Entity* sword=newSwordsmen(vec3d(((rand()%2)*2-1)*4
			,0,0),"swords");
		slog("%f sword x",sword->body.position.x);
		enemySpawnTimer=(rand()%40)*difficultyMod;
	}
	if(flySpawnTimer<0){
		newFlying( vec3d((float)rand()/((float)RAND_MAX/6)-3,
		(float)rand()/((float)RAND_MAX/5)+2,
		(float)rand()/((float)RAND_MAX/3.5)+1.5),"swords");
		flySpawnTimer=(rand()%40)*difficultyMod;
	}
	
}
int main(int argc, char *argv[])
{
    int i;
    float r = 0;
  
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,-10,2};
    Vec3D cameraRotation = {90,0,0};
    SDL_Event e;
    Obj *bgobj,*chicken;
    Sprite *bgtext;
      obsSpawnTimer=100;
	flySpawnTimer=300;
	enemySpawnTimer=500;
	difficultyMod=100;
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
    space = space_new();
    space_set_steps(space,100);  
	SDL_SetRelativeMouseMode(SDL_TRUE);

//    chicken = obj_load("models/monkey.obj");
    //bgobj = obj_load("models/mountainvillage.obj");
  //  bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
    newPlayer(vec3d(0,0,0),"Player");
    newCube(vec3d(3,10,0),"Box");
	newCrosshair(vec3d(0,0,3),"Cross");
    newSwordsmen(vec3d(3,0,0),"swords");
	newFlying(vec3d(0,7,4),"fly pupa");
  


    while (bGameLoopRunning)
    {
		spawnForThisTick();
        entity_think_all();
        for (i = 0; i < 100;i++)
        {
            space_do_step(space);
        }
        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_d)
                {
                   horizontalDir=1;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    horizontalDir=-1;
                }
				if (e.key.keysym.sym == SDLK_w){
					verticalDir = 1;
				}else if (e.key.keysym.sym == SDLK_s)
                {
                    verticalDir=-1;
                }
				if(e.key.keysym.sym ==SDLK_SPACE){
					jump=1;
				}
			}else if(e.type==SDL_KEYUP){

				if ((e.key.keysym.sym== SDLK_d)&&(horizontalDir==1))
                {
                   horizontalDir=0;
                }
                else if ((e.key.keysym.sym == SDLK_a)&&(horizontalDir==-1))
                {
                    horizontalDir=0;
                }
				if ((e.key.keysym.sym== SDLK_w)&&(verticalDir==1))
				{
					verticalDir=0;
				}
				else if ((e.key.keysym.sym == SDLK_s)&&(verticalDir==-1))
				{
					verticalDir=0;
				}
				if(e.key.keysym.sym ==SDLK_SPACE){
					jump=0;
				}
			}else if(e.type==SDL_MOUSEBUTTONDOWN){
				if(e.button.button==SDL_BUTTON_LEFT){
					leftMouseInput=1;
				}
				if(e.button.button==SDL_BUTTON_RIGHT){
					rightMouseInput=1;
				}
			}else if(e.type==SDL_MOUSEBUTTONUP){
				if(e.button.button==SDL_BUTTON_LEFT){
					leftMouseInput=0;
				}
				if(e.button.button==SDL_BUTTON_RIGHT){
					rightMouseInput=0;
				}
			}
			//slog("%i\n",jump);
        }

        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
        
        entity_draw_all();
        glPushMatrix();
        glTranslatef(-5,0,0);
        /*obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(1,1,1),
            vec4d(1,0,0,1),
            NULL
        );*/
        glPushMatrix();
        
        glTranslatef(0,1,0);
        glScalef(0.5,0.5,0.5);
        
        glRotatef(45,0.0,0.0,1.00);
       /* obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(1,1,1),
            vec4d(0,1,0,1),
            NULL
        );    */    
        glPushMatrix();
        glRotatef(45,0.0,1.0,0.0);
      /*  obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(0.5,0.5,0.5),
            vec4d(0,0,1,1),
            NULL
        );    */    
        
        glPopMatrix();
        glPopMatrix();
        
        glPopMatrix();
    /*    obj_draw(
            bgobj,
            vec3d(0,7,2),
            vec3d(90,90,0),
            vec3d(5,5,5),
            vec4d(1,1,1,1),
            bgtext
        );
        */
        if (r > 360)r -= 360;
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}

void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

/*eol@eof*/