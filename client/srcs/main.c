#include "socketclient.h"
#include "joystick.h"
#include "pilradio.h"
#include <pthread.h>

static  pthread_mutex_t my_mutex;

int         jsfd;
int         sock = 0;
tjs_data    jsdata;
t_radio     dtppm;
tjs_event       jse;

char *tab[11];

void inittab()
{
    //4,17,18,27,21,22,23,24,25
    tab[0] = strdup("17=0.%03d");
    tab[1] = strdup("18=0.%03d");
    tab[2] = strdup("00=0.%03d");
    tab[3] = strdup("23=0.%03d");
    tab[4] = strdup("22=0.%03d");
    tab[5] = strdup("00=0.%03d");
    tab[6] = strdup("24=0.%03d");
    tab[7] = strdup("25=0.%03d");
    tab[8] = strdup("4=0.%03d");
    tab[9] = strdup("4=0.%03d");
    tab[10] = strdup("4=0.%03d");
    tab[11] = strdup("4=0.%03d");
}

void    listengst(void)
{
    int pid;

    if ((pid = fork()) < 0)
    {
        puts("fils de fork !");
        exit (0);
    }
    else if (!pid)
    {
        system("gst-launch-1.0 udpsrc port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false");
        exit (0);
    }
}

void    *readcontrol(void *arg)
{
    while (1)
    {
        if (readjsevent(&jse))
        {
            pthread_mutex_lock (&my_mutex);
            if((jsdata.buttons & 0x3f) == 0x3f) // Appuis sur A B X Y LB RB pour quiter
                exit (0);
            writejsdata(&jse, &jsdata);
            jstoppm(&dtppm, &jsdata, &jse);
            mix(&dtppm);
            pthread_mutex_unlock (&my_mutex);
        }
    }
}

void    *sendcontrol(void *arg)
{
    int i;

    while (1)
    {
        pthread_mutex_lock (&my_mutex);
        i = -1;
        while (++i < 5)
        {
            if (i == 1)
                i = i + 2;
            sprintf(dtppm.chantosend[i], tab[i], dtppm.chantmp[i]);
            if (dtppm.chan[i] != dtppm.chantmp[i])
                    if (send(sock, dtppm.chantosend[i], 9 , 0) < 0)
                        puts("error");
            dtppm.chantmp[i] = dtppm.chan[i];
        }
        sprintf(dtppm.chantosend[i], "18=0.%03d", dtppm.mix);
        if (dtppm.mix != dtppm.mixtmp)
                if (send(sock, dtppm.chantosend[i], 9 , 0) < 0)
                    puts("error");
        dtppm.mixtmp = dtppm.mix;
        pthread_mutex_unlock (&my_mutex);
    }
}

int     main(void)
{
    pthread_t th1, th2;
    void *ret;

    jsfd = openjoystick();
    initjsdata(&jsdata);
    initdtppm(&dtppm);
    inittab();
    sock = initsocket("192.168.42.1"/*"127.0.0.1"*/);
    authentification(sock);
    listengst();
    pthread_mutex_init(&my_mutex, NULL);
    if (pthread_create(&th1, NULL, readcontrol, NULL) < 0)
    {
      fprintf (stderr, "pthread_create error for thread 1\n");
      exit (1);
    }
    if (pthread_create(&th2, NULL, sendcontrol, NULL) < 0)
    {
      fprintf (stderr, "pthread_create error for thread 2\n");
      exit (1);
    }
    (void)pthread_join(th1, &ret);
    (void)pthread_join(th2, &ret);

    //to do to quit properly
    close(sock);
    close(jsfd);
    return (0);
}
