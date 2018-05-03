#ifndef _DX_CONFIGURE_H
#define _DX_CONFIGURE_H


#define APP_FORMWARE       			 "/mnt/gyro/gyro_main.out"
#define SCRIPT_FILE         			 "/config/default.txt"
#define SYSCONFIG_FILE0     			 "/config/config_0.txt"
#define SYSCONFIG_FILE1     			 "/config/config_1.txt"
#define SYSCONFIG_FILE2     			 "/config/config_2.txt"

#define SYSCFG_SAVE_FILE1   			"/mnt/ubi/config_1.txt"
#define SYSCFG_SAVE_FILE2   			"/mnt/ubi/config_2.txt"

#define CFGID_BUILD( blkId, feildId )   ( ( (( blkId) << 4 ) & 0xFF0 ) | ( feildId ) )
#define CFGID_blkId( cfgId )            ( ( (cfgId) >> 4 ) & 0xFF )
#define CFGID_feildId( cfgId )          ( cfgId & 0xF )

//Device information
#define BLOCK_BASE_DEVICE            					          (1)
#define CFGID_DEV_CFG_TAG               					          CFGID_BUILD( BLOCK_BASE_DEVICE, 0 )
#define CFGID_DEV_CFG_IP                						   CFGID_BUILD( BLOCK_BASE_DEVICE, 1 )
#define CFGID_DEV_CFG_IP_MASK            					   CFGID_BUILD( BLOCK_BASE_DEVICE, 2 )
#define CFGID_DEV_CFG_GATEWAY             					   CFGID_BUILD( BLOCK_BASE_DEVICE, 3 )
#define CFGID_DEV_CFG_MSG_UDP_PORT       				   CFGID_BUILD( BLOCK_BASE_DEVICE, 4 )
#define CFGID_DEV_CFG_PING_TIMEOUT        				   CFGID_BUILD( BLOCK_BASE_DEVICE, 5 )

//Real time ctrl blck 0
#define BLOCK_BASE_RTCTRL                						  (2)
#define CFGID_RT_CTRL_0_TAG             					CFGID_BUILD(BLOCK_BASE_RTCTRL, 0)
#define CFGID_RT_CTRL_WORK_MOD           				CFGID_BUILD(BLOCK_BASE_RTCTRL, 1)
#define CFGID_RT_CTRL_BOOT_DELAY       					CFGID_BUILD(BLOCK_BASE_RTCTRL, 2)
#define CFGID_RT_CTRL_BOARD_DATE         				CFGID_BUILD(BLOCK_BASE_RTCTRL, 3)
#define CFGID_RT_CTRL_BOARD_TIME         				CFGID_BUILD(BLOCK_BASE_RTCTRL, 4)
#define CFGID_RT_CTRL_AUTO_CHECK        				CFGID_BUILD(BLOCK_BASE_RTCTRL, 5)
#define CFGID_RT_CTRL_MAIN_SENSOR        				CFGID_BUILD(BLOCK_BASE_RTCTRL, 6)
#define CFGID_RT_CTRL_INIT_OSD_ID          				CFGID_BUILD(BLOCK_BASE_RTCTRL, 7)

//blk15 video display dev
#define BLOCK_BASE_VDIS                  						  (15)
#define CFGID_VDIS_TAG                     						  CFGID_BUILD(BLOCK_BASE_VDIS, 0)
#define CFGID_VDIS_HDMI_ENABLE            					  CFGID_BUILD(BLOCK_BASE_VDIS, 1)
#define CFGID_VDIS_HDMI_RESOLUTION        				  CFGID_BUILD(BLOCK_BASE_VDIS, 2)
#define CFGID_VDIS_HDCOMP_ENABLE          				  CFGID_BUILD(BLOCK_BASE_VDIS, 3)
#define CFGID_VDIS_HDCOMP_RESOLUTION     				  CFGID_BUILD(BLOCK_BASE_VDIS, 4)
#define CFGID_VDIS_DVO2_ENABLE            					  CFGID_BUILD(BLOCK_BASE_VDIS, 5)
#define CFGID_VDIS_DVO2_RESOLUTION         				  CFGID_BUILD(BLOCK_BASE_VDIS, 6)
#define CFGID_VDIS_SD_ENABLE              					  CFGID_BUILD(BLOCK_BASE_VDIS, 7)
#define CFGID_VDIS_SD_RESOLUTION         				  CFGID_BUILD(BLOCK_BASE_VDIS, 8)
#define CFGID_VDIS_VENC_TIED             					  CFGID_BUILD(BLOCK_BASE_VDIS, 9) // 0:HDMI - HHCOMP 1:HDMI - DVO2 2:HDCOMP - DVO2

//blk16 sensor tv cfg
#define BLOCK_BASE_SENSOR_TV             					  (16)
#define CFGID_SENSOR_TV_0_TAG             					 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 0)
#define CFGID_SENSOR_TV_VDIS_DEV        					 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 1)
#define CFGID_SENSOR_TV_VDIS_CHAN       			        CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 2)
#define CFGID_SENSOR_TV_WIDTH              				 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 3)
#define CFGID_SENSOR_TV_HEIGHT             				 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 4)
#define CFGID_SENSOR_TV_SWMS_LAYOUT        			 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 5) //0 full,1 picture in picture,2 all video
#define CFGID_SENSOR_TV_LAYOUT_PRM_UPDATE  			 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 6) // 0 : NO 1: off
#define CFGID_SENSOR_TV_PICP_CHAN          				 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 7) // 0 tv, 1 flr
#define CFGID_SENSOR_TV_PICP_LAYOUT_POS   			 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 8) // 0: TL 1: TR 2:BL 3:BR
#define CFGID_SENSOR_TV_PICP_WIDTH        				 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 9) // picp width
#define CFGID_SENSOR_TV_PICP_HEIGHT   				        CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 10) // picp height
#define CFGID_SENSOR_TV_PICP_POS        					 CFGID_BUILD(BLOCK_BASE_SENSOR_TV, 11) // picp height

