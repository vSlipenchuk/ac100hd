/*
 * various events driver for a NVIDIA compliant embedded controller
 *
 * Copyright (C) 2011 Julian Andres Klode <jak@jak-linux.org>
 *
 * Authors:  Julian Andres Klode <jak@jak-linux.org>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include "nvec.h"

static struct nvec_event_device {
	struct input_dev *sleep;
	struct input_dev *power;
	struct input_dev *lid;
	struct notifier_block notifier;
	struct nvec_chip *nvec;
} event_handler;

struct nvec_sys_event {
	unsigned char command;
	unsigned char length;
	unsigned char payload[32];
};


/* unmutes the audio amplifier */
static void paz00_unmute(struct nvec_chip *nvec) {
	nvec_write_async(nvec, "\x0d\x10\x59\x95", 4);
}

/* mutes the audio amplifier */
static void paz00_mute(struct nvec_chip *nvec) {
	nvec_write_async(nvec, "\x0d\x10\x59\x94", 4);
}

static int nvec_event_notifier(struct notifier_block *nb,
			       unsigned long event_type, void *data)
{
	struct nvec_sys_event *event = data;

	if (event_type != 0x85 || (event->command & (NVEC_VAR_SIZE << 5)) == 0
	    || event->length != 4 || event->payload[1] != 0)
		return NOTIFY_DONE;

	switch (event->payload[2]) {
#if 0
	case -1:		/* invalid */
		input_report_key(event_handler.sleep, KEY_SLEEP, 1);
		input_sync(event_handler.sleep);
		input_report_key(event_handler.sleep, KEY_SLEEP, 1);
		input_sync(event_handler.sleep);
		break;
#endif
	case 0x80:		/* short power button press */
		input_report_key(event_handler.power, KEY_POWER, 1);
		input_sync(event_handler.power);
		input_report_key(event_handler.power, KEY_POWER, 0);
		input_sync(event_handler.power);
		break;
	case 0x02:		/* lid close */
		input_report_switch(event_handler.lid, SW_LID, 1);
		input_sync(event_handler.lid);
		break;
	case 0x00:		/* lid open */
		input_report_switch(event_handler.lid, SW_LID, 0);
		input_sync(event_handler.lid);
		break;
	default:
		return NOTIFY_DONE;
	}

	return NOTIFY_STOP;
}

#ifdef CONFIG_PM
 
static int nvec_event_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	paz00_mute(nvec);
	return 0;
}

static int nvec_event_resume(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	paz00_unmute(nvec);
	return 0;
}

#else
#define nvec_event_suspend NULL
#define nvec_event_resume NULL
#endif


static int __devinit nvec_event_probe(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	int err;

	event_handler.nvec = nvec;
	event_handler.sleep = input_allocate_device();
	event_handler.sleep->name = "NVEC sleep button";
	event_handler.sleep->phys = "NVEC";
	event_handler.sleep->evbit[0] = BIT_MASK(EV_KEY);
	set_bit(KEY_SLEEP, event_handler.sleep->keybit);

	event_handler.power = input_allocate_device();
	event_handler.power->name = "NVEC power button";
	event_handler.power->phys = "NVEC";
	event_handler.power->evbit[0] = BIT_MASK(EV_KEY);
	set_bit(KEY_POWER, event_handler.power->keybit);

	event_handler.lid = input_allocate_device();
	event_handler.lid->name = "NVEC lid switch button";
	event_handler.lid->phys = "NVEC";
	event_handler.lid->evbit[0] = BIT_MASK(EV_SW);
	set_bit(SW_LID, event_handler.lid->swbit);

	err = input_register_device(event_handler.sleep);
	if (err)
		goto fail;

	err = input_register_device(event_handler.power);
	if (err)
		goto fail;

	err = input_register_device(event_handler.lid);
	if (err)
		goto fail;

	event_handler.notifier.notifier_call = nvec_event_notifier;
	nvec_register_notifier(nvec, &event_handler.notifier, 0);

	/* enable lid switch event */
	nvec_write_async(nvec, "\x01\x01\x01\x00\x00\x02\x00", 7);

	/* enable power button event */
	nvec_write_async(nvec, "\x01\x01\x01\x00\x00\x80\x00", 7);

	/* unmute the audio amplifier */
	paz00_unmute(nvec);

	return 0;

fail:
	input_free_device(event_handler.sleep);
	input_free_device(event_handler.power);
	input_free_device(event_handler.lid);
	return err;
}

static struct platform_driver nvec_event_driver = {
	.probe = nvec_event_probe,
	.suspend = nvec_event_suspend,
	.resume = nvec_event_resume,
	.driver = {
		.name = "nvec-event",
		.owner = THIS_MODULE,
	},
};

static int __init nvec_event_init(void)
{
	return platform_driver_register(&nvec_event_driver);
}

module_init(nvec_event_init);

MODULE_AUTHOR("Julian Andres Klode <jak@jak-linux.org>");
MODULE_DESCRIPTION("NVEC power/sleep/lid switch driver");
MODULE_LICENSE("GPL");
