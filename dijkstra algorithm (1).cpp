#include <iostream>
using namespace std;
const int inf = 1e7;

int main()
{
    int32_t n,m;
    // where n is number of vertices and m is a edges 
    cin>>n>>m;
    vector<int>dist(n+1,inf);
    vector<vector<pair<int,int>>>graph(n+1);
    for(int i =0; i<m; i++){
        // for the edges 
        int u,v,w;
        cin>>u>>v>>w;
        graph[u].push_back({v,w});
        graph[v].push_back({u,w});
    }
    int source;
    cin>>source;
    dist[source] = 0;
    set<pair<int,int>>s;
    // {wt,vertex}
    s.insert({0,source});
    while(!s.empty())
    {
        auto x = *(s,begin());
        s.erase(x);
        for(auto it: graph[x.second]){
            if(dist[it.first]>dist[s.second]+it.second){
                s.erase({dist[it.first],it.first});
                dist[it.first] = dist[x.second]+it.second;
                s.insert({dist[it.first],it.first});
            }
        }
    }
    for(int i = 1;i<n;i++){
        if(dist[i]<inf){
            cout<<dist[i]<<" ";
        }
        else
        {
            cout<<-1<<" ";
        }
    }

}