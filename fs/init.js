load('api_config.js');
load('api_rpc.js');
load('api_timer.js');
load('api_i2c.js')
load('api_arduino_ssd1306.js');

let getTemp = ffi('float getTemp(void)');
let getHumd = ffi('float getHumd(void)');
let getECO2 = ffi('float getECO2(void)');
let getTVOC = ffi('float getTVOC(void)');
//let timer   = ffi('void timer_cb(void)');

let disp = Adafruit_SSD1306.create_i2c(-1, Adafruit_SSD1306.RES_128_64);
disp.begin(Adafruit_SSD1306.SWITCHCAPVCC, 0x3C, true)
disp.clearDisplay()
disp.setCursor(10, 10);
disp.setTextColor(Adafruit_SSD1306.WHITE);
disp.setTextSize(4);
disp.setTextWrap(true);
disp.write("TRHC");
disp.display();

disp.setTextSize(2);

let i2c = I2C.get();
print(I2C.writeRegW(i2c, 0x5B, 0x11, 39038));


Timer.set(5000, true, function() {
  let i2c = I2C.get();

  disp.clearDisplay();
  disp.setCursor(0, 0);
  disp.write(JSON.stringify(getTVOC()));
  disp.setCursor(64, 0);
  disp.write(JSON.stringify(getECO2()));
  disp.setCursor(0, 20);
  disp.write(JSON.stringify(getTemp()));
  disp.setCursor(0, 40);
  disp.write(JSON.stringify(getHumd()));
  disp.display();

  print(I2C.readRegW(i2c, 0x5B, 0x11));
}, null);