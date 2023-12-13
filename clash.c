#include <sys/types.h>

#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int 
main(int, char **);

int 
findProcess(const char * const);

int 
restartProcess(const char * const);

int
toUpperString(char *, int);

/*
 *
 */
int 
main(int argc, char **argv)
{
    const char *const processname="clash";

    (void)argc;
    (void)argv;

    if(findProcess(processname)!=0)
    {
        perror("findProcess");
        exit(1);
    }

    return 0;
}

/*
 *
 */
int 
findProcess(const char * const processname)
{
    char buff[BUFSIZ];
    FILE * fp;
    pid_t pp;

    memset(buff, 0, sizeof(buff));
    
    sprintf(buff, "pgrep %s", processname);
    fp=popen(buff, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }

    pp=fscanf(fp, "%d", &pp);
    printf("kill %s: %d\n", processname, pp);
    kill(pp, SIGKILL);
    pclose(fp);

    restartProcess(processname);

    return 0;
}

/*
 * 
 */
int 
restartProcess(const char * const processname)
{
    char buff[BUFSIZ];
    char pn[strlen(processname)+1];
    pid_t pp;
    FILE * fp;

    memset(pn,0,sizeof(pn));
    if(toUpperString(pn, strlen(pn))<0)
    {
        perror("lower2Upper");
        return -1;
    }

    sprintf(buff, "${%s}", pn);
    fp = popen(buff, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }

    sprintf(buff, "pgrep %s", processname);
    fp=popen(buff, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }
    
    pp=fscanf(fp, "%d", &pp);
    printf("restart %s: %d\n", pn, pp);
    pclose(fp);

    return 0;
}

/*
 *
 */
int 
toUpperString(char * origin, int len)
{
    int i=0;
    for(;i<len;i++)
    {
        origin[i]=toupper(origin[i]);
    }
    i=i-len;
    return i;
}