/*"Synaptics DSX I2C V2.0"*/
/*
 * Synaptics DSX touchscreen driver
 *
 * Copyright (C) 2012 Synaptics Incorporated
 *
 * Copyright (C) 2012 Alexandra Chin <alexandra.chin@tw.synaptics.com>
 * Copyright (C) 2012 Scott Lin <scott.lin@tw.synaptics.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _SYNAPTICS_DSX_RMI4_H_
#define _SYNAPTICS_DSX_RMI4_H_

#ifndef bool
#define bool int
#endif

#define SYNAPTICS_DS4 (1 << 0)
#define SYNAPTICS_DS5 (1 << 1)
#define SYNAPTICS_DSX_DRIVER_PRODUCT (SYNAPTICS_DS4 | SYNAPTICS_DS5)
#define SYNAPTICS_DSX_DRIVER_VERSION 0x2000

#include <linux/version.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#if !defined(CONFIG_HAS_EARLYSUSPEND) && defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#endif
#include <huawei_platform/touchscreen/hw_tp_common.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 38))
#define KERNEL_ABOVE_2_6_38
#endif

#ifdef KERNEL_ABOVE_2_6_38
#define sstrtoul(...) kstrtoul(__VA_ARGS__)
#else
#define sstrtoul(...) strict_strtoul(__VA_ARGS__)
#endif

#define PDT_PROPS (0X00EF)
#define PDT_START (0x00E9)
#define PDT_END (0x000A)
#define PDT_ENTRY_SIZE (0x0006)
#define PAGES_TO_SERVICE (10)
#define PAGE_SELECT_LEN (2)

#define F34_QUERY_BASE (0x00E9)
#define F01_QUERY_BASE (0x00E3)
#define F54_QUERY_BASE (0x01E9)
#define F55_QUERY_BASE (0x03E3)

#define PDT_P0_F01_RMI_CTRL_BASE	(0x0051)
#define PDT_P4_F51_CUSTOM_CTRL_BASE	(0x0419)
#define F01_CTRL_05_OFFSET	(4)
#define F01_CTRL_09_OFFSET	(5)
#define F51_CTRL_01_OFFSET	(1)
#define F51_CTRL_04_OFFSET	(4)
#define SYNAPTICS_RMI4_F01 (0x01)
#define SYNAPTICS_RMI4_F11 (0x11)
#define SYNAPTICS_RMI4_F12 (0x12)
#define SYNAPTICS_RMI4_F1A (0x1a)
#define SYNAPTICS_RMI4_F34 (0x34)
#define SYNAPTICS_RMI4_F35 (0x35)
#define SYNAPTICS_RMI4_F51 (0x51)
#define SYNAPTICS_RMI4_F54 (0x54)
#define SYNAPTICS_RMI4_F55 (0x55)

#define SYNAPTICS_RMI4_PRODUCT_INFO_SIZE 2
#define SYNAPTICS_RMI4_DATE_CODE_SIZE 3
#define SYNAPTICS_RMI4_PRODUCT_ID_SIZE 10
#define SYNAPTICS_RMI4_BUILD_ID_SIZE 3

#define F12_FINGERS_TO_SUPPORT 10

#define MAX_NUMBER_OF_BUTTONS 4
#define MAX_INTR_REGISTERS 4

#define MASK_16BIT 0xFFFF
#define MASK_8BIT 0xFF
#define MASK_7BIT 0x7F
#define MASK_6BIT 0x3F
#define MASK_5BIT 0x1F
#define MASK_4BIT 0x0F
#define MASK_3BIT 0x07
#define MASK_2BIT 0x03
#define MASK_1BIT 0x01

#define MASK_BIT2 0x02
#define MASK_BIT3 0x04

#define DEFAULT_DWT 30
enum exp_fn {
	RMI_DEV = 0,
	RMI_F54,
	RMI_FW_UPDATER,
	RMI_PROXIMITY,
	RMI_ACTIVE_PEN,
	RMI_LAST,
};

/* synaptics,sleep/wake strategy */
enum sleep_strategy {
	SLEEP_STRATEGY_DEEP_SLEEP   = 0,
	SLEEP_STRATEGY_POWER_DOWN   = 1,
	SLEEP_STRATEGY_KEEP_WAKE    = 2,
	SLEEP_STRATEGY_ALWAYS_RESET = 3,
};
enum calibration_indicater {
	NO_CALIBRATION = 0X00,
	WAKE_CALIBRATION = 0X01,
	SLEEP_CALIBRATION = 0X02,
};

/*
 * struct synaptics_rmi4_fn_desc - function descriptor fields in PDT
 * @query_base_addr: base address for query registers
 * @cmd_base_addr: base address for command registers
 * @ctrl_base_addr: base address for control registers
 * @data_base_addr: base address for data registers
 * @intr_src_count: number of interrupt sources
 * @fn_number: function number
 */
