#ifndef _ALGEBRA_H_
#define _ALGEBRA_H_

typedef struct { float x, y, z; } Vector;
typedef struct { float x, y, z, w; } HomVector;

/* Column-major order are used for the matrices here to be compatible with OpenGL.
** The indices used to access elements in the matrices are shown below.
**  _                _
** |                  |
** |   0   4   8  12  |
** |                  |
** |   1   5   9  13  |
** |                  |
** |   2   6  10  14  |
** |                  |
** |   3   7  11  15  |
** |_                _|
*/
typedef struct matrix { float e[16]; } Matrix;


Vector Add(Vector a, Vector b);
Vector Subtract(Vector a, Vector b);
Vector CrossProduct(Vector a, Vector b);
float DotProduct(Vector a, Vector b);
float Length(Vector a);
Vector Normalize(Vector a);
Vector ScalarVecMul(float t, Vector a);
HomVector MatVecMul(Matrix a, Vector b);
Vector Homogenize(HomVector a);
Matrix MatMatMul(Matrix a, Matrix b);
void PrintMatrix(char *name, Matrix m);
void PrintVector(char *name, Vector v);
void PrintHomVector(char *name, HomVector h);

void Transformations(Matrix& V, float translationX, float translationY, float translationZ, float rotationX, float rotationY, float rotationZ);

void PerspectiveProjection(Matrix &P, double fov, int screen_width, int screen_height, double nearPlane,
                           double farPlane);

void OrtoProjection(Matrix &P, Vector* vertices, int nv, double nearPlane, double farPlane);

void ModelTransforamtions(Matrix &W,double x, double y, double z, double rotationX, double rotationY,
                            double rotationZ, double ScaleX,double ScaleY,double ScaleZ);

void ScaleModel(Matrix &S,double x, double y, double z);

void RotationOfLight(Vector& V, float rotationX, float rotationY, float rotationZ);

void SetLightPositon(Vector& pos, float x, float y, float z);
#endif

