/*
 *  Key bindings:
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <stdbool.h>
#include <stdio.h>
//  Parameters
int th = 0;                 //  Azimuth of view angle
int ph = 30;                //  Elevation of view angle
int light = 1;              //  Lighting
double asp = 1;             //  Aspect ratio
double dim = 5;             //  Size of world
// Light Values
int emission = 0;           // Emission intensity (%)
int ambient = 30;           // Ambient intensity (%)
int diffuse = 50;           // Diffuse intensity (%)
int specular = 50;          // Specular intensity (%)
int shininess = 8;          // Shininess (power of two)
float shiny = 32;           // Shininess (value)
int zh = 0;                 // Light azimuth
float ylight = 9;           // Elevation of light
// Colors
float white[] = { 1,1,1,1};
float green[] = { 0,0.5,0,1 };
float black[] = { 0.29,0.29,0.29,1 };
float darkgreen[] = { 0.133, 0.545, 0.133 };
float bronze[] = { 0.690, 0.553, 0.314 };
// Textures
unsigned int texture[4];        // Game textures
int ntex = 0;                   // Selected texture
unsigned int letterNumber[16];  // Textures for letters and numbers
// Other
char moveTracker[6] = "__, __";
bool gotFromCol = false;
bool gotFromRow = false;
bool gotToCol = false;
bool gotToRow = false;
int takenWhitePieceCount = 0;
int takenBlackPieceCount = 0;
////  Motion params
//float time = 0.0;              //  Instantiate the time counter outside the loop
//float vertexHeight = 1.0;   //  Height of the vertex for the parabola
//float velocity = 0.01;      //  Increment for the piece motion
//float x = 0;
//float y = 0;
//float z = 0;
//bool takenPieceMoving = false;
//bool pieceMoving = false;
//bool timeToMove = false;

/*
*   Define structs
*/
//  2D vector
typedef struct { float x, y; } vec2;
//  3D vector
typedef struct { float x, y, z; } vec3;
//  Piece
typedef struct { char type, color; } piece;

piece takenPiece;
piece movedPiece;
/*
*   Vertex coordinates for piece shapes
*/
//  Pawn (Copied from class example but edited shape of base)
#define NpPawn 56
vec2 PP[NpPawn] = {
    {0.235,0.000},
    {0.235,0.035},
    {0.230,0.040},
    {0.225,0.046},
    {0.230,0.052},
    {0.235,0.057},
    {0.235,0.092},
    {0.248,0.108},
    {0.257,0.125},
    {0.261,0.144},
    {0.259,0.169},
    {0.248,0.195},
    {0.233,0.210},
    {0.213,0.224},
    {0.191,0.243},
    {0.180,0.258},
    {0.172,0.276},
    {0.166,0.294},
    {0.156,0.302},
    {0.146,0.307},
    {0.131,0.311},
    {0.125,0.329},
    {0.115,0.369},
    {0.105,0.411},
    {0.099,0.465},
    {0.096,0.526},
    {0.096,0.578},
    {0.099,0.599},
    {0.104,0.606},
    // Bottom of ring
    {0.139,0.613},
    {0.174,0.621},
    {0.190,0.628},
    {0.197,0.638},
    {0.202,0.652},
    {0.200,0.667},
    {0.192,0.676},
    {0.180,0.680},
    {0.162,0.685},
    {0.146,0.690},
    {0.134,0.693},
    {0.123,0.696},
    {0.120,0.701},
    // Bottom of ball
    {0.130,0.713},
    {0.142,0.731},
    {0.153,0.750},
    {0.164,0.778},
    {0.172,0.814},
    {0.171,0.846},
    {0.164,0.880},
    {0.148,0.918},
    {0.120,0.951},
    {0.085,0.976},
    {0.053,0.991},
    {0.023,0.998},
    {0.000,1.000},
};
vec2 NP[NpPawn];

//  Bishop
#define NpBishop 64
vec2 PB[NpBishop] = {
    {0.235,0.000},
    {0.235,0.035},
    {0.230,0.040},
    {0.225,0.046},
    {0.230,0.052},
    {0.235,0.057},
    {0.235,0.092},
    {0.248,0.108},
    {0.257,0.125},
    {0.261,0.144},
    {0.259,0.169},
    {0.248,0.195},
    {0.233,0.210},
    {0.213,0.224},
    {0.191,0.243},
    {0.180,0.258},
    {0.172,0.276},
    {0.166,0.294},
    {0.156,0.302},
    // Bottom of shaft
    {0.146,0.307},
    {0.131,0.316},
    {0.125,0.339},
    {0.115,0.384},
    {0.105,0.431},
    {0.099,0.490},
    {0.096,0.556},
    {0.096,0.613},
    {0.099,0.639},
    {0.104,0.651},
    // First Ring
    {0.139,0.658},
    {0.174,0.666},
    {0.190,0.673},
    {0.197,0.683},
    {0.202,0.697},
    {0.200,0.712},
    {0.192,0.721},
    {0.180,0.725},
    {0.162,0.730},
    {0.146,0.735},
    {0.134,0.738},
    {0.104,0.741},
    // Second Ring
    {0.104,0.780},
    {0.139,0.787},
    {0.174,0.795},
    {0.190,0.802},
    {0.197,0.812},
    {0.202,0.826},
    {0.200,0.841},
    {0.192,0.850},
    {0.180,0.854},
    {0.162,0.859},
    {0.146,0.864},
    {0.134,0.867},
    {0.104,0.870},
    {0.104,0.910},
    // Top Cap
    {0.139,0.925},
    {0.174,0.933},
    {0.190,0.940},
    {0.197,0.950},
    {0.202,0.964},
    {0.010,1.240},
    {0.053,1.243},
    {0.053,1.247},
    {0.000,1.250}
};
vec2 NB[NpBishop];

//  Rook
#define NpRook 46
vec2 PR[NpRook] = {
    {0.235,0.000},
    {0.235,0.035},
    {0.230,0.040},
    {0.225,0.046},
    {0.230,0.052},
    {0.235,0.057},
    {0.235,0.092},
    {0.248,0.108},
    {0.257,0.125},
    {0.261,0.144},
    {0.259,0.169},
    {0.248,0.195},
    {0.233,0.210},
    {0.213,0.224},
    {0.191,0.243},
    {0.180,0.258},
    {0.172,0.276},
    {0.166,0.294},
    {0.156,0.302},
    // Bottom of shaft
    {0.146,0.307},
    {0.131,0.316},
    {0.125,0.339},
    {0.115,0.384},
    {0.105,0.431},
    {0.099,0.490},
    {0.096,0.556},
    {0.096,0.613},
    {0.099,0.639},
    {0.104,0.651},
    // First Ring
    {0.139,0.658},
    {0.174,0.666},
    {0.190,0.673},
    {0.197,0.683},
    {0.202,0.697},
    {0.200,0.712},
    {0.192,0.721},
    {0.180,0.725},
    {0.162,0.730},
    {0.146,0.735},
    {0.134,0.738},
    {0.104,0.741},
    {0.104,0.780},
    {0.202,0.835},
    {0.202,0.945},
    {0.100,0.998},
    {0.000,1.000}    
};
vec2 NR[NpRook];

