#include "STC15F2K60S2.H"
#include "motor.h"
#include "btooth.h"
#include "dis.h"
#include "btn.h"
#include "buzzer.h"
#include "ir_re.h"
#include "vibration.h"

#include "direct.h"
#include "speed.h"
#include "model.h"


#include "uart2.h"
#include "uart1.h"
#include "sys.H"                 
#include "Key.h"
#include "displayer.H" 
#include "Beep.h"
#include "ir.h"
#include "vib.H"

void init_buzzer()
{
	BeepInit();
}

void make_noise()
{
	SetBeep(2400, 200);
}