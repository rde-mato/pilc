//Pi gpio drivable : 4,17,18,27,21,22,23,24,25
//Valeurs min GPIO max pour piloter des servomoteurs avec : blastchan
//       0.060    0.255
//Gestion variation de vitesse pour driver un moteur avec : blastdrive
//                      pwm 0% a 100%
//D'autres pins peuvent etre piloter en tout ou rien avec : blastset
//                                     0.000  ou  1.000

#ifndef PILRADIO_H
# define PILRADIO_H

# include "joystick.h"

# define PPMMAX     250
# define PPMMIN     060
# define PPMCHAN(x) dtppm->chan[x]
# define PPMCTMP(x) dtppm->chantmp[x]
# define PPMCREV    dtppm->chanrev
# define PPMCRNG(x) dtppm->range[x]
# define PPMOFFS(x) dtppm->offset[x]
# define PPMDDZN(x) dtppm->deadzone[x]
# define CHANNEL(x) dtppm->chantosend[x]

typedef struct      s_radio
{
    short           chan[8];
    short           chantmp[8];
    unsigned char   chanrev;
    float           range[8];
    short           offset[8];
    short           deadzone[6];
    unsigned char   chantosend[8][9];
}                   t_radio;

void    initdtppm(t_radio *dtppm);
int     waychan(unsigned char chanrev, unsigned char n);
void    jstoppm(t_radio *dtppm, tjs_data *jsdata, tjs_event *jse);
void    mix(t_radio *dtppm);
int     trim(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm);
int     range(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm);
int     reverse(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm);

#endif