//  Knight Base
#define NpKnightBase 21
vec2 PKnightBase[NpKnightBase] =
{
    {0.235,0.000},
    {0.235,0.035},
    {0.230,0.040},
    {0.225,0.046},
    {0.230,0.052},
    {0.235,0.057},
    {0.235,0.092},
    {0.248,0.108},
    {0.257,0.125},
    {0.261,0.144},
    {0.259,0.169},
    {0.248,0.195},
    {0.233,0.210},
    {0.213,0.224},
    {0.191,0.243},
    {0.180,0.258},
    {0.172,0.276},
    {0.166,0.294},
    {0.156,0.302},
    {0.146,0.307},
    {0.000, 0.307}   
};
vec2 NKnightBase[NpKnightBase];

//  Knight Head
#define NpKnightHead 64
vec2 PKnightHead[NpKnightHead] =
{
    {0.130, 0.295},
    {0.125, 0.338},
    {0.120, 0.368},
    {0.115, 0.399},
    {0.120,	0.430},
    {0.125,	0.461},
    {0.130,	0.490},
    {0.130,	0.520},
    {0.120,	0.560},
    {0.110,	0.580},
    {0.100,	0.610},
    {0.090,	0.640},
    {0.080,	0.660},
    {0.060,	0.690},
    {0.030,	0.720},
    {0.010,	0.750},
    {0.000,	0.790},
    {-0.010, 0.810},
    // Pivot 2
    {-0.020, 0.880},
    {0.000,	0.880},
    {0.020,	0.870},
    {0.040,	0.860},
    {0.060,	0.850},
    {0.080,	0.840},
    {0.110,	0.830},
    {0.130,	0.820},
    {0.160,	0.830},
    {0.180,	0.850},
    {0.190,	0.880},
    {0.180,	0.910},
    {0.160,	0.940},
    {0.150,	0.960},
    {0.130,	0.980},
    {0.110,	1.000},
    {0.090,	1.040},
    {0.080,	1.060},
    {0.070,	1.080},
    {0.030,	1.090},
    {0.010,	1.095},
    {0.000,	1.100},
    {-0.090, 1.100},
    {-0.110, 1.090},
    {-0.120, 1.060},
    {-0.140, 1.030},
    {-0.150, 1.000},
    {-0.160, 0.970},
    {-0.170, 0.930},
    {-0.180, 0.890},
    {-0.190, 0.820},
    {-0.200, 0.730},
    {-0.190, 0.660},
    {-0.185, 0.620},
    {-0.180, 0.550},
    {-0.175, 0.520},
    {-0.170, 0.510},
    {-0.165, 0.500},
    {-0.160, 0.490},
    {-0.155, 0.461},
    {-0.150, 0.430},
    {-0.145, 0.399},
    {-0.140, 0.368},
    {-0.135, 0.338},
    // pivot 1
    {-0.130, 0.295},
    {0.000, 0.295}
};

//  King and Queen
#define NpQK 57
vec2 PQK[NpQK] =
{
    {0.235,0.000},
    {0.235,0.035},
    {0.230,0.040},
    {0.225,0.046},
    {0.230,0.052},
    {0.235,0.057},
    {0.235,0.092},
    {0.248,0.108},
    {0.257,0.125},
    {0.261,0.144},
    {0.259,0.169},
    {0.248,0.195},
    {0.233,0.210},
    {0.213,0.224},
    {0.191,0.243},
    {0.180,0.258},
    {0.172,0.276},
    {0.166,0.294},
    {0.156,0.302},
    // Bottom of shaft
    {0.146,0.307},
    {0.131,0.316},
    {0.125,0.339},
    {0.115,0.384},
    {0.105,0.431},
    {0.099,0.490},
    {0.096,0.556},
    {0.096,0.613},
    {0.099,0.639},
    {0.104,0.651},
    // First Ring
    {0.139,0.658},
    {0.174,0.666},
    {0.190,0.673},
    {0.197,0.683},
    {0.202,0.697},
    {0.200,0.712},
    {0.192,0.721},
    {0.180,0.725},
    {0.162,0.730},
    {0.146,0.735},
    {0.134,0.738},
    {0.104,0.741},
    // Second Ring
    {0.104,0.780},
    {0.139,0.787},
    {0.174,0.795},
    {0.190,0.802},
    {0.197,0.812},
    {0.202,0.826},
    {0.200,0.841},
    {0.192,0.850},
    {0.180,0.854},
    {0.162,0.859},
    {0.146,0.864},
    {0.134,0.867},
    {0.104,0.870},
    {0.104,0.910},
    // Top Cap
    {0.202, 1.200},
    {0,1.300}
};
vec2 NQK[NpQK];

/*
*   Vectors and arrays for tracking piece location and draw coordinates
*/
#define BoardSize 8
//  Coordinates for pieces on board
vec3 PBoard[BoardSize][BoardSize] =
{
    {{-1.75,0, 1.75},{-1.75,0, 1.25},{-1.75,0, 0.75},{-1.75,0, 0.25},{-1.75,0, -0.25},{-1.75,0, -0.75},{-1.75,0, -1.25},{-1.75,0, -1.75}},
    {{-1.25,0, 1.75},{-1.25,0, 1.25},{-1.25,0, 0.75},{-1.25,0, 0.25},{-1.25,0, -0.25},{-1.25,0, -0.75},{-1.25,0, -1.25},{-1.25,0, -1.75}},
    {{-0.75,0, 1.75},{-0.75,0, 1.25},{-0.75,0, 0.75},{-0.75,0, 0.25},{-0.75,0, -0.25},{-0.75,0, -0.75},{-0.75,0, -1.25},{-0.75,0, -1.75}},
    {{-0.25,0, 1.75},{-0.25,0, 1.25},{-0.25,0, 0.75},{-0.25,0, 0.25},{-0.25,0, -0.25},{-0.25,0, -0.75},{-0.25,0, -1.25},{-0.25,0, -1.75}},
    {{0.25,0, 1.75},{0.25,0, 1.25},{0.25,0, 0.75},{0.25,0, 0.25},{0.25,0, -0.25},{0.25,0, -0.75},{0.25,0, -1.25},{0.25,0, -1.75}},
    {{0.75,0, 1.75},{0.75,0, 1.25},{0.75,0, 0.75},{0.75,0, 0.25},{0.75,0, -0.25},{0.75,0, -0.75},{0.75,0, -1.25},{0.75,0, -1.75}},
    {{1.25,0, 1.75},{1.25,0, 1.25},{1.25,0, 0.75},{1.25,0, 0.25},{1.25,0, -0.25},{1.25,0, -0.75},{1.25,0, -1.25},{1.25,0, -1.75}},
    {{1.75,0, 1.75},{1.75,0, 1.25},{1.75,0, 0.75},{1.75,0, 0.25},{1.75,0, -0.25},{1.75,0, -0.75},{1.75,0, -1.25},{1.75,0, -1.75}}
};

//  Location of pieces on board
//  type:  R = Rook, K = Knight, B = Bishop, Q = Queen, X = King, P = Pawn
//  color:  W = White, B = Black
//  N = None (empty square)
piece GamePiece[BoardSize][BoardSize] =
{
    {{'R', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'R', 'B'}},
    {{'K', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'K', 'B'}},
    {{'B', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'B', 'B'}},
    {{'Q', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'Q', 'B'}},
    {{'X', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'X', 'B'}},
    {{'B', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'B', 'B'}},
    {{'K', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'K', 'B'}},
    {{'R', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'R', 'B'}}
};
piece GamePieceReset[BoardSize][BoardSize] =
{
    {{'R', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'R', 'B'}},
    {{'K', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'K', 'B'}},
    {{'B', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'B', 'B'}},
    {{'Q', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'Q', 'B'}},
    {{'X', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'X', 'B'}},
    {{'B', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'B', 'B'}},
    {{'K', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'K', 'B'}},
    {{'R', 'W'},{'P', 'W'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'P', 'B'},{'R', 'B'}}
};

