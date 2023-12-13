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
 * 用于刷新clash
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
 * 根据processname通过查找进程号
 */
int 
findProcess(const char * const processname)
{
    char command[BUFSIZ];
    FILE * fp;
    pid_t pp;

    memset(command, 0, sizeof(command));
    
    /*
     * 使用pgrep通过shell查找
     */
    sprintf(command, "pgrep %s", processname);
    fp=popen(command, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }
    fscanf(fp, "%d", &pp);

    /*
     * 若之前存在clash进程则杀死之前的clash进程
     */
    if(pp!=0)
    {
        printf("LOG: kill %s: %d\n", processname, pp);
        kill(pp, SIGKILL);
    }
    pclose(fp);

    /*
     * 重新启动clash
     */
    restartProcess(processname);

    return 0;
}

/*
 * 根据processname, 重新启动进程
 */
int 
restartProcess(const char * const processname)
{
    char command[BUFSIZ];
    char pn[strlen(processname)+1];
    pid_t pp;
    FILE * fp;

    /*
     * 根据processname获取环境变量${CLASH}
     */
    memset(pn,0,sizeof(pn));
    strncpy(pn, processname, strlen(processname));
    if(toUpperString(pn, strlen(pn))<0)
    {
        perror("lower2Upper");
        return -1;
    }

    /*
     * 根据${CLASH}启动clash进程
     */
    sprintf(command, "${%s}", pn);
    fp = popen(command, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }

    /*
     * 输出调试信息测试是否成功启动
     */
    sprintf(command, "pgrep %s", processname);
    fp=popen(command, "r");
    if(fp==NULL)
    {
        perror("popen");
        return -1;
    }
    fscanf(fp, "%d", &pp);
    if(pp!=0)
    {
        printf("restart %s: %d\n", pn, pp);
    } else {
        perror("popen");
        return -1;
    }
    pclose(fp);

    return 0;
}

/*
 * 将小写的origin字符串全部转变为大写
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