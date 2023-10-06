#ifndef __HDLIB_H__
#define __HDLIB_H__

#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#ifndef WIN_FAT32_LBA
        #define WIN_FAT32_LBA   0x0c
#endif

#ifndef LINUX_NATIVE
        #define LINUX_NATIVE    0x83
#endif

#define	HD_LABEL_LEN	10
#define	HD_LABEL_RECORD	"vv_record"
#define	HD_LABEL_BACKUP	"vv_backup"

/* hdLib.c */
int comm_hd_initLib(); 
int comm_hd_deinitLib(); 

/* parted.c */
int comm_hd_partedDisk(int disk_id, int part_num, int sys_id);

/* mkext4fs */
int comm_hd_mkExt4Fs(int disk_id, int part_id, int flag);
int comm_hd_setExtFormatPercent(int percent);
int comm_hd_getExtFormatPercent(void);

/* mount.c */
int comm_hd_isMount(int disk_id, int part_id);
int comm_hd_getMountDir(int disk_id, int part_id, char *mount_dir);
int comm_hd_mount(int disk_id, int part_id, const char *target, int filesystem); //filesystem: 1: ext4/ext3,2: fat32 
int comm_hd_umount(const char *target);

/* hdparm.c */
int comm_hd_sleepHd(int disk_id);
int comm_hd_standbyNowHd(int disk_id);
int comm_hd_standbyHd(int disk_id, int seconds);
int comm_hd_wakeUpHd(int disk_id);

/* hdinfo */
int comm_hd_getDiskNum(void);
int comm_hd_getDiskName(int disk_id, char *disk_name);
int comm_hd_getDiskModel(int disk_id, char *model);
int comm_hd_getPartNum(int disk_id);
int comm_hd_isFdisk(int disk_id);
int comm_hd_getPartName(int disk_id, int part_id, char *part_name);
int comm_hd_getPartType(int disk_id, int part_id);
int comm_hd_isFormat(int disk_id, int part_id); 
long long comm_hd_getDiskTotalSize(int disk_id);
long long comm_hd_getPartTotalSize(int disk_id, int part_id);
long long comm_hd_getPartUsedSize(int disk_id, int part_id);
long long comm_hd_getPartFreeSize(int disk_id, int part_id);
int comm_hd_getPartLabel(int disk_id, int part_id, char *label);
int comm_hd_setPartLabel(int disk_id, int part_id, char *label);
int comm_hd_delPart(int disk_id, int part_id);

/* fsck.c */
int comm_hd_fsckExt4(int disk_id, int part_id);

/* hderr.c */
int comm_hd_getErrorNum(void);
int comm_hd_getErrorStr(int errorcode, char *buf, size_t n);
int comm_hd_printfErrorStr(const char *prefix);

#ifdef __cplusplus
}
#endif

#endif