//  Coordinates for taken black pieces
vec3 takenBlackCoord[16] =
{
    {-2.75, -0.25, 1.75},{-2.75, -0.25, 1.25},{-2.75, -0.25, 0.75},{-2.75, -0.25, 0.25},{-2.75, -0.25, -0.25},{-2.75, -0.25, -0.75},
    {-2.75, -0.25, -1.25},{-2.75, -0.25, -1.75},{-3.25, -0.25, 1.75},{-3.25, -0.25, 1.25},{-3.25, -0.25, 0.75},{-3.25, -0.25, 0.25},
    {-3.25, -0.25, -0.25},{-3.25, -0.25, -0.75},{-3.25, -0.25, -1.25},{-3.25, -0.25, -1.75}
};

//  Coordinates for taken white pieces
vec3 takenWhiteCoord[16] =
{
    {2.75, -0.25, 1.75},{2.75, -0.25, 1.25},{2.75, -0.25, 0.75},{2.75, -0.25, 0.25},{2.75, -0.25, -0.25},{2.75, -0.25, -0.75},
    {2.75, -0.25, -1.25},{2.75, -0.25, -1.75},{3.25, -0.25, 1.75},{3.25, -0.25, 1.25},{3.25, -0.25, 0.75},{3.25, -0.25, 0.25},
    {3.25, -0.25, -0.25},{3.25, -0.25, -0.75},{3.25, -0.25, -1.25},{3.25, -0.25, -1.75}
};
//  Location for taken black pieces
piece takenBlackPieces[16] =
{
    {'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},
    {'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'}
};

//  Location for taken white pieces
piece takenWhitePieces[16] =
{
    {'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},
    {'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'},{'N', 'N'}
};

/*
*   Copied from class examples
*   normalizes vector to unit length
*/
vec2 normalize(float x, float y)
{
    float len = sqrt(x * x + y * y);
    if (len > 0)
    {
        x /= len;
        y /= len;
    }
    return (vec2) { x, y };
}

/*
*   Finds the normal vector for a quad
*   Used for all other shapes than round piece bodies
*/
vec3 QuadNormal(vec3 A, vec3 B, vec3 C, vec3 D)
{
    float x1 = A.x - C.x;
    float y1 = A.y - C.y;
    float z1 = A.z - C.z;
    float x2 = B.x - D.x;
    float y2 = B.y - D.y;
    float z2 = B.z - D.z;
    vec3 N =
    {
        ((y1 * z2) - (z1 * y2)) * -1,
        ((x1 * z2) - (z1 * x2)) * -1,
        ((x1 * y2) - (y1 * x2)) * -1
    };
    return N;
}

/*
*   Copied from class examples
*   Finds the normal vector for a series of points
*   Used for the body of round pieces
*/
void CalcNorm(vec2 N[], vec2 P[], int Np)
{
    for (int i = 0; i < Np - 2; i++)
    {
        float dx = P[i + 1].x - P[i].x;
        float dy = P[i + 1].y - P[i].y;
        N[i] = normalize(dy, -dx);
    }
    N[Np - 1] = (vec2){ 0,1 };
    vec2 N2 = N[0];
    for (int i = 1; i < Np - 2; i++)
    {
        vec2 N1 = N2;
        N2 = N[i];
        N[i] = normalize(N1.x + N2.x, N1.y + N2.y);
    }
}

/*
 *  Copied from clas example and edited
 *  Draw the pawn
 */
static void Pawn(double x, double y, double z, double s, char isWhite)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PP[0].x, 0, Sin(th) * PP[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the pawn
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpPawn - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PP[i].y);
            glNormal3f(c * NP[i].x, NP[i].y, s * NP[i].x);
            glVertex3f(c * PP[i].x, PP[i].y, s * PP[i].x);

            glTexCoord2f(th / 360.0, PP[i + 1].y);
            glNormal3f(c * NP[i + 1].x, NP[i + 1].y, s * NP[i + 1].x);
            glVertex3f(c * PP[i + 1].x, PP[i + 1].y, s * PP[i + 1].x);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
 *  Draw the bishop
 */
static void Bishop(double x, double y, double z, double s, char isWhite)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PB[0].x, 0, Sin(th) * PB[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the bishop
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpBishop - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PB[i].y);
            glNormal3f(c * NB[i].x, NB[i].y, s * NB[i].x);
            glVertex3f(c * PB[i].x, PB[i].y, s * PB[i].x);

            glTexCoord2f(th / 360.0, PB[i + 1].y);
            glNormal3f(c * NB[i + 1].x, NB[i + 1].y, s * NB[i + 1].x);
            glVertex3f(c * PB[i + 1].x, PB[i + 1].y, s * PB[i + 1].x);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
 *  Draw the Rook
 */
static void Rook(double x, double y, double z, double s, char isWhite)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PR[0].x, 0, Sin(th) * PR[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the rook
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpRook - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PR[i].y);
            glNormal3f(c * NR[i].x, NR[i].y, s * NR[i].x);
            glVertex3f(c * PR[i].x, PR[i].y, s * PR[i].x);

            glTexCoord2f(th / 360.0, PB[i + 1].y);
            glNormal3f(c * NR[i + 1].x, NR[i + 1].y, s * NR[i + 1].x);
            glVertex3f(c * PR[i + 1].x, PR[i + 1].y, s * PR[i + 1].x);
        }
        glEnd();
    }
    //  Draw Top Caps for Rook
    for (int i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);
        int angle = 45 + 90 * i;
        //  Side 1
        vec3 A = { Cos(angle - 20) * 0.101, 0.945, Sin(angle - 20) * 0.101 };
        vec3 B = { Cos(angle - 20) * 0.202, 0.945, Sin(angle - 20) * 0.202 };
        vec3 C = { Cos(angle - 20) * 0.202, 1.050, Sin(angle - 20) * 0.202 };
        vec3 D = { Cos(angle - 20) * 0.101, 1.050, Sin(angle - 20) * 0.101 };
        vec3 N1 = QuadNormal(A, B, C, D);
        glTexCoord2f(th / 360.0, 1.050);
        glNormal3f(N1.x, N1.y, N1.z);
        glVertex3f(Cos(angle - 20) * 0.101, 0.945, Sin(angle - 20) * 0.101);    //  A
        glVertex3f(Cos(angle - 20) * 0.202, 0.945, Sin(angle - 20) * 0.202);    //  B
        glVertex3f(Cos(angle - 20) * 0.202, 1.050, Sin(angle - 20) * 0.202);    //  C
        glVertex3f(Cos(angle - 20) * 0.101, 1.050, Sin(angle - 20) * 0.101);    //  D
        // Side 2
        vec3 E = { Cos(angle + 20) * 0.101, 0.945, Sin(angle + 20) * 0.101 };
        vec3 F = { Cos(angle + 20) * 0.101, 1.050, Sin(angle + 20) * 0.101 };
        vec3 G = { Cos(angle + 20) * 0.202, 1.050, Sin(angle + 20) * 0.202 };
        vec3 H = { Cos(angle + 20) * 0.202, 0.945, Sin(angle + 20) * 0.202 };
        vec3 N2 = QuadNormal(E, F, G, H);
        glTexCoord2f(th / 360.0, 1.050);
        glNormal3f(N2.x, N2.y, N2.z);
        glVertex3f(Cos(angle + 20) * 0.101, 0.945, Sin(angle + 20) * 0.101);    //  E
        glVertex3f(Cos(angle + 20) * 0.101, 1.050, Sin(angle + 20) * 0.101);    //  F
        glVertex3f(Cos(angle + 20) * 0.202, 1.050, Sin(angle + 20) * 0.202);    //  G
        glVertex3f(Cos(angle + 20) * 0.202, 0.945, Sin(angle + 20) * 0.202);    //  H
        //  Top
        //glTexCoord2f(th / 360.0, 1.050);
        glNormal3f(0, 1, 0);
        for (int th = angle - 18; th <= angle + 18; th += 4)
        {
            glVertex3f(Cos(th - 2) * 0.101, 1.050, Sin(th - 2) * 0.101);
            glVertex3f(Cos(th - 2) * 0.202, 1.050, Sin(th - 2) * 0.202);
            glVertex3f(Cos(th + 2) * 0.202, 1.050, Sin(th + 2) * 0.202);
            glVertex3f(Cos(th + 2) * 0.101, 1.050, Sin(th + 2) * 0.101);
        }
        glEnd();
        //  Inside
        glBegin(GL_QUAD_STRIP);
        for (int th = angle - 20; th <= angle + 20; th += 4)
        {
            vec3 A = { Cos(th) * 0.101, 0.945, Sin(th) * 0.101 };
            vec3 B = { Cos(th) * 0.101, 1.050, Sin(th) * 0.101 };
            vec3 C = { Cos(th + 4) * 0.101, 1.050, Sin(th + 4) * 0.101 };
            vec3 D = { Cos(th + 4) * 0.101, 0.945, Sin(th + 4) * 0.101 };
            vec3 N1 = QuadNormal(A, B, C, D);
            glNormal3f(N1.x, N1.y, N1.z);
            glTexCoord2f(th / 360.0, 1.050);
            
            glVertex3f(Cos(th) * 0.101, 0.945, Sin(th) * 0.101);
            glVertex3f(Cos(th) * 0.101, 1.050, Sin(th) * 0.101);
        }
        glEnd();
        //  Outside
        glBegin(GL_QUAD_STRIP);
        for (int th = angle - 20; th <= angle + 20; th += 4)
        {
            vec3 A = { Cos(th) * 0.202, 0.945, Sin(th) * 0.202 };
            vec3 B = { Cos(th) * 0.202, 1.050, Sin(th) * 0.202 };
            vec3 C = { Cos(th + 4) * 0.202, 1.050, Sin(th + 4) * 0.202 };
            vec3 D = { Cos(th + 4) * 0.202, 0.945, Sin(th + 4) * 0.202 };
            vec3 N1 = QuadNormal(A, B, C, D);
            glNormal3f(-1 * N1.x, -1 * N1.y, -1 * N1.z);
            glTexCoord2f(th / 360.0, 1.050);
            glVertex3f(Cos(th) * 0.202, 0.945, Sin(th) * 0.202);
            glVertex3f(Cos(th) * 0.202, 1.050, Sin(th) * 0.202);
        }
        glEnd();

    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
