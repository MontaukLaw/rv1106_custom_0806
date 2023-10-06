include ../build/Makefile.param

commLib_obj = rv1106
LIBPATH := ../lib/$(commLib_obj)
INC := ../include \
       ../include/test

ROCKCHIP_INC := -I$(commLib_inc)/rockchip
			
RK_INC :=   -I${SYSROOT}/usr/include/rockchip/  \
			-I${SYSROOT}/usr/include/rkaiq/uAPI/ \
			-I${SYSROOT}/usr/include/rkaiq/uAPI2/ \
			-I${SYSROOT}/usr/include/rkaiq/xcore/ \
			-I${SYSROOT}/usr/include/rkaiq \
			-I${SYSROOT}/usr/include/rkaiq/common \
			-I${SYSROOT}/usr/include/rkaiq/algos/adebayer \
			-I${SYSROOT}/usr/include/rkaiq/algos/ahdr \
			-I${SYSROOT}/usr/include/rkaiq/algos \
			-I${SYSROOT}/usr/include/rkaiq/iq_parser \
			-I${SYSROOT}/usr/include/rkaiq/iq_parser_v2 \
			-I${SYSROOT}/usr/include/rga \
			-I${SYSROOT}/usr/include \
			-I${SYSROOT}/usr/include/freetype2 \
			-I${SYSROOT}/usr/include
			
CODEC_RKMEDIA_INC := -I$(commLib_inc)/rkmedia
CODEC_RKMEDIA_INC += -I${MEDIA_PATH}/include/easymedia 

OBJPATH = ./obj_rv1106

SRCS-C := $(wildcard *.c)
OBJS-COMM := $(patsubst %.c,$(OBJPATH)/%.o,$(SRCS-C))

SRCS-CXX := $(wildcard *.cpp)
OBJS-COMM-CXX := $(patsubst %.cpp,$(OBJPATH)/%.o,$(SRCS-CXX))
OBJS := $(OBJS-COMM) $(OBJS-COMM-CXX)

# RGA_TEST_LIB_ROOT = /home/marc/rv1106/rv1106_sdk_custom/lib
# LIB_PATH = -L $(RGA_TEST_LIB_ROOT)
# TEST_LIBS := /home/marc/rv1106/rv1106_sdk_custom/lib/librt_test_comm.a
# TEST_RGA_LIB = -l rt_test_comm

TARGETLIB :=  $(LIBPATH)/libAI.a
all: $(TARGETLIB)	

$(OBJS-COMM):$(OBJPATH)/%.o:%.c
	@[ -e $(OBJPATH) ] || mkdir $(OBJPATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I$(INC)  $(RK_INC) -I$(commLib_inc) -I$(commLib_extra_inc) $(MPP_INC) $(CODEC_RKMEDIA_INC) 

$(OBJS-COMM-CXX):$(OBJPATH)/%.o:%.cpp
	@[ -e $(OBJPATH) ] || mkdir $(OBJPATH)
	$(CXX) $(CXXFLAGS) -o $@ -c $< -I$(INC)  $(RK_INC) -I$(commLib_inc) -I$(commLib_extra_inc) $(MPP_INC) $(CODEC_RKMEDIA_INC) 

$(TARGETLIB):$(OBJS)
	@[ -e $(LIBPATH) ] || mkdir $(LIBPATH)
	$(AR) rcv $@ $^ 
	
clean:
	-rm -rf  $(OBJS) $(TARGETLIB)
	
