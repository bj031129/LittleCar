#include "direct.h"

#ifndef __MOTOR_H__
#define __MOTOR_H__


void init_motor(); //��ʼ���������������ģʽ

void go_forward(); // ��������ͬʱ��ת����ǰ�˶�

void go_back();

void go_left(); // ���ֲ��������ֶ�����������ת��

void go_right();

void stop(); // ���ֹͣ

void pwm();

void set_compare(int cmp);

void set_mydirect(enum direct new_direct);


#endif