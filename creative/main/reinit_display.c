#include "creative.h"

void reinit_display(_Bool flipped) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DISPLAY_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true);
    if (flipped == true) {
        i2c_master_write_byte(cmd, 0xA0, true);
        i2c_master_write_byte(cmd, 0xC0, true);  	
    }
    else {
        i2c_master_write_byte(cmd, 0xA1, true);
        i2c_master_write_byte(cmd, 0xC8, true); 
    }
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(DISPLAY_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