//blk17 sensor flr cfg
#define  BLOCK_BASE_SENSOR_FR             					 (17)
#define CFGID_SENSOR_FR_0_TAG           					  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 0)
#define CFGID_SENSOR_FR_VDIS_DEV          				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 1)
#define CFGID_SENSOR_FR_VDIS_CHAN         				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 2)
#define CFGID_SENSOR_FR_WIDTH             					  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 3)
#define CFGID_SENSOR_FR_HEIGHT             				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 4)
#define CFGID_SENSOR_FR_SWMS_LAYOUT        			  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 5) // 0 full,1 picture in picture,2 all video
#define CFGID_SENSOR_FR_LAYOUT_PRM_UPDATE 			  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 6) // 0 : NO 1: off
#define CFGID_SENSOR_FR_PICP_CHAN         				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 7) // 0 tv, 1 flr
#define CFGID_SENSOR_FR_PICP_LAYOUT_POS    			  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 8) // 0: TL 1: TR 2:BL 3:BR
#define CFGID_SENSOR_FR_PICP_WIDTH        				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 9) // picp width
#define CFGID_SENSOR_FR_PICP_HEIGHT      				  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 10) // picp height
#define CFGID_SENSOR_FR_PICP_POS       					  CFGID_BUILD(BLOCK_BASE_SENSOR_FR, 11) // picp height

//blk18 video crop used DEI0 cfg
#define BLOCK_BASE_VID_CROP_DEI0         					 (18)
#define CFGID_VID_CROP_DEI0_TAG           				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 0)
#define CFGID_VID_CROP_DEI0_PRM_UPDATE   				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 1) // param update
#define CFGID_VID_CROP_DEI0_ENABLE       				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 2) // video 0 used
#define CFGID_VID_CROP_DEI0_VIDEO_CHAN  				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 3) // video chan output from dei 0
#define CFGID_VID_CROP_DEI0_IMG_CROP_X 			         CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 4) // x coordinate
#define CFGID_VID_CROP_DEI0_IMG_CROP_Y   				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 5) // y coordinate
#define CFGID_VID_CROP_DEI0_IMG_CROP_W    			  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 6) // width
#define CFGID_VID_CROP_DEI0_IMG_CROP_H    				  CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI0, 7) // height

//blk19 video crop 0 use DEI1 cfg
#define BLOCK_BASE_VID_CROP_DEI1       				        (19)
#define CFGID_VID_CROP_DEI1_TAG        				        CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 0)
#define CFGID_VID_CROP_DEI1_PRM_UPDATE   				 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 1) // param update
#define CFGID_VID_CROP_DEI1_ENABLE        				 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 2) // video 0 used
#define CFGID_VID_CROP_DEI1_VIDEO_CHAN   				 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 3) // video chan output from dei 0
#define CFGID_VID_CROP_DEI1_IMG_CROP_X   				 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 4) // x coordinate
#define CFGID_VID_CROP_DEI1_IMG_CROP_Y   				 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 5) // y coordinate
#define CFGID_VID_CROP_DEI1_IMG_CROP_W   			 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 6) // width
#define CFGID_VID_CROP_DEI1_IMG_CROP_H    			 CFGID_BUILD(BLOCK_BASE_VID_CROP_DEI1, 7) // height

//blk20 video image enth cfg
#define BLOCK_BASE_VIDEO_IMG_ENTH          				 (20)
#define CFGID_VIDEO_IMG_ENTH_TAG         				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 0)
#define CFGID_VIDEO_IMG_ENTH_TV_EN       				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 1)
#define CFGID_VIDEO_IMG_ENTH_FR_EN        				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 2)
#define CFGID_VIDEO_IMG_ENTH_PRM_UPDATE  			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 3)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_X    				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 4)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_Y     			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 5)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_W    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 6)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_H    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 7)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_X     			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 8)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_Y     			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 9)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_W    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 10)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_H    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 11)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_MAXW 			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 12)
#define CFGID_VIDEO_IMG_ENTH_TV_RECT_MAXH 			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 13)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_MAXW  			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 14)
#define CFGID_VIDEO_IMG_ENTH_FR_RECT_MAXH 			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_ENTH, 15)

//blk21 video image stb cfg
#define BLOCK_BASE_VIDEO_IMG_STB        					 (21)
#define CFGID_VIDEO_IMG_STB_TAG           					 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_STB, 0)
#define CFGID_VIDEO_IMG_STB_TV_EN         				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_STB, 1)
#define CFGID_VIDEO_IMG_STB_FR_EN         				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_STB, 2)
#define CFGID_VIDEO_IMG_STB_PRM_UPDATE    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_STB, 3)

//blk22 video image rst cfg
#define BLOCK_BASE_VIDEO_IMG_RST           				(22)
#define CFGID_VIDEO_IMG_RST_TAG          					 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_RST, 0)
#define CFGID_VIDEO_IMG_RST_TV_EN          				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_RST, 1)
#define CFGID_VIDEO_IMG_RST_FR_EN          				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_RST, 2)
#define CFGID_VIDEO_IMG_RST_PRM_UPDATE    			 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_RST, 3)
#define CFGID_VIDEO_IMG_RST_THETA         				 CFGID_BUILD(BLOCK_BASE_VIDEO_IMG_RST, 4)

