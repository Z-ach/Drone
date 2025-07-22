const http = require('http');
const net = require('net');
const url = require('url');

const hostname = '127.0.0.1';
const port = 3000;

const D_PORT = 8888;
const D_HOST = '192.168.7.2';

/*
bitmap = {
    'read cfg'          : 0xFFF0000,
    'set cfg(kP)'       : 0xFFF1000,
    'set cfg(kI)'       : 0xFFF2000,
    'set cfg(kD)'       : 0xFFF3000,
    'set cfg(thr)'      : 0xFFF4000,
    'request telem'     : 0xFFFF000,
}
*/
var re_tlm = /tlm:/g
var re_cmd = /cmd:/g
var re_cfg = /cfg:/g
var telemetry = "";
var last_cfg_resp = "";
const data_req = "FFFF0000";
const cfg_req = "FFF00000";
const cfg_set_thr = 0xFFF10000;
const cfg_set_kP_roll = 0xFFF20000;
const cfg_set_kI_roll = 0xFFF30000;
const cfg_set_kD_roll = 0xFFF40000;
const cfg_set_kP_pitch = 0xFFF50000;
const cfg_set_kI_pitch = 0xFFF60000;
const cfg_set_kD_pitch = 0xFFF70000;
const cfg_set_kP_yaw = 0xFFF80000;
const cfg_set_kI_yaw = 0xFFF90000;
const cfg_set_kD_yaw = 0xFFFA0000;
const hover_cmd = "13C00000";
const land_cmd = "22031100";
const idle_cmd = "35000000";
var client = new net.Socket();
var telem_handler_running = 0;
var telem_paused = false;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'OPTIONS, GET');
  res.setHeader('Content-Type', 'text/plain');
  var value = dispatch_url(req.url);
  res.write(value);
  res.end();
});

function dispatch_url(url){
  if(url == '/fetch_telemetry'){
    if(!telem_handler_running){
      start_telem_handler();
    }
    return telemetry;
  }else if(url == '/hover'){
    send_command(hover_cmd);
  }else if(url == '/land'){
    send_command(land_cmd);
  }else if(url == '/idle'){
    send_command(idle_cmd);
  }else if(url.startsWith('/cfg')){
    return handle_cfg(url);
  }
  return "<h3>Not supported.</h3>";
}

function handle_cfg(cfg_url){
  sliced_url = cfg_url.slice(5);
  console.log(sliced_url)
  if(sliced_url.startsWith('update')){
    queryObject = url.parse(cfg_url, true).query;
    console.log(queryObject);
    last_cfg_resp = '';
    send_command(((cfg_set_thr | (queryObject.thr*10000))>>>0).toString(16));
    send_command(((cfg_set_kP_roll | (queryObject.kP_roll*10000))>>>0).toString(16));
    send_command(((cfg_set_kI_roll | (queryObject.kI_roll*10000))>>>0).toString(16));
    send_command(((cfg_set_kD_roll | (queryObject.kD_roll*10000))>>>0).toString(16));
    send_command(((cfg_set_kP_pitch | (queryObject.kP_pitch*10000))>>>0).toString(16));
    send_command(((cfg_set_kI_pitch | (queryObject.kI_pitch*10000))>>>0).toString(16));
    send_command(((cfg_set_kD_pitch | (queryObject.kD_pitch*10000))>>>0).toString(16));
    send_command(((cfg_set_kP_yaw | (queryObject.kP_yaw*10000))>>>0).toString(16));
    send_command(((cfg_set_kI_yaw | (queryObject.kI_yaw*10000))>>>0).toString(16));
    send_command(((cfg_set_kD_yaw | (queryObject.kD_yaw*10000))>>>0).toString(16));
  }
  send_command(cfg_req);
  return last_cfg_resp;
}

function send_command(command){
  // stop telem handler to send the data
  console.log("Pausing telemtry downlink to send command.");
  clearInterval(telem_handler_running);
  telem_handler_running = 0;
  telem_paused = true;
  setTimeout(function(){
    client.write(command);
  }, 500);
}

function start_telem_handler(){
  if(last_cfg_resp == ""){
    setTimeout(function(){
      client.write(cfg_req);
    }, 500);
  }
  // issue restarting twice?
  if(!telem_paused){
    client.write(data_req);
    /*
    client.write(data_req);
    client.write(data_req);
    client.write(data_req);
    client.write(data_req);
    */
    if(telem_handler_running == 0){
      console.log("Starting telemetry handler.");
      telem_handler_running = setInterval(start_telem_handler, 250);
    }
  }
}

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});

client.connect(D_PORT, D_HOST, function() {
	console.log('Connected');
	client.write(cfg_req);
  //start_telem_handler();
});

client.on('data', function(response) {
	console.log('Received: ' + response);
  let responses = String(response).split(';');

  for(let i = 0; i < responses.length; i++){
    data = responses[i];
    if(data == ''){
      break;
    }

    if(data.startsWith("tlm:")){
      telemetry = data.slice(4);
    }else if(data.startsWith("cmd:")){
      // resume telem downlink
      console.log("Resuming telemetry downlink.");
      telem_paused = false;
      start_telem_handler();
    }else if(data.startsWith("cfg:")){
      last_cfg_resp = data.slice(4);
      console.log("Resuming telemetry downlink, cfg received:");
      console.log(last_cfg_resp);
      telem_paused = false;
      start_telem_handler();
    }
  }
	// client.destroy(); // kill client after server's response
});

client.on('error', function(e) {
  if(e.code == 'ECONNREFUSED') {
      console.log('Is the server running?');

      client.setTimeout(5000, function() {
          client.connect(D_PORT, D_HOST, function(){
              console.log('CONNECTED TO: ' + D_HOST + ':' + D_PORT);
              //client.write(data_req);
          });
      });
      console.log('Timeout for 5 seconds before trying port:' + D_PORT + ' again');
  }   
});

client.on('close', function() {
	console.log('Connection closed');
});
