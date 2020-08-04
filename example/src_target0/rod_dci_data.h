#ifndef ROD_DCI_DATA_H_
#define ROD_DCI_DATA_H_

#include <rod.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct s_GLPrimitive
{
  GLfloat lineWidth;
  GLenum glWhich;
  uint32_t count;
  uint32_t isStipple;
  uint32_t stipleFactor;
  uint32_t stiplePattern;
} t_GLPrimitive;

typedef struct s_RodDciFragment
{
  t_GLPrimitive * pPrimitives;
  GLfloat * pVertex;
  GLubyte * pColors;
  uint32_t  primitiveCount;
  int32_t   x;
  int32_t   y;
  uint32_t   size;
  uint32_t   trianglesCount;
  uint32_t   linesCount;
  float      trianglesSquare;
  struct
  {
    uint32_t  list;
    uint32_t  shown;
  } user_data;
} t_RodDciFragment;

typedef struct s_RodDciMap
{
  t_RodDciFragment * pFragments;
  uint32_t fragmentCount;
  uint32_t fragmentSize;
} t_RodDciMap;

extern t_RodDciMap RodDciMaps[];
extern uint32_t RodDciMapCount;

extern GLfloat RodDciMinX;
extern GLfloat RodDciMinY;

#ifdef __cplusplus
}
#endif

#endif // header guard
