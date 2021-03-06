#ifndef __NMGLADAPTER_H__
#define __NMGLADAPTER_H__
#include "nmgl.h"

#define	GLboolean	NMGLboolean
#define	GLbyte		NMGLbyte
#define	GLubyte		NMGLubyte
#define	GLshort		NMGLshort
#define	GLushort	NMGLushort
#define	GLint		NMGLint
#define	GLuint		NMGLuint
#define	GLsizei		NMGLsizei
#define	GLfloat		NMGLfloat
#define	GLclampf	NMGLclampf
#define	GLdouble	NMGLdouble
#define	GLclampd	NMGLclampd
#define	GLenum		NMGLenum
#define	GLbitfield	NMGLbitfield
#define	GLvoid		NMGLvoid


#define GL_VERSION_ES_CL_1_0               NMGL_VERSION_ES_CL_1_0              
#define GL_VERSION_ES_CM_1_1               NMGL_VERSION_ES_CM_1_1              
#define GL_VERSION_ES_CL_1_1               NMGL_VERSION_ES_CL_1_1              
#define GL_DEPTH_BUFFER_BIT                NMGL_DEPTH_BUFFER_BIT               
#define GL_STENCIL_BUFFER_BIT              NMGL_STENCIL_BUFFER_BIT             
#define GL_COLOR_BUFFER_BIT                NMGL_COLOR_BUFFER_BIT               
#define GL_FALSE                           NMGL_FALSE                          
#define GL_TRUE                            NMGL_TRUE                           
#define GL_POINTS                          NMGL_POINTS                         
#define GL_LINES                           NMGL_LINES                          
#define GL_LINE_LOOP                       NMGL_LINE_LOOP                      
#define GL_LINE_STRIP                      NMGL_LINE_STRIP                     
#define GL_TRIANGLES                       NMGL_TRIANGLES                      
#define GL_TRIANGLE_STRIP                  NMGL_TRIANGLE_STRIP                 
#define GL_TRIANGLE_FAN                    NMGL_TRIANGLE_FAN                   
#define GL_NEVER                           NMGL_NEVER                          
#define GL_LESS                            NMGL_LESS                           
#define GL_EQUAL                           NMGL_EQUAL                          
#define GL_LEQUAL                          NMGL_LEQUAL                         
#define GL_GREATER                         NMGL_GREATER                        
#define GL_NOTEQUAL                        NMGL_NOTEQUAL                       
#define GL_GEQUAL                          NMGL_GEQUAL                         
#define GL_ALWAYS                          NMGL_ALWAYS                         
#define GL_ZERO                            NMGL_ZERO                           
#define GL_ONE                             NMGL_ONE                            
#define GL_SRC_COLOR                       NMGL_SRC_COLOR                      
#define GL_ONE_MINUS_SRC_COLOR             NMGL_ONE_MINUS_SRC_COLOR            
#define GL_SRC_ALPHA                       NMGL_SRC_ALPHA                      
#define GL_ONE_MINUS_SRC_ALPHA             NMGL_ONE_MINUS_SRC_ALPHA            
#define GL_DST_ALPHA                       NMGL_DST_ALPHA                      
#define GL_ONE_MINUS_DST_ALPHA             NMGL_ONE_MINUS_DST_ALPHA            
#define GL_DST_COLOR                       NMGL_DST_COLOR                      
#define GL_ONE_MINUS_DST_COLOR             NMGL_ONE_MINUS_DST_COLOR            
#define GL_SRC_ALPHA_SATURATE              NMGL_SRC_ALPHA_SATURATE             
#define GL_CLIP_PLANE0                     NMGL_CLIP_PLANE0                    
#define GL_CLIP_PLANE1                     NMGL_CLIP_PLANE1                    
#define GL_CLIP_PLANE2                     NMGL_CLIP_PLANE2                    
#define GL_CLIP_PLANE3                     NMGL_CLIP_PLANE3                    
#define GL_CLIP_PLANE4                     NMGL_CLIP_PLANE4                    
#define GL_CLIP_PLANE5                     NMGL_CLIP_PLANE5                    
#define GL_FRONT                           NMGL_FRONT                          
#define GL_BACK                            NMGL_BACK                           
#define GL_FRONT_AND_BACK                  NMGL_FRONT_AND_BACK                 
#define GL_FOG                             NMGL_FOG                            
#define GL_LIGHTING                        NMGL_LIGHTING                       
#define GL_TEXTURE_2D                      NMGL_TEXTURE_2D                     
#define GL_CULL_FACE                       NMGL_CULL_FACE                      
#define GL_ALPHA_TEST                      NMGL_ALPHA_TEST                     
#define GL_BLEND                           NMGL_BLEND                          
#define GL_COLOR_LOGIC_OP                  NMGL_COLOR_LOGIC_OP                 
#define GL_DITHER                          NMGL_DITHER                         
#define GL_STENCIL_TEST                    NMGL_STENCIL_TEST                   
#define GL_DEPTH_TEST                      NMGL_DEPTH_TEST                     
#define GL_POINT_SMOOTH                    NMGL_POINT_SMOOTH                   
#define GL_LINE_SMOOTH                     NMGL_LINE_SMOOTH                    
#define GL_SCISSOR_TEST                    NMGL_SCISSOR_TEST                   
#define GL_COLOR_MATERIAL                  NMGL_COLOR_MATERIAL                 
#define GL_NORMALIZE                       NMGL_NORMALIZE                      
#define GL_RESCALE_NORMAL                  NMGL_RESCALE_NORMAL                 
#define GL_VERTEX_ARRAY                    NMGL_VERTEX_ARRAY                   
#define GL_NORMAL_ARRAY                    NMGL_NORMAL_ARRAY                   
#define GL_COLOR_ARRAY                     NMGL_COLOR_ARRAY                    
#define GL_TEXTURE_COORD_ARRAY             NMGL_TEXTURE_COORD_ARRAY            
#define GL_MULTISAMPLE                     NMGL_MULTISAMPLE                    
#define GL_SAMPLE_ALPHA_TO_COVERAGE        NMGL_SAMPLE_ALPHA_TO_COVERAGE       
#define GL_SAMPLE_ALPHA_TO_ONE             NMGL_SAMPLE_ALPHA_TO_ONE            
#define GL_SAMPLE_COVERAGE                 NMGL_SAMPLE_COVERAGE                
#define GL_NO_ERROR                        NMGL_NO_ERROR                       
#define GL_INVALID_ENUM                    NMGL_INVALID_ENUM                   
#define GL_INVALID_VALUE                   NMGL_INVALID_VALUE                  
#define GL_INVALID_OPERATION               NMGL_INVALID_OPERATION              
#define GL_STACK_OVERFLOW                  NMGL_STACK_OVERFLOW                 
#define GL_STACK_UNDERFLOW                 NMGL_STACK_UNDERFLOW                
#define GL_OUT_OF_MEMORY                   NMGL_OUT_OF_MEMORY                  
#define GL_EXP                             NMGL_EXP                            
#define GL_EXP2                            NMGL_EXP2                           
#define GL_FOG_DENSITY                     NMGL_FOG_DENSITY                    
#define GL_FOG_START                       NMGL_FOG_START                      
#define GL_FOG_END                         NMGL_FOG_END                        
#define GL_FOG_MODE                        NMGL_FOG_MODE                       
#define GL_FOG_COLOR                       NMGL_FOG_COLOR                      
#define GL_CW                              NMGL_CW                             
#define GL_CCW                             NMGL_CCW                            
#define GL_CURRENT_COLOR                   NMGL_CURRENT_COLOR                  
#define GL_CURRENT_NORMAL                  NMGL_CURRENT_NORMAL                 
#define GL_CURRENT_TEXTURE_COORDS          NMGL_CURRENT_TEXTURE_COORDS         
#define GL_POINT_SIZE                      NMGL_POINT_SIZE                     
#define GL_POINT_SIZE_MIN                  NMGL_POINT_SIZE_MIN                 
#define GL_POINT_SIZE_MAX                  NMGL_POINT_SIZE_MAX                 
#define GL_POINT_FADE_THRESHOLD_SIZE       NMGL_POINT_FADE_THRESHOLD_SIZE      
#define GL_POINT_DISTANCE_ATTENUATION      NMGL_POINT_DISTANCE_ATTENUATION     
#define GL_SMOOTH_POINT_SIZE_RANGE         NMGL_SMOOTH_POINT_SIZE_RANGE        
#define GL_LINE_WIDTH                      NMGL_LINE_WIDTH                     
#define GL_SMOOTH_LINE_WIDTH_RANGE         NMGL_SMOOTH_LINE_WIDTH_RANGE        
#define GL_ALIASED_POINT_SIZE_RANGE        NMGL_ALIASED_POINT_SIZE_RANGE       
#define GL_ALIASED_LINE_WIDTH_RANGE        NMGL_ALIASED_LINE_WIDTH_RANGE       
#define GL_CULL_FACE_MODE                  NMGL_CULL_FACE_MODE                 
#define GL_FRONT_FACE                      NMGL_FRONT_FACE                     
#define GL_SHADE_MODEL                     NMGL_SHADE_MODEL                    
#define GL_DEPTH_RANGE                     NMGL_DEPTH_RANGE                    
#define GL_DEPTH_WRITEMASK                 NMGL_DEPTH_WRITEMASK                
#define GL_DEPTH_CLEAR_VALUE               NMGL_DEPTH_CLEAR_VALUE              
#define GL_DEPTH_FUNC                      NMGL_DEPTH_FUNC                     
#define GL_STENCIL_CLEAR_VALUE             NMGL_STENCIL_CLEAR_VALUE            
#define GL_STENCIL_FUNC                    NMGL_STENCIL_FUNC                   
#define GL_STENCIL_VALUE_MASK              NMGL_STENCIL_VALUE_MASK             
#define GL_STENCIL_FAIL                    NMGL_STENCIL_FAIL                   
#define GL_STENCIL_PASS_DEPTH_FAIL         NMGL_STENCIL_PASS_DEPTH_FAIL        
#define GL_STENCIL_PASS_DEPTH_PASS         NMGL_STENCIL_PASS_DEPTH_PASS        
#define GL_STENCIL_REF                     NMGL_STENCIL_REF                    
#define GL_STENCIL_WRITEMASK               NMGL_STENCIL_WRITEMASK              
#define GL_MATRIX_MODE                     NMGL_MATRIX_MODE                    
#define GL_VIEWPORT                        NMGL_VIEWPORT                       
#define GL_MODELVIEW_STACK_DEPTH           NMGL_MODELVIEW_STACK_DEPTH          
#define GL_PROJECTION_STACK_DEPTH          NMGL_PROJECTION_STACK_DEPTH         
#define GL_TEXTURE_STACK_DEPTH             NMGL_TEXTURE_STACK_DEPTH            
#define GL_MODELVIEW_MATRIX                NMGL_MODELVIEW_MATRIX               
#define GL_PROJECTION_MATRIX               NMGL_PROJECTION_MATRIX              
#define GL_TEXTURE_MATRIX                  NMGL_TEXTURE_MATRIX                 
#define GL_ALPHA_TEST_FUNC                 NMGL_ALPHA_TEST_FUNC                
#define GL_ALPHA_TEST_REF                  NMGL_ALPHA_TEST_REF                 
#define GL_BLEND_DST                       NMGL_BLEND_DST                      
#define GL_BLEND_SRC                       NMGL_BLEND_SRC                      
#define GL_LOGIC_OP_MODE                   NMGL_LOGIC_OP_MODE                  
#define GL_SCISSOR_BOX                     NMGL_SCISSOR_BOX                    
#define GL_COLOR_CLEAR_VALUE               NMGL_COLOR_CLEAR_VALUE              
#define GL_COLOR_WRITEMASK                 NMGL_COLOR_WRITEMASK                
#define GL_MAX_LIGHTS                      NMGL_MAX_LIGHTS                     
#define GL_MAX_CLIP_PLANES                 NMGL_MAX_CLIP_PLANES                
#define GL_MAX_TEXTURE_SIZE                NMGL_MAX_TEXTURE_SIZE               
#define GL_MAX_MODELVIEW_STACK_DEPTH       NMGL_MAX_MODELVIEW_STACK_DEPTH      
#define GL_MAX_PROJECTION_STACK_DEPTH      NMGL_MAX_PROJECTION_STACK_DEPTH     
#define GL_MAX_TEXTURE_STACK_DEPTH         NMGL_MAX_TEXTURE_STACK_DEPTH        
#define GL_MAX_VIEWPORT_DIMS               NMGL_MAX_VIEWPORT_DIMS              
#define GL_MAX_TEXTURE_UNITS               NMGL_MAX_TEXTURE_UNITS              
#define GL_SUBPIXEL_BITS                   NMGL_SUBPIXEL_BITS                  
#define GL_RED_BITS                        NMGL_RED_BITS                       
#define GL_GREEN_BITS                      NMGL_GREEN_BITS                     
#define GL_BLUE_BITS                       NMGL_BLUE_BITS                      
#define GL_ALPHA_BITS                      NMGL_ALPHA_BITS                     
#define GL_DEPTH_BITS                      NMGL_DEPTH_BITS                     
#define GL_STENCIL_BITS                    NMGL_STENCIL_BITS                   
#define GL_POLYGON_OFFSET_UNITS            NMGL_POLYGON_OFFSET_UNITS           
#define GL_POLYGON_OFFSET_FILL             NMGL_POLYGON_OFFSET_FILL            
#define GL_POLYGON_OFFSET_FACTOR           NMGL_POLYGON_OFFSET_FACTOR          
#define GL_TEXTURE_BINDING_2D              NMGL_TEXTURE_BINDING_2D             
#define GL_VERTEX_ARRAY_SIZE               NMGL_VERTEX_ARRAY_SIZE              
#define GL_VERTEX_ARRAY_TYPE               NMGL_VERTEX_ARRAY_TYPE              
#define GL_VERTEX_ARRAY_STRIDE             NMGL_VERTEX_ARRAY_STRIDE            
#define GL_NORMAL_ARRAY_TYPE               NMGL_NORMAL_ARRAY_TYPE              
#define GL_NORMAL_ARRAY_STRIDE             NMGL_NORMAL_ARRAY_STRIDE            
#define GL_COLOR_ARRAY_SIZE                NMGL_COLOR_ARRAY_SIZE               
#define GL_COLOR_ARRAY_TYPE                NMGL_COLOR_ARRAY_TYPE               
#define GL_COLOR_ARRAY_STRIDE              NMGL_COLOR_ARRAY_STRIDE             
#define GL_TEXTURE_COORD_ARRAY_SIZE        NMGL_TEXTURE_COORD_ARRAY_SIZE       
#define GL_TEXTURE_COORD_ARRAY_TYPE        NMGL_TEXTURE_COORD_ARRAY_TYPE       
#define GL_TEXTURE_COORD_ARRAY_STRIDE      NMGL_TEXTURE_COORD_ARRAY_STRIDE     
#define GL_VERTEX_ARRAY_POINTER            NMGL_VERTEX_ARRAY_POINTER           
#define GL_NORMAL_ARRAY_POINTER            NMGL_NORMAL_ARRAY_POINTER           
#define GL_COLOR_ARRAY_POINTER             NMGL_COLOR_ARRAY_POINTER            
#define GL_TEXTURE_COORD_ARRAY_POINTER     NMGL_TEXTURE_COORD_ARRAY_POINTER    
#define GL_SAMPLE_BUFFERS                  NMGL_SAMPLE_BUFFERS                 
#define GL_SAMPLES                         NMGL_SAMPLES                        
#define GL_SAMPLE_COVERAGE_VALUE           NMGL_SAMPLE_COVERAGE_VALUE          
#define GL_SAMPLE_COVERAGE_INVERT          NMGL_SAMPLE_COVERAGE_INVERT         
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS  NMGL_NUM_COMPRESSED_TEXTURE_FORMATS 
#define GL_COMPRESSED_TEXTURE_FORMATS      NMGL_COMPRESSED_TEXTURE_FORMATS     
#define GL_DONT_CARE                       NMGL_DONT_CARE                      
#define GL_FASTEST                         NMGL_FASTEST                        
#define GL_NICEST                          NMGL_NICEST                         
#define GL_PERSPECTIVE_CORRECTION_HINT     NMGL_PERSPECTIVE_CORRECTION_HINT    
#define GL_POINT_SMOOTH_HINT               NMGL_POINT_SMOOTH_HINT              
#define GL_LINE_SMOOTH_HINT                NMGL_LINE_SMOOTH_HINT               
#define GL_FOG_HINT                        NMGL_FOG_HINT                       
#define GL_GENERATE_MIPMAP_HINT            NMGL_GENERATE_MIPMAP_HINT           
#define GL_LIGHT_MODEL_AMBIENT             NMGL_LIGHT_MODEL_AMBIENT            
#define GL_LIGHT_MODEL_TWO_SIDE            NMGL_LIGHT_MODEL_TWO_SIDE           
#define GL_AMBIENT                         NMGL_AMBIENT                        
#define GL_DIFFUSE                         NMGL_DIFFUSE                        
#define GL_SPECULAR                        NMGL_SPECULAR                       
#define GL_POSITION                        NMGL_POSITION                       
#define GL_SPOT_DIRECTION                  NMGL_SPOT_DIRECTION                 
#define GL_SPOT_EXPONENT                   NMGL_SPOT_EXPONENT                  
#define GL_SPOT_CUTOFF                     NMGL_SPOT_CUTOFF                    
#define GL_CONSTANT_ATTENUATION            NMGL_CONSTANT_ATTENUATION           
#define GL_LINEAR_ATTENUATION              NMGL_LINEAR_ATTENUATION             
#define GL_QUADRATIC_ATTENUATION           NMGL_QUADRATIC_ATTENUATION          
#define GL_BYTE                            NMGL_BYTE                           
#define GL_UNSIGNED_BYTE                   NMGL_UNSIGNED_BYTE                  
#define GL_SHORT                           NMGL_SHORT                          
#define GL_UNSIGNED_SHORT                  NMGL_UNSIGNED_SHORT                 
#define GL_FLOAT                           NMGL_FLOAT                          
#define GL_FIXED                           NMGL_FIXED                          
#define GL_CLEAR                           NMGL_CLEAR                          
#define GL_AND                             NMGL_AND                            
#define GL_AND_REVERSE                     NMGL_AND_REVERSE                    
#define GL_COPY                            NMGL_COPY                           
#define GL_AND_INVERTED                    NMGL_AND_INVERTED                   
#define GL_NOOP                            NMGL_NOOP                           
#define GL_XOR                             NMGL_XOR                            
#define GL_OR                              NMGL_OR                             
#define GL_NOR                             NMGL_NOR                            
#define GL_EQUIV                           NMGL_EQUIV                          
#define GL_INVERT                          NMGL_INVERT                         
#define GL_OR_REVERSE                      NMGL_OR_REVERSE                     
#define GL_COPY_INVERTED                   NMGL_COPY_INVERTED                  
#define GL_OR_INVERTED                     NMGL_OR_INVERTED                    
#define GL_NAND                            NMGL_NAND                           
#define GL_SET                             NMGL_SET                            
#define GL_EMISSION                        NMGL_EMISSION                       
#define GL_SHININESS                       NMGL_SHININESS                      
#define GL_AMBIENT_AND_DIFFUSE             NMGL_AMBIENT_AND_DIFFUSE            
#define GL_MODELVIEW                       NMGL_MODELVIEW                      
#define GL_PROJECTION                      NMGL_PROJECTION                     
#define GL_TEXTURE                         NMGL_TEXTURE                        
#define GL_ALPHA                           NMGL_ALPHA                          
#define GL_RGB                             NMGL_RGB                            
#define GL_RGBA                            NMGL_RGBA                           
#define GL_LUMINANCE                       NMGL_LUMINANCE                      
#define GL_LUMINANCE_ALPHA                 NMGL_LUMINANCE_ALPHA                
#define GL_UNPACK_ALIGNMENT                NMGL_UNPACK_ALIGNMENT               
#define GL_PACK_ALIGNMENT                  NMGL_PACK_ALIGNMENT                 
#define GL_UNSIGNED_SHORT_4_4_4_4          NMGL_UNSIGNED_SHORT_4_4_4_4         
#define GL_UNSIGNED_SHORT_5_5_5_1          NMGL_UNSIGNED_SHORT_5_5_5_1         
#define GL_UNSIGNED_SHORT_5_6_5            NMGL_UNSIGNED_SHORT_5_6_5           
#define GL_FLAT                            NMGL_FLAT                           
#define GL_SMOOTH                          NMGL_SMOOTH                         
#define GL_KEEP                            NMGL_KEEP                           
#define GL_REPLACE                         NMGL_REPLACE                        
#define GL_INCR                            NMGL_INCR                           
#define GL_DECR                            NMGL_DECR                           
#define GL_VENDOR                          NMGL_VENDOR                         
#define GL_RENDERER                        NMGL_RENDERER                       
#define GL_VERSION                         NMGL_VERSION                        
#define GL_EXTENSIONS                      NMGL_EXTENSIONS                     
#define GL_MODULATE                        NMGL_MODULATE                       
#define GL_DECAL                           NMGL_DECAL                          
#define GL_ADD                             NMGL_ADD                            
#define GL_TEXTURE_ENV_MODE                NMGL_TEXTURE_ENV_MODE               
#define GL_TEXTURE_ENV_COLOR               NMGL_TEXTURE_ENV_COLOR              
#define GL_TEXTURE_ENV                     NMGL_TEXTURE_ENV                    
#define GL_NEAREST                         NMGL_NEAREST                        
#define GL_LINEAR                          NMGL_LINEAR                         
#define GL_NEAREST_MIPMAP_NEAREST          NMGL_NEAREST_MIPMAP_NEAREST         
#define GL_LINEAR_MIPMAP_NEAREST           NMGL_LINEAR_MIPMAP_NEAREST          
#define GL_NEAREST_MIPMAP_LINEAR           NMGL_NEAREST_MIPMAP_LINEAR          
#define GL_LINEAR_MIPMAP_LINEAR            NMGL_LINEAR_MIPMAP_LINEAR           
#define GL_TEXTURE_MAG_FILTER              NMGL_TEXTURE_MAG_FILTER             
#define GL_TEXTURE_MIN_FILTER              NMGL_TEXTURE_MIN_FILTER             
#define GL_TEXTURE_WRAP_S                  NMGL_TEXTURE_WRAP_S                 
#define GL_TEXTURE_WRAP_T                  NMGL_TEXTURE_WRAP_T                 
#define GL_GENERATE_MIPMAP                 NMGL_GENERATE_MIPMAP                
#define GL_TEXTURE0                        NMGL_TEXTURE0                       
#define GL_TEXTURE1                        NMGL_TEXTURE1                       
#define GL_TEXTURE2                        NMGL_TEXTURE2                       
#define GL_TEXTURE3                        NMGL_TEXTURE3                       
#define GL_TEXTURE4                        NMGL_TEXTURE4                       
#define GL_TEXTURE5                        NMGL_TEXTURE5                       
#define GL_TEXTURE6                        NMGL_TEXTURE6                       
#define GL_TEXTURE7                        NMGL_TEXTURE7                       
#define GL_TEXTURE8                        NMGL_TEXTURE8                       
#define GL_TEXTURE9                        NMGL_TEXTURE9                       
#define GL_TEXTURE10                       NMGL_TEXTURE10                      
#define GL_TEXTURE11                       NMGL_TEXTURE11                      
#define GL_TEXTURE12                       NMGL_TEXTURE12                      
#define GL_TEXTURE13                       NMGL_TEXTURE13                      
#define GL_TEXTURE14                       NMGL_TEXTURE14                      
#define GL_TEXTURE15                       NMGL_TEXTURE15                      
#define GL_TEXTURE16                       NMGL_TEXTURE16                      
#define GL_TEXTURE17                       NMGL_TEXTURE17                      
#define GL_TEXTURE18                       NMGL_TEXTURE18                      
#define GL_TEXTURE19                       NMGL_TEXTURE19                      
#define GL_TEXTURE20                       NMGL_TEXTURE20                      
#define GL_TEXTURE21                       NMGL_TEXTURE21                      
#define GL_TEXTURE22                       NMGL_TEXTURE22                      
#define GL_TEXTURE23                       NMGL_TEXTURE23                      
#define GL_TEXTURE24                       NMGL_TEXTURE24                      
#define GL_TEXTURE25                       NMGL_TEXTURE25                      
#define GL_TEXTURE26                       NMGL_TEXTURE26                      
#define GL_TEXTURE27                       NMGL_TEXTURE27                      
#define GL_TEXTURE28                       NMGL_TEXTURE28                      
#define GL_TEXTURE29                       NMGL_TEXTURE29                      
#define GL_TEXTURE30                       NMGL_TEXTURE30                      
#define GL_TEXTURE31                       NMGL_TEXTURE31                      
#define GL_ACTIVE_TEXTURE                  NMGL_ACTIVE_TEXTURE                 
#define GL_CLIENT_ACTIVE_TEXTURE           NMGL_CLIENT_ACTIVE_TEXTURE          
#define GL_REPEAT                          NMGL_REPEAT                         
#define GL_CLAMP_TO_EDGE                   NMGL_CLAMP_TO_EDGE                  
#define GL_LIGHT0                          NMGL_LIGHT0                         
#define GL_LIGHT1                          NMGL_LIGHT1                         
#define GL_LIGHT2                          NMGL_LIGHT2                         
#define GL_LIGHT3                          NMGL_LIGHT3                         
#define GL_LIGHT4                          NMGL_LIGHT4                         
#define GL_LIGHT5                          NMGL_LIGHT5                         
#define GL_LIGHT6                          NMGL_LIGHT6                         
#define GL_LIGHT7                          NMGL_LIGHT7                         
#define GL_ARRAY_BUFFER                    NMGL_ARRAY_BUFFER                   
#define GL_ELEMENT_ARRAY_BUFFER            NMGL_ELEMENT_ARRAY_BUFFER           
#define GL_ARRAY_BUFFER_BINDING            NMGL_ARRAY_BUFFER_BINDING           
#define GL_ELEMENT_ARRAY_BUFFER_BINDING    NMGL_ELEMENT_ARRAY_BUFFER_BINDING   
#define GL_VERTEX_ARRAY_BUFFER_BINDING     NMGL_VERTEX_ARRAY_BUFFER_BINDING    
#define GL_NORMAL_ARRAY_BUFFER_BINDING     NMGL_NORMAL_ARRAY_BUFFER_BINDING    
#define GL_COLOR_ARRAY_BUFFER_BINDING      NMGL_COLOR_ARRAY_BUFFER_BINDING     
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BIND NMGL_TEXTURE_COORD_ARRAY_BUFFER_BIND
#define GL_STATIC_DRAW                     NMGL_STATIC_DRAW                    
#define GL_DYNAMIC_DRAW                    NMGL_DYNAMIC_DRAW                   
#define GL_BUFFER_SIZE                     NMGL_BUFFER_SIZE                    
#define GL_BUFFER_USAGE                    NMGL_BUFFER_USAGE                   
#define GL_SUBTRACT                        NMGL_SUBTRACT                       
#define GL_COMBINE                         NMGL_COMBINE                        
#define GL_COMBINE_RGB                     NMGL_COMBINE_RGB                    
#define GL_COMBINE_ALPHA                   NMGL_COMBINE_ALPHA                  
#define GL_RGB_SCALE                       NMGL_RGB_SCALE                      
#define GL_ADD_SIGNED                      NMGL_ADD_SIGNED                     
#define GL_INTERPOLATE                     NMGL_INTERPOLATE                    
#define GL_CONSTANT                        NMGL_CONSTANT                       
#define GL_PRIMARY_COLOR                   NMGL_PRIMARY_COLOR                  
#define GL_PREVIOUS                        NMGL_PREVIOUS                       
#define GL_OPERAND0_RGB                    NMGL_OPERAND0_RGB                   
#define GL_OPERAND1_RGB                    NMGL_OPERAND1_RGB                   
#define GL_OPERAND2_RGB                    NMGL_OPERAND2_RGB                   
#define GL_OPERAND0_ALPHA                  NMGL_OPERAND0_ALPHA                 
#define GL_OPERAND1_ALPHA                  NMGL_OPERAND1_ALPHA                 
#define GL_OPERAND2_ALPHA                  NMGL_OPERAND2_ALPHA                 
#define GL_ALPHA_SCALE                     NMGL_ALPHA_SCALE                    
#define GL_SRC0_RGB                        NMGL_SRC0_RGB                       
#define GL_SRC1_RGB                        NMGL_SRC1_RGB                       
#define GL_SRC2_RGB                        NMGL_SRC2_RGB                       
#define GL_SRC0_ALPHA                      NMGL_SRC0_ALPHA                     
#define GL_SRC1_ALPHA                      NMGL_SRC1_ALPHA                     
#define GL_SRC2_ALPHA                      NMGL_SRC2_ALPHA                     
#define GL_DOT3_RGB                        NMGL_DOT3_RGB                       
#define GL_DOT3_RGBA                       NMGL_DOT3_RGBA           