//blk23 video image mmt cfg
#define BLOCK_BASE_IMG_MMT               					(23)
#define CFGID_IMG_MMT_TAG                  					CFGID_BUILD(BLOCK_BASE_IMG_MMT, 0)
#define CFGID_TV_IMG_MMT_EN               					CFGID_BUILD(BLOCK_BASE_IMG_MMT, 1)
#define CFGID_FR_IMG_MMT_EN               					CFGID_BUILD(BLOCK_BASE_IMG_MMT, 2)
#define CFGID_IMG_MMT_PRM_UPDATE           				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 3)
#define CFGID_TV_IMG_MMT_RECT_WIDTH       				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 4)
#define CFGID_TV_IMG_MMT_RECT_HEIGHT      				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 5)
#define CFGID_TV_IMG_MMT_TARGET_NUM        				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 6)
#define CFGID_TV_OSD_MMT_MTARGET          				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 7)	// main target osd id (start from 0)
#define CFGID_TV_OSD_MMT_MTARGET_TYPE      			CFGID_BUILD(BLOCK_BASE_IMG_MMT, 8)	// main target osd type (0-cross/1-gap)
#define CFGID_TV_OSD_MMT_STARGET_TYPE      			CFGID_BUILD(BLOCK_BASE_IMG_MMT, 9)	// minor target osd type (0-cross/1-gap)
#define CFGID_FR_IMG_MMT_RECT_WIDTH        				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 10)
#define CFGID_FR_IMG_MMT_RECT_HEIGHT       				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 11)
#define CFGID_FR_IMG_MMT_TARGET_NUM        				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 12)
#define CFGID_FR_OSD_MMT_MTARGET           				CFGID_BUILD(BLOCK_BASE_IMG_MMT, 13)	// major target osd id (start from 0)
#define CFGID_FR_OSD_MMT_MTARGET_TYPE      			CFGID_BUILD(BLOCK_BASE_IMG_MMT, 14)	// major target osd type (0-cross/1-gap)
#define CFGID_FR_OSD_MMT_STARGET_TYPE      			CFGID_BUILD(BLOCK_BASE_IMG_MMT, 15)	// minor target osd type (0-cross/1-gap)

//blk24 video image mmt target position 0
#define BLOCK_BASE_MMT_TARGET_0            				(24)
#define CFGID_TV_MMT_TARGET_TAG            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 0)
#define CFGID_TV_MMT_TARGET_VAILD          				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 1) // 0 ~7bit
#define CFGID_TV_MMT_TARGET_X_0            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 2)
#define CFGID_TV_MMT_TARGET_Y_0            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 3)
#define CFGID_TV_MMT_TARGET_X_1           				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 4)
#define CFGID_TV_MMT_TARGET_Y_1            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 5)
#define CFGID_TV_MMT_TARGET_X_2            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 6)
#define CFGID_TV_MMT_TARGET_Y_2            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 7)
#define CFGID_TV_MMT_TARGET_X_3            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 8)
#define CFGID_TV_MMT_TARGET_Y_3            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 9)
#define CFGID_TV_MMT_TARGET_X_4            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 10)
#define CFGID_TV_MMT_TARGET_Y_4            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 11)
#define CFGID_TV_MMT_TARGET_X_5            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 12)
#define CFGID_TV_MMT_TARGET_Y_5            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 13)
#define CFGID_TV_MMT_TARGET_X_6            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 14)
#define CFGID_TV_MMT_TARGET_Y_6            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_0, 15)

//blk25 video image mmt target position 1
#define BLOCK_BASE_MMT_TARGET_1            				(25)
#define CFGID_FR_MMT_TARGET_TAG           				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 0)
#define CFGID_FR_MMT_TARGET_VAILD          				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 1) // 0 ~7bit
#define CFGID_FR_MMT_TARGET_X_0            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 2)
#define CFGID_FR_MMT_TARGET_Y_0            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 3)
#define CFGID_FR_MMT_TARGET_X_1            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 4)
#define CFGID_FR_MMT_TARGET_Y_1            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 5)
#define CFGID_FR_MMT_TARGET_X_2            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 6)
#define CFGID_FR_MMT_TARGET_Y_2            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 7)
#define CFGID_FR_MMT_TARGET_X_3            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 8)
#define CFGID_FR_MMT_TARGET_Y_3            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 9)
#define CFGID_FR_MMT_TARGET_X_4            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 10)
#define CFGID_FR_MMT_TARGET_Y_4            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 11)
#define CFGID_FR_MMT_TARGET_X_5            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 12)
#define CFGID_FR_MMT_TARGET_Y_5            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 13)
#define CFGID_FR_MMT_TARGET_X_6            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 14)
#define CFGID_FR_MMT_TARGET_Y_6           				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_1, 15)

//blk26 video image mmt target position 2
#define BLOCK_BASE_MMT_TARGET_2            				(26)
#define CFGID_MMT_TARGET_TAG               					CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 0)
#define CFGID_TV_MMT_TARGET_X_7            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 1)
#define CFGID_TV_MMT_TARGET_Y_7           				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 2)
#define CFGID_TV_MMT_TARGET_X_8            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 3)
#define CFGID_TV_MMT_TARGET_Y_8            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 4)
#define CFGID_TV_MMT_TARGET_X_9            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 5)
#define CFGID_TV_MMT_TARGET_Y_9            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 6)

#define CFGID_FR_MMT_TARGET_X_7            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 8)
#define CFGID_FR_MMT_TARGET_Y_7            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 9)
#define CFGID_FR_MMT_TARGET_X_8            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 10)
#define CFGID_FR_MMT_TARGET_Y_8            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 11)
#define CFGID_FR_MMT_TARGET_X_9            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 12)
#define CFGID_FR_MMT_TARGET_Y_9            				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 13)

#define CFGID_TV_MMT_SHOW				 				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 14)
#define CFGID_FR_MMT_SHOW				 				CFGID_BUILD(BLOCK_BASE_MMT_TARGET_2, 15)

