#include "adcif.h"
#include "esp_adc/adc_oneshot.h"

void adcif_init(void)
{
    adc_oneshot_unit_handle_t adc2_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_2,
    };
    adc_oneshot_new_unit(&init_config, &adc2_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_13,
    };
    adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_3, &config);
}