*   Draw the Knight
*/
static void Knight(double x, double y, double z, double s, char isWhite, int isTaken)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    
	//	Orient based on color
    if (isWhite == 'W')
        glRotatef(180 + isTaken * 90, 0, 1, 0);
    else
        glRotatef(isTaken * 90, 0, 1, 0);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PKnightBase[0].x, 0, Sin(th) * PKnightBase[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the base
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpKnightBase - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PKnightBase[i].y);
            glNormal3f(c * NKnightBase[i].x, NKnightBase[i].y, s * NKnightBase[i].x);
            glVertex3f(c * PKnightBase[i].x, PKnightBase[i].y, s * PKnightBase[i].x);

            glTexCoord2f(th / 360.0, PKnightBase[i + 1].y);
            glNormal3f(c * NKnightBase[i + 1].x, NKnightBase[i + 1].y, s * NKnightBase[i + 1].x);
            glVertex3f(c * PKnightBase[i + 1].x, PKnightBase[i + 1].y, s * PKnightBase[i + 1].x);
        }
        glEnd();
    }
    //  Draw the head
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glNormal3f(1, 0, 0);
    glVertex3f(0.1, PKnightHead[NpKnightHead - 2].y, PKnightHead[NpKnightHead - 2].x);
    for (int i = 0; i < 19; i++)
        glVertex3f(0.1, PKnightHead[i].y, PKnightHead[i].x);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glNormal3f(1, 0, 0);
    glVertex3f(0.1, PKnightHead[18].y, PKnightHead[18].x);
    for (int i = 19; i < NpKnightHead - 1; i++)
        glVertex3f(0.1, PKnightHead[i].y, PKnightHead[i].x);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.1, PKnightHead[NpKnightHead - 2].y, PKnightHead[NpKnightHead - 2].x);
    for (int i = 0; i < 19; i++)
        glVertex3f(-0.1, PKnightHead[i].y, PKnightHead[i].x);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.1, PKnightHead[18].y, PKnightHead[18].x);
    for (int i = 19; i < NpKnightHead - 1; i++)
        glVertex3f(-0.1, PKnightHead[i].y, PKnightHead[i].x);
    glEnd();

    glBegin(GL_QUADS);
    for (int i = 0; i < NpKnightHead - 2; i++)
    {
        vec3 A = { -0.1, PKnightHead[i].y, PKnightHead[i].x };
        vec3 B = { -0.1, PKnightHead[i + 1].y, PKnightHead[i + 1].x };
        vec3 C = { 0.1, PKnightHead[i + 1].y, PKnightHead[i + 1].x };
        vec3 D = { 0.1, PKnightHead[i].y, PKnightHead[i].x };
        vec3 N = QuadNormal(A, B, C, D);
        glNormal3f(N.x, -N.y, N.z);
        glVertex3f(-0.1, PKnightHead[i].y, PKnightHead[i].x);
        glVertex3f(-0.1, PKnightHead[i + 1].y, PKnightHead[i + 1].x);
        glVertex3f(0.1, PKnightHead[i + 1].y, PKnightHead[i + 1].x);
        glVertex3f(0.1, PKnightHead[i].y, PKnightHead[i].x);
    }
    glEnd();
    //  Draw ears
    glBegin(GL_TRIANGLES);
    glNormal3f(1, 0, 0);
    glVertex3f(0.1, 1.100, -0.090);
    glVertex3f(0.1, 1.20, -0.075);
    glVertex3f(0.1, 1.100, -0.060);

    glNormal3f(-1, 0, 0);
    glVertex3f(-0.1, 1.100, -0.090);
    glVertex3f(-0.1, 1.20, -0.075);
    glVertex3f(-0.1, 1.100, -0.060);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    vec3 A = { 0.1, 1.20, -0.075 };
    glVertex3f(0.1, 1.20, -0.075);
    for (int th = 90; th <= 270; th += inc)
    {
        vec3 B = { (Cos(th) * 0.015) + 0.1, 1.100, (Sin(th) * 0.015) - 0.075 };
        vec3 C = { (Cos(th + inc) * 0.015) + 0.1, 1.100, (Sin(th + inc) * 0.015) - 0.075 };
        vec3 N = QuadNormal(A, B, C, A);
        glNormal3f(N.x, N.y, N.z);
        glVertex3f((Cos(th) * 0.015) + 0.1, 1.100, (Sin(th) * 0.015) - 0.075);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    vec3 E = { -0.1, 1.20, -0.075 };
    glVertex3f(-0.1, 1.20, -0.075);
    for (int th = 270; th >= 90; th -= inc)
    {
        vec3 B = { (Cos(th) * -0.015) - 0.1, 1.100, (Sin(th) * -0.015) - 0.075 };
        vec3 C = { (Cos(th - inc) * -0.015) - 0.1, 1.100, (Sin(th - inc) * -0.015) - 0.075 };
        vec3 N = QuadNormal(E, B, C, E);
        glNormal3f(-1 * N.x, -1 * N.y, -1 * N.z);
        glVertex3f((Cos(th) * -0.015) - 0.1, 1.100, (Sin(th) * -0.015) - 0.075);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
*   Draw the Queen
*/
static void Queen(double x, double y, double z, double s, char isWhite)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PQK[0].x, 0, Sin(th) * PQK[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the queen
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpQK - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PB[i].y);
            glNormal3f(c * NQK[i].x, NQK[i].y, s * NQK[i].x);
            glVertex3f(c * PQK[i].x, PQK[i].y, s * PQK[i].x);

            glTexCoord2f(th / 360.0, PB[i + 1].y);
            glNormal3f(c * NQK[i + 1].x, NQK[i + 1].y, s * NQK[i + 1].x);
            glVertex3f(c * PQK[i + 1].x, PQK[i + 1].y, s * PQK[i + 1].x);
        }
        glEnd();
    }
    // draw crown
    for (int th = 20; th <= 360; th += 40)
    {
        glBegin(GL_TRIANGLE_FAN);
        vec3 A = { Cos(th) * 0.23, 1.32, Sin(th) * 0.23 };
        glVertex3f(Cos(th) * 0.23, 1.32, Sin(th) * 0.23);
        for (int ph = 0; ph <= 360; ph += inc)
        {
            vec3 B = { Cos(ph) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph) * 0.020 + Sin(th) * 0.16 };
            vec3 C = { Cos(ph - inc) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph - inc) * 0.020 + Sin(th) * 0.16 };
            vec3 N = QuadNormal(A, B, C, A);
            glNormal3f(-1 * N.x, -1 * N.y, -1 * N.z);
            glVertex3f(Cos(ph) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph) * 0.020 + Sin(th) * 0.16);
        }
        glEnd();

    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