//blk27 video image mmt target size 0
#define BLOCK_BASE_MMT_TAR_SIZE_0          (27)
#define CFGID_TV_MMT_TAR_SIZE_TAG          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 0)
#define CFGID_TV_MMT_TAR_SIZE_X_0          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 1)
#define CFGID_TV_MMT_TAR_SIZE_Y_0          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 2)
#define CFGID_TV_MMT_TAR_SIZE_X_1          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 3)
#define CFGID_TV_MMT_TAR_SIZE_Y_1          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 4)
#define CFGID_TV_MMT_TAR_SIZE_X_2          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 5)
#define CFGID_TV_MMT_TAR_SIZE_Y_2          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 6)
#define CFGID_TV_MMT_TAR_SIZE_X_3          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 7)
#define CFGID_TV_MMT_TAR_SIZE_Y_3          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 8)
#define CFGID_TV_MMT_TAR_SIZE_X_4          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 9)
#define CFGID_TV_MMT_TAR_SIZE_Y_4          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 10)
#define CFGID_TV_MMT_TAR_SIZE_X_5          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 11)
#define CFGID_TV_MMT_TAR_SIZE_Y_5          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 12)
#define CFGID_TV_MMT_TAR_SIZE_X_6          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 13)
#define CFGID_TV_MMT_TAR_SIZE_Y_6          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_0, 14)

//blk28 video image mmt target size 1
#define BLOCK_BASE_MMT_TAR_SIZE_1          (28)
#define CFGID_FR_MMT_TAR_SIZE_TAG          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 0)
#define CFGID_FR_MMT_TAR_SIZE_X_0          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 1)
#define CFGID_FR_MMT_TAR_SIZE_Y_0          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 2)
#define CFGID_FR_MMT_TAR_SIZE_X_1          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 3)
#define CFGID_FR_MMT_TAR_SIZE_Y_1          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 4)
#define CFGID_FR_MMT_TAR_SIZE_X_2          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 5)
#define CFGID_FR_MMT_TAR_SIZE_Y_2          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 6)
#define CFGID_FR_MMT_TAR_SIZE_X_3          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 7)
#define CFGID_FR_MMT_TAR_SIZE_Y_3          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 8)
#define CFGID_FR_MMT_TAR_SIZE_X_4          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 9)
#define CFGID_FR_MMT_TAR_SIZE_Y_4          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 10)
#define CFGID_FR_MMT_TAR_SIZE_X_5          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 11)
#define CFGID_FR_MMT_TAR_SIZE_Y_5          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 12)
#define CFGID_FR_MMT_TAR_SIZE_X_6          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 13)
#define CFGID_FR_MMT_TAR_SIZE_Y_6          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_1, 14)

//blk29 video image mmt target size 2
#define BLOCK_BASE_MMT_TAR_SIZE_2          (29)
#define CFGID_MMT_TAR_SIZE_TAG             CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 0)
#define CFGID_TV_MMT_TAR_SIZE_X_7          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 1)
#define CFGID_TV_MMT_TAR_SIZE_Y_7          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 2)
#define CFGID_TV_MMT_TAR_SIZE_X_8          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 3)
#define CFGID_TV_MMT_TAR_SIZE_Y_8          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 4)
#define CFGID_TV_MMT_TAR_SIZE_X_9          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 5)
#define CFGID_TV_MMT_TAR_SIZE_Y_9          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 6)

#define CFGID_FR_MMT_TAR_SIZE_X_7          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 8)
#define CFGID_FR_MMT_TAR_SIZE_Y_7          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 9)
#define CFGID_FR_MMT_TAR_SIZE_X_8          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 10)
#define CFGID_FR_MMT_TAR_SIZE_Y_8          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 11)
#define CFGID_FR_MMT_TAR_SIZE_X_9          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 12)
#define CFGID_FR_MMT_TAR_SIZE_Y_9          CFGID_BUILD(BLOCK_BASE_MMT_TAR_SIZE_2, 13)

//blk30 video image tv track cfg
#define BLOCK_BASE_TV_TRACK                (30)
#define CFGID_TRACK_TV_TAG                 CFGID_BUILD(BLOCK_BASE_TV_TRACK, 0)
#define CFGID_TRACK_TV_EN                  CFGID_BUILD(BLOCK_BASE_TV_TRACK, 1)
#define CFGID_TRACK_TV_PRM_UPDATE          CFGID_BUILD(BLOCK_BASE_TV_TRACK, 2)
#define CFGID_TRACK_TV_ACQ_X               CFGID_BUILD(BLOCK_BASE_TV_TRACK, 3)
#define CFGID_TRACK_TV_ACQ_Y               CFGID_BUILD(BLOCK_BASE_TV_TRACK, 4)
#define CFGID_TRACK_TV_ACQ_W               CFGID_BUILD(BLOCK_BASE_TV_TRACK, 5)
#define CFGID_TRACK_TV_ACQ_H               CFGID_BUILD(BLOCK_BASE_TV_TRACK, 6)
#define CFGID_TRACK_TV_ACQ_ENDRAW          CFGID_BUILD(BLOCK_BASE_TV_TRACK, 7)
#define CFGID_TRACK_TV_ACQ_TRKTIME         CFGID_BUILD(BLOCK_BASE_TV_TRACK, 8)
#define CFGID_TRACK_TV_AXIX         CFGID_BUILD(BLOCK_BASE_TV_TRACK, 9)
#define CFGID_TRACK_TV_AXIY         CFGID_BUILD(BLOCK_BASE_TV_TRACK, 10)

