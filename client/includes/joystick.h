#ifndef JOYSTICK_H
# define JOYSTICK_H

# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# define JOYSTICK_DEVNAME	"/dev/input/js0"

# define JS_EVENT_BUTTON	0x01	/* button pressed/released */
# define JS_EVENT_AXIS		0x02	/* joystick moved */
# define JS_EVENT_INIT		0x80	/* initial state of device */
# define JSEN				jse->number
# define JSET				jse->type

# define A					0x0001
# define B					0x0002
# define X					0x0004
# define Y					0x0008
# define LB					0x0010
# define RB					0x0020
# define BACK				0x0040
# define START				0x0080
# define HOME				0x0100
# define L					0x0200
# define R					0x0400
# define JSBUT				jsdata->buttons
# define JSAXIS(x)			jsdata->axis[x]
# define LX 				jsdata->axis[0]
# define LY 				jsdata->axis[1]
# define LT 				jsdata->axis[2]
# define RX 				jsdata->axis[3]
# define RY 				jsdata->axis[4]
# define RT 				jsdata->axis[5]
# define PX 				jsdata->axis[6]
# define PY 				jsdata->axis[7]

typedef struct 		sjs_event
{
	unsigned int	time;			/* event timestamp in milliseconds */
	short			value;			/* value */
	unsigned char	type;			/* event type */
	unsigned char	number;			/* axis/button number */
}					tjs_event;

typedef struct		sjs_data
{
	short int		buttons;
	char			axis[8];
}					tjs_data;

short int	bitweight(short int nb);
extern int	openjoystick(void);
extern int	readjsevent(tjs_event *jse);
extern void close_joystick();
void  		initjsdata(tjs_data *jsdata);
void 		writejsdata(tjs_event *jse, tjs_data *jsdata);

#endif