*   Draw the King
*/
static void King(double x, double y, double z, double s, char isWhite)
{
    int inc = 15;
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glColor4fv(white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Draw bottom
    glColor4fv(green);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int th = 0; th <= 360; th += inc)
        glVertex3f(Cos(th) * PQK[0].x, 0, Sin(th) * PQK[0].x);
    glEnd();

    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  Draw the King
    if (isWhite == 'W')
        glColor4fv(white);
    else
    {
        if (ntex == 3)
            glColor4fv(bronze);
        else
            glColor4fv(black);
    }
    for (int i = 0; i < NpQK - 1; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);
            glTexCoord2f(th / 360.0, PB[i].y);
            glNormal3f(c* NQK[i].x, NQK[i].y, s* NQK[i].x);
            glVertex3f(c* PQK[i].x, PQK[i].y, s* PQK[i].x);

            glTexCoord2f(th / 360.0, PB[i + 1].y);
            glNormal3f(c* NQK[i + 1].x, NQK[i + 1].y, s* NQK[i + 1].x);
            glVertex3f(c* PQK[i + 1].x, PQK[i + 1].y, s* PQK[i + 1].x);
        }
        glEnd();
    }

    // draw crown
    glBegin(GL_QUADS);
    for (int th = 0; th <= 360; th += 5)
    {

        vec3 A = { Cos(th) * 0.1, 1.2, Sin(th) * 0.1 };
        vec3 B = { Cos(th + 5) * 0.1, 1.2, Sin(th + 5) * 0.1 };
        vec3 C = { Cos(th + 5) * 0.1, 1.32, Sin(th + 5) * 0.1 };
        vec3 D = { Cos(th) * 0.1, 1.32, Sin(th) * 0.1 };
        vec3 N = QuadNormal(A, B, C, D);
        glNormal3f(N.x, N.y, N.z);
        glTexCoord2f(th / 360.0, 1.200);
        glVertex3f(Cos(th) * 0.1, 1.2, Sin(th) * 0.1);
        glVertex3f(Cos(th + 5) * 0.1, 1.2, Sin(th + 5) * 0.1);
        glVertex3f(Cos(th + 5) * 0.1, 1.32, Sin(th + 5) * 0.1);
        glVertex3f(Cos(th) * 0.1, 1.32, Sin(th) * 0.1);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 1.32, 0);
    for (int th = 0; th <= 360; th += 5)
    {
        glTexCoord2f(th, 1.32);
        glVertex3f(Cos(th) * 0.1, 1.32, Sin(th) * 0.1);
    }
    glEnd();

    for (int th = 20; th <= 360; th += 40)
    {
        glBegin(GL_TRIANGLE_FAN);
        vec3 A = { Cos(th) * 0.23, 1.32, Sin(th) * 0.23 };
        glVertex3f(Cos(th) * 0.23, 1.32, Sin(th) * 0.23);
        for (int ph = 0; ph <= 360; ph += inc)
        {
            vec3 B = { Cos(ph) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph) * 0.020 + Sin(th) * 0.16 };
            vec3 C = { Cos(ph - inc) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph - inc) * 0.020 + Sin(th) * 0.16 };
            vec3 N = QuadNormal(A, B, C, A);
            glNormal3f(-1 * N.x, -1 * N.y, -1 * N.z);
            glVertex3f(Cos(ph) * 0.020 + Cos(th) * 0.16, 1.150, Sin(ph) * 0.020 + Sin(th) * 0.16);
        }
        glEnd();

    }
    for (int th = 20; th <= 360; th += 40)
    {
        glBegin(GL_TRIANGLE_FAN);
        vec3 A = { Cos(th) * 0.12, 1.37, Sin(th) * 0.12 };
        glVertex3f(Cos(th) * 0.12, 1.37, Sin(th) * 0.12);
        for (int ph = 0; ph <= 360; ph += inc)
        {
            vec3 B = { Cos(ph) * 0.020 + Cos(th) * 0.08, 1.3, Sin(ph) * 0.020 + Sin(th) * 0.08 };
            vec3 C = { Cos(ph - inc) * 0.020 + Cos(th) * 0.08, 1.3, Sin(ph - inc) * 0.020 + Sin(th) * 0.08 };
            vec3 N = QuadNormal(A, B, C, A);
            glNormal3f(-1 * N.x, -1 * N.y, -1 * N.z);
            glVertex3f(Cos(ph) * 0.020 + Cos(th) * 0.08, 1.3, Sin(ph) * 0.020 + Sin(th) * 0.08);
        }
        glEnd();

    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
*   Draw the shape and textures for board indexes
*/
static void LettersAndNumbers(double x, double y, double z, double s)
{
    //  Set lighting params
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    //  Set matrix params
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4fv(black);
    for (int i = 0; i < 8; i++)
    {
        glBindTexture(GL_TEXTURE_2D, letterNumber[i]);
        float centerPoint = -1.75 + (i * 0.5);
        glBegin(GL_QUADS);

        // Draw Letter
        glTexCoord2f(0.0, 1);     glVertex3f(centerPoint - 0.125, 0.0001, 2.0);
        glTexCoord2f(1, 1);       glVertex3f(centerPoint + 0.125, 0.0001, 2.0);
        glTexCoord2f(1, 0.0);     glVertex3f(centerPoint + 0.125, 0.0001, 2.25);
        glTexCoord2f(0.0, 0.0);   glVertex3f(centerPoint - 0.125, 0.0001, 2.25);

        glTexCoord2f(1, 1);         glVertex3f(centerPoint - 0.125, 0.0001, -2.0);
        glTexCoord2f(0.0, 1);       glVertex3f(centerPoint + 0.125, 0.0001, -2.0);
        glTexCoord2f(0.0, 0.0);     glVertex3f(centerPoint + 0.125, 0.0001, -2.25);
        glTexCoord2f(1, 0.0);       glVertex3f(centerPoint - 0.125, 0.0001, -2.25);

        glEnd();
    }
    for (int i = 8; i < 16; i++)
    {
        glBindTexture(GL_TEXTURE_2D, letterNumber[i]);
        float centerPoint = 1.75 - ((i-8) * 0.5);
        glBegin(GL_QUADS);

        // Draw Letter
        glTexCoord2f(1, 1);         glVertex3f(2.0, 0.0001, centerPoint - 0.125);
        glTexCoord2f(0.0, 1);       glVertex3f(2.0, 0.0001, centerPoint + 0.125);
        glTexCoord2f(0.0, 0.0);     glVertex3f(2.25, 0.0001, centerPoint + 0.125);
        glTexCoord2f(1, 0.0);       glVertex3f(2.25, 0.0001, centerPoint - 0.125);

        glTexCoord2f(0.0, 1);       glVertex3f(-2.0, 0.0001, centerPoint - 0.125);
        glTexCoord2f(1, 1);         glVertex3f(-2.0, 0.0001, centerPoint + 0.125);
        glTexCoord2f(1, 0.0);       glVertex3f(-2.25, 0.0001, centerPoint + 0.125);
        glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0.0001, centerPoint - 0.125);

        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
*   Draw the Board
*/
static void Board(double x, double y, double z, double s)
{
    //  Set lighting params
    float Emission[] = { 0.0,0.0,0.01 * emission,1.0 };
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
    //  Set matrix params
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(s, s, s);
    //  Set texture params
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[ntex]);

    //  draw Squares
    glBegin(GL_QUADS);
    int isBlack = 1;
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (isBlack == 1)
            {
                if (ntex == 3)
                    glColor4fv(bronze);
                else
                    glColor4fv(black);
            }
            else
            { 
                glColor4fv(white); 
            }
            glNormal3f(0, 1, 0);
            glTexCoord2f(0.0, 0.0); glVertex3f(PBoard[j][i].x + 0.25, PBoard[j][i].y, PBoard[j][i].z + 0.25);
            glTexCoord2f(1, 0.0);   glVertex3f(PBoard[j][i].x + 0.25, PBoard[j][i].y, PBoard[j][i].z - 0.25);
            glTexCoord2f(1, 1);     glVertex3f(PBoard[j][i].x - 0.25, PBoard[j][i].y, PBoard[j][i].z - 0.25);
            glTexCoord2f(0.0, 1);   glVertex3f(PBoard[j][i].x - 0.25, PBoard[j][i].y, PBoard[j][i].z + 0.25);



            isBlack = 1 - isBlack;
        }
        isBlack = 1 - isBlack;
    }
    // draw boarder
    glColor4fv(black);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, 2.0);
    glTexCoord2f(1, 0.0);       glVertex3f(2.25, 0, 2.0);
    glTexCoord2f(1, 1);         glVertex3f(2.25, 0, 2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(-2.25, 0, 2.25);

    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, -2.0);
    glTexCoord2f(1, 0.0);       glVertex3f(2.25, 0, -2.0);
    glTexCoord2f(1, 1);         glVertex3f(2.25, 0, -2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(-2.25, 0, -2.25);

    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, 2.0);
    glTexCoord2f(1, 0.0);       glVertex3f(-2.0, 0, 2.0);
    glTexCoord2f(1, 1);         glVertex3f(-2.0, 0, -2.0);
    glTexCoord2f(0.0, 1);       glVertex3f(-2.25, 0, -2.0);

    glTexCoord2f(0.0, 0.0);     glVertex3f(2.25, 0, 2.0);
    glTexCoord2f(1, 0.0);       glVertex3f(2.0, 0, 2.0);
    glTexCoord2f(1, 1);         glVertex3f(2.0, 0, -2.0);
    glTexCoord2f(0.0, 1);       glVertex3f(2.25, 0, -2.0);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, 2.25);
    glTexCoord2f(1, 0.0);       glVertex3f(-2.25, -0.25, 2.25);
    glTexCoord2f(1, 1);         glVertex3f(2.25, -0.25, 2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(2.25, 0, 2.25);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0, 0.0);     glVertex3f(2.25, 0, 2.25);
    glTexCoord2f(1, 0.0);       glVertex3f(2.25, -0.25, 2.25);
    glTexCoord2f(1, 1);         glVertex3f(2.25, -0.25, -2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(2.25, 0, -2.25);

    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, -2.25);
    glTexCoord2f(1, 0.0);       glVertex3f(-2.25, -0.25, -2.25);
    glTexCoord2f(1, 1);         glVertex3f(2.25, -0.25, -2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(2.25, 0, -2.25);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, 0, 2.25);
    glTexCoord2f(1, 0.0);       glVertex3f(-2.25, -0.25, 2.25);
    glTexCoord2f(1, 1);         glVertex3f(-2.25, -0.25, -2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(-2.25, 0, -2.25);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0, 0.0);     glVertex3f(-2.25, -0.25, 2.25);
    glTexCoord2f(1, 0.0);       glVertex3f(-2.25, -0.25, -2.25);
    glTexCoord2f(1, 1);         glVertex3f(2.25, -0.25, -2.25);
    glTexCoord2f(0.0, 1);       glVertex3f(2.25, -0.25, 2.25);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    LettersAndNumbers(x, y, z, s);
    glPopMatrix();
}

