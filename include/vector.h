#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * This vector has two components i and j.
 * Is generally used for two dimensional measurements.
 */
typedef struct 
{
  double i;
  double j;
} vector2;

void vector2_create(vector2 *vec2, double i, double j);
vector2 vector2_add(vector2 a, vector2 b);
vector2 vector2_subtract(vector2 a, vector2 b);
vector2 vector2_multiply(vector2 vec2, double scalar);
vector2 vector2_divide(vector2 vec2, double scalar);
double vector2_magnitude(vector2 vec2);
char *vector2_toString(vector2 vec2);

/*
 * This type of vector has three components i, j, and k.
 * A vector is used for finding distance with directions.
 */
typedef struct 
{
  double i;
  double j;
  double k;
} vector3;

void vector3_create(vector3 *vec3, double i, double j, double k);
vector3 vector3_add(vector3 a, vector3 b);
vector3 vector3_subtract(vector3 a, vector3 b);
vector3 vector3_multiply(vector3 vec3, double scalar);
vector3 vector3_divide(vector3 vec3, double scalar);
double vector3_magnitude(vector3 vec3);
// The Scalar Product. How much do the two vectors point in
// the same direction?
double vector3_dotProduct(vector3 a, vector3 b);
// How much do the two vectors point in orthogonal directions?
vector3 vector3_crossProduct(vector3 a, vector3 b);

#endif
