#include "mgos.h"
#include "mgos_i2c.h"
#include "mgos_si7021.h"
#include "mgos_mqtt.h"
#include "ssd1306.h"

static struct mgos_si7021 *s_si7021;
static struct mgos_ssd1306 *d_ssd1306;

static void timer_cb(void *user_data) {
  float temperature, humidity;
  char *d = malloc(34);

  temperature = mgos_si7021_getTemperature(s_si7021);
  humidity    = mgos_si7021_getHumidity(s_si7021);

  LOG(LL_INFO, ("si7021 temperature=%.2f humidity=%.2f", temperature, humidity));
  mgos_ssd1306_clear(d_ssd1306);
  sprintf(d, "Temp: %.2f C", temperature);
  mgos_ssd1306_draw_string(d_ssd1306, 0, 0, d);
  sprintf(d, "Humd: %.2f (RH)", humidity);
  mgos_ssd1306_draw_string(d_ssd1306, 0, 20, d);
  mgos_ssd1306_refresh(d_ssd1306, true);

  (void) user_data;
}

static void tempInit(void) {
  struct mgos_i2c *i2c;
  i2c = mgos_i2c_get_global();
  if (i2c) {
    s_si7021 = mgos_si7021_create(i2c, 0x40); // Default I2C address
    if (s_si7021) {
      mgos_set_timer(1000, true, timer_cb, NULL);
    } else {
      LOG(LL_ERROR, ("Could not initialize sensor"));
    }
  }
}

void displayInit(void) {
  if(mgos_ssd1306_init()) {
    d_ssd1306 = mgos_ssd1306_get_global();
  } else {
    LOG(LL_ERROR, ("Could not initiate display"));
  }
}

void drawWelcome(void) {
  char *d = malloc(34);
  mgos_ssd1306_select_font(d_ssd1306, 1);

  strcpy(d, "MCU ESP8266");
  mgos_ssd1306_draw_string(d_ssd1306, 32, 32, d);

  strcpy(d, "TRH sensor");
  mgos_ssd1306_draw_string(d_ssd1306, 32, 44, d);
  
  mgos_ssd1306_refresh(d_ssd1306, true);
  free(d);
}

enum mgos_app_init_result mgos_app_init(void) {
  // displayInit();
  // drawWelcome();
  tempInit();
  return MGOS_APP_INIT_SUCCESS;
}