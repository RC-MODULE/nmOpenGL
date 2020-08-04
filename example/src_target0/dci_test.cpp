//#include "dci_test.h"
#include <rod_dci_data.h>
//#include <rod_status.h>

#include <math.h>
//#include <pthread.h>
#include <stdio.h>
//#include <unistd.h>
#include <string.h>

//static pthread_t DciThread;
static volatile int32_t   DciStatus = 0;
static volatile int32_t   DciIsBegin = 0;
static volatile int32_t   DciFrames = 0;
static volatile double    DciSeconds = -1.0;
static volatile double    DciFps    = 0.0;
static volatile double    DciTime    = 0.0;

static double    DciOffsetX =   0.0f;
static double    DciOffsetY =  -0.0f;
static double    DciSpecialX = -300000;
static double    DciSpecialY = 5700000;

static double    DciScale   = 1.0/500.0;
static double    DciRotate  = 0.0;
static int32_t   DciCurrentMap = 3;
static int32_t   DciFunc = 0;
static int32_t   DciMode = 0;
static int32_t   DciList = 0;
static int32_t   DciImgNo = 0;


static double    DciTriangleRate    = 0.0;
static double    DciLineRate    = 0.0;
static double    DciFillRate = 0;


static int32_t   DciFramesArray[ROD_DATA_ARRAY_SIZE];
static double    DciTimeArray[ROD_DATA_ARRAY_SIZE];
static double    DciLinesArray[ROD_DATA_ARRAY_SIZE];
static double    DciTrianglesArray[ROD_DATA_ARRAY_SIZE];
static double    DciPixelsArray[ROD_DATA_ARRAY_SIZE];

static int32_t   DciFpsIndex = 0;


static t_RodColor  DciRgba[ROD_VIDEOBUFFER_H][ROD_VIDEOBUFFER_W];

 typedef struct s_MapResults
{
  uint64_t  triangleCount;
  uint64_t  lineCount;
  double    triangleSquare;
} t_MapResults;


typedef struct s_PathPoint
{
  double t;
  double x;
  double y;
  double s;
  double r;
  int32_t map;
} t_PathPoint;

t_PathPoint DciTrack[] =
{
  {
    0.0,
    3*100000.0f,
    3*100000.0f,
    1.0/250.0,
    -25.0,
    3
  },
  {
    7.0f,
    2.05*100000.0f,
    4*100000.0f,
    1.0/120.0,
    0.0,
    2
  },
  {
    8.0f,
    2.05*100000.0f,
    4.20*100000.0f,
    1.0/85.0,
    0.0,
    2
  },
  {
    10.0f,
    2.05*100000.0f,
    4.55*100000.0f,
    1.0/85.0,
    0.0,
    2
  },
  {
    15.0f,
    2.05*100000.0f,
    4.75*100000.0f,
    1.0/40.0,
    0.0,
    1
  },
  {
    20.0f,
    2.05*100000.0f,
    4.85*100000.0f,
    1.0/25.0,
    0.0,
    0
  },
  {
    24.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/15.0,
    -25.0,
    0
  },
  {
    25.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/15.0,
    -25.0,
    0
  }
};

t_PathPoint DciTrackImgPoint[] =
{
  {
    0.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/10.0,
    0.0,
    0
  },
  {
    1.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/20.0,
    0.0,
    0
  },
  {
    2.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/20.0,
    -25.0,
    0
  },
  {
    3.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/20.0,
    0.0,
    1
  },
  {
    4.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/40.0,
    0.0,
    1
  },
  {
    5.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/40.0,
    -25.0,
    1
  },
  {
    6.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/40.0,
    0.0,
    2
  },
  {
    7.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/80.0,
    0.0,
    2
  },
  {
    8.0f,
    2.15*100000.0f,
    4.75*100000.0f,
    1.0/80.0,
    -25.0,
    2
  },
  {
    9.0,
    2.15*100000.0f,
    4.25*100000.0f,
    1.0/120.0,
    0.0,
    3
  },
  {
    10.0,
    2.15*100000.0f,
    4.25*100000.0f,
    1.0/240.0,
    0.0,
    3
  },
  {
    11.0,
    2.15*100000.0f,
    4.25*100000.0f,
    1.0/240.0,
    -25.0,
    3
  }
};

