/*!\file basic.fs
 *
 * \brief fragment shader basique qui applique une couleur à chaque
 * fragment de l'image.
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date March 11, 2018
 */
/* Version GLSL 3.30 */
#version 330
/* sortie du frament shader : une couleur */
out vec4 fragColor;

in vec3 vsoNormal;
in vec4 vsoMPosition; 


uniform vec4 scolor;
uniform vec3 Lp;


void main(void) {
  vec3 Ld = normalize(Lp - vsoMPosition.xyz);
  float li = clamp(dot(Ld, vsoNormal), 0.0f, 1.0f);
  /* mettre la couleur de sortie à color */
  fragColor = li * scolor;
}
