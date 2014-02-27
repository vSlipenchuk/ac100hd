/*
 * tegra_alc5632.c - tegra machine ASoC driver for boards using an alc5632 codec.
 *
 * Copyright (C) 2011 The AC100 Kernel Team <ac100@lists.lauchpad.net>
 *
 * Authors:  Leon Romanovsky <leon@leon.nu>
 *           Andrey Danin <danindrey@mail.ru>
 *           Marc Dietrich <marvin24@gmx.de>
 *
 * Based on tegra_wm8903.c by Stephen Warren
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <asm/mach-types.h>

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>

#include <mach/tegra_alc5632_pdata.h>

#include <sound/core.h>
#include <sound/jack.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "../codecs/alc5632.h"

#include "tegra_das.h"
#include "tegra_i2s.h"
#include "tegra_pcm.h"
#include "tegra_asoc_utils.h"

#define DRV_NAME "tegra-snd-alc5632"

#define GPIO_HP_DET	BIT(0)

struct tegra_alc5632 {
	struct tegra_asoc_utils_data util_data;
	struct tegra_alc5632_audio_platform_data *pdata;
	int gpio_requested;
};

static int tegra_alc5632_asoc_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_card *card = codec->card;
	struct tegra_alc5632 *alc5632 = snd_soc_card_get_drvdata(card);
	int srate, mclk;
	int err;

	srate = params_rate(params);
	switch (srate) {
	case 64000:
	case 88200:
	case 96000:
		mclk = 128 * srate;
		break;
	default:
		mclk = 512 * srate;
		break;
	}
	/* FIXME: Codec only requires >= 3MHz if OSR==0 */
	while (mclk < 6000000)
		mclk *= 2;

	err = tegra_asoc_utils_set_rate(&alc5632->util_data, srate, mclk);
	if (err < 0) {
		dev_err(card->dev, "Can't configure clocks\n");
		return err;
	}

	err = snd_soc_dai_set_fmt(codec_dai,
					SND_SOC_DAIFMT_I2S |
					SND_SOC_DAIFMT_NB_NF |
					SND_SOC_DAIFMT_CBS_CFS);
	if (err < 0) {
		dev_err(card->dev, "codec_dai fmt not set\n");
		return err;
	}

	err = snd_soc_dai_set_fmt(cpu_dai,
					SND_SOC_DAIFMT_I2S |
					SND_SOC_DAIFMT_NB_NF |
					SND_SOC_DAIFMT_CBS_CFS);
	if (err < 0) {
		dev_err(card->dev, "cpu_dai fmt not set\n");
		return err;
	}

	err = snd_soc_dai_set_sysclk(codec_dai, 0, mclk,
					SND_SOC_CLOCK_IN);
	if (err < 0) {
		dev_err(card->dev, "codec_dai clock not set\n");
		return err;
	}

	return 0;
}

static struct snd_soc_ops tegra_alc5632_asoc_ops = {
	.hw_params = tegra_alc5632_asoc_hw_params,
};

static struct snd_soc_jack tegra_alc5632_hp_jack;

static struct snd_soc_jack_pin tegra_alc5632_hp_jack_pins[] = {
	{
		.pin = "Headphone Jack",
		.mask = SND_JACK_HEADPHONE,
	},
};

static struct snd_soc_jack_gpio tegra_alc5632_hp_jack_gpio = {
	.name = "headphone detect",
	.report = SND_JACK_HEADPHONE,
	.debounce_time = 150,
};

/* static struct snd_soc_jack tegra_alc5632_mic_jack;

static struct snd_soc_jack_pin tegra_alc5632_mic_jack_pins[] = {
	{
		.pin = "Mic Jack",
		.mask = SND_JACK_MICROPHONE,
	},
};
*/

static int tegra_alc5632_event_int_spk(struct snd_soc_dapm_widget *w,
					struct snd_kcontrol *k, int event)
{

        printk("spk event: %d\n", event);
/* 
    ToDo: speaker enable via nvec
	struct snd_soc_codec *codec = w->codec;
	struct snd_soc_card *card = codec->card;
	struct tegra_alc5632 *alc5632 = snd_soc_card_get_drvdata(card);
	struct tegra_alc5632_audio_platform_data *pdata = alc5632->pdata;

	gpio_set_value_cansleep(pdata->gpio_spkr_en,
				SND_SOC_DAPM_EVENT_ON(event));
*/
	return 0;
}