struct synaptics_rmi4_fn_desc {
	union {
		struct {
			unsigned char query_base_addr;
			unsigned char cmd_base_addr;
			unsigned char ctrl_base_addr;
			unsigned char data_base_addr;
			unsigned char intr_src_count:3;
			unsigned char reserved_1:2;
			unsigned char fn_version:2;
			unsigned char reserved_2:1;
			unsigned char fn_number;
		} __packed;
		unsigned char data[6];
	};
};

/*
 * synaptics_rmi4_fn_full_addr - full 16-bit base addresses
 * @query_base: 16-bit base address for query registers
 * @cmd_base: 16-bit base address for data registers
 * @ctrl_base: 16-bit base address for command registers
 * @data_base: 16-bit base address for control registers
 */
struct synaptics_rmi4_fn_full_addr {
	unsigned short query_base;
	unsigned short cmd_base;
	unsigned short ctrl_base;
	unsigned short data_base;
};

struct synaptics_rmi4_f12_extra_data {
	unsigned char data1_offset;
	unsigned char data15_offset;
	unsigned char data15_size;
	unsigned char data15_data[(F12_FINGERS_TO_SUPPORT + 7) / 8];
};

/*
 * struct synaptics_rmi4_fn - function handler data structure
 * @fn_number: function number
 * @num_of_data_sources: number of data sources
 * @num_of_data_points: maximum number of fingers supported
 * @size_of_data_register_block: data register block size
 * @intr_reg_num: index to associated interrupt register
 * @intr_mask: interrupt mask
 * @full_addr: full 16-bit base addresses of function registers
 * @link: linked list for function handlers
 * @data_size: size of private data
 * @data: pointer to private data
 */
struct synaptics_rmi4_fn {
	unsigned char fn_number;
	unsigned char num_of_data_sources;
	unsigned char num_of_data_points;
	unsigned char size_of_data_register_block;
	unsigned char intr_reg_num;
	unsigned char intr_mask;
	struct synaptics_rmi4_fn_full_addr full_addr;
	struct list_head link;
	int data_size;
	void *data;
	void *extra;
};

/*
 * struct synaptics_rmi4_device_info - device information
 * @version_major: rmi protocol major version number
 * @version_minor: rmi protocol minor version number
 * @manufacturer_id: manufacturer id
 * @product_props: product properties information
 * @product_info: product info array
 * @date_code: device manufacture date
 * @tester_id: tester id array
 * @serial_number: device serial number
 * @product_id_string: device product id
 * @support_fn_list: linked list for function handlers
 */
struct synaptics_rmi4_device_info {
	unsigned int version_major;
	unsigned int version_minor;
	unsigned char manufacturer_id;
	unsigned char product_props;
	unsigned char product_info[SYNAPTICS_RMI4_PRODUCT_INFO_SIZE];
	unsigned char date_code[SYNAPTICS_RMI4_DATE_CODE_SIZE];
	unsigned short tester_id;
	unsigned short serial_number;
	unsigned char product_id_string[SYNAPTICS_RMI4_PRODUCT_ID_SIZE + 1];
	unsigned char build_id[SYNAPTICS_RMI4_BUILD_ID_SIZE];
	struct list_head support_fn_list;
};


/*
 * struct synaptics_rmi4_data - rmi4 device instance data
 * @i2c_client: pointer to associated i2c client
 * @input_dev: pointer to associated input device
 * @board: constant pointer to platform data
 * @rmi4_mod_info: device information
 * @regulator: pointer to associated regulator
 * @rmi4_io_ctrl_mutex: mutex for i2c i/o control
 * @early_suspend: instance to support early suspend power management
 * @current_page: current page in sensor to acess
 * @button_0d_enabled: flag for 0d button support
 * @full_pm_cycle: flag for full power management cycle in early suspend stage
 * @num_of_intr_regs: number of interrupt registers
 * @f01_query_base_addr: query base address for f01
 * @f01_cmd_base_addr: command base address for f01
 * @f01_ctrl_base_addr: control base address for f01
 * @f01_data_base_addr: data base address for f01
 * @irq: attention interrupt
 * @sensor_max_x: sensor maximum x value
 * @sensor_max_y: sensor maximum y value
 * @irq_enabled: flag for indicating interrupt enable status
 * @fingers_on_2d: flag to indicate presence of fingers in 2d area
 * @wait: wait queue for touch data polling in interrupt thread
 * @i2c_read: pointer to i2c read function
 * @i2c_write: pointer to i2c write function
 * @irq_enable: pointer to irq enable function
 */


