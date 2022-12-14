#include <iostream>
using namespace std;

class Node{
    // distance from node and from where to where node 
    public:
    unsigned dist[20];
    unsigned from[20];
}
rt[10];
int main()
{
    // for the distance matrix 
    int dmat[20][20];
    int n,i,j,k,count =0;
    cout<<"Enter the number of nodes:";
    cin>>n;
    cout<<"Enter the cost matrix(999 for no link):";

    // one loop for the rows and other loop for the 
    // column of the distance matrix 

    for(i = 0;i<n;i++)
    for(j = 0;j<n;j++)
    {
        cin>>dmat[i][j];
        // node to same node distance matrix value is 0
        dmat[i][i]=0;

        // if there is no root the value of distance is 999
        rt[i].dist[j]=dmat[i][j];
        rt[i].from[j]=j;
    }
    do
    {
        count = 0;
        // whether the given distance is lesser than the 
        //  alternate path  if it is grater than the alternative 
        // path . path is replaced by alternate path 

        for(i=0;i<n;i++)
        for(j=0;j<n;j++)
        for(k=0;k<n;k++)
        if(rt[i].dist[j]>dmat[i][k]+rt[k].dist[j])
        {
            rt[i].dist[j]=rt[i].dist[k]+rt[k].dist[j];
            rt[i].from[j]=k;
            count++;
        }
    }
    while(count!=0);
    for(i=0;i<n;i++)
    {
        cout<<"State value for router is"<<i+1<<endl;

        for(j=0;j<n;j++)
        {
            cout<<"Node via Distance "<<j+1<<(rt[i].from[j]+1,rt[i].dist[j]) <<endl;
        }
    }

    cout<<"\n";
}