typedef int32_t (* t_FuncDrawMap)(t_RodDciMap *, t_MapResults *);

static int32_t RodClearImage()
{
  int32_t i,j,z;
  memset(&DciRgba, 0, sizeof(DciRgba));
  for (i = 0; i < ROD_VIDEOBUFFER_H; ++i)
  {
    for (j = 0; j < ROD_VIDEOBUFFER_W; ++j)
    {
      z = ((i+4)*(i+4) + ((j+4)*(j+4)))%32;
      if ( z >= 8 && z <= 18)
      {
        DciRgba[i][j].rgba.r = 0xFF;
        DciRgba[i][j].rgba.g = 0;
        DciRgba[i][j].rgba.b = 0;
        DciRgba[i][j].rgba.a = 0xFF;
      }
    }

  }
  return 0;
}

static const t_PathPoint * RodTrackGet(double time)
{
  static int32_t lastPoint = 0;
  int32_t i;
  static t_PathPoint point;
  double part;
  int32_t trackCount = COUNT(DciTrack);

  if (time >= DciTrack[lastPoint].t)
  {
    for (i = lastPoint; i < trackCount - 1; i++)
    {
       if (1
           && time >= DciTrack[i].t
           && time <  DciTrack[i + 1].t)
       {
         lastPoint = i;
         part = (time - DciTrack[i].t)/(DciTrack[i + 1].t - DciTrack[i].t);
         point.x = DciTrack[i].x + part * (DciTrack[i + 1].x - DciTrack[i].x);
         point.y = DciTrack[i].y + part * (DciTrack[i + 1].y - DciTrack[i].y);
         point.r = DciTrack[i].r + part * (DciTrack[i + 1].r - DciTrack[i].r);
         point.s = DciTrack[i].s + part * (DciTrack[i + 1].s - DciTrack[i].s);
         point.map = DciTrack[i].map;
         return &point;
       }
    }
  }
  else
  {
    for (i = 0; i < lastPoint; i++)
    {
       if (1
           && time >= DciTrack[i].t
           && time <  DciTrack[i + 1].t)
       {
         lastPoint = i;
         part = (time - DciTrack[i].t)/(DciTrack[i + 1].t - DciTrack[i].t);
         point.x = DciTrack[i].x + part * (DciTrack[i + 1].x - DciTrack[i].x);
         point.y = DciTrack[i].y + part * (DciTrack[i + 1].y - DciTrack[i].y);
         point.r = DciTrack[i].r + part * (DciTrack[i + 1].r - DciTrack[i].r);
         point.s = DciTrack[i].s + part * (DciTrack[i + 1].s - DciTrack[i].s);
         point.map = DciTrack[i].map;
         return &point;
       }
    }
  }

  return 0;

}

