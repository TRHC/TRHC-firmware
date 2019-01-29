load('api_config.js');
load('api_rpc.js');
load('api_timer.js');
load('api_arduino_ssd1306.js');

let getTemp = ffi('float getTemp(void)');
let getHumd = ffi('float getHumd(void)');
let getECO2 = ffi('float getECO2(void)');
let getTVOC = ffi('float getTVOC(void)');
//let timer   = ffi('void timer_cb(void)');

let disp = Adafruit_SSD1306.create_i2c(-1, Adafruit_SSD1306.RES_128_64);
disp.begin(Adafruit_SSD1306.SWITCHCAPVCC, 0x3C, true)
disp.clearDisplay()
disp.setCursor(0, 0);
disp.setTextColor(Adafruit_SSD1306.WHITE);
disp.setTextSize(3);
disp.setTextWrap(true);
disp.write("TRHC");
disp.display();

disp.setTextSize(2);


Timer.set(1000, true, function() {
  disp.clearDisplay();
  disp.setCursor(0, 0);
  disp.write(JSON.stringify(getTVOC()));
  disp.setCursor(64, 0);
  disp.write(JSON.stringify(getECO2()));
  disp.setCursor(0, 20);
  disp.write(JSON.stringify(Math.round(getTemp())));
  disp.setCursor(64, 20);
  disp.write(JSON.stringify(Math.round(getHumd())));
  disp.display();
}, null);