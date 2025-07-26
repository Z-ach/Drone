google.charts.load('current', {packages: ['corechart']});
google.charts.setOnLoadCallback(drawChart);

var text_field = document.getElementById("text_field");
var connection_field = document.getElementById("connection_field");
var table_roll = document.getElementById("roll")
var table_pitch = document.getElementById("pitch")
var table_yaw = document.getElementById("yaw")
var table_accel_x = document.getElementById("accel.x")
var table_accel_y = document.getElementById("accel.y")
var table_accel_z = document.getElementById("accel.z")
var table_gyro_x = document.getElementById("gyro.x")
var table_gyro_y = document.getElementById("gyro.y")
var table_gyro_z = document.getElementById("gyro.z")
var table_alt = document.getElementById("altitude")
var table_esc_bl = document.getElementById("m_bl")
var table_esc_br = document.getElementById("m_br")
var table_esc_fl = document.getElementById("m_fl")
var table_esc_fr = document.getElementById("m_fr")
var table_bat = document.getElementById("battery")

var cfg_kP_roll_elem = document.getElementById("kP_roll")
var cfg_kI_roll_elem = document.getElementById("kI_roll")
var cfg_kD_roll_elem = document.getElementById("kD_roll")
var cfg_kP_pitch_elem = document.getElementById("kP_pitch")
var cfg_kI_pitch_elem = document.getElementById("kI_pitch")
var cfg_kD_pitch_elem = document.getElementById("kD_pitch")
var cfg_kP_yaw_elem = document.getElementById("kP_yaw")
var cfg_kI_yaw_elem = document.getElementById("kI_yaw")
var cfg_kD_yaw_elem = document.getElementById("kD_yaw")
var cfg_thr_elem = document.getElementById("thr")

//const SAMPLE_RATE = 500;
const SAMPLE_RATE = 100;
const HZ = (1000 / SAMPLE_RATE) * 4;
const max_chart_entries = 2 * HZ;

var telem_interval_id;
var telem_running = false;
var cfg_timer = 0;
var telem_req_counter = 0;
var poll_status = false;

class TelemetryHandler{
    constructor(){
        this.running = false;
        this.interval_id = null;
    }

    start(){
        if(!this.running){
            console.log("Starting telemetry requester.")
            this.running = true
            this.interval_id = setInterval(connecter, SAMPLE_RATE);
        }
        else{
            console.log("Cannot start telem handler, it's already running.");
        }
    }

    stop(){
        if(!this.running){
            console.log("Cannot stop telem handler, it's not running.");
        }
        else{
            this.running = false;
            clearInterval(this.interval_id);
        }
    }
}

var telem_handler = new TelemetryHandler();

class GraphInfo{
    constructor(title, elem_id, axis_list){
        this.title = title;
        this.axis_list = axis_list;
        this.elem_id = elem_id;
        this.idx = 0;

        let plot_list = ["Time"].concat(axis_list);
        let first_entry = [0];
        for(let i =0; i < axis_list.length; i++){
            first_entry = first_entry.concat(0);
        }
        this.data = google.visualization.arrayToDataTable([
            plot_list,
            first_entry
        ]);
        // create options object with titles, colors, etc.
        this.options = {
            title: this.title,
            hAxis: {
            title: "Time"
            },
            vAxis: {
            title: "Values"
            }
        };

        // Instantiate and draw the chart.
        this.chart = new google.visualization.LineChart(document.getElementById(this.elem_id));
        this.view = new google.visualization.DataView(this.data);
        this.chart.draw(this.view, this.options);
    }
    update_chart(entry){
        let nextRow = [this.idx/HZ].concat(entry);
        this.data.addRow(nextRow);
        this.idx++;
        this.view.setRows(Math.max(0, this.idx - max_chart_entries), this.idx);
        this.chart.draw(this.view, this.options);
    }
}

var attitude_chart;
var accel_chart;
var gyro_chart;
var alt_chart;
var esc_chart;

var telem_vals = {
    roll: 0,
    pitch: 0,
    yaw: 0,
    accel_x : 0,
    accel_y : 0,
    accel_z : 0,
    gyro_x : 0,
    gyro_y : 0,
    gyro_z : 0,
    alt : 0,
    m_bl : 0,
    m_br : 0,
    m_fl : 0,
    m_fr : 0
};

var cfg_vals = {
    kP: 0,
    kI: 0,
    kD: 0,
    thr: 0
};

function connect_to_server(){
    /*
    if(!telem_running && telem_req_counter < 1){
        telem_req_counter += 1;
        httpGetAsync("http://127.0.0.1:3000/cfg_read", read_config);
        telem_running = true;
        telem_interval_id = setInterval(connecter, SAMPLE_RATE);
    }
    */
    httpGetAsync("http://127.0.0.1:3000/cfg_read", read_config);
    telem_handler.start();
}

