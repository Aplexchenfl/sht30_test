#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define I2C_DEV "/dev/i2c-2"

int main(void){

    int sht30_fd;
    int ret;
    unsigned char slaveAddr = 0x45;
    unsigned char buf[8] = {0};

    // 打开设备
    sht30_fd = open(I2C_DEV, O_RDWR);
    if ( sht30_fd < 0 ){
        printf("faile to open the i2c bus: %s.\n", I2C_DEV);
        return -1;
    }

    // 设置7位地址
    if ( ioctl(sht30_fd, I2C_TENBIT, 0) < 0) {
        printf("faile to set bits.\n");
        return -1;
    }
    // 强制设置地址
    //if ( ioctl(sht30_fd, I2C_SLAVE, 0x4c) < 0 ) {
    if ( ioctl(sht30_fd, I2C_SLAVE_FORCE, slaveAddr) < 0 ) {
        perror("faile to set address.\n");
        return -1;
    }

    while ( 1 ) {
        buf[0] = 0x2C;
        buf[1] = 0x0D;
        if ( write(sht30_fd, buf, 2) != 2 ) {
            perror("faile to write config.\n");
            return -1;
        }

        //buf[0] = 0xE0;
        //buf[1] = 0x00;
        //if ( write(sht30_fd, buf, 2) != 2 ) {
        //    perror("faile to write config.\n");
        //    return -1;
        //}

        buf[0] = 0;
        buf[1] = 0;
        if ( read(sht30_fd, buf, 8) != 8 ) {
            perror("faile to read back configure data.\n");
            return -1;
        }
        printf("sht30 result: 0x%x.0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7] );
        usleep(500000);
    }

    // 貌似是多余的
    close(sht30_fd);

    return 0;
}
