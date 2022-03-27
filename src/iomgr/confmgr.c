#include "confmgr.h"

char default_conf[] = "~/.drone_conf";
char file_path[64] = {0};
config_t config;
int conf_read_status = 0;

void read_config(){
    if(file_path[0] == '\0'){
        resolve_conf_path();
    }
    LOG_IO("Reading config file: %s\n", file_path);
    if(access(file_path, F_OK) == 0){
        // File exists, read values
        int idx = 0;
        int pid_comp = 0;
        char key_buf[32];
        float val;
        kPID_t roll_pid, pitch_pid, yaw_pid;
        kPID_t *pid_arr[] = {&roll_pid, &pitch_pid, &yaw_pid};
        kPID_t *cur_pid_ax;
        PIDContainer pid_container;
        FILE *fp = fopen(file_path, "r");

        while(fscanf(fp, "%s = %f", key_buf, &val) != EOF){
            if(idx == 0){
                config.base_thr = val;
                LOG_IO("Base thr set to %f\n", config.base_thr);
            }else{
                cur_pid_ax = pid_arr[idx/3];
                pid_comp = idx % 3;
                
                switch(pid_comp){
                    case 0:
                        cur_pid_ax->kP = val;
                        LOG_IO("Set kP of ax %d to %f\n", idx/3, val);
                        break;
                    case 1:
                        cur_pid_ax->kI = val;
                        LOG_IO("Set kI of ax %d to %f\n", idx/3, val);
                        break;
                    case 2:
                        cur_pid_ax->kD = val;
                        LOG_IO("Set kD of ax %d to %f\n", idx/3, val);
                        break;
                }
            }
        }
        pid_container.roll = roll_pid;
        pid_container.pitch = pitch_pid;
        pid_container.yaw = yaw_pid;
        config.pid_container = pid_container;
        fclose(fp);
        conf_read_status = 1;
    }else{
        LOG_IO("No config file found, using defaults.\n");
        // Set defaults and write
        kPID_t default_pid;
        default_pid.kP = 0.1;
        default_pid.kI = 0.0;
        default_pid.kD = 0.01;
        config.pid_container.roll = default_pid;
        config.pid_container.pitch = default_pid;
        config.pid_container.yaw = default_pid;
        config.base_thr = 0.5;
        write_config(config);
    }
}

void resolve_conf_path(){
    wordexp_t exp_result;
    wordexp(default_conf, &exp_result, 0);
    snprintf(file_path, 64, "%s", exp_result.we_wordv[0]);
    wordfree(&exp_result);
}

config_t get_config(){
    LOG_IO("Config requested by command.\n");
    if(!conf_read_status){
        LOG_IO("Config not yet loaded, initializing config.\n");
        read_config();
    }
    LOG_IO("Config (thr) loaded, returning: %f\n", config.base_thr);
    kPID_t dbg_pid;
    for(int i = 0; i < 3; i++){
        if(i == 0){
            dbg_pid = config.pid_container.roll;
        }else if(i == 1){
            dbg_pid = config.pid_container.pitch;
        }else{
            dbg_pid = config.pid_container.yaw;
        }
        LOG_IO("Config loaded, returning ax[%d]: %f, %f, %f\n", i, dbg_pid.kP, dbg_pid.kI, dbg_pid.kD);
    }
    return config;
}

void update_cfg_from_net(cfg_t type, double val){
    // lazy way to send double as int over socket
    val /= 10000;
    switch(type){
        case roll_kP:
            config.pid_container.roll.kP = val;
            LOG_IO("Set roll kP to %f\n", val);
            break;
        case roll_kI:
            config.pid_container.roll.kI = val;
            LOG_IO("Set roll kI to %f\n", val);
            break;
        case roll_kD:
            config.pid_container.roll.kD = val;
            LOG_IO("Set roll kD to %f\n", val);
            break;
        case pitch_kP:
            config.pid_container.roll.kP = val;
            LOG_IO("Set pitch kP to %f\n", val);
            break;
        case pitch_kI:
            config.pid_container.roll.kI = val;
            LOG_IO("Set pitch kI to %f\n", val);
            break;
        case pitch_kD:
            config.pid_container.roll.kD = val;
            LOG_IO("Set pitch kD to %f\n", val);
            break;
        case yaw_kP:
            config.pid_container.roll.kP = val;
            LOG_IO("Set yaw kP to %f\n", val);
            break;
        case yaw_kI:
            config.pid_container.roll.kI = val;
            LOG_IO("Set yaw kI to %f\n", val);
            break;
        case yaw_kD:
            config.pid_container.roll.kD = val;
            LOG_IO("Set yaw kD to %f\n", val);
            break;
        case thr:
            config.base_thr = val;
            LOG_IO("Set base thr to %f\n", val);
            break;
    }
    write_config(config);
}


int cfg_to_resp(char *resp_buf, int buf_size){
    int resp_ptr = 0;
    int bytes_written;
    memset(resp_buf, 0, buf_size);
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "cfg:");
    resp_ptr += bytes_written;

    // Write values stored in cfg to buffer
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f", config.base_thr);
    resp_ptr += bytes_written;
    kPID_t cur_pid;
    for(int i = 0; i < 3; i++){
        if(i == 0){
            cur_pid = config.pid_container.roll;
        }else if(i == 1){
            cur_pid = config.pid_container.pitch;
        }else{
            cur_pid = config.pid_container.yaw;
        }
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, ",%3.4f,%3.4f,%3.4f", cur_pid.kP, cur_pid.kI, cur_pid.kD);
        resp_ptr += bytes_written;
    }
    return resp_ptr;
}


void write_config(config_t new_config){
    if(file_path[0] == '\0'){
        resolve_conf_path();
    }
    FILE *fp = fopen(file_path, "w");

    fprintf(fp, "baseThr = %f\n", new_config.base_thr);
    fprintf(fp, "roll.kP = %f\n", new_config.pid_container.roll.kP);
    fprintf(fp, "roll.kI = %f\n", new_config.pid_container.roll.kI);
    fprintf(fp, "roll.kD = %f\n", new_config.pid_container.roll.kD);
    fprintf(fp, "pitch.kP = %f\n", new_config.pid_container.pitch.kP);
    fprintf(fp, "pitch.kI = %f\n", new_config.pid_container.pitch.kI);
    fprintf(fp, "pitch.kD = %f\n", new_config.pid_container.pitch.kD);
    fprintf(fp, "yaw.kP = %f\n", new_config.pid_container.yaw.kP);
    fprintf(fp, "yaw.kI = %f\n", new_config.pid_container.yaw.kI);
    fprintf(fp, "yaw.kD = %f\n", new_config.pid_container.yaw.kD);

    LOG_IO("Successfully wrote new config file.\n");

    fclose(fp);

}