#define glActiveTexture                nmglActiveTexture 
#define glAlphaFunc                    nmglAlphaFunc 
#define glBegin                        nmglBegin
#define glBindTexture                  nmglBindTexture 
#define glBitmap                       nmglBitmap 
#define glBlendFunc                    nmglBlendFunc 
#define glCallLists                    nmglCallLists 
#define glClear                        nmglClear 
#define glClearColor                   nmglClearColor
#define glClearDepthf                  nmglClearDepthf 
#define glClearStencil                 nmglClearStencil 
#define glClientActiveTexture          nmglClientActiveTexture 
#define glColor4f                      nmglColor4f 
#define glColor4fv                     nmglColor4fv 
#define glColor4ub                     nmglColor4ub 
#define glColorMask                    nmglColorMask 
#define glColorPointer                 nmglColorPointer 
#define glColorSubTableEXT             nmglColorSubTableEXT 
#define glColorTableEXT                nmglColorTableEXT 
#define glCopyPixels                   nmglCopyPixels 
#define glCullFace                     nmglCullFace 
#define glDepthFunc                    nmglDepthFunc 
#define glDepthMask                    nmglDepthMask 
#define glDepthRangef                  nmglDepthRangef 
#define glDisable                      nmglDisable 
#define glDisableClientState           nmglDisableClientState 
#define glDrawArrays                   nmglDrawArrays 
#define glDrawElements                 nmglDrawElements 
#define glDrawPixels                   nmglDrawPixels 
#define glEnable                       nmglEnable 
#define glEnableClientState            nmglEnableClientState 
#define glEnd                          nmglEnd 
#define glEndList                      nmglEndList 
#define glFinish                       nmglFinish 
#define glFlush                        nmglFlush 
#define glFrontFace                    nmglFrontFace 
#define glFrustumf                     nmglFrustumf 
#define glGenLists                     nmglGenLists 
#define glGenTextures                  nmglGenTextures 
#define glGetError                     nmglGetError 
#define glGetBooleanv                  nmglGetBooleanv 
#define glGetColorTableEXT             nmglGetColorTableEXT 
#define glGetColorTableParameterivEXT  nmglGetColorTableParameterivEXT 
#define glGetFloatv                    nmglGetFloatv 
#define glGetIntegerv                  nmglGetIntegerv 
#define glGetLightfv                   nmglGetLightfv 
#define glGetMaterialfv                nmglGetMaterialfv 
#define glGetPointerv                  nmglGetPointerv 
#define glGetPolygonStipple            nmglGetPolygonStipple
#define glGetTexEnvfv                  nmglGetTexEnvfv 
#define glGetTexEnviv                  nmglGetTexEnviv 
#define glGetTexParameteriv            nmglGetTexParameteriv 
#define glGetString                    nmglGetString 
#define glHint 						nmglHint 
#define glIsEnabled 				nmglIsEnabled 
#define glLightfv 					nmglLightfv 				
#define glLightModelfv 				nmglLightModelfv 			
#define glLineStipple 				nmglLineStipple 			
#define glLineWidth 				nmglLineWidth 			
#define glListBase 					nmglListBase 				
#define glLoadIdentity 				nmglLoadIdentity 			
#define glLoadMatrixf 				nmglLoadMatrixf 			
#define glMaterialf 				nmglMaterialf 			
#define glMaterialfv 				nmglMaterialfv 			
#define glMatrixMode 				nmglMatrixMode 			
#define glMultMatrixf 				nmglMultMatrixf 			
#define glMultiTexCoord2f 			nmglMultiTexCoord2f 		
#define glMultiTexCoord2fv 			nmglMultiTexCoord2fv 	
#define glNewList 					nmglNewList 				
#define glNormal3f 					nmglNormal3f 				
#define glNormal3fv                 nmglNormal3fv 
#define glNormalPointer             nmglNormalPointer 
#define glNormalPointerNM           nmglNormalPointerNM 
#define glOrthof                    nmglOrthof 
#define glPixelStorei               nmglPixelStorei 
#define glPointSize                 nmglPointSize 
#define glPolygonOffset             nmglPolygonOffset 
#define glPolygonStipple            nmglPolygonStipple 
#define glPopMatrix                 nmglPopMatrix 
#define glPushMatrix                nmglPushMatrix 
#define glRasterPos3f               nmglRasterPos3f 
#define glReadPixels                nmglReadPixels 
#define glRotatef                   nmglRotatef 
#define glScalef                    nmglScalef 
#define glScissor                   nmglScissor 
#define glShadeModel                nmglShadeModel 
#define glStencilFunc               nmglStencilFunc 
#define glStencilMask               nmglStencilMask 
#define glStencilOp                 nmglStencilOp 
#define glTexCoordPointer           nmglTexCoordPointer 
#define glTexEnvfv                  nmglTexEnvfv 
#define glTexEnvi                   nmglTexEnvi 
#define glTexImage2D                nmglTexImage2D 
#define glTexParameteri             nmglTexParameteri 
#define glTexSubImage2D             nmglTexSubImage2D 
#define glTranslatef                nmglTranslatef 
#define glVertex2f                  nmglVertex2f 
#define glVertex2fv                 nmglVertex2fv 
#define glVertex3f                  nmglVertex3f 
#define glVertex3fv                 nmglVertex3fv 
#define glVertexPointer             nmglVertexPointer 
#define glViewport                  nmglViewport 
	
#endif
