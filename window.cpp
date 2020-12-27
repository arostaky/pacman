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

/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId = 0;

static Environnement * _plateau = NULL;
static Perso * _pacman = NULL;

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
  _plateau = new Environnement(45, 25);
  // créer pacman:
  _pacman = new Perso(1);
  /* Création du programme shader (voir le dossier shader) */
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  /* Set de la couleur (RGBA) d'effacement OpenGL */
  glClearColor(1.2f, 0.2f, 0.2f, 1.0f);
  /* dans quelle partie de l'écran on effectue le rendu */
  glViewport(0, 0, _wW, _wH);

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
  GLfloat bleu[] = { 0, 0, 1, 1 }, vert[] = { 0, 1, 0, 1 };
  GLfloat lump0[] = { 5, 60, 10, 1 }, lump[4];
  GLfloat* mat;
  static GLfloat a = 0;
  lump0[1] -= a;
  a += 0.05f;
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
  
  _plateau->draw(_pId, vert, bleu);
  // creer pacman:
  _pacman->draw(_pId);
  /* désactiver le programme shader */
  glUseProgram(0);
}
/*!\brief appelée au moment de sortir du programme (atexit), elle
 *  libère les éléments OpenGL utilisés.*/
static void quit(void) {
  delete _plateau;
  delete _pacman;
  /* nettoyage des éléments utilisés par la bibliothèque GL4Dummies */
  gl4duClean(GL4DU_ALL);
}
