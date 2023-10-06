include ../build/Makefile.param

commLib_obj = rv1106

RK_INC :=   -I${MEDIA_PATH}/include/rockchip/  \
			-I${MEDIA_PATH}/include/rkaiq/uAPI/ \
			-I${MEDIA_PATH}/include/rkaiq/uAPI2/ \
			-I${MEDIA_PATH}/include/rkaiq/xcore/ \
			-I${MEDIA_PATH}/include/rkaiq \
			-I${MEDIA_PATH}/include/rkaiq/common \
			-I${MEDIA_PATH}/include/rkaiq/algos/adebayer \
			-I${MEDIA_PATH}/include/rkaiq/algos/ahdr \
			-I${MEDIA_PATH}/include/rkaiq/algos \
			-I${MEDIA_PATH}/include/rkaiq/iq_parser \
			-I${MEDIA_PATH}/include/rkaiq/iq_parser_v2 \
			-I${MEDIA_PATH}/include/rga \
			-I${MEDIA_PATH}/include \
			-I${APP_PATH}/include/freetype2 \
			-I${APP_PATH}/include 

LIBS := -lsys
LIBS += -lapp
LIBS += -lAI
LIBS += -lasound
#LIBS += -lBase
LIBS += -lrtspServ
LIBS += -lcodec2
LIBS += -lnv12jpg
LIBS += -lmcu
LIBS += -lstreamCache
LIBS += -ltask
LIBS += -lmisc
LIBS += -lnetwork
LIBS += -lvendor_storage
LIBS += -lparam
LIBS += -llog
LIBS += -lgpio
LIBS += -lptz
LIBS += -lOnvif
LIBS += -lencnet
LIBS += -lsetting
LIBS += -lgb28181 -leXosip2 -lmxml -losip2 -losipparser2
LIBS += -lssl
LIBS += -lcrypto
LIBS += -lresolv
LIBS += -lrockit -lrockchip_mpp -lrkaiq -lrga -lrksysutils -lrockiva -lrknnmrt -lfreetype  -lpthread -lm -Wl,-rpath-link -ldl
LIBS +=	-ljson-c
LIBS += -lhd
LIBS += -lparted
LIBS += -lparted-fs-resize
LIBS += -latLib
LIBS += -lcurl

TEST_LIBS := /home/marc/rv1106/rv1106_sdk_custom/lib/librt_test_comm.a

LIBPATH := -L../lib/$(commLib_obj)
LIBPATH += -L../lib/comm
LIBPATH += -L/data/rockchip/RV1106G/media/lib
LIBPATH += -L/data/rockchip/RV1106G/app/lib
INC := ../include
OBJPATH = ./obj_rv1106

$(OBJPATH)/%.o:./%.c
	@[ -e $(OBJPATH) ] || mkdir $(OBJPATH)
	@echo "$(commLib_inc)"
	@echo "$(commLib_extra_inc)"
	$(CC) $(CFLAGS) -o $@ -c $< -I$(INC) $(RK_INC) -I$(commLib_inc) -I$(commLib_extra_inc)

SRCS = $(wildcard ./*.c)
OBJS = $(SRCS:./%.c=./$(OBJPATH)/%.o)
TARGETLIB :=  $(OBJPATH)/rv1106_ipc

all:$(TARGETLIB)
$(TARGETLIB):$(OBJS)
	@echo "build rv1106"	
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lm -lpthread -lrt $(LIBPATH) -lstdc++ ${TEST_LIBS}
	$(STRIP) $(TARGETLIB)
	cp $(TARGETLIB)  ../build

clean:
	-rm -rf $(OBJS) $(TARGETLIB)