//blk31 video image fr track cfg
#define BLOCK_BASE_FR_TRACK                (31)
#define CFGID_TRACK_FR_TAG                 CFGID_BUILD(BLOCK_BASE_FR_TRACK, 0)
#define CFGID_TRACK_FR_EN                  CFGID_BUILD(BLOCK_BASE_FR_TRACK, 1)
#define CFGID_TRACK_FR_PRM_UPDATE          CFGID_BUILD(BLOCK_BASE_FR_TRACK, 2)
#define CFGID_TRACK_FR_ACQ_X               CFGID_BUILD(BLOCK_BASE_FR_TRACK, 3)
#define CFGID_TRACK_FR_ACQ_Y               CFGID_BUILD(BLOCK_BASE_FR_TRACK, 4)
#define CFGID_TRACK_FR_ACQ_W               CFGID_BUILD(BLOCK_BASE_FR_TRACK, 5)
#define CFGID_TRACK_FR_ACQ_H               CFGID_BUILD(BLOCK_BASE_FR_TRACK, 6)
#define CFGID_TRACK_FR_ACQ_ENDRAW          CFGID_BUILD(BLOCK_BASE_FR_TRACK, 7)
#define CFGID_TRACK_FR_ACQ_TRKTIME         CFGID_BUILD(BLOCK_BASE_FR_TRACK, 8)
#define CFGID_TRACK_FR_AXIX         		    CFGID_BUILD(BLOCK_BASE_FR_TRACK, 9)
#define CFGID_TRACK_FR_AXIY         		    CFGID_BUILD(BLOCK_BASE_FR_TRACK, 10)

//blk32 video image adjust cfg
#define BLOCK_BASE_ADJUST                (32)
#define CFGID_IMAGE_ADJUST_TAG                	 	  CFGID_BUILD(BLOCK_BASE_ADJUST, 0)
#define CFGID_IMAGE_FR_ADJUST_EN                 		  CFGID_BUILD(BLOCK_BASE_ADJUST, 1)
#define CFGID_IMAGE_ADJUST_PRM_UPDATE		  CFGID_BUILD(BLOCK_BASE_ADJUST, 2)
#define CFGID_IMAGE_FR_ADJUST_CROSS_X             	  CFGID_BUILD(BLOCK_BASE_ADJUST, 3)
#define CFGID_IMAGE_FR_ADJUST_CROSS_Y            	  CFGID_BUILD(BLOCK_BASE_ADJUST, 4)


//blk34 video image axix cfg
#define BLOCK_BASE_AXIX               (34)
#define CFGID_IMAGE_AXIX1               	 	         CFGID_BUILD(BLOCK_BASE_ADJUST, 0)
#define CFGID_IMAGE_AXIX2                 		  CFGID_BUILD(BLOCK_BASE_ADJUST, 1)
#define CFGID_IMAGE_AXIX3		  			  CFGID_BUILD(BLOCK_BASE_ADJUST, 2)
#define CFGID_IMAGE_AXIX4            			  CFGID_BUILD(BLOCK_BASE_ADJUST, 3)
#define CFGID_IMAGE_AXIX5            	  		  CFGID_BUILD(BLOCK_BASE_ADJUST, 4)
#define CFGID_IMAGE_AXIX6              	 	         CFGID_BUILD(BLOCK_BASE_ADJUST, 5)
#define CFGID_IMAGE_AXIX7                 		  CFGID_BUILD(BLOCK_BASE_ADJUST, 6)
#define CFGID_IMAGE_AXIX8		  			  CFGID_BUILD(BLOCK_BASE_ADJUST, 7)
#define CFGID_IMAGE_AXIX9           			  CFGID_BUILD(BLOCK_BASE_ADJUST, 8)
#define CFGID_IMAGE_AXIX10            	  		  CFGID_BUILD(BLOCK_BASE_ADJUST, 9)


//blk35 video image axix cfg
#define BLOCK_BASE_AXIY              (35)
#define CFGID_IMAGE_AXIY1               	 	         CFGID_BUILD(BLOCK_BASE_ADJUST, 0)
#define CFGID_IMAGE_AXIY2                 		  CFGID_BUILD(BLOCK_BASE_ADJUST, 1)
#define CFGID_IMAGE_AXIY3		  			  CFGID_BUILD(BLOCK_BASE_ADJUST, 2)
#define CFGID_IMAGE_AXIY4            			  CFGID_BUILD(BLOCK_BASE_ADJUST, 3)
#define CFGID_IMAGE_AXIY5            	  		  CFGID_BUILD(BLOCK_BASE_ADJUST, 4)
#define CFGID_IMAGE_AXIY6               	 	         CFGID_BUILD(BLOCK_BASE_ADJUST, 5)
#define CFGID_IMAGE_AXIY7                		  	  CFGID_BUILD(BLOCK_BASE_ADJUST, 6)
#define CFGID_IMAGE_AXIY8		  			  CFGID_BUILD(BLOCK_BASE_ADJUST, 7)
#define CFGID_IMAGE_AXIY9            			  CFGID_BUILD(BLOCK_BASE_ADJUST, 8)
#define CFGID_IMAGE_AXIY10           	  		  CFGID_BUILD(BLOCK_BASE_ADJUST, 9)

/*******************************************************************
 * blk60~93 osd rect cfg
 * blk60/61 cross axis
 * blk62/63 trk win
 * blk64~73,tv mtd win
 * blk74~83 fr mtd win
 * blk84~85,hori menu
 * blk86~87 vert menu
 * blk88~93 fr graphic reserve
 ******************************************************************/