static const struct snd_soc_dapm_widget tegra_alc5632_dapm_widgets[] = {
	SND_SOC_DAPM_SPK("Int Spk", tegra_alc5632_event_int_spk),
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
};

static const struct snd_soc_dapm_route tegra_alc5632_audio_map[] = {
	{"Headphone Jack", NULL, "HPR"},
	{"Headphone Jack", NULL, "HPL"},
	{"Int Spk", NULL, "SPKOUT"},
	{"Int Spk", NULL, "SPKOUTN"},
/*	{"Mic Bias1", NULL, "Mic Jack"}, */
};

static const struct snd_kcontrol_new tegra_alc5632_controls[] = {
	SOC_DAPM_PIN_SWITCH("Int Spk"),
};

static int tegra_alc5632_asoc_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct snd_soc_card *card = codec->card;
	struct tegra_alc5632 *alc5632 = snd_soc_card_get_drvdata(card);
	struct tegra_alc5632_audio_platform_data *pdata = alc5632->pdata;
	int ret;

/* Todo: speaker enable via nvec */
/*	ret = gpio_request(pdata->gpio_spkr_en, "spkr_en");
	if (ret) {
		dev_err(card->dev, "cannot get spkr_en gpio\n");
		return ret;
	}
	alc5632->gpio_requested |= GPIO_SPKR_EN;

	gpio_direction_output(pdata->gpio_spkr_en, 0);

	ret = gpio_request(pdata->gpio_int_mic_en, "int_mic_en");
	if (ret) {
		dev_err(card->dev, "cannot get int_mic_en gpio\n");
		return ret;
	}
	alc5632->gpio_requested |= GPIO_INT_MIC_EN;
*/
	/* Disable int mic; enable signal is active-high */
/*	gpio_direction_output(pdata->gpio_int_mic_en, 0);

	ret = gpio_request(pdata->gpio_ext_mic_en, "ext_mic_en");
	if (ret) {
		dev_err(card->dev, "cannot get ext_mic_en gpio\n");
		return ret;
	}
	alc5632->gpio_requested |= GPIO_EXT_MIC_EN;
*/
	/* Enable ext mic; enable signal is active-low */
/*	gpio_direction_output(pdata->gpio_ext_mic_en, 0);
*/
	ret = snd_soc_add_controls(codec, tegra_alc5632_controls,
				   ARRAY_SIZE(tegra_alc5632_controls));
	if (ret < 0)
		return ret;

	snd_soc_dapm_new_controls(dapm, tegra_alc5632_dapm_widgets,
					ARRAY_SIZE(tegra_alc5632_dapm_widgets));

	snd_soc_dapm_add_routes(dapm, tegra_alc5632_audio_map,
				ARRAY_SIZE(tegra_alc5632_audio_map));

	if (gpio_is_valid(pdata->gpio_hp_det)) {
		tegra_alc5632_hp_jack_gpio.gpio = pdata->gpio_hp_det;

		snd_soc_jack_new(codec, "Headphone Jack", SND_JACK_HEADPHONE,
				 &tegra_alc5632_hp_jack);
		snd_soc_jack_add_pins(&tegra_alc5632_hp_jack,
					ARRAY_SIZE(tegra_alc5632_hp_jack_pins),
					tegra_alc5632_hp_jack_pins);
		snd_soc_jack_add_gpios(&tegra_alc5632_hp_jack, 1,
					&tegra_alc5632_hp_jack_gpio);
		alc5632->gpio_requested |= GPIO_HP_DET;
	}

/*	snd_soc_dapm_force_enable_pin(dapm, "Mic Bias1"); */

	snd_soc_dapm_nc_pin(dapm, "AUXOUT");
	snd_soc_dapm_nc_pin(dapm, "LINEINL");
	snd_soc_dapm_nc_pin(dapm, "LINEINR");
	snd_soc_dapm_nc_pin(dapm, "PHONEP");
	snd_soc_dapm_nc_pin(dapm, "PHONEN");
	snd_soc_dapm_nc_pin(dapm, "MIC2");

	snd_soc_dapm_sync(dapm);

	return 0;
}