#ifdef CONFIG_HUAWEI_KERNEL
#define F54_ANALOG_CTRL13_OFFSET 16
#define F11_2D_CTRL92_OFFSET 45
#define F51_CUSTOM_CTRL63_OFFSET 14
#define F51_CUSTOM_CTRL07_OFFSET 3
#define F51_CUSTOM_CTRL19_OFFSET 15
#define F51_CUSTOM_CTRL29_OFFSET 25

enum synaptics_gesture_num {
	GESTURE_DOUBLE_CLICK = 0,
	GESTURE_SLIDE_L2R,
	GESTURE_SLIDE_R2L,
	GESTURE_SLIDE_T2B,
	GESTURE_SLIDE_B2T,
	GESTURE_SLIDE_T2B2,
	GESTURE_CIRCLE_SLIDE,
	GESTURE_LETTER_c,
	GESTURE_LETTER_e,
	GESTURE_LETTER_m,
	GESTURE_LETTER_w,
	GESTURE_PALM_COVERED,
	GESTURE_MAX,
};

#ifdef TP_LOG_NAME
#undef TP_LOG_NAME
#define TP_LOG_NAME "[SYNAP]"
#endif
#endif /*CONFIG_HUAWEI_KERNEL*/

struct synaptics_rmi4_data {
	struct i2c_client *i2c_client;
	struct input_dev *input_dev;
	struct synaptics_dsx_platform_data *board;
	struct synaptics_rmi4_device_info rmi4_mod_info;
	struct mutex rmi4_reset_mutex;
	struct mutex rmi4_io_ctrl_mutex;
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif
	unsigned char current_page;
	unsigned char button_0d_enabled;
	unsigned char full_pm_cycle;
	unsigned char num_of_rx;
	unsigned char num_of_tx;
	unsigned char num_of_fingers;
	unsigned char max_touch_width;
	unsigned char report_enable;
	unsigned char no_sleep_setting;
	/*Add doze wakeup threshold param for tempreture performance*/
	unsigned char dwt_default;//for wakeup mode
	unsigned char dwt_gesture;//for gesture mode
	unsigned char intr_mask[MAX_INTR_REGISTERS];
	unsigned char *button_txrx_mapping;
	unsigned short num_of_intr_regs;
	unsigned short f01_query_base_addr;
	unsigned short f01_cmd_base_addr;
	unsigned short f01_ctrl_base_addr;
	unsigned short f01_data_base_addr;
	unsigned int firmware_id;
	int irq;
	int sensor_max_x;
	int sensor_max_y;
	bool flash_prog_mode;
	bool irq_enabled;
	bool fingers_on_2d;
	bool firmware_updating;
	char product_id[SYNAPTICS_RMI4_PRODUCT_ID_SIZE + 1];
	int (*i2c_read)(struct synaptics_rmi4_data *pdata, unsigned short addr,
			unsigned char *data, unsigned short length);
	int (*i2c_write)(struct synaptics_rmi4_data *pdata, unsigned short addr,
			unsigned char *data, unsigned short length);
	int (*irq_enable)(struct synaptics_rmi4_data *rmi4_data, bool enable);
	int (*reset_device)(struct synaptics_rmi4_data *rmi4_data);
#ifdef CONFIG_HUAWEI_KERNEL
	struct work_struct  work;
	/* add a delay work for easy wake up work */
	struct delayed_work delay_work;
	struct delayed_work glove_delay_work;
	struct notifier_block fb_notif;
	unsigned short f11_query_base_addr;
	unsigned short f11_cmd_base_addr;
	unsigned short f11_ctrl_base_addr;
	unsigned short f11_data_base_addr;
	unsigned short f51_query_base_addr;
	unsigned short f51_cmd_base_addr;
	unsigned short f51_ctrl_base_addr;
	unsigned short f51_data_base_addr;
	unsigned short f54_query_base_addr;
	unsigned short f54_cmd_base_addr;
	unsigned short f54_ctrl_base_addr;
	unsigned short f54_data_base_addr;
	unsigned char fast_relax_normal;
	unsigned short f12_data_base_addr;
	unsigned short f12_ctrl_base_addr;
	unsigned char f12_2d_data04_gesture;
	unsigned char f12_2d_ctrl20_lpm;
	unsigned char f12_2d_ctrl22_palm;
	unsigned char f12_2d_ctrl23_palm;
	unsigned char f12_2d_ctrl26_glove;
	struct mutex rmi4_sysfs_mutex;
	struct mutex rmi4_holster_mutex;
	struct mutex rmi4_glove_mutex;
	struct mutex rmi4_window_mutex;
	/* pinctrl data*/
	struct pinctrl *pinctrl;
	struct pinctrl_state *pin_active;
	struct pinctrl_state *pin_sleep;
	unsigned int easy_wakeup_gesture;
	bool gesture_enabled;
	bool palm_enabled;
	bool glove_enabled;
	bool holster_enabled;
	bool sleep_gesture_flag;
	unsigned char f51_ctrl_01_val;
	unsigned char f51_ctrl_04_val;
	unsigned char f01_ctrl_05_val;
	unsigned char f01_ctrl_09_val;
	unsigned char f51_ctrl_01_low_sensitivity_val;
	unsigned char f51_ctrl_04_low_sensitivity_val;
	unsigned char f01_ctrl_05_low_sensitivity_val;
	unsigned char f01_ctrl_09_low_sensitivity_val;
	bool wedge_sensor;
#endif /*CONFIG_HUAWEI_KERNEL*/
};