#define BLOCK_BASE_OSD_GRAPH               (60)
#define BLOCK_BASE_OSD_GRAPH_AXIS            (0)
#define BLOCK_BASE_OSD_GRAPH_AIM            (2)
#define BLOCK_BASE_OSD_GRAPH_MTD_TV            (4)
#define BLOCK_BASE_OSD_GRAPH_MTD_FR            (14)
#define CFGID_OSD_GRAPH_TAG(ID)            CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 0)
#define CFGID_OSD_GRAPH_EN_WIN(ID)         CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 1)
#define CFGID_OSD_GRAPH_OBJ_ID(ID)         CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 2)
#define CFGID_OSD_GRAPH_COLOR(ID)          CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 3)
#define CFGID_OSD_GRAPH_UPDATE(ID)         CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 4)
#define CFGID_OSD_GRAPH_OBJ_TYPE(ID)       CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 5)
#define CFGID_OSD_GRAPH_CENTER_X(ID)       CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 6)
#define CFGID_OSD_GRAPH_CENTER_Y(ID)       CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 7)
#define CFGID_OSD_GRAPH_WIDTH(ID)          CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 8)
#define CFGID_OSD_GRAPH_HEIGHT(ID)         CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 9)
#define CFGID_OSD_GRAPH_OBJ_MARK(ID)       CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 10)
#define CFGID_OSD_GRAPH_LINE_BOLD(ID)      CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 11)
#define CFGID_OSD_GRAPH_GAP_WIDTH(ID)      CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 12)
#define CFGID_OSD_GRAPH_GAP_HEIGHT(ID)     CFGID_BUILD(BLOCK_BASE_OSD_GRAPH+ID, 13)

/*******************************************************************
 * blk94~109 osd text cfg
 * blk94/95 system init
 * blk96/97 sensor angle
 * blk98/99 work mode
 * blk100/101 update stat
 * blk102/103 axis
 * blk104/105 gts&system
 * blk106/107 track tbe
 * blk108/109 target coord
 * blk110/111 fov choose_0
 * blk112/113 fov choose_1
 * blk114/115 update_menu_0
 * blk116/117 update_menu_1
 * blk118/119 sysinfo_loop_0
 * blk120/121 sysinfo_loop_1
 ******************************************************************/
#define BLOCK_BASE_OSD_TEXT                (94)

#define BLOCK_BASE_OSD_TEXT_SYSINIT            (0)
#define BLOCK_BASE_OSD_TEXT_SENSANGLE          (2)
#define BLOCK_BASE_OSD_TEXT_WKMODE            (4)
#define BLOCK_BASE_OSD_TEXT_UPDATESTAT            (6)
#define BLOCK_BASE_OSD_TEXT_AXISXY            (8)
#define BLOCK_BASE_OSD_TEXT_GTS_SYS           (10)
#define BLOCK_BASE_OSD_TEXT_TRKTBE            (12)
#define BLOCK_BASE_OSD_TEXT_TARCOORD            (14)
#define BLOCK_BASE_OSD_TEXT_FOVCHOOSE_0            (16)
#define BLOCK_BASE_OSD_TEXT_FOVCHOOSE_1         (18)
#define BLOCK_BASE_OSD_TEXT_UPDATE_MENU_0            (20)
#define BLOCK_BASE_OSD_TEXT_UPDATE_MENU_1           (22)
#define BLOCK_BASE_OSD_TEXT_SYSINFO_LOOP_0            (24)
#define BLOCK_BASE_OSD_TEXT_SYSINFO_LOOP_1           (26)

#define CFGID_OSD_TEXT_TAG(ID)             CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 0)
#define CFGID_OSD_TEXT_EN_WIN(ID)          CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 1)
#define CFGID_OSD_TEXT_OBJ_ID(ID)          CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 2)
#define CFGID_OSD_TEXT_COLOR(ID)           CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 3)
#define CFGID_OSD_TEXT_UPDATE(ID)          CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 4)
#define CFGID_OSD_TEXT_FONT(ID)            CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 5)
#define CFGID_OSD_TEXT_VAILD(ID)           CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 6)
#define CFGID_OSD_TEXT_0_LEFT_X(ID)        CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 7)
#define CFGID_OSD_TEXT_0_TOP_Y(ID)         CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 8)
#define CFGID_OSD_TEXT_0_LEN(ID)           CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 9)
#define CFGID_OSD_TEXT_1_LEFT_X(ID)        CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 10)
#define CFGID_OSD_TEXT_1_TOP_Y(ID)         CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 11)
#define CFGID_OSD_TEXT_1_LEN(ID)           CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 12)
#define CFGID_OSD_TEXT_2_LEFT_X(ID)        CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 13)
#define CFGID_OSD_TEXT_2_TOP_Y(ID)         CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 14)
#define CFGID_OSD_TEXT_2_LEN(ID)           CFGID_BUILD(BLOCK_BASE_OSD_TEXT +ID, 15)

//blk123 realtime status cfg mian sensor
#define BLOCK_BASE_RTS_MAIN_SEN            (123)
#define CFGID_RT_MAIN_SEN_TAG              CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,0)
#define CFGID_RT_MAIN_SEN_WRK_MODE         CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,1)
#define CFGID_RT_MAIN_SEN_CHAN_ID          CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,2)
#define CFGID_RT_MAIN_SEN_COMM_STATE       CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,3)
#define CFGID_RT_MAIN_SEN_BROESIGHT_X      CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,4)
#define CFGID_RT_MAIN_SEN_BROESIGHT_Y      CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,5)
#define CFGID_RT_MAIN_SEN_TRACK_STATE      CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,6)
#define CFGID_RT_MAIN_SEN_TRACK_TBE_X      CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,7)
#define CFGID_RT_MAIN_SEN_TRACK_TBE_Y      CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,8)
#define CFGID_RT_MAIN_SEN_PLATFM_COMPE     CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,9)
#define CFGID_RT_MAIN_SEN_PLATFM_PAN       CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,10)
#define CFGID_RT_MAIN_SEN_PLATFM_TIL       CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,11)
#define CFGID_OC25_TV_OSD_LEVEL 	           CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,12)
#define CFGID_OC25_FR_OSD_LEVEL 	           CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,13)
#define CFGID_OC25_MAIN_SENSOR_KIND 	CFGID_BUILD(BLOCK_BASE_RTS_MAIN_SEN,14)

