#include <string.h>
#include <freertos/freeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <esp_err.h>
#include <driver/spi_master.h>
#include <tft.h>
#include <st7735r.h>
#include <st7789.h>
#include <adafruit_096_tft.h>
#include <adafruit_114_tft.h>
#include <adafruit_130_tft.h>
#include <adafruit_144_tft.h>


#define ST7735R 0x80
#define ST7789  0x40

#define ADAFRUIT_096_INCH_TFT ( 0x01 | ST7735R )
#define ADAFRUIT_114_INCH_TFT ( 0x02 | ST7789 )
#define ADAFRUIT_130_INCH_TFT ( 0x03 | ST7789 )
#define ADAFRUIT_144_INCH_TFT ( 0x04 | ST7735R )

// Set this to switch between panels
#define CURRENT_TFT_PANEL ADAFRUIT_114_INCH_TFT

static spi_bus_config_t spi_bus_cfg;
static tft_handle_t tft;

static void tft_fill(uint16_t * buffer, uint16_t color, size_t width, size_t height) {
    size_t pixels = width * height;
    for (size_t i = 0; i < pixels; i++) {
      buffer[i] = color;
    }
}

static void spi_init() {
    spi_bus_cfg.miso_io_num = 12;
    spi_bus_cfg.mosi_io_num = 13;
    spi_bus_cfg.sclk_io_num = 14;
    spi_bus_cfg.quadwp_io_num = -1;
    spi_bus_cfg.quadhd_io_num = -1;
    spi_bus_cfg.max_transfer_sz = 116000;
    esp_err_t ret = spi_bus_initialize(1, &spi_bus_cfg, 2);
    ESP_ERROR_CHECK(ret);
}

static void tft_test_pattern_1(uint16_t * buffer)
{
    uint8_t width = tft->info.width;
    uint8_t height = tft->info.height;
    uint16_t red = tft16_color_rgb565(255, 0, 0);
    uint16_t blue = tft16_color_rgb565(0, 0, 255);
    uint16_t green = tft16_color_rgb565(0, 255, 0);
    uint16_t white = tft16_color_rgb565(255, 255, 255);

    // Fill screen w/black pixels
    memset(buffer, 0x00, 2 * width * height);
    tft16_render(tft, buffer, 0, 0, width - 1, height - 1);

    // Paint a 9x9 red square in the upper left corner,
    // one pixel from the edge
    tft_fill(buffer, red, 9, 9);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, 1, height - 10, 9, height - 2));

    // Paint a 19x19 white square in the upper right corner,
    // one pixel from the edge
    tft_fill(buffer, white, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, width - 20, height - 20, width - 2, height - 2));

    // Paint a 9x9 blue square in the lower right corner,
    // one pixel from the edge
    tft_fill(buffer, blue, 9, 9);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, width - 10, 1, width - 2, 9));

    // Paint a 19x19 green square in the lower left corner,
    // one pixel from the edge
    tft_fill(buffer, green, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, 1, 1, 19, 19));

    // Paint a 19x19 red square in the middle
    uint8_t mid_x = width / 2;
    uint8_t mid_y = height / 2;
    tft_fill(buffer, red, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, mid_x - 9, mid_y - 9, mid_x + 9, mid_y + 9));
}

static void tft_test_pattern_2(uint16_t * buffer)
{
    uint8_t width = tft->info.width;
    uint8_t height = tft->info.height;
    uint16_t red = tft16_color_rgb565(255, 0, 0);
    uint16_t blue = tft16_color_rgb565(0, 0, 255);
    uint16_t green = tft16_color_rgb565(0, 255, 0);
    uint16_t black = tft16_color_rgb565(0, 0, 0);

    // Fill screen w/white pixels
    memset(buffer, 0xFF, 2 * width * height);
    tft16_render(tft, buffer, 0, 0, width - 1, height - 1);

    // Paint a 9x9 blue square in the upper left corner,
    // one pixel from the edge
    tft_fill(buffer, blue, 9, 9);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, 1, height - 10, 9, height - 2));

    // Paint a 19x19 black square in the upper right corner,
    // one pixel from the edge
    tft_fill(buffer, black, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, width - 20, height - 20, width - 2, height - 2));

    // Paint a 9x9 red square in the lower right corner,
    // one pixel from the edge
    tft_fill(buffer, red, 9, 9);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, width - 10, 1, width - 2, 9));

    // Paint a 19x19 green square in the lower left corner,
    // one pixel from the edge
    tft_fill(buffer, green, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, 1, 1, 19, 19));

    // Paint a 19x19 blue square in the middle
    uint8_t mid_x = width / 2;
    uint8_t mid_y = height / 2;
    tft_fill(buffer, blue, 19, 19);
    ESP_ERROR_CHECK(tft16_render(tft, buffer, mid_x - 9, mid_y - 9, mid_x + 9, mid_y + 9));
}

void tft_init_7735r(void) {
    st7735r_params_t params;
    params.host = 1;
    params.gpio_cs = 27;
    params.gpio_dc = 25;
    params.gpio_rst = 26;
    params.gpio_bckl = 32;

    switch (CURRENT_TFT_PANEL) {
        case ADAFRUIT_096_INCH_TFT:
            tft = adafruit_096_tft_init(&params);
            break;
        case ADAFRUIT_144_INCH_TFT:
            tft = adafruit_144_tft_init(&params);
            break;
    }
}

void tft_init_7789(void) {
    st7789_params_t params;
    params.host = 1;
    params.gpio_cs = 27;
    params.gpio_dc = 25;
    params.gpio_rst = 26;
    params.gpio_bckl = 32;

    switch (CURRENT_TFT_PANEL) {
        case ADAFRUIT_114_INCH_TFT:
            tft = adafruit_114_tft_init(&params);
            break;
        case ADAFRUIT_130_INCH_TFT:
            tft = adafruit_130_tft_init(&params);
            break;
    }
}

void tft_test_task(void * ignored)
{
    spi_init();
    if ((CURRENT_TFT_PANEL & ST7735R) == ST7735R) {
        tft_init_7735r();
    } else {
        tft_init_7789();
    }

    uint8_t width = tft->info.width;
    uint8_t height = tft->info.height;
    size_t buffer_len = 2 * width * height;

    uint16_t * buffer;
    buffer = (uint16_t *) malloc(buffer_len);

    while (true) {
        tft_test_pattern_1(buffer);
        vTaskDelay(5000.0 / portTICK_PERIOD_MS);
        tft_test_pattern_2(buffer);
        vTaskDelay(5000.0 / portTICK_PERIOD_MS);
    }
}