struct synaptics_rmi4_f12_ctrl_31 {
	union {
		struct {
			unsigned char max_x_coord_lsb;
			unsigned char max_x_coord_msb;
			unsigned char max_y_coord_lsb;
			unsigned char max_y_coord_msb;
			unsigned char rx_pitch_lsb;
			unsigned char rx_pitch_msb;
			unsigned char rx_clip_low;
			unsigned char rx_clip_high;
			unsigned char wedge_clip_low;
			unsigned char wedge_clip_high;
			unsigned char num_of_p;
			unsigned char num_of_q;
		};
		unsigned char data[12];
	};
};

enum synaptics_rmi4_exp_type {
	EXP_TYPE_INIT,
	EXP_TYPE_REMOVE,
	EXP_TYPE_RESET,
	EXP_TYPE_REINIT,
	EXP_TYPE_EARLY_SUSPEND,
	EXP_TYPE_SUSPEND,
	EXP_TYPE_RESUME,
	EXP_TYPE_LATE_RESUME,
	EXP_TYPE_ATTN,
};

struct synaptics_rmi4_exp_fn {
	enum exp_fn fn_type;
	int (*init)(struct synaptics_rmi4_data *rmi4_data);
	void (*remove)(struct synaptics_rmi4_data *rmi4_data);
	void (*reset)(struct synaptics_rmi4_data *rmi4_data);
	void (*reinit)(struct synaptics_rmi4_data *rmi4_data);
	void (*early_suspend)(struct synaptics_rmi4_data *rmi4_data);
	void (*suspend)(struct synaptics_rmi4_data *rmi4_data);
	void (*resume)(struct synaptics_rmi4_data *rmi4_data);
	void (*late_resume)(struct synaptics_rmi4_data *rmi4_data);
	void (*attn)(struct synaptics_rmi4_data *rmi4_data,
			unsigned char intr_mask);
};

struct synaptics_rmi4_access_ptr {
	int (*read)(struct synaptics_rmi4_data *rmi4_data, unsigned short addr,
			unsigned char *data, unsigned short length);
	int (*write)(struct synaptics_rmi4_data *rmi4_data, unsigned short addr,
			unsigned char *data, unsigned short length);
	int (*enable)(struct synaptics_rmi4_data *rmi4_data, bool enable);
};


int synaptics_fw_updater(unsigned char *fw_data);

static inline ssize_t synaptics_rmi4_show_error(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	dev_warn(dev, "%s Attempted to read from write-only attribute %s\n",
			__func__, attr->attr.name);
	return -EPERM;
}

static inline ssize_t synaptics_rmi4_store_error(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	dev_warn(dev, "%s Attempted to write to read-only attribute %s\n",
			__func__, attr->attr.name);
	return -EPERM;
}

static inline void batohs(unsigned short *dest, unsigned char *src)
{
	*dest = src[1] * 0x100 + src[0];
}

static inline void hstoba(unsigned char *dest, unsigned short src)
{
	dest[0] = src % 0x100;
	dest[1] = src / 0x100;
}

void synaptics_dsx_hardware_reset(struct synaptics_rmi4_data *rmi4_data);

long get_module_id_by_string(const char * product_id_ptr);

void synaptics_rmi4_new_func(struct synaptics_rmi4_exp_fn *exp_fn, bool insert);

void synaptics_rmi4_func_exec(struct synaptics_rmi4_data *rmi4_data, 
	enum synaptics_rmi4_exp_type exp_type, unsigned char intr_mask);
int synap_update_work_init(struct synaptics_rmi4_data *rmi4_data);
void synaptics_rmi4_fwu_s3718_attn(struct synaptics_rmi4_data *rmi4_data,unsigned char intr_mask);

#define DEFAULT_IC_NAME_LEN 6
const char * get_ic_name(unsigned int ic_type);
int synaptics_rmi4_chip_irq_enable(void);

long int get_irq_count(void);
#endif