//blk 124 realtime status cfg tv
#define BLOCK_BASE_RTS_TV_SEN              (124)
#define CFGID_RTS_TV_SEN_TAG               CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,0)
#define CFGID_RTS_TV_SEN_PAN_ANG           CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,1)
#define CFGID_RTS_TV_SEN_TIL_ANG           CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,2)
#define CFGID_RTS_TV_SEN_ZOOM              CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,3)
#define CFGID_RTS_TV_SEN_ZOOM_COEF         CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,4)
#define CFGID_RTS_TV_SEN_ENTH              CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,5)
#define CFGID_RTS_TV_SEN_GRAPH             CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,6)

#define CFGID_RTS_FR_SEN_PAN_ANG           CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,7)
#define CFGID_RTS_FR_SEN_TIL_ANG           CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,8)
#define CFGID_RTS_FR_SEN_ZOOM              CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,9)
#define CFGID_RTS_FR_SEN_ZOOM_COEFF        CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,10)
#define CFGID_RTS_FR_SEN_ENTH              CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,11)
#define CFGID_RTS_FR_SEN_GRAPH             CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,12)

#define CFGID_RTS_TV_SEN_COLOR             CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,13)
#define CFGID_RTS_FR_SEN_COLOR             CFGID_BUILD(BLOCK_BASE_RTS_TV_SEN,14)

//blk 125  oc25 text show
#define BLKID_BASE_OC25_ORDER            (125)
#define CFGID_OC25_TAB			     		  CFGID_BUILD( BLKID_BASE_OC25_ORDER,  0 )
#define CFGID_OC25_MODECHOOSE       	  CFGID_BUILD( BLKID_BASE_OC25_ORDER,  4 )
#define CFGID_OC25_FOVCHOOSE       	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  5 )
#define CFGID_OC25_TVSTAT       	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  6 )
#define CFGID_OC25_IRPOWER  	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  7 )
#define CFGID_OC25_IRSTAT              CFGID_BUILD( BLKID_BASE_OC25_ORDER,  8 )
#define CFGID_OC25_IRPAL              CFGID_BUILD( BLKID_BASE_OC25_ORDER,  15 )

#define CFGID_OC25_GTSSTAT       	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  9)
#define CFGID_OC25_MENUINFO       	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  10)
#define CFGID_OC25_TAR_LON      	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  11)
#define CFGID_OC25_TAR_LAT      	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  12)
#define CFGID_OC25_TAR_ALT      	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  13)
#define CFGID_OC25_TAR_RNG      	CFGID_BUILD( BLKID_BASE_OC25_ORDER,  14) //LASER DISTANCE

//blk 126  OC25 SYSINFO
#define BLKID_BASE_OC25_ORDER_SYSINFO              (126)
#define CFGID_OC25_SYSINFO_TAB            	   	CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,0)
#define CFGID_OC25_SYSINFO_STATE         		CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,1)
#define CFGID_OC25_SYSINFO_DATA_0 		CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,2)
#define CFGID_OC25_SYSINFO_DATA_1            	CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,3)
#define CFGID_OC25_SYSINFO_STRING              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,4)
#define CFGID_OC25_SOFTVER_0       		       CFGID_BUILD( BLKID_BASE_OC25_ORDER_SYSINFO,  5 )
#define CFGID_OC25_SOFTVER_1       		       CFGID_BUILD( BLKID_BASE_OC25_ORDER_SYSINFO,  6 )
#define CFGID_OC25_SOFTVER_YEAR       		CFGID_BUILD( BLKID_BASE_OC25_ORDER_SYSINFO,  7 )
#define CFGID_OC25_SOFTVER_MON       		CFGID_BUILD( BLKID_BASE_OC25_ORDER_SYSINFO,  8 )
#define CFGID_OC25_SOFTVER_DAY       		CFGID_BUILD( BLKID_BASE_OC25_ORDER_SYSINFO,  9 )
#define CFGID_OC25_SYSINFO_STRING1              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,10)
#define CFGID_OC25_SYSINFO_STRING2              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,11)
#define CFGID_OC25_SYSINFO_STRING3              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,12)
#define CFGID_OC25_SYSINFO_STRING4              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,13)
#define CFGID_OC25_SOFTVER_EN                         CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,14)
#define CFGID_OC25_SYSINFO_STRINGLEN              CFGID_BUILD(BLKID_BASE_OC25_ORDER_SYSINFO,15)

// blk 127 OC25 LRF STAT
#define BLKID_BASE_OC25_LRF_ORDER		(127)
#define CFGID_OC25_LRF_TAB            	   	CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,0)
#define CFGID_OC25_LRFPOWER			CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,1)
#define CFGID_OC25_LRFSTAT				CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,2)
#define CFGID_OC25_LRFNSTAT				CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,3)
#define CFGID_OC25_LRFREADY				CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,4)
#define CFGID_OC25_LRFBRIGHT			CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,5)
#define CFGID_OC25_LRFRUN				CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,6)
#define CFGID_OC25_LRFALLOW			CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,7)
#define CFGID_OC25_LRFENGLEV			CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,8)
#define CFGID_OC25_SYSINFO_ENABLE       	CFGID_BUILD(BLKID_BASE_OC25_LRF_ORDER,9 )

