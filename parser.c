#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // access()


/* ======================================== */
/* DEFINITION
/* ======================================== */
#define TRUE 0
#define DEBUG 1
#define FILE_EXIST 0

#define LOG(fmt , ... )    \
        if(DEBUG) {    \
            printf("[%s %d] "fmt"\n",__FUNCTION__,__LINE__,__VA_ARGS__);    \
        }


/* ======================================== */
/* STRUCTURE
/* ======================================== */


/* ======================================== */
/* PRIVATE FUNCTION
/* ======================================== */
int saveConfig(char *key, char *val)
{
	int age = 0;

    if(strcmp(key, "EDDIE") == 0)
    {
        age = atoi(val);
        LOG("key: %s, age: %d", key, age);
    }
    else if(strcmp(key, "JESSIE") == 0)
    {
        age = atoi(val);
        LOG("key: %s, age: %d", key, age);
    }
}

void splitString(char *str)
{
    const char *delim = "=";
    char *key = NULL;
    char *val = NULL;

    key = strtok(str, delim);
    val = strtok(NULL, delim);
    // LOG("key: %s, val: %s\n", key, val);

    if(key != NULL && val != NULL)
    {
        saveConfig(key, val);
    }
}

void parseLine(char *pBuff, int strSize)
{
    char tempBuff[128] = {0};
    int index = 0;


    while(strSize >= 0)
    {
        if(*pBuff != ' ' && *pBuff != '\t')
        {
            tempBuff[index] = *pBuff;
            index++;
        }

        pBuff++;
        strSize--;
    }
    // printf("tempBuff: %s\n", tempBuff);

    splitString(tempBuff);
}

int getLine(FILE *pFile, char *pBuff, int maxSize)
{
    int ret = 0;
    int len = 0;
    int mark = 0;    // if mark == 1, means that line is comment


    memset(pBuff, 0, maxSize);
    while(1)
    {
        // just read one char every time
        ret = fread(pBuff, 1, 1, pFile);

        if(ret == 0)
        {
            if(len == 0)
            {
                len = -1;
            }
            break;
        }

        if(*pBuff == '\r')
        {
            continue;
        }

        if(*pBuff == '\n')
        {
            if(*(pBuff-1) && *(pBuff-1) == '\r')
            {
                *(pBuff-1) = '\0';
            }
            *pBuff = '\0';
            break;
        }

        if(*pBuff == '#')
        {
            mark = 1;
        }

        if(mark != 1)
        {
           if(len >= maxSize)
           {
               len = -1;
               break;
           }
           len++;
           pBuff++;
        }
    }

    return len;
}

void paresFile(char *cfgFile)
{
	int len = 0;
	char buff[128] = {0};
    FILE *pFile = NULL;

    pFile = fopen(cfgFile, "r");
    if(pFile == NULL)
    {
        printf("the file %s can not open\n", cfgFile);
        return;
    }

    while(len != -1)
    {
        len = getLine(pFile, buff, sizeof(buff));

        if(len > 0)
        {
        	parseLine(buff, len);
        }
    }
}

/* ======================================== */
/* MAIN FUNCTION
/* ======================================== */
void main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: parser.exe [config]\n");
        return;
    }

    if(access(argv[1], R_OK|W_OK) != FILE_EXIST)
    {
        printf("the file %s does not exist\n", argv[1]);
        return;
    }

    paresFile(argv[1]);
}