#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <windows.h>
int maxP,minP,maX,maY;

using namespace std;
struct data
{
    int date,open,high,low,close,qwe[3];
}sh[4000];

int projection(int price)
{

    int rtn  = maY-(price-minP)*(maY)/(maxP-minP);

    return rtn;
}

int avg(int n,int m)
{
    int sum=0;
    for(int i=n-m;i<n;i++)
        sum += sh[i].close;
    return projection(sum/m);

}

void run(int num)
{
    int startMI=1,endMI=1,i,open,close,high,low
    ,maxInc=0,startNum=-1,endNum=-1
    ,startDate,endDate,left,right,middle=1,lastMiddle;
    char text[20];
    printf("请输入日期区间：");
    scanf("%d %d",&startDate,&endDate);
    for(i=1;i<=num;i++)
        if(sh[i].date >= startDate)
        {
           startNum = i;
           break;
        }
    for(i=1;i<=num;i++)
        if(sh[i].date >= endDate)
        {
           endNum = i;
           break;
        }

    initwindow(1000,400,"大·作·业",0,0);
    maX = getmaxx();
    maY = getmaxy();

    setfillstyle(1,LIGHTGRAY);
    bar(0,0,maX,maY);

    maxP = sh[startNum].close;
    minP = sh[startNum].close;

    for(int i=startNum;i<=endNum;i++){
            maxP = max(maxP,sh[i].high);
            minP = min(minP,sh[i].low);
        }

    for(int i=startNum;i<=endNum;i++){
        lastMiddle = middle;
        middle  = (int)(((float)i-startNum)*2+1)/(((float)endNum-startNum)*2)*maX;
        left    = -(maX/((endNum-startNum)*2))+middle;
        right   = (maX/((endNum-startNum)*2))+middle;
        open    = projection(sh[i].open);
        close   = projection(sh[i].close);
        high    = projection(sh[i].high);
        low     = projection(sh[i].low);
        if(sh[i].open > sh[i].close)
        {
            setcolor(GREEN);
            setfillstyle(1,GREEN);
            line(middle,high,middle,low);
            bar(left,open,right,close);
        }
        else
        {
            setcolor(RED);
            line(middle,open,middle,low);
            line(middle,close,middle,high);
            rectangle(left,open,right,close);
        }
            setcolor(DARKGRAY);
            if(i>startNum+5)
            line(middle,avg(i,5),lastMiddle,avg(i-1,5));
                setcolor(BLUE);
            if(i>startNum+10)
                line(middle,avg(i,10),lastMiddle,avg(i-1,10));
            setcolor(BROWN);
            if(i>startNum+20)
                line(middle,avg(i,20),lastMiddle,avg(i-1,20));
            setcolor(YELLOW);
            if(i>startNum+60)
                line(middle,avg(i,60),lastMiddle,avg(i-1,60));
    }

    maxInc = 0,startMI=1,endMI=1;
    for(int i=startNum;i<=endNum;i++)
        for(int j=i;j<=endNum;j++)
                if(sh[j].close-sh[i].close > maxInc)
                {
                    maxInc = sh[j].close-sh[i].close;
                    startMI = i;
                    endMI = j;
                }

    for(int i=startNum;i<=endNum;i++)
         if(i == startMI || i == endMI)
        {
            middle = (int)(((float)i-startNum)*2+1)/(((float)endNum-startNum)*2)*maX;
            close = projection(sh[i].close);
            sprintf(text,"%2.2f",sh[i].close/100.0);
            setbkcolor(LIGHTGRAY);
            setcolor(RED);
            settextstyle(SMALL_FONT, HORIZ_DIR, 5);
            outtextxy(middle, close, text);
        }

    printf("输入日期区间\t\t :%d-%d\n",startDate,endDate);
    printf("最大涨幅的日期区间\t :%d-%d\n",sh[startMI].date,sh[endMI].date);
    printf("对应收盘价\t\t :%2.2f    %2.2f\n",sh[startMI].close/100.0,sh[endMI].close/100.0);
    printf("对应涨幅\t\t :%2.2f%%\n",((float)sh[endMI].close-(float)sh[startMI].close)/(float)sh[startMI].close*100.0);
    getch();
    closegraph();
}

int main()
{
    FILE *fp;
    fp = fopen("sh000001.day","rb");
    int i=1;
    while(1)
    {
        fread(sh+i,sizeof(struct data),1,fp);
        i++;
        if(feof(fp))
            break;
    }
    fclose(fp);
    run(i);
    return 0;
}

