/////////////////////////////////////////////////// Ouverture du Xbox Controller
/////////////////////////////////////////////// Aquisitions et stock des valeurs
#include "joystick.h"

static int joystick_fd = -1;

int	openjoystick(void)
{
	joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK);
	return (joystick_fd);
}

int readjsevent(tjs_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse));
	if (bytes == -1)
		return (0);
	if (bytes == sizeof(*jse))
		return (1);
	printf("Unexpected bytes from joystick:%d\n", bytes);
	return (-1);
}

void close_joystick()
{
	close(joystick_fd);
}

void 	initjsdata(tjs_data *jsdata)
{
	char i;

	i = 8;
	jsdata->buttons = 0;
	while (i--)
		jsdata->axis[i] = 0;
	jsdata->axis[2] = -128;
	jsdata->axis[5] = -128;
}

short int	bitweight(short int nb)
{
	short res;

	res = 1;
	while (nb--)
		res *= 2;
	return (res);
}

void  	writejsdata(tjs_event *jse, tjs_data *jsdata)
{
	//write jsdata.buttons
	if (jse->type == 1 && jse->value == 1)
		jsdata->buttons += bitweight(jse->number);
	if (jse->type == 1 && jse->value == 0)
		jsdata->buttons -= bitweight(jse->number);
	//write jsdata.axis
	if (jse->type == 2)
		jsdata->axis[jse->number] = jse->value / 256;
}