/*
*   Copied from class examples
*   Light source for game
*/
static void ball(double x, double y, double z, double r)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(r, r, r);
    glColor3f(1, 1, 1);
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();
}

/*
*   Game Functions to move pieces and draw pieces at correct locations
*/
void drawGame()
{
    Board(0, 0, 0, 1.0);
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
            if (GamePiece[col][row].type == 'P')
            {
                Pawn(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color);
            }
            else if (GamePiece[col][row].type == 'R')
            {
                Rook(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color);
            }
            else if (GamePiece[col][row].type == 'K')
            {
                Knight(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color, 0);
            }
            else if (GamePiece[col][row].type == 'Q')
            {
                Queen(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color);
            }
            else if (GamePiece[col][row].type == 'X')
            {
                King(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color);
            }
            else if (GamePiece[col][row].type == 'B')
            {
                Bishop(PBoard[col][row].x, PBoard[col][row].y, PBoard[col][row].z, 0.7, GamePiece[col][row].color);
            }
        }
    }
    for (int i = 0; i < 16; i++)
    {
        if (takenWhitePieces[i].type == 'P')
        {
            Pawn(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color);
        }
        else if (takenWhitePieces[i].type == 'R')
        {
            Rook(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color);
        }
        else if (takenWhitePieces[i].type == 'K')
        {
            Knight(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color, 1);
        }
        else if (takenWhitePieces[i].type == 'Q')
        {
            Queen(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color);
        }
        else if (takenWhitePieces[i].type == 'X')
        {
            King(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color);
        }
        else if (takenWhitePieces[i].type == 'B')
        {
            Bishop(takenWhiteCoord[i].x, takenWhiteCoord[i].y, takenWhiteCoord[i].z, 0.7, takenWhitePieces[i].color);
        }
    }
    for (int i = 0; i < 16; i++)
    {
        if (takenBlackPieces[i].type == 'P')
        {
            Pawn(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color);
        }
        else if (takenBlackPieces[i].type == 'R')
        {
            Rook(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color);
        }
        else if (takenBlackPieces[i].type == 'K')
        {
            Knight(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color, 1);
        }
        else if (takenBlackPieces[i].type == 'Q')
        {
            Queen(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color);
        }
        else if (takenBlackPieces[i].type == 'X')
        {
            King(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color);
        }
        else if (takenBlackPieces[i].type == 'B')
        {
            Bishop(takenBlackCoord[i].x, takenBlackCoord[i].y, takenBlackCoord[i].z, 0.7, takenBlackPieces[i].color);
        }
    }
}

