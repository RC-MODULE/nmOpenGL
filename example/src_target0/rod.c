#include <rod.h>
#include <time.h>

#ifdef ROD_NANOTIME
  double Nanotime()
  {
    struct timespec ts;
 //   clock_gettime(CLOCK_REALTIME, &ts);
    return 1e-9 * ts.tv_nsec + ts.tv_sec;
  }
#endif
#ifdef ROD_EPOCH_TIME
  double NanotimeEpoch()
  {
    struct timeval time;
    gettimeofday(&time, NULL);
    return 1.0e-9 * time.tv_usec + time.tv_sec;
  }
#endif


static const char* LogTypes[] = {
  0,
  "[EE]",
  "[WW]",
  "[DBG]",
  "[II]",
  "[OUTPUT]"
};

static const char* SizePostfixListDec[] = {
  "",
  "thousand",
  "million",
  "billion",
  "trillion",
  "quadrillion",
  "quintillion",
  "sextillion",
  "septillion",
};

#define SIZEPOSTFIXLIST_DEC (COUNT(SizePostfixListDec) - 1)

static const char* SizePostfixListBin[] = {
  "B",
  "KiB",
  "MiB",
  "GiB",
  "TiB",
  "PiB",
  "EiB",
  "ZiB",
  "YiB",
};

#define SIZEPOSTFIXLIST_BIN (COUNT(SizePostfixListBin) - 1 )


const char* SizeSuffixBin(double __n)
{
  volatile double n = __n;
  volatile int16_t endian = 1;
  volatile uint32_t * _n = (uint32_t*)&n;
  volatile int16_t e;
  endian = *(char*)&endian;

  e = ((_n[endian] >> 20) & 0x7FF) - 1023;
  if (e < 0) return SizePostfixListBin[0];
  e /= 10;
  if (e > (int)SIZEPOSTFIXLIST_BIN) return SizePostfixListBin[SIZEPOSTFIXLIST_BIN];
  return SizePostfixListBin[e];
}

double SizeRoundBin(double __n)
{
  volatile double n = __n;
  volatile int16_t endian = 1;
  volatile uint32_t* _n = (uint32_t*)&n;
  volatile int16_t ee, e;
  endian = *(char*)&endian;

  e = ((_n[endian] >> 20) & 0x7FF) - 1023;
  if (e < 0) return 0.0;
  ee = e % 10;
  e /= 10;
  if (e > (int)SIZEPOSTFIXLIST_BIN) e -= 8; else e = 0;
  e = e * 10 + ee + 1023;
  _n[endian] = (_n[endian] & 0x800FFFFF) + (e << 20);

  return n;
}

double NumberRoundDec(double n)
{
  while (n >= 1e3 || n <= -1e3)
  {
    n /= 1e3;
  }
  return n;
}

const char* NumberSuffixDec(double n)
{
  int i = 0;
  while (n >= 1e3 || n <= -1e3)
  {
    n /= 1e3;
    ++i;
  }
  if (i > (int)SIZEPOSTFIXLIST_DEC)
    i = SIZEPOSTFIXLIST_DEC;
  return SizePostfixListDec[i];
}

int32_t RodCheckGLFilter(int32_t filter)
{
  int32_t fail = 0;
  switch(filter)
  {
    case GL_NEAREST:
    case GL_LINEAR:
      break;
    default:
      fail|=0x01;
  }
  return fail;
}

int32_t RodCheckGLWrap(int32_t wrap)
{
  int32_t fail = 0;
  switch(wrap)
  {
    case GL_REPEAT:
    case GL_CLAMP_TO_EDGE:
      break;
    default:
      fail|=0x01;
  }
  return fail;
}


int32_t RodCheckGLDepth(int32_t func)
{
  int32_t fail = 0;
  switch(func)
  {
    case GL_NEVER:
    case GL_LESS:
    case GL_EQUAL:
    case GL_LEQUAL:
    case GL_GREATER:
    case GL_NOTEQUAL:
    case GL_GEQUAL:
    case GL_ALWAYS:
      break;
    default:
      fail|=0x01;
  }
  return fail;
}

int32_t RodCheckGLBlend(int32_t sfactor, int32_t dfactor)
{
  int32_t fail = 0;
  switch(sfactor)
  {
    case GL_ZERO:
    case GL_ONE:
    case GL_SRC_ALPHA_SATURATE:
      break;
    default:
      fail|=0x01;
  }
  switch(dfactor)
  {
    case GL_ZERO:
    case GL_ONE:
    case GL_ONE_MINUS_SRC_ALPHA:
      break;
    default:
      fail|=0x02;
  }
  return fail;
}


int32_t RodCheckGLAlpha(int32_t func)
{
  int32_t fail = 0;
  switch(func)
  {
    case GL_NEVER:
    case GL_LESS:
    case GL_EQUAL:
    case GL_LEQUAL:
    case GL_GREATER:
    case GL_NOTEQUAL:
    case GL_GEQUAL:
    case GL_ALWAYS:
      break;
    default:
      fail|=0x01;
  }
  return fail;
}

#define CASE_RET_STR(dd) case dd: return  #dd; break;

const char * WhichToString(GLenum which)
{
  switch (which)
  {
    CASE_RET_STR(GL_POINTS)
    CASE_RET_STR(GL_LINES)
    CASE_RET_STR(GL_LINE_LOOP)
    CASE_RET_STR(GL_LINE_STRIP)
    CASE_RET_STR(GL_TRIANGLES)
    CASE_RET_STR(GL_TRIANGLE_STRIP)
    CASE_RET_STR(GL_TRIANGLE_FAN)
  default:
    break;
  }
  return "UNKNOWN";
}

const char * LogTypeToString(int type)
{
  if ((type) < (int)(COUNT(LogTypes)))
  {
    return LogTypes[type];
  }
  return "UNKNOWN";
}
