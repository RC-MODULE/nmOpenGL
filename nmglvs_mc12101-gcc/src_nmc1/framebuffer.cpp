#include "framebuffer.h"
#include "demo3d_nm1.h"

void NMGL_FrameBufferInit(NMGL_FrameBuffer *fb, int width, int height){
    fb->head = 0;
    fb->tail = 0;
    fb->sizeOfAddr = sizeof(void *);
    fb->sizeOfInt = sizeof(int);
    fb->width = width;
    fb->height = height;
}
inline void fbReadDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex);
inline void fbWriteDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex);
inline void fbReadRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex);
inline void fbWriteRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex);



void readColorBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbReadDMA(data, fb, x, y, width, height, 0);
}

void writeColorBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbWriteDMA(data, fb, x, y, width, height, 0);
}

void readDepthBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbReadDMA(data, fb, x, y, width, height, 2);
}

void writeDepthBufferDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbWriteDMA(data, fb, x, y, width, height, 2);
}



void readColorBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbReadRISC(data, fb, x, y, width, height, 0);
}

void writeColorBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbWriteRISC(data, fb, x, y, width, height, 0);
}

void readDepthBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbReadRISC(data, fb, x, y, width, height, 2);
}

void writeDepthBufferRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
    fbWriteRISC(data, fb, x, y, width, height, 2);
}

inline void fbReadDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex)
{

    MyDmaTask task;
    task.type = MSD_DMA_2D;
    task.size = width * height;
    task.width = width;

    int offset = y * fb->width + x;

    task.src = (int *)fb->buffers[bufferIndex] + offset;
    task.dst = data;
    task.srcStride = fb->width;
    task.dstStride = width;
    msdAdd(task, 1);
    //msdStart();
}

inline void fbWriteDMA(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex)
{

    MyDmaTask task;
    task.type = MSD_DMA_2D;
    task.size = width * height;
    task.width = width;

    int offset = y * fb->width + x;

    task.src = data;
    task.dst = (int *)fb->buffers[bufferIndex] + offset;
    task.srcStride = width;
    task.dstStride = fb->width;
    msdAdd(task, 1);

    //msdStart();
}
inline void fbReadRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex)
{

   for(int j = 0; j < height; j++){
       for(int i = 0; i < width; i++){
           ((int *)data)[j * width + i] = ((int *)fb->buffers[bufferIndex])[(j + y) * fb->width + (i + x)];
       }
   }
}

inline void fbWriteRISC(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height, int bufferIndex)
{
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            ((int *)fb->buffers[bufferIndex])[(j + y) * fb->width + (i + x)] = ((int *)data)[j * width + i];
        }
    }
}