/*
*   User input to index conversion
*/
int rowLookup(char x)
{
    if (x == '1')
        return 0;
    else if (x == '2')
        return 1;
    else if (x == '3')
        return 2;
    else if (x == '4')
        return 3;
    else if (x == '5')
        return 4;
    else if (x == '6')
        return 5;
    else if (x == '7')
        return 6;
    else if (x == '8')
        return 7;
    return 0;
}
int colLookup(char x)
{
    if (x == 'A')
        return 0;
    else if (x == 'B')
        return 1;
    else if (x == 'C')
        return 2;
    else if (x == 'D')
        return 3;
    else if (x == 'E')
        return 4;
    else if (x == 'F')
        return 5;
    else if (x == 'G')
        return 6;
    else if (x == 'H')
        return 7;
    return 0;
}

/*
*   Copied framework from class examples
*   Heavily modified to preform project tasks 
*/
void display()
{
    double Ex = -2 * dim * Sin(th) * Cos(ph);
    double Ey = +2 * dim * Sin(ph);
    double Ez = +2 * dim * Cos(th) * Cos(ph);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
    gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);

    float Ambient[] = { 0.01 * ambient ,0.01 * ambient ,0.01 * ambient ,1.0 };
    float Diffuse[] = { 0.01 * diffuse ,0.01 * diffuse ,0.01 * diffuse ,1.0 };
    float Specular[] = { 0.01 * specular,0.01 * specular,0.01 * specular,1.0 };

    float Position[] = { 5 * Cos(zh),ylight,5 * Sin(zh),1 };

    glColor3f(1, 1, 1);
    ball(Position[0], Position[1], Position[2], 0.1);

    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    //  Draw the board with the current piece setup
    drawGame();
    //  Get user input of two char[] values for a fromSquare and toSquare
        // Handled in Key function
    //  Check if a piece is on the toSquare
    if (gotFromCol && gotFromRow && gotToCol && gotToRow)
    {
        int fc = colLookup(moveTracker[0]);
        int fr = rowLookup(moveTracker[1]);
        int tc = colLookup(moveTracker[4]);
        int tr = rowLookup(moveTracker[5]);
        // only need to check moves if there is a piece to move
        if (GamePiece[fc][fr].type != 'N')      //&& !pieceMoving
        {
            // check if there is a piece on the to square
            if (GamePiece[tc][tr].type != 'N')
            {
                //  There is already a piece in the to square. This piece must be taken first.
                //  takenPieceMoving = true;
                takenPiece = GamePiece[tc][tr];
                //  Remove the taken piece from the game board
                GamePiece[tc][tr].type = 'N';   GamePiece[tc][tr].color = 'N';
                //  Place the taken piece where it goes
                if (takenPiece.color == 'W')
                {
                    takenWhitePieces[takenWhitePieceCount] = takenPiece;
                    takenWhitePieceCount += 1;
                }
                else
                {
                    takenBlackPieces[takenBlackPieceCount] = takenPiece;
                    takenBlackPieceCount += 1;
                }
            }
            //  pieceMoving = true;

            //  Set the moved piece on the board and reset the input params
            movedPiece = GamePiece[fc][fr];
            GamePiece[tc][tr] = movedPiece;
            GamePiece[fc][fr].type = 'N';   GamePiece[fc][fr].color = 'N';
            moveTracker[0] = '_';
            moveTracker[1] = '_';
            moveTracker[4] = '_';
            moveTracker[5] = '_';
            gotFromCol = false;
            gotFromRow = false;
            gotToCol = false;
            gotToRow = false;
            // timeToMove = false;
            // pieceMoving = false;
        }
        else
        {
            // Invalid move, search for new input
            moveTracker[0] = '_';
            moveTracker[1] = '_';
            moveTracker[4] = '_';
            moveTracker[5] = '_';
            gotFromCol = false;
            gotFromRow = false;
            gotToCol = false;
            gotToRow = false;
        }
        /*
        *   I was unable to get the motion of the piece to work as a parabola, but here is my code.
        */
        
        ////  Motion of the taken piece
        //if (takenPieceMoving && timeToMove)
        //{
        //    if (time == 0)
        //        timeToMove = true;
        //    if (time < 1 && timeToMove)     // piece is still in motion
        //    {
        //        //  The destination is different for white and black pieces
        //        if (takenPiece.color == 'W')
        //        {
        //            x = PBoard[fc][fr].x + time * (takenWhiteCoord[takenWhitePieceCount].x - PBoard[fc][fr].x);  // Linear interpolation in x
        //            z = PBoard[fc][fr].z + time * (takenWhiteCoord[takenWhitePieceCount].z - PBoard[fc][fr].z);  // Linear interpolation in z
        //            /*
        //            y = PBoard[fc][fr].y +
        //                (vertexHeight - PBoard[fc][fr].y) * (2 * t - t * t) +
        //                (takenWhiteCoord[takenWhitePieceCount].y - vertexHeight) * (2 - 2 * t + t * t);       // Parabolic equation for y
        //            */
        //        }
        //        else
        //        {
        //            x = PBoard[fc][fr].x + time * (takenBlackCoord[takenBlackPieceCount].x - PBoard[fc][fr].x);  // Linear interpolation in x
        //            z = PBoard[fc][fr].z + time * (takenBlackCoord[takenBlackPieceCount].z - PBoard[fc][fr].z);  // Linear interpolation in z
        //            /*
        //            y = PBoard[fc][fr].y +
        //                (vertexHeight - PBoard[fc][fr].y) * (2 * t - t * t) +
        //                (takenBlackCoord[takenBlackPieceCount].y - vertexHeight) * (2 - 2 * t + t * t);       // Parabolic equation for y
        //            */
        //        }
        //        //  Piece to draw depends on color and piece type
        //        if (takenPiece.type == 'P')
        //        {
        //            Pawn(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'R')
        //        {
        //            Rook(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'K')
        //        {
        //            Knight(x, y, z, 0.7, takenPiece.color, 0);
        //        }
        //        else if (takenPiece.type == 'Q')
        //        {
        //            Queen(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'X')
        //        {
        //            King(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'B')
        //        {
        //            Bishop(x, y, z, 0.7, takenPiece.color);
        //        }
        //    }

        //    else    // Piece finished motion, Set piece on the taken tile
        //    {
        //        if (takenPiece.color == 'W')
        //        {
        //            takenWhitePieces[takenWhitePieceCount] = takenPiece;
        //            takenWhitePieceCount += 1;
        //        }
        //        else
        //        {
        //            takenBlackPieces[takenBlackPieceCount] = takenPiece;
        //            takenBlackPieceCount += 1;
        //        }
        //        //  Reset motion params
        //        timeToMove = false;
        //        takenPieceMoving = false;
        //    }
        //}
        //if (pieceMoving && !takenPieceMoving)   // Only move one piece at a time, wait for the taken piece to move first. 
        //{
        //    //  Remove the piece from the game board
        //    GamePiece[fc][fr].type = 'N';   GamePiece[fc][fr].color = 'N';
        //    if (time == 0)
        //        timeToMove = true;
        //    if (time < 1 && timeToMove)
        //    {
        //        x = PBoard[fc][fr].x + time * (PBoard[tc][tr].x - PBoard[fc][fr].x);  // Linear interpolation in x
        //        z = PBoard[fc][fr].z + time * (PBoard[tc][tr].z - PBoard[fc][fr].z);  // Linear interpolation in z
        //        /*
        //        y = PBoard[fc][fr].y +
        //            (vertexHeight - PBoard[fc][fr].y) * (2 * t - t * t) +
        //            (PBoard[tc][tr].y - vertexHeight) * (2 - 2 * t + t * t);       // Parabolic equation for y
        //        */
        //        if (takenPiece.type == 'P')
        //        {
        //            Pawn(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'R')
        //        {
        //            Rook(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'K')
        //        {
        //            Knight(x, y, z, 0.7, takenPiece.color, 0);
        //        }
        //        else if (takenPiece.type == 'Q')
        //        {
        //            Queen(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'X')
        //        {
        //            King(x, y, z, 0.7, takenPiece.color);
        //        }
        //        else if (takenPiece.type == 'B')
        //        {
        //            Bishop(x, y, z, 0.7, takenPiece.color);
        //        }
        //    }
        //    else
        //    {
        //        //  Set the moved piece on the board and reset the input params
        //        GamePiece[tc][tr] = movedPiece;
        //        moveTracker[0] = '_';
        //        moveTracker[1] = '_';
        //        moveTracker[4] = '_';
        //        moveTracker[5] = '_';
        //        gotFromCol = false;
        //        gotFromRow = false;
        //        gotToCol = false;
        //        gotToRow = false;
        //        timeToMove = false;
        //        pieceMoving = false;
        //    }
        //}
    }
    //  Display current user input
    glColor4fv(white);
    glWindowPos2i(5, 5);
    Print("Enter your move: %s",moveTracker);
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
*   Copied from class examples
*/
void idle()
{
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    // time = fmod(t, 11) / 10;
    zh = fmod(90 * t, 360.0);
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim > 1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    Project(45, asp, dim);
    glutPostRedisplay();
}

