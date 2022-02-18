#include "utility_float.h"
#include "uassert.h"
#include "extnmtype.h"
#include "debugprint.h"
#include "section-hal.h"

#define MAX_SIZE 1024

#ifdef __GNUC__
__attribute__((section(".data_imu0"))) v4nm32f src0[MAX_SIZE];
__attribute__((section(".data_imu0"))) v4nm32f src1[MAX_SIZE];
__attribute__((section(".data_imu0"))) v2nm32f dst[MAX_SIZE + 1];

__attribute__((section(".data_imu0"))) v2nm32f expected[MAX_SIZE + 1];
#else
v4nm32f src0[MAX_SIZE];
v4nm32f src1[MAX_SIZE];
v2nm32f dst[MAX_SIZE + 1];

v2nm32f expected[MAX_SIZE + 1];
#endif



void valueTest(){
    int sizes[] = {5, 24, 32, 36, 64, 96, 128, 530, 1024 };

    v4nm32f src_var[] = { {0, 0, 0, 0}, {-2, 2, -4, -5}, {4, 5, 6, 7}, { -1, 2, 1,-2 }};
    for(int s = 0; s < sizeof(sizes) / sizeof(int); s++){
        int size = sizes[s];

        for(int i0 = 0; i0 < sizeof(src_var) / sizeof(v4nm32f); i0++){
            for(int i1 = 0; i1 < sizeof(src_var) / sizeof(v4nm32f); i1++){


                for(int i = 0; i < size; i++){        
                    src0[i] = src_var[i0];
                    src1[i] = src_var[i1];
                    expected[i].v0  = 0;
                    for(int j = 0; j < 4; j++){
                        expected[i].v0 += src0[i].vec[j] * src1[i].vec[j];
                    }
                    expected[i].v1 = expected[i].v0;
                }



                dotV_v4nm32f(src0, src1, dst, size);
                for(int i = 0; i < size; i++){
                    DEBUG_PLOG_LEVEL_1("%3d: dst= {%f, %f}, expect={%f, %f}\n", i,
                                    dst[i].v0, dst[i].v1, 
                                    expected[i].v0, expected[i].v1 );
                    uassert(dst[i] == expected[i]);
                }



                for(int i = 0; i < size; i++){
                    expected[i].v0 = (expected[i].v0 > 0) ? expected[i].v0: 0;
                    expected[i].v1 = expected[i].v0;
                }

                dotV_gt0_v4nm32f(src0, src1, dst, size);

                for(int i = 0; i < size; i++){
                    uassert(dst[i] == expected[i]);
                }


                
            }
        }
    }
    DEBUG_PLOG_LEVEL_0("Test value OK!\n");
}
void sizeTest(){
    for(int i = 0; i < MAX_SIZE + 1; i++){
        dst[i] = { -1, -1 };
    }
    for(int i = 0; i < MAX_SIZE; i++){
        src0[i] = {1,1,1,1};
        src1[i] = {1,1,1,1};
    }
    v2nm32f excepted = { -1, -1 };
    for(int size = 0; size <= MAX_SIZE; size++){
        dotV_v4nm32f(src0, src1, dst, size);

        uassert(dst[size] == excepted);

        if(size == 0) continue;

        uassert(dst[size - 1] != excepted);

    }
    DEBUG_PLOG_LEVEL_0("Test size OK!\n");
}

int main(){
    valueTest();
    sizeTest();
    return 0;
}