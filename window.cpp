/*!\file window.c
 *
 * \brief premier exemple de code GL4Dummies/OpenGL avec création et
 * utilisation d'un shader sur une géométrie de quadrilatère. Le
 * quadrilatère subit un changement d'échelle variable utilisant une
 * sinusoïdale.
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date March 11, 2018
 */
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include "makeLabyrinth.h"
#include "Objets.h"
 /* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void draw(void);
static void quit(void);
/*!\brief largeur et hauteur de la fenêtre */
static int _wW = 800, _wH = 600;
/*!\brief identifiant du quadrilatère faisant office de sol pour le plateau */
static GLuint _quad = 0;
/*!\brief identifiant du cube servant à modéliser des murs */
static GLuint _cube = 0;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId = 0;
/*!\brief pointeur vers le plateau */
static GLuint* _plateau = NULL;
/*!\brief largeur du plateau (doit être impair) */
static int _pW = 35;
/*!\brief hauteur du plateau (doit être impair) */
static int _pH = 25;
/*!\brief créé la fenêtre d'affichage, initialise GL et les données,
 * affecte les fonctions d'événements et lance la boucle principale
 * d'affichage. */
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 20, 20, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données. 
 */
static void init(void) {
  /* Activer le test de profondeur */
  glEnable(GL_DEPTH_TEST);
  /* générer le plateau */
  _plateau = labyrinth(_pW, _pH);
  /* Création du programme shader (voir le dossier shader) */
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  /* Set de la couleur (RGBA) d'effacement OpenGL */
  glClearColor(1.2f, 0.2f, 0.2f, 1.0f);
  /* dans quelle partie de l'écran on effectue le rendu */
  glViewport(0, 0, _wW, _wH);
  /* génération de la géométrie du quadrilatère */
  _quad = gl4dgGenQuadf();
  /* génération de la géométrie du cube */
  _cube = gl4dgGenCubef();
  gl4duGenMatrix(GL_FLOAT, "projMat");
  gl4duGenMatrix(GL_FLOAT, "mvMat");

  gl4duBindMatrix("projMat");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5f, 0.5f, -0.5f * _wH / _wW, 0.5f * _wH / _wW, 1.0f, 1000.0f);
  gl4duBindMatrix("mvMat");
  gl4duLoadIdentityf();
}
/*!\brief Cette fonction dessine dans le contexte OpenGL actif. */
static void draw(void) {
  const GLfloat bleu[] = { 0, 0, 1, 1 };
  const GLfloat vert[] = { 0, 1, 0, 1 };
  GLfloat lump0[] = { 5, 60, 10, 1 }, lump[4];
  GLfloat* mat;
  int i, j;
  gl4duBindMatrix("mvMat");
  gl4duLoadIdentityf();
  gl4duLookAtf(0, 50, 10, 0, 0, 0, 0, 1, 0);
  /* récupérer la matrice pour modéliser la lumière dans le repère lookAt */
  mat = (GLfloat *)gl4duGetMatrixData();
  MMAT4XVEC4(lump, mat, lump0);
  MVEC4WEIGHT(lump); 
  /* effacement du buffer de couleur */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* activation du programme de rendu _pId */
  glUseProgram(_pId);
  glUniform3fv(glGetUniformLocation(_pId, "Lp"), 1, lump);
  /* placer le sol */
  gl4duPushMatrix();
  gl4duRotatef(-90, 1, 0, 0);
  gl4duScalef(_pW * 0.5f, _pH * 0.5f, 1);
  gl4duSendMatrices();
  gl4duPopMatrix();
  /* dessin de la géométrie du sol */
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, vert);
  gl4dgDraw(_quad);
  /* mettre du bleu pour les cubes "murs" */
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, bleu);
  /* placer des cubes là où il y a des murs */
  for (i = -_pH / 2; i <= _pH / 2; ++i) {
	  for (j = -_pW / 2; j <= _pW / 2; ++j) {
		  if (_plateau[(i + _pH / 2) * _pW + j + _pW / 2] == 0) 
			  continue;
		  gl4duPushMatrix();
		  gl4duTranslatef(j, 0.5f, i);
		  gl4duScalef(0.5f, 0.5f, 0.5f);
		  gl4duSendMatrices();
		  gl4duPopMatrix();
		  /* dessin d'un cube */
		  gl4dgDraw(_cube);
	  }
  }
  /* désactiver le programme shader */
  glUseProgram(0);
}
/*!\brief appelée au moment de sortir du programme (atexit), elle
 *  libère les éléments OpenGL utilisés.*/
static void quit(void) {
  /* nettoyage des éléments utilisés par la bibliothèque GL4Dummies */
  gl4duClean(GL4DU_ALL);
}
