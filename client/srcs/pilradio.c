////////////////////////////////////// Ce fichier est destine au mixing channels
/////////////////// Il permet de regler les Deadzones, Trims, Ranges, et Reverse
# include "pilradio.h"
# include <stdio.h>

int     waychan(unsigned char chanrev, unsigned char n)
{
    if (chanrev & bitweight(n) == n)
        return(-1);
    return(1);
}

void    initdtppm(t_radio *dtppm)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < 8)
    {
        PPMCHAN(i) = 155;
        PPMCTMP(i) = 155;
        PPMOFFS(i) = 155;
        if (i < 6)
            PPMDDZN(i) = 20;
        PPMCRNG(i) = 0.3;
        while (j < 8)
        {
            CHANNEL(i)[j] = '0';
            j++;
        }
        CHANNEL(i)[8] = '\0';
        i++;
    }


    PPMOFFS(0) = 162;
    dtppm->mix = 155;
    dtppm->mixtmp = 155;
    PPMCREV = 8;
}

void    jstoppm(t_radio *dtppm, tjs_data *jsdata, tjs_event *jse)
{
    if (JSET == 2)
    {
        if (PPMCREV & bitweight(JSEN))
            JSAXIS(JSEN) = -JSAXIS(JSEN);
        if (JSAXIS(JSEN) < -PPMDDZN(JSEN))
            PPMCHAN(JSEN) = (JSAXIS(JSEN) + PPMDDZN(JSEN))
                            * PPMCRNG(JSEN) + PPMOFFS(JSEN);
        else if (JSAXIS(JSEN) > PPMDDZN(JSEN))
            PPMCHAN(JSEN) = (JSAXIS(JSEN) - PPMDDZN(JSEN))
                            * PPMCRNG(JSEN) + PPMOFFS(JSEN);
        else
            PPMCHAN(JSEN) = PPMOFFS(JSEN);
        if (PPMCHAN(JSEN) > 250)
            PPMCHAN(JSEN) = 250;
        if (PPMCHAN(JSEN) < 60)
            PPMCHAN(JSEN) = 60;
    }
}

void    mix(t_radio *dtppm)
{
    dtppm->mix = (PPMCHAN(2) - PPMCHAN(5)) / 1.8 + PPMOFFS(2);
}

int     trim(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm)
{
    if (JSBUT & START) ///////////////////////////////////// Press start to trim
    {
        if (JSAXIS(JSEN) < -PPMDDZN(JSEN) && PPMOFFS(JSEN) > 100)
            PPMOFFS(JSEN)--;
        else if (JSAXIS(JSEN) > PPMDDZN(JSEN) && PPMOFFS(JSEN) < 220)
            PPMOFFS(JSEN)++;
        return (1);
    }
    return (0);
}

int     range(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm)
{
    if(JSBUT & HOME) /////////////////////////////////////// Press home to range
    {
        if (JSAXIS(JSEN) < -PPMDDZN(JSEN) && PPMCRNG(JSEN) >= 0.2)
            PPMCRNG(JSEN) -= 0.01;
        else if (JSAXIS(JSEN) > PPMDDZN(JSEN) && PPMCRNG(JSEN) <= 0.75)
            PPMCRNG(JSEN) += 0.01;
        return (1);
    }
    return (0);
}

int     reverse(tjs_event *jse, tjs_data *jsdata, t_radio *dtppm)
{
    if(JSBUT & BACK) ///////////////////////////////////// Press back to reverse
    {
        if (JSAXIS(JSEN) < -PPMDDZN(JSEN))
            PPMCREV |= bitweight(JSEN);
        else if (JSAXIS(JSEN) > PPMDDZN(JSEN))
            PPMCREV &= ~bitweight(JSEN);
        return (1);
    }
    return (0);
}
