struct distance_table
{
    int costs[4][4];
} ALTER_SEQNUM(dt);

extern struct rtpkt
{
    int sourceid;   /* id of sending router sending this pkt */
    int destid;     /* id of router to which pkt being sent
                       (must be an immediate neighbor) */
    int mincost[4]; /* min cost to node 0 ... 3 */
};

extern int TRACE;
extern int YES;
extern int NO;

//Function
void tolayer2(struct rtpkt packet);

void creatertpkt(struct rtpkt *initrtpkt,
            int srcid,
            int destid,
            int mincosts[]);

void printall(struct distance_table *dtptr, int n);


void ALTER_SEQNUM(rtremind)()
{	  
    //遍历所有节点
    for (int i = 0; i < 4; ++i)
    {	  
        //当节点为自己或者节点不相邻时continue
        if (i == N || ALTER_SEQNUM(connectcosts)[i] == 999)
            continue;
		    //若是相邻节点则创建包并且发送至layer2		
        else
        {
            struct rtpkt *newpkt = (struct rtpkt *)malloc(sizeof(struct rtpkt));
            creatertpkt(newpkt, N, i, ALTER_SEQNUM(dt).costs[N]);
            tolayer2(*newpkt);
            printf("src %d send cost to dest %d\n",N,i);
        }
    }
    // printall(ALTER_SEQNUM(&dt),N);
}

void ALTER_SEQNUM(rtinit)()
{
	  //先将dt表所有边置为无穷
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ALTER_SEQNUM(dt).costs[i][j] = 999;
	  //然后将自己临近的边置为初始化的值
    for (int i = 0; i < 4; ++i)
        ALTER_SEQNUM(dt).costs[N][i] = ALTER_SEQNUM(connectcosts)[i];
	  //接着通知临近节点
    ALTER_SEQNUM(rtremind)();
}

void ALTER_SEQNUM(rtupdate)(rcvdpkt) struct rtpkt *rcvdpkt;
{
    printall(ALTER_SEQNUM(&dt), N);

    for (int j = 0; j < 4; ++j)
    {
        ALTER_SEQNUM(dt).costs[rcvdpkt->sourceid][j] = rcvdpkt->mincost[j];
    }
        
    int flag = 0;
    for (int i = 0; i < 4; ++i)
    {   
        int min = ALTER_SEQNUM(connectcosts)[i];
        for(int j=0;j<4;++j)
        {   if(j == N)
                continue;

            if (min > ALTER_SEQNUM(connectcosts)[j] + ALTER_SEQNUM(dt).costs[j][i])
            {
                min = ALTER_SEQNUM(connectcosts)[j] + ALTER_SEQNUM(dt).costs[j][i];
            }
        }
        if(min != ALTER_SEQNUM(dt).costs[N][i]){
          flag = 1;
        }
          
        ALTER_SEQNUM(dt).costs[N][i] = min;
    }
    if (flag)
    {
        ALTER_SEQNUM(rtremind)();
    }
  
    printall(ALTER_SEQNUM(&dt), N);
}

void ALTER_SEQNUM(linkhandler)(int linkid, int newcost) 
{
    printf("\nLINK CHANGE!!!\n");
    ALTER_SEQNUM(connectcosts)[linkid] = newcost;
    for (int i = 0; i < 4; ++i)
        ALTER_SEQNUM(dt).costs[N][i] = ALTER_SEQNUM(connectcosts)[i];

    ALTER_SEQNUM(rtremind)();
    printall(ALTER_SEQNUM(&dt), N);

}