int32_t RodDrawFragmentv1(t_RodDciFragment * pFragment)
{
  uint32_t i = 0, j = 0;
  uint32_t pointOffset = 0;
  GLfloat lastWidth = 1.0f;
  GLfloat * pVertex = 0;
  GLubyte * pColors = 0;
  t_GLPrimitive * pPrimitive = 0;

  pVertex = pFragment->pVertex;
  pColors = pFragment->pColors;
  for (i = 0; i < pFragment->primitiveCount; ++i)
  {
    pPrimitive = pFragment->pPrimitives + i;
	if (pPrimitive->glWhich != GL_TRIANGLES) {
		pointOffset += pPrimitive->count;
	}

    if (pPrimitive->isStipple)
    {
		pointOffset += pPrimitive->count;
		continue;
      //glEnable(GL_LINE_STIPPLE);
      //glLineStipple(pPrimitive->stipleFactor, pPrimitive->stiplePattern);
    }

    if (1
        && pPrimitive->glWhich == GL_LINES
        && lastWidth != pPrimitive->lineWidth)
    {
      lastWidth = pPrimitive->lineWidth;
      //glLineWidth(pPrimitive->lineWidth);
    }
    glBegin(pPrimitive->glWhich);
    for (j = pointOffset; j < pointOffset + pPrimitive->count; ++j)
    {
      glColor4ub(pColors[0], pColors[1], pColors[2], pColors[3]);
      glVertex2f(pVertex[0], pVertex[1]);
      pColors += 4;
      pVertex += 2;
    }
	
    glEnd();

    if (pPrimitive->isStipple)
    {
      //glDisable(GL_LINE_STIPPLE);
    }

    pointOffset += pPrimitive->count;
  }
  return 0;
}