//blk 128 OC25 SYSINFO VALUE
#define BLKID_BASE_OC25_SYSINFO_VALUE  (128) 
#define CFGID_OC25_SYS_TAB           	   	                CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,0)
#define CFGID_OC25_SYSINFO_SUBSYS0           	   	        CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,1)
#define CFGID_OC25_SYSINFO_SUBSYS1			        CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,2)
#define CFGID_OC25_SYSINFO_SUBSYS2				CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,3)
#define CFGID_OC25_SYSINFO_SUBSYS3				CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,4)
#define CFGID_OC25_SYSINFO_SUBSYS4				CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,5)
#define CFGID_OC25_SYSINFO_SYSSTAT0			        CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,6)
#define CFGID_OC25_SYSINFO_SYSSTAT1				CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,7)
#define CFGID_OC25_SYSINFO_SYSSTAT2			        CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,8)
#define CFGID_OC25_SYSINFO_SYSSTAT3			   	CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,9)
#define CFGID_OC25_SYSINFO_SYSSTAT4			        CFGID_BUILD(BLKID_BASE_OC25_SYSINFO_VALUE,10)

//blk 129 RtStatus DevInfo
#define BLKID_BASE_RTDATA                  (129)
#define CFGID_DEV_INFO_TAB                 CFGID_BUILD( BLKID_BASE_RTDATA,  0 )
#define CFGID_DEV_INFO_VER                 CFGID_BUILD( BLKID_BASE_RTDATA,  1 )
#define CFGID_DEV_INFO_ID                  CFGID_BUILD( BLKID_BASE_RTDATA,  2 )
#define CFGID_DEV_INFO_IP                  CFGID_BUILD( BLKID_BASE_RTDATA,  3 )
#define CFGID_DEV_INFO_MASK                CFGID_BUILD( BLKID_BASE_RTDATA,  4 )
#define CFGID_DEV_INFO_GATEWAY             CFGID_BUILD( BLKID_BASE_RTDATA,  5 )
#define CFGID_DEV_INFO_MSG_UDP_PORT        CFGID_BUILD( BLKID_BASE_RTDATA,  6 )
#define CFGID_DEV_INFO_PING_TIMEOUT        CFGID_BUILD( BLKID_BASE_RTDATA,  7 )
#define CFGID_DEV_INFO_STAT                CFGID_BUILD( BLKID_BASE_RTDATA,  8 )
#define CFGID_DEV_INFO_STREAM_STAT         CFGID_BUILD( BLKID_BASE_RTDATA,  9 )

//blk130 video image tv track state cfg
#define BLOCK_BASE_TV_TRACK_STATE          (130)
#define CFGID_TRACK_TV_STATE_TAG           CFGID_BUILD(BLOCK_BASE_TV_TRACK_STATE, 0)
#define CFGID_TRACK_TV_STATE               CFGID_BUILD(BLOCK_BASE_TV_TRACK_STATE, 1)
#define CFGID_TRACK_TV_TBE_X               CFGID_BUILD(BLOCK_BASE_TV_TRACK_STATE, 2)
#define CFGID_TRACK_TV_TBE_Y               CFGID_BUILD(BLOCK_BASE_TV_TRACK_STATE, 3)

//blk131 video image fr track state cfg
#define BLOCK_BASE_FR_TRACK_STATE          (131)
#define CFGID_TRACK_FR_STATE_TAG           CFGID_BUILD(BLOCK_BASE_FR_TRACK_STATE, 0)
#define CFGID_TRACK_FR_STATE               CFGID_BUILD(BLOCK_BASE_FR_TRACK_STATE, 1)
#define CFGID_TRACK_FR_TBE_X               CFGID_BUILD(BLOCK_BASE_FR_TRACK_STATE, 2)
#define CFGID_TRACK_FR_TBE_Y               CFGID_BUILD(BLOCK_BASE_FR_TRACK_STATE, 3)

//codec params
#define BLKID_BASE_CODEC                   (140)
#define CFGID_CODEC_Stat(chId)             CFGID_BUILD( BLKID_BASE_CODEC+chId, 1)
#define CFGID_CODEC_IFrameQpMax(chId)      CFGID_BUILD( BLKID_BASE_CODEC+chId, 2)
#define CFGID_CODEC_IFrameQpMin(chId)      CFGID_BUILD( BLKID_BASE_CODEC+chId, 3)
#define CFGID_CODEC_IFrameQp(chId)         CFGID_BUILD( BLKID_BASE_CODEC+chId, 4)
#define CFGID_CODEC_PFrameQpMax(chId)      CFGID_BUILD( BLKID_BASE_CODEC+chId, 5)
#define CFGID_CODEC_PFrameQpMin(chId)      CFGID_BUILD( BLKID_BASE_CODEC+chId, 6)
#define CFGID_CODEC_PFrameQp(chId)         CFGID_BUILD( BLKID_BASE_CODEC+chId, 7)
#define CFGID_CODEC_GopLen(chId)           CFGID_BUILD( BLKID_BASE_CODEC+chId, 8)
#define CFGID_CODEC_FrameRate(chId)        CFGID_BUILD( BLKID_BASE_CODEC+chId, 9)
#define CFGID_CODEC_RateCtrlType(chId)     CFGID_BUILD( BLKID_BASE_CODEC+chId, 10)
#define CFGID_CODEC_BitRate(chId)          CFGID_BUILD( BLKID_BASE_CODEC+chId, 11)
#define CFGID_CODEC_IDRFrame(chId)         CFGID_BUILD( BLKID_BASE_CODEC+chId, 12)
#define CFGID_CODEC_EncInfo(chId)          CFGID_BUILD( BLKID_BASE_CODEC+chId, 13)


//#define BLKID_BASE_CODEC                   (150)

#endif
/***********************************END**************************************/