function read_config(val){
    if(typeof val == 'string' && val.length != 0){
        console.log(`Received cfg str: ${val}`);
        let parsed_vals = val.split(',');
        let idx = 0;
        cfg_vals.thr = parseFloat(parsed_vals[idx++]);
        cfg_vals.kP_roll = parseFloat(parsed_vals[idx++]);
        cfg_vals.kI_roll = parseFloat(parsed_vals[idx++]);
        cfg_vals.kD_roll = parseFloat(parsed_vals[idx++]);
        cfg_vals.kP_pitch = parseFloat(parsed_vals[idx++]);
        cfg_vals.kI_pitch = parseFloat(parsed_vals[idx++]);
        cfg_vals.kD_pitch = parseFloat(parsed_vals[idx++]);
        cfg_vals.kP_yaw = parseFloat(parsed_vals[idx++]);
        cfg_vals.kI_yaw = parseFloat(parsed_vals[idx++]);
        cfg_vals.kD_yaw = parseFloat(parsed_vals[idx++]);
        update_cfg_table();
    }else{
        console.log(`Invalid format detected: ${typeof val} (${val})`);
        if(typeof val == 'string'){
            console.log(`String had length: ${val.length}`)
        }
        /*
        if(cfg_timer == 0){
            cfg_timer = setTimeout(function(){
                httpGetAsync("http://127.0.0.1:3000/cfg_read", read_config);
            }, 500);
        }
        */
    }
}

function update_cfg_table(){
    cfg_thr_elem.value = cfg_vals.thr;
    cfg_kP_roll_elem.value = cfg_vals.kP_roll;
    cfg_kI_roll_elem.value = cfg_vals.kI_roll;
    cfg_kD_roll_elem.value = cfg_vals.kD_roll;
    cfg_kP_pitch_elem.value = cfg_vals.kP_pitch;
    cfg_kI_pitch_elem.value = cfg_vals.kI_pitch;
    cfg_kD_pitch_elem.value = cfg_vals.kD_pitch;
    cfg_kP_yaw_elem.value = cfg_vals.kP_yaw;
    cfg_kI_yaw_elem.value = cfg_vals.kI_yaw;
    cfg_kD_yaw_elem.value = cfg_vals.kD_yaw;
}

function update_conf(){
    let base = "http://127.0.0.1:3000/cfg_update?"

    let update_items = [
      cfg_kP_roll_elem.value,
      cfg_kI_roll_elem.value,
      cfg_kD_roll_elem.value,
      cfg_kP_pitch_elem.value,
      cfg_kI_pitch_elem.value,
      cfg_kD_pitch_elem.value,
      cfg_kP_yaw_elem.value,
      cfg_kI_yaw_elem.value,
      cfg_kD_yaw_elem.value,
    ];
    for(let i = 0; i < update_items.length; i++){
        let elem = update_items[i];
        base = base.concat(`${elem.id}=${elem.value}`);
        if(i < update_items.length-1){
            base = base.concat("&");
        }
    }
    httpGetAsync(base, read_config);
}

function print_func(){
    httpGetAsync("http://127.0.0.1:3000/fake", test_update);
}

function test_update(val){
    text_field.innerHTML = val;
}

function send_cmd(cmd){
    httpGetAsync("http://127.0.0.1:3000/".concat(cmd), null)
    if(cmd == "land"){
        telem_handler.stop();
    }
}

function start_connection_poll(){
    console.log("Connection poll hit.")
    if(!telem_handler.running && !poll_status){
        poll_status = true;
        httpGetAsync("http://127.0.0.1:3000/fetch_telemetry", check_for_conn);
    }
}

function check_for_conn(val){
    if(val == 0){
        console.log("No connection found, waiting before retry.")
        setTimeout(() => {
            poll_status = false;
            start_connection_poll();
        }, 1000)
    }else{
        console.log("Connection established, starting telem handler.")
        poll_status = false;
        connect_to_server();
    }

}

