#include "confmgr.h"

char default_conf[] = "~/.drone_conf";
char file_path[64] = {0};
config_t *config;
int conf_read_status = 0;

void read_config(){
    config = malloc(sizeof(*config));
    if(file_path[0] == '\0'){
        resolve_conf_path();
    }
    LOG_IO("Reading config file: %s\n", file_path);
    if(access(file_path, F_OK) == 0){
        // File exists, read values
        char key_buf[32];
        float val;
        FILE *fp = fopen(file_path, "r");
        while(fscanf(fp, "%s = %f", key_buf, &val) != EOF){
            if(strcmp(key_buf, "kP") == 0){
                config->pid_vals.kP = val;
                LOG_IO("kP set to %f\n", config->pid_vals.kP);
            }else if(strcmp(key_buf, "kI") == 0){
                config->pid_vals.kI = val;
                LOG_IO("kI set to %f\n", config->pid_vals.kI);
            }else if(strcmp(key_buf, "kD") == 0){
                config->pid_vals.kD = val;
                LOG_IO("kD set to %f\n", config->pid_vals.kD);
            }else if(strcmp(key_buf, "baseThr") == 0){
                config->base_thr = val;
                LOG_IO("Base thr set to %f\n", config->base_thr);
            }
        }
        fclose(fp);
        conf_read_status = 1;
    }else{
        LOG_IO("No config file found, using defaults.\n");
        // Set defaults and write
        config->pid_vals.kP = 0.1;
        config->pid_vals.kI = 0.0;
        config->pid_vals.kD = 0.01;
        config->base_thr = 0.5;
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
    if(!conf_read_status){
        read_config();
    }
    return *config;
}

void update_cfg_from_net(cfg_t type, double val){
    // lazy way to send double as int over socket
    val /= 10000;
    switch(type){
        case kP:
            config->pid_vals.kP = val;
            LOG_IO("Set kP to %f\n", val);
            break;
        case kI:
            config->pid_vals.kI = val;
            LOG_IO("Set kI to %f\n", val);
            break;
        case kD:
            config->pid_vals.kD = val;
            LOG_IO("Set kD to %f\n", val);
            break;
        case thr:
            config->base_thr = val;
            LOG_IO("Set base thr to %f\n", val);
            break;
    }
    write_config(config);
}


void cfg_to_resp(char *resp_buf, int buf_size){
    int resp_ptr = 0;
    int bytes_written;
    memset(resp_buf, 0, buf_size);
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "cfg:");
    resp_ptr += bytes_written;

    // Write values stored in cfg to buffer
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,%3.4f,%3.4f,%3.4f", config->pid_vals.kP, config->pid_vals.kI, config->pid_vals.kD, config->base_thr );
    resp_ptr += bytes_written;
}


void write_config(config_t *new_config){
    if(file_path[0] == '\0'){
        resolve_conf_path();
    }
    FILE *fp = fopen(file_path, "w");

    fprintf(fp, "kP = %f\n", new_config->pid_vals.kP);
    fprintf(fp, "kI = %f\n", new_config->pid_vals.kI);
    fprintf(fp, "kD = %f\n", new_config->pid_vals.kD);
    fprintf(fp, "baseThr = %f\n", new_config->base_thr);

    LOG_IO("Successfully wrote new config file.\n");

    fclose(fp);

}
