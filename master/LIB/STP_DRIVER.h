#ifndef __STP_DRIVER_H__
#define __STP_DRIVER_H__

char set_speed(char device_id, int speed);
char step_forward(char device_id, char step, char speed);
char set_step_forward();
#endif
