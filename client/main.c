#include "socketclient.h"
#include "joystick.h"
#include "pilradio.h"
#include <pthread.h>

static  pthread_mutex_t my_mutex;

int         jsfd;
int         sock = 0;
tjs_data    jsdata;
t_radio     dtppm;

void    *readcontrol(void * arg)
{
    tjs_event       jse;

    while (1)
    {
        pthread_mutex_lock (&my_mutex);
    ////////////////////////////////////////////////////////////////////////////////
        if (readjsevent(&jse))
        {
            if((jsdata.buttons & 0x3f) == 0x3f) // Appuis sur A B X Y LB RB pour quiter
                exit (0);
            writejsdata(&jse, &jsdata);
            jstoppm(&dtppm, &jsdata, &jse);
        }
    ////////////////////////////////////////////////////////////////////////////////
        pthread_mutex_unlock (&my_mutex);
    }
    //pthread_exit (0);
}

void    *sendcontrol(void * arg)
{
    tjs_event       jse;

    while (1)
    {
        pthread_mutex_lock (&my_mutex);
    ////////////////////////////////////////////////////////////////////////////////
        sprintf(dtppm.chantosend[2], "17=0.%03d", dtppm.chantmp[2]);
        if (dtppm.chan[2] != dtppm.chantmp[2])
                if (send(sock, dtppm.chantosend[2], 9 , 0) < 0)
                    //error++;
                    puts("error");
        dtppm.chantmp[2] = dtppm.chan[2];
    ////////////////////////////////////////////////////////////////////////////////
        pthread_mutex_unlock (&my_mutex);
    }
    //pthread_exit (0);
}


int     main(void)
{
    pthread_t th1, th2;
    void *ret;

    jsfd = openjoystick();
    initjsdata(&jsdata);
    initdtppm(&dtppm);
    sock = initsocket(/*"192.168.42.1"*/"127.0.0.1");
    authentification(sock);
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
