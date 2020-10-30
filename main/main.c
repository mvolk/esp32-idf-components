#include <freertos/freeRTOS.h>
#include <freertos/task.h>

extern void tft_test_task(void *);

void app_main(void)
{
  xTaskCreate(&tft_test_task, "tftTestTask", 4096, NULL, 5, NULL);
}
