#include "mgos.h"
#include "mgos_i2c.h"
#include "mgos_si7021.h"
#include "mgos_mqtt.h"
#include "mgos_ccs811.h"

static struct mgos_ccs811 *s_ccs811;
static struct mgos_si7021 *s_si7021;
static struct mgos_i2c *i2c;
int ret;


float getECO2(void) {
  return mgos_ccs811_get_eco2(s_ccs811);
}

float getTVOC(void) {
  return mgos_ccs811_get_tvoc(s_ccs811);
}

float getTemp(void) {
  return mgos_si7021_getTemperature(s_si7021);
}

float getHumd(void) {
  return mgos_si7021_getHumidity(s_si7021);
}

static void timer_cb(void *user_data) {
  //float temperature, humidity;
  //char *d = malloc(34);
  i2c = mgos_i2c_get_global();

  //mgos_usleep(5000);
  //uint8_t data[5] = { 0xFF, 0x11, 0xE5, 0x72, 0x8A };
  //ret = mgos_i2c_write(i2c, 0x5B, data, 5, true);
  //LOG(LL_INFO, ("CCS811 RESET CODE! :: 0x%02x", ret));

  mgos_usleep(5000);

  //ret = mgos_i2c_read_reg_b(i2c, 0x5B, 0x17);
  //LOG(LL_INFO, ("CCS811 INIT CODE! :: 0x%02x", ret));

  //ret = mgos_i2c_write(i2c, 0x5B, data, 5, true);
  //LOG(LL_INFO, ("CCS811 RESET CODE! :: 0x%02x", ret));

  float temp = getTemp();
  float humd = getHumd();
  float eco2 = getECO2();
  float tvoc = getTVOC();

  LOG(LL_INFO, ("ccs811 eco2=%.2f ; tvoc=%.2f", eco2, tvoc));
  LOG(LL_INFO, ("si7021 temp=%.2f ; humd=%.2f", temp, humd));
  //sprintf(d, "Temp: %.2f C", temperature);
  //sprintf(d, "Humd: %.2f (RH)", humidity);
  //free(d);
  (void) user_data;
}

static void tempInit(void) {
  i2c = mgos_i2c_get_global();
  if (i2c) {
    mgos_usleep(5000);
    s_si7021 = mgos_si7021_create(i2c, 0x40); // Default I2C address
    mgos_set_timer(10000, true, timer_cb, NULL);    
    mgos_usleep(5000);
    s_ccs811 = mgos_ccs811_create(i2c, 0x5b); // Default I2C address
    if (s_si7021 && s_ccs811) {
      mgos_ccs811_setDriveMode(s_ccs811, CCS811_DRIVE_MODE_10SEC);
    } else {
      LOG(LL_ERROR, ("Could not initialize sensor"));
    }
  }
}

enum mgos_app_init_result mgos_app_init(void) {
  tempInit();
  return MGOS_APP_INIT_SUCCESS;
}