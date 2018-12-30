load('api_config.js');
load('api_rpc.js');
load('api_timer.js');


Timer.set(1000, true, function() {
  print('Temperature:', dht.getTemp());
}, null);

RPC.addHandler('Temp.Read', function(args) {
  return { value: dht.getTemp() };
});