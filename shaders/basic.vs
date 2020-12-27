/*!\file basic.vs
 *
 * \brief vertex shader, point d'entrée dans le pipeline GPU. C'est
 * ici que sont traités les sommets (transformations spaciales, ajouts
 * d'attributs ...) afin de produire les primitives puis les
 * fragments.
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date March 11, 2018
 */

/* Version GLSL 3.30 */
#version 330

/* Le Vertex Shader est directement en relatrion avec le code-CPU,
 * dans le code GL4Dummies, les géométries générées représentent
 * souvent une coordonnée spaciale 3D (vec3), un vecteur normal (vec3)
 * et une cooronnée de texture (vec2). Ces trois éléments sont mappés
 * avec les 3 déclaration de variable qui suivent. */

/* variable cablée sur l'attribut 0 du sommet dans le code CPU (dans
 * gl4dg.c, dans l'appel glVertexAttribPointer(0, ...), le 0
 * correspond au location = 0) */
layout (location = 0) in vec3 vsiPosition;
/* variable cablée sur l'attribut 1 du sommet dans le code CPU (dans
 * gl4dg.c, dans l'appel glVertexAttribPointer(1, ...), le 1
 * correspond au location = 1) */
layout (location = 1) in vec3 vsiNormal;
/* variable cablée sur l'attribut 2 du sommet dans le code CPU (dans
 * gl4dg.c, dans l'appel glVertexAttribPointer(2, ...), le 2
 * correspond au location = 2) */
layout (location = 2) in vec2 vsiTexCoord;

uniform mat4 projMat, mvMat;

out vec3 vsoNormal;
out vec4 vsoMPosition;

void main(void) {
  vsoNormal = normalize((transpose(inverse(mvMat)) * vec4(vsiNormal, 0.0f)).xyz);
  vsoMPosition = mvMat * vec4(vsiPosition, 1.0f);
  gl_Position = projMat * vsoMPosition;
}