static struct snd_soc_dai_link tegra_alc5632_alc5632_dai = {
	.name = "ALC5632",
	.stream_name = "ALC5632 PCM",
	.codec_name = "alc5632.0-001e",
	.platform_name = "tegra-pcm-audio",
	.cpu_dai_name = "tegra-i2s.0",
	.codec_dai_name = "alc5632-hifi",
	.init = tegra_alc5632_asoc_init,
	.ops = &tegra_alc5632_asoc_ops,
};

static struct snd_soc_card snd_soc_tegra_alc5632 = {
	.name = "tegra-alc5632",
	.dai_link = &tegra_alc5632_alc5632_dai,
	.num_links = 1,
};

static __devinit int tegra_snd_tegra_alc5632_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &snd_soc_tegra_alc5632;
	struct tegra_alc5632 *alc5632;
	struct tegra_alc5632_audio_platform_data *pdata;
	int ret;

	if (!machine_is_paz00()) {
		dev_err(&pdev->dev, "Not running on Toshiba AC100!\n");
		return -ENODEV;
	}

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data supplied\n");
		return -EINVAL;
	}

	alc5632 = kzalloc(sizeof(struct tegra_alc5632), GFP_KERNEL);
	if (!alc5632) {
		dev_err(&pdev->dev, "Can't allocate tegra_alc5632\n");
		return -ENOMEM;
	}

	alc5632->pdata = pdata;

	ret = tegra_asoc_utils_init(&alc5632->util_data, &pdev->dev);
	if (ret)
		goto err_free_tegra_alc5632;

	card->dev = &pdev->dev;
	platform_set_drvdata(pdev, card);
	snd_soc_card_set_drvdata(card, alc5632);

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n",
			ret);
		goto err_clear_drvdata;
	}

	return 0;

err_clear_drvdata:
	snd_soc_card_set_drvdata(card, NULL);
	platform_set_drvdata(pdev, NULL);
	card->dev = NULL;
	tegra_asoc_utils_fini(&alc5632->util_data);
err_free_tegra_alc5632:
	kfree(alc5632);
	return ret;
}

static int __devexit tegra_snd_tegra_alc5632_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct tegra_alc5632 *alc5632 = snd_soc_card_get_drvdata(card);

	if (alc5632->gpio_requested & GPIO_HP_DET)
		snd_soc_jack_free_gpios(&tegra_alc5632_hp_jack, 1,
			&tegra_alc5632_hp_jack_gpio);

	snd_soc_unregister_card(card);

	snd_soc_card_set_drvdata(card, NULL);
	platform_set_drvdata(pdev, NULL);
	card->dev = NULL;

	tegra_asoc_utils_fini(&alc5632->util_data);

/*	if (alc5632->gpio_requested & GPIO_EXT_MIC_EN)
		gpio_free(pdata->gpio_ext_mic_en);
	if (alc5632->gpio_requested & GPIO_INT_MIC_EN)
		gpio_free(pdata->gpio_int_mic_en);
	if (alc5632->gpio_requested & GPIO_SPKR_EN)
		gpio_free(pdata->gpio_spkr_en);
*/
	kfree(alc5632);

	return 0;
}

static struct platform_driver tegra_snd_tegra_alc5632_driver = {
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
		.pm = &snd_soc_pm_ops,
	},
	.probe = tegra_snd_tegra_alc5632_probe,
	.remove = __devexit_p(tegra_snd_tegra_alc5632_remove),
};

static int __init snd_tegra_tegra_alc5632_init(void)
{
	return platform_driver_register(&tegra_snd_tegra_alc5632_driver);
}
module_init(snd_tegra_tegra_alc5632_init);

static void __exit snd_tegra_tegra_alc5632_exit(void)
{
	platform_driver_unregister(&tegra_snd_tegra_alc5632_driver);
}
module_exit(snd_tegra_tegra_alc5632_exit);

MODULE_DESCRIPTION("Tegra+ALC5632 machine ASoC driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
