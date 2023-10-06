/*
 * This file is part of the dmidecode project.
 *
 *   Copyright (C) 2005-2008 Jean Delvare <khali@linux-fr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#ifndef __DMI_DECODE_H
#define __DMI_DECODE_H

#define DMI_STR_LEN	64
#define DMI_MAX_NIC    8
typedef struct
{
	char nic_name[32];
	char nic_mac[32];
}nic_info_t;
typedef struct __dmi_info_
{
	char bios_vendor[DMI_STR_LEN];
	char system_serial[DMI_STR_LEN];
	char system_uuid[DMI_STR_LEN];
	char board_serial[DMI_STR_LEN];
	char Chassis_serial[DMI_STR_LEN];
	nic_info_t nic_info;
}dmi_info_t;

/****************************
* return : 0, succesfull ; -1 ,faild
*****************************/
extern int dmi_init(void);
/******************************
* return dmi_info_t;
*******************************/
extern void dmi_get_info(dmi_info_t *pDmiINfo);
extern int dmi_get_nic_info(nic_info_t *nic_info_p);
extern void dmi_print_info(const dmi_info_t *pDmiInfo);
#endif

