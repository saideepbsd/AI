#include <iostream>
#include <bits/stdc++.h>
#include <string.h>

using namespace std;

#define gi(n) scanf("%d",&n)
#define fr(i,n) for(int i=0;i<n;i++)
#define pb push_back
typedef vector<string> vst;
typedef vector<int> vii;

float ti;
int numchar;
int numstrings;
vst genpat;
int CC;
int score[100][100];

int DP[900][500][500];
int CCcount[900][500][500];

struct Node{
   string stn;
   Node * parent;
   //childstrings of node to be generated
   int cost;  //cost  to reach that node
   vii indices; //indices of k strings
   Node(){
   }
   Node(string st, vii points,int ct,Node  *par){
    stn=st;
    cost=ct;
    indices= points;
    parent= par;
   }
   int lengthcovered(){
   	int ans=0;
   	fr(i,numstrings){
   		ans+=indices[i];
   	}
   	return ans;
   }
   int heuristic(){
    int ans=0;   
    vii res;
    fr(i,numstrings){
       res.pb(genpat[i].length()-indices[i]);
    }
    sort(res.begin(),res.end());
    //int max=res[res.size()-1];
    //fr(i,res.size()) ans+=(max-res[i])*CC;
    int numcccount=0;
    int manumcount=INT_MIN;
    int minumcount=INT_MAX;
    fr(i,numstrings-1){
        for(int j=i+1;j<numstrings;j++){
            int l1=genpat[i].length();
            int l2=genpat[j].length();
            int x=indices[i];
            int y=indices[j];
            ans+=DP[i*numstrings+j][l1-x][l2-y];
            manumcount=max(manumcount,CCcount[i*numstrings+j][l1-x][l2-y]);
            minumcount=min(minumcount,CCcount[i*numstrings+j][l1-x][l2-y]);
            numcccount+=CCcount[i*numstrings+j][l1-x][l2-y];
        }
    }
    ans-=numcccount;
    //manumcount=max(manumcount,numstrings*minumcount);
    //ans+=manumcount;
    return ans;
   }
};

class comparision{           
    public:
        bool operator()(Node *a,Node *b){
        	bool er=a->cost+a->heuristic() > b->cost+b->heuristic();
        	//return er;
            return (er || ((a->cost+a->heuristic()==b->cost+b->heuristic()) && a->lengthcovered() < b->lengthcovered()) );
        }
};//sort in order of heurisitic + cost till that

priority_queue<Node * ,vector<Node *>,comparision> PQNode;