void key(unsigned char ch, int x, int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
    {
        th = 0;
        ph = 30;
    }
    else if (ch == 't')
        ntex = (1 + ntex) % 4;
    else if (ch == 'T')
        ntex = (ntex - 1) % 4;
    else if (ch == 'a' || ch == 'A')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'A';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'A';
            gotToCol = true;
        }
    }
    else if (ch == 'b' || ch == 'B')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'B';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'B';
            gotToCol = true;
        }
    }
    else if (ch == 'c' || ch == 'C')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'C';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'C';
            gotToCol = true;
        }
    }
    else if (ch == 'd' || ch == 'D')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'D';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'D';
            gotToCol = true;
        }
    }
    else if (ch == 'e' || ch == 'E')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'E';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'E';
            gotToCol = true;
        }
    }
    else if (ch == 'f' || ch == 'F')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'F';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'F';
            gotToCol = true;
        }
    }
    else if (ch == 'g' || ch == 'G')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'G';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'G';
            gotToCol = true;
        }
    }
    else if (ch == 'h' || ch == 'H')
    {
        if (!gotFromCol)
        {
            moveTracker[0] = 'H';
            gotFromCol = true;
        }
        else if (gotFromCol && gotFromRow && !gotToCol)
        {
            moveTracker[4] = 'H';
            gotToCol = true;
        }
    }
    else if (ch == '1')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '1';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '1';
            gotToRow = true;
        }
    }
    else if (ch == '2')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '2';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '2';
            gotToRow = true;
        }
    }
    else if (ch == '3')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '3';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '3';
            gotToRow = true;
        }
    }
    else if (ch == '4')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '4';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '4';
            gotToRow = true;
        }
    }
    else if (ch == '5')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '5';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '5';
            gotToRow = true;
        }
    }
    else if (ch == '6')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '6';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '6';
            gotToRow = true;
        }
    }
    else if (ch == '7')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '7';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '7';
            gotToRow = true;
        }
    }
    else if (ch == '8')
    {
        if (gotFromCol && !gotFromRow)
        {
            moveTracker[1] = '8';
            gotFromRow = true;
        }
        else if (gotFromCol && gotFromRow && gotToCol && !gotToRow)
        {
            moveTracker[5] = '8';
            gotToRow = true;
        }
    }
    else if (ch == 'R')
    {
        for (int col = 0; col < 8; col++)
        {
            for (int row = 0; row < 8; row++)
            {
                GamePiece[col][row] = GamePieceReset[col][row];
            }
        }
        for (int i = 0; i < 16; i++)
        {
            takenBlackPieces[i].type = 'N';
            takenBlackPieces[i].color= 'N';
            takenWhitePieces[i].type = 'N';
            takenWhitePieces[i].color = 'N';
        }
        takenWhitePieceCount = 0;
        takenBlackPieceCount = 0;
    }
    Project(45, asp, dim);
    glutPostRedisplay();
}

/*
*   Copied from class examples
*/
void reshape(int width, int height)
{
    asp = (height > 0) ? (double)width / height : 1;
    glViewport(0, 0, RES * width, RES * height);
    Project(45, asp, dim);
}

/*
*   Copied framework from class examples
*   Heavily modified to preform project tasks
*/
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(750, 750);
    glutCreateWindow("Ian Scheuermann");
#ifdef USEGLEW
    if (glewInit() != GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //  Compute normals for pieces
    CalcNorm(NP, PP, NpPawn);
    CalcNorm(NB, PB, NpBishop);
    CalcNorm(NR, PR, NpRook);
    CalcNorm(NKnightBase, PKnightBase, NpKnightBase);
    CalcNorm(NQK, PQK, NpQK);
	//	Load texture
    texture[0] = LoadTexBMP("none.bmp");
    texture[1] = LoadTexBMP("wood.bmp");
    texture[2] = LoadTexBMP("marble.bmp");
    texture[3] = LoadTexBMP("metal.bmp");
    letterNumber[0] = LoadTexBMP("A.bmp");
    letterNumber[1] = LoadTexBMP("B.bmp");
    letterNumber[2] = LoadTexBMP("C.bmp");
    letterNumber[3] = LoadTexBMP("D.bmp");
    letterNumber[4] = LoadTexBMP("E.bmp");
    letterNumber[5] = LoadTexBMP("F.bmp");
    letterNumber[6] = LoadTexBMP("G.bmp");
    letterNumber[7] = LoadTexBMP("H.bmp");
    letterNumber[8] = LoadTexBMP("1.bmp");
    letterNumber[9] = LoadTexBMP("2.bmp");
    letterNumber[10] = LoadTexBMP("3.bmp");
    letterNumber[11] = LoadTexBMP("4.bmp");
    letterNumber[12] = LoadTexBMP("5.bmp");
    letterNumber[13] = LoadTexBMP("6.bmp");
    letterNumber[14] = LoadTexBMP("7.bmp");
    letterNumber[15] = LoadTexBMP("8.bmp");
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