/*static int32_t RodDrawFragmentv2(t_RodDciFragment * pFragment)
{
  uint32_t i = 0;
  uint32_t pointOffset = 0;
  GLfloat lastWidth = 1.0f;
  t_GLPrimitive * pPrimitive = 0;
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT,         0, pFragment->pVertex);
  glColorPointer (4, GL_UNSIGNED_BYTE, 0, pFragment->pColors);
  for (i = 0; i < pFragment->primitiveCount; ++i)
  {
    pPrimitive = pFragment->pPrimitives + i;

    if (pPrimitive->isStipple)
    {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(pPrimitive->stipleFactor, pPrimitive->stiplePattern);
    }

    if (1
        && pPrimitive->glWhich == GL_LINES
        && lastWidth != pPrimitive->lineWidth)
    {
      lastWidth = pPrimitive->lineWidth;
      glLineWidth(pPrimitive->lineWidth);
    }

    glDrawArrays(pPrimitive->glWhich, pointOffset, pPrimitive->count);

    if (pPrimitive->isStipple)
    {
      glDisable(GL_LINE_STIPPLE);
    }
    pointOffset += pPrimitive->count;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  return 0;
}

static int32_t RodDrawFragmentv3(t_RodDciFragment * pFragment)
{
  uint32_t i = 0, j = 0;
  uint32_t pointOffset = 0;
  GLfloat * pVertex = 0;
  GLubyte * pColors = 0;
  t_GLPrimitive * pPrimitive = 0;
  pVertex = pFragment->pVertex;
  pColors = pFragment->pColors;
  for (i = 0; i < pFragment->primitiveCount; ++i)
  {
    pPrimitive = pFragment->pPrimitives + i;
    glBegin(pPrimitive->glWhich);
    for (j = pointOffset; j < pointOffset + pPrimitive->count; ++j)
    {
      glColor4ub(pColors[0], pColors[1], pColors[2], pColors[3]);
      glVertex2f(pVertex[0], pVertex[1]);
      pColors += 4;
      pVertex += 2;
    }
    glEnd();
    pointOffset += pPrimitive->count;
  }
  return 0;
}

static int32_t RodDrawFragmentv4(t_RodDciFragment * pFragment)

{
  uint32_t i = 0;
  uint32_t pointOffset = 0;
  t_GLPrimitive * pPrimitive = 0;
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT,         0, pFragment->pVertex);
  glColorPointer (4, GL_UNSIGNED_BYTE, 0, pFragment->pColors);
  for (i = 0; i < pFragment->primitiveCount; ++i)
  {
    pPrimitive = pFragment->pPrimitives + i;
    glDrawArrays(pPrimitive->glWhich, pointOffset, pPrimitive->count);
    pointOffset += pPrimitive->count;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  return 0;
}

static int32_t RodTranspose(GLfloat m[][4])
{
  uint32_t i = 0, j = 0;
  GLfloat t = 0.0;
  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 4; j ++)
    {
      t = m[j][i];
      m[j][i] = m[i][j];
      m[i][j] = t;
    }
  }
  return 0;
}

static int32_t RodTestPoint(GLfloat tp[][4], GLfloat m[][4], float x, float y, float size)
{
  uint32_t i = 0, j = 0;
  static GLfloat mp[4][4];
  double tx, ty, rx, ry;
  double r;
  for (i = 0; i < 4; i ++)
  {
    for (j = 0; j < 4; j ++)
    {
      mp[i][j] = tp[i][0] * m[j][0] + tp[i][1] * m[j][1] + tp[i][2] * m[j][2] + tp[i][3] * m[j][3];
    }
  }
  tx = mp[0][0]*1.0*(x + size/2.0) + mp[0][1]*1.0*(y + size/2.0) + mp[0][3];
  ty = mp[1][0]*1.0*(x + size/2.0) + mp[1][1]*1.0*(y + size/2.0) + mp[1][3];


  rx  = mp[0][0]*1.0*(size/2.0) + mp[0][1]*1.0*(size/2.0);
  ry  = mp[1][0]*1.0*(size/2.0) + mp[1][1]*1.0*(size/2.0);
  r = sqrt(rx*rx + ry*ry) + 1.41421356237309504880;
  if (sqrt(tx*tx + ty*ty)<=r)
    return 0;
  return 1;
}

static int32_t RodDrawMapDirect(t_RodDciMap * pMap, t_MapResults * pResults)
{
  static GLfloat m[4][4];
  static GLfloat p[4][4];
  uint32_t i = 0;
  t_RodDciFragment * pFragment;
  glGetFloatv(GL_MODELVIEW_MATRIX,  (GLfloat*)m);
  glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)p);
  RodTranspose(p);
  for (i = 0; i < pMap->fragmentCount; ++i)
  {
    pFragment = pMap->pFragments + i;
    if (RodTestPoint(p, m, (GLfloat)pFragment->x*pFragment->size, (GLfloat)pFragment->y*pFragment->size, pFragment->size))
    {
      continue;
    }
    glPushMatrix();
    glScalef((GLfloat)pFragment->size, (GLfloat)pFragment->size, 1.0f);
    glTranslatef((GLfloat)pFragment->x, (GLfloat)pFragment->y, 0.0f);
    glLineWidth(1.0f);
    switch(DciFunc)
    {
      case ROD_RENDER_DIRECT:
        RodDrawFragmentv1(pFragment);
        break;
      case ROD_RENDER_VARRAY:
        RodDrawFragmentv2(pFragment);
        break;
      case ROD_RENDER_DIRECT_SIMPLE:
        RodDrawFragmentv3(pFragment);
        break;
      case ROD_RENDER_VARRAY_SIMPLE:
        RodDrawFragmentv4(pFragment);
        break;
      default:
        break;
    }
    glPopMatrix();

    pFragment->user_data.shown = 1;
    pResults->triangleSquare += pFragment->trianglesSquare * ((double)pFragment->size * (double)pFragment->size);
    pResults->triangleCount  += pFragment->trianglesCount;
    pResults->lineCount      += pFragment->linesCount;
  }
  return 0;
}

static int32_t RodDrawMapList(t_RodDciMap * pMap, t_MapResults * pResults)
{
  static GLfloat m[4][4];
  static GLfloat p[4][4];
  uint32_t i = 0;
  t_RodDciFragment * pFragment;
  glGetFloatv(GL_MODELVIEW_MATRIX,  (GLfloat*)m);
  glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)p);
  RodTranspose(p);
  for (i = 0; i < pMap->fragmentCount; ++i)
  {
    pFragment = pMap->pFragments + i;
    if (RodTestPoint(p, m, (GLfloat)pFragment->x*pFragment->size, (GLfloat)pFragment->y*pFragment->size, pFragment->size))
    {
      continue;
    }
    glPushMatrix();
    glScalef((GLfloat)pFragment->size, (GLfloat)pFragment->size, 1.0f);
    glTranslatef((GLfloat)pFragment->x, (GLfloat)pFragment->y, 0.0f);
    glLineWidth(1.0f);
    glCallLists(1,GL_UNSIGNED_INT,&pFragment->user_data.list);
    glPopMatrix();
    pFragment->user_data.shown = 1;
    pResults->triangleSquare += pFragment->trianglesSquare * ((double)pFragment->size * (double)pFragment->size);
    pResults->triangleCount  += pFragment->trianglesCount;
    pResults->lineCount      += pFragment->linesCount;
  }
  return 0;
}

static int32_t RodGenMapList(int32_t map_id)
{
  t_RodDciFragment * pFragment;
  uint32_t i = 0;
  t_RodDciMap * pMap = &RodDciMaps[map_id];
  for (i = 0; i < pMap->fragmentCount; ++i)
  {
    pFragment = pMap->pFragments + i;
    pFragment->user_data.list = glGenLists(1);
    glLineWidth(1.0f);
    glNewList(pFragment->user_data.list, GL_COMPILE);
    switch(DciFunc)
    {
      case ROD_RENDER_DIRECT:
        RodDrawFragmentv1(pFragment);
        break;
      case ROD_RENDER_VARRAY:
        RodDrawFragmentv2(pFragment);
        break;
      case ROD_RENDER_DIRECT_SIMPLE:
        RodDrawFragmentv3(pFragment);
        break;
      case ROD_RENDER_VARRAY_SIMPLE:
        RodDrawFragmentv4(pFragment);
        break;
      default:
        break;
    }
    glEndList();
    RodStatusSetStage(map_id, RodDciMapCount, (i + 1.0)/pMap->fragmentCount);

  }
  return 0;
}

static int32_t RodDciDrawScene(const t_PathPoint * pPoint, t_FuncDrawMap FuncDrawMap, t_MapResults * pResults)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();

    DciOffsetX = pPoint->x + DciSpecialX;
    DciOffsetY = pPoint->y + DciSpecialY;

    DciScale   = pPoint->s;
    DciRotate  = pPoint->r;
    DciCurrentMap = pPoint->map;
    glScalef(DciScale, DciScale, 1.0f);
    glRotatef(DciRotate, 0.0f, 0.0f, 1.0f);
    glTranslatef(- DciOffsetX, - DciOffsetY, 0.0f);
    FuncDrawMap(&RodDciMaps[DciCurrentMap], pResults);
    pResults->triangleSquare *= (DciScale * DciScale);
  glPopMatrix();
  return 0;
}

static void * RodDciScene(t_RodConfig * pConfig)
{
  double    triangleSquare = 0;
  double t_start = 0.0, t_sec = 0.0, tfps_prev = 0.0 , t_now = 0.0 ;
  double time_multy = 1.0;
  double fpsDiffTime = 0.0;
  uint64_t  triangleCount = 0;
  uint64_t  lineCount = 0;
  t_MapResults mapResults;

  const t_PathPoint * pPoint = 0;
  t_FuncDrawMap FuncDrawMap;
  int32_t i = 0;

  RodStatusAcquireGL();
  if (ROD_INITIALIZER(pConfig))
  {
    return 0;
  }

  if (DciMode)
  {
    glViewport(0, 0, (GLint)ROD_VIDEOBUFFER_W, (GLint)ROD_VIDEOBUFFER_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-(ROD_VIDEOBUFFER_W/2.0), (ROD_VIDEOBUFFER_W/2.0), -(ROD_VIDEOBUFFER_H/2.0), (ROD_VIDEOBUFFER_H/2.0), 0, ROD_VIDEOBUFFER_Z);
  }
  else
  {
    glViewport(0, 0, (GLint)pConfig->xResolution, (GLint)pConfig->yResolution);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-(pConfig->xResolution/2.0), (pConfig->xResolution/2.0), -(pConfig->yResolution/2.0), (pConfig->yResolution/2.0), 0, ROD_VIDEOBUFFER_Z);
  }


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glPixelStorei(GL_PACK_ALIGNMENT, 4);

  glClearColor(0.90f, 0.90f, 0.90f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ROD_SWAPBUFFERS(pConfig);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ROD_SWAPBUFFERS(pConfig);



  if (DciList)
  {
    RodStatusSetStage(1, RodDciMapCount, 0.0);
    for (i = 0; i < RodDciMapCount; ++i)
    {
      RodGenMapList(i);
    }
    FuncDrawMap = RodDrawMapList;
  }
  else
  {
    FuncDrawMap = RodDrawMapDirect;
  }


  if (DciSeconds <= 0.0f)
  {
    DciSeconds = DciTrack[COUNT(DciTrack)-1].t;
  }
  else
  {
    time_multy = DciTrack[COUNT(DciTrack)-1].t/DciSeconds;
  }

  if (DciMode)
  {
    mapResults.lineCount = 0;
    mapResults.triangleCount = 0;
    mapResults.triangleSquare = 0;
    t_start = Nanotime();
    RodDciDrawScene(&DciTrackImgPoint[DciImgNo], FuncDrawMap, &mapResults);
    glFlush();
    glFinish();
    t_sec = Nanotime() - t_start;

    triangleCount  += mapResults.triangleCount;
    triangleSquare += mapResults.triangleSquare;
    lineCount      += mapResults.lineCount;

    DciFrames ++;
    glReadPixels(0, 0, ROD_VIDEOBUFFER_W,ROD_VIDEOBUFFER_H, GL_RGBA,GL_UNSIGNED_BYTE, DciRgba);
    ROD_SWAPBUFFERS(pConfig);

    DciFps = 1.0*DciFrames/t_sec;
    DciTriangleRate = 1.0*triangleCount / t_sec;
    DciLineRate = 1.0*lineCount / t_sec;
    DciFillRate = 1.0*triangleSquare / t_sec;
  }
  else
  {
    tfps_prev = t_start = Nanotime();
    fpsDiffTime = (DciSeconds)/ ROD_DATA_ARRAY_SIZE;
    triangleCount = 0;
    lineCount = 0;
    triangleSquare = 0;
    while (DciStatus && t_sec < DciSeconds && (pPoint = RodTrackGet(time_multy*t_sec)))
    {
      mapResults.lineCount = 0;
      mapResults.triangleCount = 0;
      mapResults.triangleSquare = 0;

      RodDciDrawScene(pPoint, FuncDrawMap, &mapResults);
      glFlush();
      glFinish();
      ROD_SWAPBUFFERS(pConfig);
      t_now = Nanotime();
      t_sec = t_now - t_start;
      triangleCount  += mapResults.triangleCount;
      triangleSquare += mapResults.triangleSquare;
      lineCount      += mapResults.lineCount;

      DciFrames ++;

      if (t_now - tfps_prev > fpsDiffTime)
      {
        if (DciFpsIndex < ROD_DATA_ARRAY_SIZE)
        {
          DciTimeArray[DciFpsIndex] = t_sec;
          DciFramesArray[DciFpsIndex] = DciFrames;
          DciTrianglesArray[DciFpsIndex] = triangleCount;
          DciLinesArray[DciFpsIndex] = lineCount;
          DciPixelsArray[DciFpsIndex] = triangleSquare;
          if (DciFpsIndex > 0)
          {
            RodStatusSetFPS( (1.0*DciFramesArray[DciFpsIndex] - DciFramesArray[DciFpsIndex - 1])/ (DciTimeArray[DciFpsIndex] - DciTimeArray[DciFpsIndex-1]));
            RodStatusSetStage(RodDciMapCount, RodDciMapCount, t_sec/DciSeconds);
          }
          ++DciFpsIndex;
          tfps_prev = t_now;
        }
      }
    }
    t_sec = Nanotime() - t_start;
    DciFps = 1.0*DciFrames/t_sec;
    DciTriangleRate = 1.0*triangleCount / t_sec;
    DciLineRate = 1.0*lineCount / t_sec;
    DciFillRate = 1.0*triangleSquare / t_sec;

  }
  DciStatus = 0;
  RodStatusSetStage(RodDciMapCount, RodDciMapCount, 1.0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ROD_SWAPBUFFERS(pConfig);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ROD_SWAPBUFFERS(pConfig);

  if (ROD_DEINITIALIZER(pConfig))
  {
    return 0;
  }
  RodStatusReleaseGL();

  return 0;
}

int32_t RodDciSceneBegin(t_RodConfig * pConfig, t_RodItemDciInput * input, t_RodItemHeader * output)
{
  struct sched_param thread_schedparam;
  pthread_attr_t thread_tattr;
  int32_t priority = 0;

  output->size = sizeof(*output);
  if (DciStatus)
  {
    output->param = ROD_ERROR_ACTION;
    return output->param;
  }
  if (DciIsBegin)
  {
    output->param = ROD_ERROR_ACTION;
    return output->param;
  }

  vbe2h(input->payload.priority);
  vbe2h(input->payload.time);
  vbe2h(input->payload.render);
  vbe2h(input->payload.mode);
  vbe2h(input->payload.imageNo);
  vbe2h(input->payload.list);

  priority = input->payload.priority;
  DciSeconds = input->payload.time;
  DciFunc    = input->payload.render;
  DciMode    = input->payload.mode;
  DciImgNo = input->payload.imageNo;
  DciList = input->payload.list;

  if (DciSeconds > 120.0)
  {
    output->param = ROD_ERROR_VALUE;
    return output->param;
  }
  if (DciImgNo >= COUNT(DciTrackImgPoint))
  {
    output->param = ROD_ERROR_VALUE;
    return output->param;
  }

  LOGFT(ROD_LOG_INFO, "\tThread priority: %d\n", priority);
  LOGFT(ROD_LOG_INFO, "\tMode:  %d\n", DciMode);
  LOGFT(ROD_LOG_INFO, "\tFunc:  %d\n", DciFunc);
  LOGFT(ROD_LOG_INFO, "\tImgNo: %d\n", DciImgNo);
  if (DciSeconds>= 0.0)
  {
    LOGFT(ROD_LOG_INFO, "\t[II]: Time for test:  %f\n", DciSeconds);
  }
  else
  {
    LOGT(ROD_LOG_INFO, "\tTime for test:  DEFAULT\n");
  }
  LOGFT(ROD_LOG_INFO, "\tDisplay lists: %s\n", DciList ? "enabled" : "disabled");

  DciStatus = 1;
  DciFrames = 0;
  DciFps    = 0.0;
  DciTriangleRate = 0;
  DciLineRate = 0;
  DciFillRate = 0;
  DciOffsetX = 0.0;
  DciOffsetY = 0.0;
  DciFpsIndex         = 0;
  DciTime   = Nanotime();
  RodClearImage();

  pthread_attr_init(&thread_tattr);
  pthread_attr_getschedparam(&thread_tattr, &thread_schedparam);
  pthread_attr_setschedpolicy(&thread_tattr, SCHED_OTHER);
  thread_schedparam.sched_priority = priority;
  pthread_attr_setschedparam(&thread_tattr, &thread_schedparam);
  RodStatusClearStage();
  if (pthread_create(&DciThread, &thread_tattr, (void * (*) (void*))RodDciScene, pConfig) < 0)
  {
    DciStatus = 0;
    output->param = ROD_ERROR_THREAD;
    return output->param;
  }
  DciIsBegin = 1;
  output->param = ROD_OK;
  return ROD_OK;

}

int32_t RodDciAbort()
{
  if (DciStatus)
  {
    LOGT(ROD_LOG_INFO, "\tAbort DCI\n");
    DciStatus = 0;
    pthread_join(DciThread, NULL);
    RodStatusClearStage();
  }
  DciIsBegin = 0;
  return ROD_OK;
}

int32_t RodDciSceneEnd(t_RodConfig * pConfig, t_RodItemHeader * input, t_RodItemDciOutput * output)
{
  int32_t i;
  output->header.size = sizeof(*output);
  if (!DciIsBegin)
  {
    output->header.param = ROD_ERROR;
    return ROD_ERROR;
  }
  if (input->param)
    DciStatus = 0;
  LOGT(ROD_LOG_INFO, "\tWaiting on pthread_join\n");
  pthread_join(DciThread, NULL);
  RodStatusClearStage();
  DciStatus = 0;

  DciTime   = Nanotime() - DciTime;

  output->payload.fps        = DciFps;
  output->payload.frameCount = DciFrames;
  output->payload.totalTime  = DciTime;
  output->payload.arrayCount   = DciFpsIndex;
  output->payload.triangleRate   = DciTriangleRate;
  output->payload.lineRate       = DciLineRate;
  output->payload.fillRate       = DciFillRate;
  for (i = 0; i < DciFpsIndex; ++i)
  {

    output->payload.timeArray[i] = DciTimeArray[i];
    output->payload.linesArray[i] = DciLinesArray[i];
    output->payload.trianglesArray[i] = DciTrianglesArray[i];
    output->payload.pixelsArray[i] = DciPixelsArray[i];
    output->payload.frameArray[i] = DciFramesArray[i];

    vh2be(output->payload.timeArray[i]);
    vh2be(output->payload.linesArray[i]);
    vh2be(output->payload.trianglesArray[i]);
    vh2be(output->payload.pixelsArray[i]);
    vh2be(output->payload.frameArray[i]);

    if (i > 0)
    {
       LOGFT(ROD_LOG_OUTPUT, "\tIFPS[%03d]: %f\n", i, (DciFramesArray[i]- DciFramesArray[i-1]) / (DciTimeArray[i]- DciTimeArray[i-1]));
    } else {
       LOGFT(ROD_LOG_OUTPUT, "\tIFPS[%03d]: %f\n", i, (DciFramesArray[i] / DciTimeArray[i]));
    }

  }
  if (DciMode == 1)
  {
    memcpy(&output->payload.result_rgba, &DciRgba, sizeof(DciRgba));
    output->payload.have_rgba  = 1;
  }
  vh2be(output->payload.arrayCount);
  vh2be(output->payload.fps);
  vh2be(output->payload.frameCount);
  vh2be(output->payload.totalTime);
  vh2be(output->payload.triangleRate);
  vh2be(output->payload.lineRate);
  vh2be(output->payload.fillRate);
  vh2be(output->payload.have_rgba);


  LOGFT(ROD_LOG_OUTPUT, "\tFPS: \t%5.2f\n", DciFps);
  LOGFT(ROD_LOG_OUTPUT, "\tLine Rate: \t%5.2f %s lines/sec\n", NumberRoundDec(DciLineRate), NumberSuffixDec(DciLineRate));
  LOGFT(ROD_LOG_OUTPUT, "\tTriangle Rate: \t%5.2f %s triangles/sec\n", NumberRoundDec(DciTriangleRate), NumberSuffixDec(DciTriangleRate));
  LOGFT(ROD_LOG_OUTPUT, "\tFill Rate: \t%5.2f %s pixels/sec\n", NumberRoundDec(DciFillRate), NumberSuffixDec(DciFillRate)) ;
  LOGFT(ROD_LOG_OUTPUT, "\tTotal Time: \t%5.2f\n", DciTime);
  LOGFT(ROD_LOG_OUTPUT, "\tFrames: \t%d\n", DciFrames);

  output->header.param = ROD_OK;
  DciIsBegin = 0;
  return ROD_OK;
}
 */