int main(){ 
    scanf("%f",&ti);// cout<<ti<<endl; //ti=time
    memset(DP,-1,sizeof(DP));
    memset(CCcount,0,sizeof(CCcount));
    gi(numchar);    //cout<<numchar<<endl;// numchar = num of characters in dictionary
    map<char,int> ifd; //ifd=maps char to its ixndex
    char dict[numchar+1]; //dict =stores the characters of dictionary 
    string vocab; 
    int ser=0;
    for(int i=0;i<numchar;i++){
      cin>>vocab;
      dict[ser]=vocab[0];
      ser++;
    }
    dict[ser]='-';
    for(int i=0;i<numchar+1;i++){
       ifd[dict[i]]=i;
    }
    gi(numstrings);  
    fr(i,numstrings){
       string arbitmax;
       cin>>arbitmax;
       genpat.push_back(arbitmax);
    }
    gi(CC); 
    fr(i,numchar+1){
        fr(j,numchar+1){
            int x; gi(x);
            score[i][j]=x;
        }
    }
    char rf;
    cin>>rf; 
    for(int i=0;i<numstrings-1;i++){
        for(int j=i+1;j<numstrings;j++){
            int a=i*numstrings+j;
            string s1=genpat[i];
            reverse(s1.begin(), s1.end());
            string s2=genpat[j];
            reverse(s2.begin(), s2.end());
            int l1=s1.length();
            int l2=s2.length();
            DP[a][0][0]=0;
            CCcount[a][0][0]=0;
            for(int xy=1;xy<=s1.length();xy++) {
                char c1=s1[xy]; 
                DP[a][xy][0]=CC+score[numchar][ifd[c1]]+DP[a][xy-1][0];
                CCcount[a][xy][0]=xy*CC;
            }
            for(int xy=1;xy<=s2.length();xy++) {
                char c1=s2[xy]; 
                DP[a][0][xy]=CC+score[ifd[c1]][numchar]+DP[a][0][xy-1];
                CCcount[a][0][xy]=xy*CC;
            }
            for(int x=1;x<=l1;x++){
                for(int y=1;y<=l2;y++){
                    char c1=s1[x-1];
                    char c2=s2[y-1];
                    if(c1==c2){ 
                        DP[a][x][y]=DP[a][x-1][y-1]; CCcount[a][x][y]=CCcount[a][x-1][y-1]; 
                    } 
                    else{
                        int w1=DP[a][x-1][y-1]+score[ifd[c1]][ifd[c2]];
                        int w2=DP[a][x-1][y]+CC+score[numchar][ifd[c2]];
                        int w3=DP[a][x][y-1]+CC+score[ifd[c1]][numchar];
                        DP[a][x][y]=min(w1,min(w2,w3));
                        CCcount[a][x][y]=INT_MAX;
                        if(w1==DP[a][x][y]) CCcount[a][x][y]=min(CCcount[a][x-1][y-1],CCcount[a][x][y]);
                        if(w2==DP[a][x][y]) CCcount[a][x][y]=min(CCcount[a][x-1][y]+CC,CCcount[a][x][y]);
                        if(w3==DP[a][x][y]) CCcount[a][x][y]=min(CCcount[a][x][y-1]+CC,CCcount[a][x][y]);
                    }
                }
            }
        }
    } 
    string trial(numstrings,' ');   
    vii initindex;
    int sumlength=0;
    fr(i,numstrings) sumlength+=genpat[i].length();
    fr(i,numstrings) initindex.push_back(0);
    Node * root= new Node(trial,initindex,0,NULL);
    PQNode.push(root);
   // cout<<root->heuristic()<<endl;
    Node *finalans;
    int finalcost;
    int runtimes=0;
    int count=0;
    while(!PQNode.empty()){  
        Node *t=PQNode.top();
        if(runtimes>100000){
            count++;
            runtimes-=100000;
        } 
        PQNode.pop();
        if(t->lengthcovered()==sumlength) {             
            finalans=t;
            break;
        } 
        bool checkcont; 
        for(int i=1; i< (1<<numstrings) ;i++ ){
               runtimes++;
               checkcont=false;
               string curr(numstrings,'-');
               vii cindex=t->indices;
               int topcost=t->cost;    
               for(int j=0;j<numstrings;j++){
                    if(i & (1<<j)) {
                        if(genpat[j].length()<=cindex[j]) {
                            checkcont=true;
                            continue;
                        }
                        curr[j]=genpat[j][cindex[j]];
                        cindex[j]++;
                    }
                    else{
                        topcost+=CC;
                        curr[j]='-';
                    }
               }
               if(checkcont) continue;
               for(int a=0;a<numstrings-1;a++){
                    for(int b=a+1;b<numstrings;b++){
                        char x=curr[a];
                        char y=curr[b];
                        topcost+=score[ifd[x]][ifd[y]];
                    }
                }
                Node *child= new Node(curr,cindex,topcost,t);
                PQNode.push(child);     
        }
    }
    int ancost=finalans->cost;
  //  cout<<ancost<<endl;
    vst ans;
    fr(i,numstrings) ans.push_back("");
    int out=0;
    while(finalans->cost>0){
        fr(i,numstrings){
            ans[i]=finalans->stn[i]+ans[i];
        } 
        finalans=finalans->parent;
    }
    fr(i,numstrings){
        cout<<ans[i]<<endl;
    }
}