function connection_updater(val){
    if(val == 0){
        return;
        connection_field.innerHTML = "Not connected."
        /*
        clearInterval(telem_interval_id);
        telem_req_counter -= 1;
        telem_running = false;
        */
        telem_handler.stop();
        start_connection_poll();
        return;
    }
    let parsed_vals = val.split(',');
    let idx = 0;
    telem_vals.roll = parseFloat(parsed_vals[idx++]);
    telem_vals.pitch = parseFloat(parsed_vals[idx++]);
    telem_vals.yaw = parseFloat(parsed_vals[idx++]);
    telem_vals.accel_x = parseFloat(parsed_vals[idx++]);
    telem_vals.accel_y = parseFloat(parsed_vals[idx++]);
    telem_vals.accel_z = parseFloat(parsed_vals[idx++]) - 9.806;
    telem_vals.gyro_x = parseFloat(parsed_vals[idx++]);
    telem_vals.gyro_y = parseFloat(parsed_vals[idx++]);
    telem_vals.gyro_z = parseFloat(parsed_vals[idx++]);
    telem_vals.alt = parseFloat(parsed_vals[idx++]);
    telem_vals.m_bl = parseFloat(parsed_vals[idx++]);
    telem_vals.m_fl = parseFloat(parsed_vals[idx++]);
    telem_vals.m_br = parseFloat(parsed_vals[idx++]);
    telem_vals.m_fr = parseFloat(parsed_vals[idx++]);
    telem_vals.bat = parseFloat(parsed_vals[idx++]);
    updateDroneOrientation(telem_vals.roll, telem_vals.pitch, telem_vals.yaw);
    updateBatteryBar(telem_vals.bat);
    format_table();
    update_chart();
    updateEscGauges(telem_vals);
    connection_field.innerHTML = val;
}

function format_table(){
    table_roll.innerHTML = telem_vals.roll.toFixed(3);
    table_pitch.innerHTML = telem_vals.pitch.toFixed(3);
    table_yaw.innerHTML = telem_vals.yaw.toFixed(3);
    table_accel_x.innerHTML = telem_vals.accel_x.toFixed(3);
    table_accel_y.innerHTML = telem_vals.accel_y.toFixed(3);
    table_accel_z.innerHTML = telem_vals.accel_z.toFixed(3);
    table_gyro_x.innerHTML = telem_vals.gyro_x.toFixed(3);
    table_gyro_y.innerHTML = telem_vals.gyro_y.toFixed(3);
    table_gyro_z.innerHTML = telem_vals.gyro_z.toFixed(3);
    table_alt.innerHTML = telem_vals.alt.toFixed(3);
    table_esc_bl.innerHTML = telem_vals.m_bl.toFixed(3);
    table_esc_br.innerHTML = telem_vals.m_br.toFixed(3);
    table_esc_fl.innerHTML = telem_vals.m_fl.toFixed(3);
    table_esc_fr.innerHTML = telem_vals.m_fr.toFixed(3);
    table_bat.innerHTML = telem_vals.bat.toFixed(3);
}

function connecter(){
    httpGetAsync("http://127.0.0.1:3000/fetch_telemetry", connection_updater);
    //setTimeout(connecter, 500);
}

function httpGetAsync(theUrl, callback){
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        setTimeout(() => {
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
                callback(xmlHttp.responseText);
            else if(xmlHttp.status == 0){
                telem_handler.stop();
            }
            else if(xmlHttp.status != 200){
                console.log("provided cfg with val 0 for url req: ", theUrl);
                console.log(xmlHttp)
                callback(0);
            }
        }, 50);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

function drawChart(){
    attitude_chart = new GraphInfo("Attitude", "att_chart", ["roll", "pitch", "yaw"]);
    accel_chart = new GraphInfo("Accelerometer", "accel_chart", ["x", "y", "z"]);
    gyro_chart = new GraphInfo("Gyro", "gyro_chart", ["x", "y", "z"]);
    alt_chart = new GraphInfo("Altitude", "alt_chart", ["alt"]);
    esc_chart = new GraphInfo("ESCs", "esc_chart", ["bl", "br", "fl", "fr"]);
}

function update_chart(){
    attitude_chart.update_chart([telem_vals.roll, telem_vals.pitch, telem_vals.yaw]);
    accel_chart.update_chart([telem_vals.accel_x, telem_vals.accel_y, telem_vals.accel_z]);
    gyro_chart.update_chart([telem_vals.gyro_x, telem_vals.gyro_y, telem_vals.gyro_z]);
    alt_chart.update_chart([telem_vals.alt]);
    esc_chart.update_chart([telem_vals.m_bl, telem_vals.m_br, telem_vals.m_fl, telem_vals.m_fr]);
}

function updateBatteryBar(voltage) {
  const bar = document.getElementById("battery-bar");
  const minV = 9.6;
  const maxV = 12.6;
  let percent = Math.max(0, Math.min(1, (voltage - minV) / (maxV - minV)));

  bar.style.width = `${(percent * 100).toFixed(1)}%`;

  // Set color based on voltage level
  if (percent > 0.66) {
    bar.style.backgroundColor = "#4CAF50"; // green
  } else if (percent > 0.33) {
    bar.style.backgroundColor = "#FFC107"; // amber
  } else {
    bar.style.backgroundColor = "#F44336"; // red
  }
}
