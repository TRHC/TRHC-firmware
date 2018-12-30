load('api_config.js');
load('api_rpc.js');
load('api_timer.js');

let getTemp = ffi('float getTemp(void)');
let getHumd = ffi('float getHumd(void)');


Timer.set(5000, true, function() {
  print('Hello,', getTemp());
}, null);