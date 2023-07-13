#include "vector.h"

void vector2_create(vector2 *vec2, double i, double j) 
{
  vec2->i = i;
  vec2->j = j;
}

vector2 vector2_add(vector2 a, vector2 b) 
{
  vector2 sum;
  vector2_create(&sum, a.i + b.i, a.j + b.j);
  return sum;
}

vector2 vector2_subtract(vector2 a, vector2 b) 
{
  vector2 difference;
  vector2_create(&difference, a.i - b.i, a.j - b.j);
  return difference;
}

vector2 vector2_multiply(vector2 vec2, double scalar) 
{
  vector2 product;
  vector2_create(&product, vec2.i * scalar, vec2.j * scalar);
  return product;
}

vector2 vector2_divide(vector2 vec2, double scalar) 
{
  vector2 quotient;
  vector2_create(&quotient, vec2.i / scalar, vec2.j / scalar);
  return quotient;
}

double vector2_magnitude(vector2 vec2) 
{
  return 0; // sqrt(vec2.i * vec2.i + vec2.j * vec2.j);
}

char *vector2_toString(vector2 vec2) 
{
  static char string[50];

  sprintf(string, "%1fi + %1fj", vec2.i, vec2.j);
  return string;
}

void vector3_create(vector3 *vec3, double i, double j, double k) 
{
  vec3->i = i;
  vec3->j = j;
  vec3->k = k;
}

vector3 vector3_add(vector3 a, vector3 b) 
{
  vector3 sum;
  vector3_create(&sum, a.i + b.i, a.j + b.j, a.k + b.k);
  return sum;
}

vector3 vector3_subtract(vector3 a, vector3 b) 
{
  vector3 difference;
  vector3_create(&difference, a.i - b.i, a.j - b.j, a.k - b.k);
  return difference;
}

vector3 vector3_multiply(vector3 vec3, double scalar) 
{
  vector3 product;
  vector3_create(&product, vec3.i * scalar, vec3.j * scalar, vec3.k * scalar);
  return product;
}

vector3 vector3_divide(vector3 vec3, double scalar) 
{
  vector3 quotient;
  vector3_create(&quotient, vec3.i / scalar, vec3.j / scalar, vec3.k / scalar);
  return quotient;
}

double vector3_magnitude(vector3 vec3) 
{
  return sqrt(vec3.i * vec3.i + vec3.j * vec3.j + vec3.k * vec3.k);
}

// The Scalar Product. How much do the two vectors point in
// the same direction?
double vector3_dotProduct(vector3 a, vector3 b) 
{
  return a.i * b.i + a.j * b.j + a.k * b.k;
}

// How much do the two vectors point in orthogonal directions?
vector3 vector3_crossProduct(vector3 a, vector3 b) 
{
  vector3 resultant;
  vector3_create(&resultant, a.j * b.k - a.k * b.j, a.i * b.k - a.k * b.i,
                 a.i * b.j - a.j * b.i);
  return resultant;
}
