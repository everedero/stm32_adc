/*
 * Copyright (c) 2020 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

adc_sequence_callback adc_callback(const struct device *dev,
		const struct adc_sequence *sequence,
		uint16_t sampling_index)
{
	if (sampling_index == 31) {
		/* Total number of samples reached (for all channels) */
		LOG_INF("Sampling %d", sampling_index);
		LOG_HEXDUMP_DBG(sequence->buffer, 32 * 6, "Buffer:");
	}
	return(ADC_ACTION_CONTINUE);

}

int main(void)
{
	int err;
	uint32_t count = 0;
	uint16_t buf[32 * 6];
	const struct adc_sequence_options adc_options = {
		.interval_us = 100000,
		.callback = &adc_callback,
		/* How many to read -1 */
		.extra_samplings = 31,
	};
	struct adc_sequence sequence = {
		.buffer = buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		.options = &adc_options,
		.channels = 0xf9, /* 0b111110011, adc channels bitmask */
	};

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			LOG_ERR("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			LOG_ERR("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}
		/* Initializes all channels */
		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			//int32_t val_mv;

			printk("- %s, channel %d: \n",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);
			//LOG_INF("1- 0x%x", sequence.channels);
			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);
			//LOG_INF("2- 0x%x", sequence.channels);
		}
		/* Fix: reset sequence.channels to the value we wanted */

		sequence.channels = 0xf9;
		/* Perform 10 batch readings of 32 samples on all channels*/
		for (size_t i = 0U; i < 10; i++) {
			err = adc_read_dt(&adc_channels[i], &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			}
			for (i = 0; i < 32; i++) {
				/* Print 32 values from 2nd ADC channel */
				printk("%d ", ((int16_t*)sequence.buffer)[1 + i * 6]);
			}
			k_sleep(K_MSEC(1000));
		}
	return 0;
}
