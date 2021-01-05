#ifndef OBJETS_H_SEEN
#define OBJETS_H_SEEN

#include <GL4D/gl4du.h>
#include <SDL_mixer.h>
#include "makeLabyrinth.h"
class Environnement {
	private:
		static GLuint cube, quad;
		GLuint* grille;
		int w, h;
	// Constructeur:
	public:
		Environnement(int w, int h){
			if(quad == 0) quad = gl4dgGenQuadf();
			if(cube == 0) cube = gl4dgGenCubef();
			this->w= w;
			this->h= h;
			grille = labyrinth(this->w, this->h);
		}
	// Destructeur:
	~Environnement(){
		freeLabyrinth((unsigned int *)grille);
	}
	bool estValide(GLfloat x, GLfloat z){
		int i , j;
		i = ((int)z) + h / 2;
		j = ((int)x) + w / 2;
		if(j >= 0 && j < w && i >= 0 && i < h)
			return true ;
		return false;
	}
	bool estLibre(GLfloat x, GLfloat z){
		int i , j;
		i = ((int)z) + h / 2;
		j = ((int)x) + w / 2;
		if(j >= 0 && j < w && i >= 0 && i < h)
			return grille[i*w+j] == 0 ? true : false;
		return false;
	}
	void suprimerMur(GLfloat x , GLfloat z){
		if(estValide(x,z)){
			int i , j;
			i = ((int)z) + h / 2;
			j = ((int)x) + w / 2;
			grille[i*w+j] = 0;
		}
	}
	void draw(GLuint pId, GLfloat * floorColor, GLfloat * wallColor){
		int i, j;
		/* placer le sol */
		gl4duPushMatrix();
		gl4duRotatef(-90, 1, 0, 0);
		gl4duScalef(w * 0.5f, h * 0.5f, 1);
		gl4duSendMatrices();
		gl4duPopMatrix();
		/* dessin de la géométrie du sol */
		glUniform4fv(glGetUniformLocation(pId, "scolor"), 1, floorColor);
		gl4dgDraw(quad);
		/* mettre du bleu pour les cubes "murs" */
		glUniform4fv(glGetUniformLocation(pId, "scolor"), 1, wallColor);
  		/* placer des cubes là où il y a des murs */
  		for (i = -h / 2; i <= h / 2; ++i) {
	  		for (j = -w / 2; j <= w / 2; ++j) {
		  		if (grille[(i + h / 2) * w + j + w / 2] == 0) 
			  		continue;
				gl4duPushMatrix();
				gl4duTranslatef(j, 0.5f, i);
				gl4duScalef(0.5f, 0.5f, 0.5f);
				gl4duSendMatrices();
				gl4duPopMatrix();
				/* dessin d'un cube */
				gl4dgDraw(cube);
	  		}
  		}
	}
};
class Perso {
private:
	static GLuint sphere, cone;
	static Mix_Chunk* gsound;
	GLfloat x, z, r;
	int type;
	Environnement * env;
	void init(Environnement * env, GLfloat x, GLfloat z, GLfloat r, int type){
		if(sphere == 0) sphere = gl4dgGenSpheref(8,8);
		if(cone == 0) cone = gl4dgGenConef(8, GL_FALSE);
		
		this->env = env;
		this->x = x;
		this->z = z;
		this->r = r;
		this->type = type;
		this->env->suprimerMur(this->x,this->z);

	} 
public:
	static void initAudio(){
		if(gsound == NULL) gsound = Mix_LoadWAV("ghost.wav");
	}
	static void freeAudio(){
		if(gsound){
			Mix_FreeChunk(gsound);
			gsound = NULL;
		} 
	}
	Perso(Environnement * env){
		init(env, 0,0,0.75f,0);
	}
	Perso(Environnement* env, int type){
		init(env, 0,0,0.5f,type);
	}
	void move(int dx, int dz){
		int nx = ((int)x) + dx;
		int nz = ((int)z) + dz;
		if(env->estLibre(nx,nz)){
			x += dx;
			z += dz;
		}
	}
	void ia(){
		if(type == 0){
			static int a = 0;
			if((a % 2) == 0){
				r = 1.0f;
				Mix_PlayChannel(-1, gsound, 0);
			}
			else
				r = 0.75f;
			a++;
		}
	}
	void draw(GLuint pId){
		const GLfloat color[2][4] = {{1,0,0,1},{1,1,0.1}};
		glUniform4fv(glGetUniformLocation(pId, "scolor"), 1, color[type]);
		gl4duPushMatrix();
		gl4duTranslatef(x, r, z);
		gl4duScalef(r,r,r);
		gl4duSendMatrices();
		gl4duPopMatrix();
		/* dessin d'un cube */
		gl4dgDraw(type == 0 ? cone: sphere);
	}
};
#endif
