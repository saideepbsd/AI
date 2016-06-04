#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <ctime>
using namespace std;

#define gi(n) scanf("%d",&n)
#define fr(i,n) for(int i=0;i<n;i++)
#define pb push_back
typedef pair<int,int> pii;
typedef vector<string> vst;
typedef vector<int> vii;

float ti;
int numchar;

int CC;
int score[100][100];
map<char,int> ifd; 
vst overfinal;
int DP[8][100][150][150];
vst splitstrings[10];
vector<vii> fsc;
int bestscore;
struct Node{
   string stn;
   int blcnum;
   Node * parent;
   int cost;  
   vii indices; 
   Node(string st, vii points,int ct,Node  *par){
    //blcnum=blc;
    stn=st;
    cost=ct;
    indices= points;
    parent= par;
   }
   int heuristic(){
  //  vst gencopy=splitstrings[blcnum];
    int nums=indices.size();
    int ans=0;   
    fr(i,nums-1){
        for(int j=i+1;j<nums;j++){
            int l1=splitstrings[blcnum][i].length();
            int l2=splitstrings[blcnum][j].length();
            int x=indices[i];
            int y=indices[j];
            ans+=DP[blcnum][i*nums+j][l1-x][l2-y];
        }
    }
    return ans;
   }
   bool breakcont(){
        bool ans=true;
        int nums=indices.size();
        fr(i,nums){
            if(indices[i]<splitstrings[blcnum][i].length()) {
                ans=false;
                break;
            }
        }
        return ans;
   }
};

bool comparision(Node *a,Node *b){
    bool er=a->cost+a->heuristic() < b->cost+b->heuristic();
    return(er);
}

int sumscore(vst tired){  
    int l=tired.size();
    int commlen=tired[0].length();
    int ans=0;
    for(int i=0;i<l;i++){
        for(int j=0;j<commlen;j++){
           for(int k=0;k<l;k++){
               if(k==i) continue;
               ans+=score[ifd[tired[i][j]]][ifd[tired[k][j]]];
           }
        }
    }
    ans/=2;
    fr(i,l){
        fr(j,commlen){
            if(tired[i][j]=='-') ans+=CC;
        }
    }
    return ans;
}

vector<vii> caloverallscore(vst tired){  
    int l=tired.size();
    int commlen=tired[0].length();
    vector<vii> ans(l,vii (commlen,0));
    for(int i=0;i<l;i++){
        for(int j=0;j<commlen;j++){
           for(int k=0;k<l;k++){
               if(k==i) continue;
               ans[i][j]+=score[ifd[tired[i][j]]][ifd[tired[k][j]]];
           }
        }
    }
    fr(i,l){
        fr(j,commlen){
            if(tired[i][j]=='-') ans[i][j]+=CC;
        }
    }
    return ans;
}

vst useonly(vst stringblock,int blocknum){
    vst retans;
    int numstr=stringblock.size();
    for(int i=0;i<numstr-1;i++){
        for(int j=i+1;j<numstr;j++){
            int a=i*numstr+j;
            string s1=stringblock[i];
            reverse(s1.begin(), s1.end());
            string s2=stringblock[j];
            reverse(s2.begin(), s2.end());
            int l1=s1.length();
            int l2=s2.length();
            DP[blocknum][a][0][0]=0;
            for(int xy=1;xy<=s1.length();xy++) {
                char c1=s1[xy]; 
                DP[blocknum][a][xy][0]=CC+score[numchar][ifd[c1]]+DP[blocknum][a][xy-1][0];
            }
            for(int xy=1;xy<=s2.length();xy++) {
                char c1=s2[xy]; 
                DP[blocknum][a][0][xy]=CC+score[ifd[c1]][numchar]+DP[blocknum][a][0][xy-1];
            }
            for(int x=1;x<=l1;x++){
                for(int y=1;y<=l2;y++){
                    char c1=s1[x-1];
                    char c2=s2[y-1];
                    if(c1==c2){ 
                        DP[blocknum][a][x][y]=DP[blocknum][a][x-1][y-1]; 
                    } 
                    else{
                        int w1=DP[blocknum][a][x-1][y-1] + score[ifd[c1]][ifd[c2]];
                        int w2=DP[blocknum][a][x-1][y]+ CC +score[numchar][ifd[c2]];
                        int w3=DP[blocknum][a][x][y-1]+ CC +score[ifd[c1]][numchar];
                        DP[blocknum][a][x][y]=min(w1,min(w2,w3));
                    }
                }
            }
        }
    }    
    string trial(numstr,'*');   
    vii initindex;
    fr(i,numstr) initindex.push_back(0);
    Node * root= new Node(trial,initindex,0,NULL);
    root->blcnum=blocknum;
    vector<Node *> LNode;
    Node *finalans ;
    LNode.push_back(root);
    bool found=false;
    while(!found){
        vector<Node * >trial;
        for(int ni=0; ni<LNode.size();ni++){
            Node* t=LNode[ni];   
            bool checkcont;
            for(int i=1;i<(1<<numstr);i++){         
                checkcont=false;
                string curr(numstr,'-');
                vii cindex=t->indices;
                int topcost=t->cost;   
                for(int j=0;j<numstr;j++){
                    if(i & (1<<j)){
                            if(stringblock[j].length()<=cindex[j]){
                                checkcont=true;
                                continue;
                            }
                            curr[j]=stringblock[j][cindex[j]];
                            cindex[j]++;
                    }
                    else{
                            topcost+=CC;
                            curr[j]='-';
                    }
                }
                if(checkcont) continue;
                for(int a=0;a<numstr-1;a++){
                    for(int b=a+1;b<numstr;b++){
                        char x=curr[a];
                        char y=curr[b];
                        topcost+=score[ifd[x]][ifd[y]];
                    }
                }
                Node *child= new Node(curr,cindex,topcost,t);
                child->blcnum=blocknum;
                trial.push_back(child); 
                if(child->breakcont()) {
                    found=true;
                    finalans= child;
                    break;
                }
                if(found) break;  
            }
            if(found) break;
        }
        sort(trial.begin(),trial.end(),comparision);
        LNode.clear();
        for(int i=0; i<trial.size() && i<10;i++) LNode.push_back(trial[i]);
        trial.clear();
    }
    vst ans;
    fr(ui,numstr) ans.push_back("");
    while(finalans->cost>0){
        fr(ui,numstr){
            ans[ui]=finalans->stn[ui]+ans[ui];
        } 
        finalans=finalans->parent;
    }
    fr(i,ans.size()){
        retans.push_back(ans[i]);
    }
    return retans;
}

void swap(int a,int b){
    vst copy=overfinal;
    int lx=copy[a].length();
    int intialscore=0;
    fr(i,lx) intialscore+=fsc[a][i];
    string temp;
    copy[a]=copy[a].substr(0,b)+copy[a].substr(a+1,lx-b-1);
    for(int i=0;i<overfinal[a].length();i++){
        if(copy[a][i]=='-') continue;
        copy[a]= copy[a].substr(0,i)+'-'+copy[a].substr(i,copy[a].length()-i);
        if(bestscore<sumscore(copy)) {
            overfinal=copy;
            bestscore=sumscore(copy);
            break;
        }
    }
}

int main(){ 
    scanf("%f",&ti);
    ti=ti*60-2; //time in seconds
    clock_t begin = clock();
    memset(DP,-1,sizeof(DP));
    gi(numchar);       
    char dict[numchar+1]; 
    string vocab; 
    int numstrings;
    vst genpat;
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
    int sumlength=0;
    fr(i,numstrings){
        sumlength+=genpat[i].length();
    }
    int blksize=10;
    if(ti>140) blksize=10;
    else if(ti>90) blksize=8;
    else blksize=6;
    int blocks;    
    blocks=(numstrings-1)/blksize+1;
    for(int i=0,j=0;i<numstrings;i++){
        splitstrings[j].push_back(genpat[i]);
        if((i+1)%blksize==0) j++; 
    }
    fr(i,blocks){
       //cout<<i<<endl;
        fr(j,splitstrings[i].size()){
            //cout<<splitstrings[i][j]<<endl;
        }
    } 
    int max_size=INT_MIN;
    int maxblocklength[blocks];
    int maxpoints[blocks];
    memset(maxblocklength,0,sizeof(maxblocklength));

    fr(i,blocks){
       vst here=useonly(splitstrings[i],i);
       int fun=here[0].length();
       fr(j,here.size()){
           int fu=here[i].length();
           max_size=max(max_size,fu);
           if(maxblocklength[i]<fu) {
             maxblocklength[i]=fu;
             maxpoints[i]=j;
            }
           overfinal.push_back(here[j]);
       }
    }
    fr(i,overfinal.size()){
        while(overfinal[i].length()<max_size){
            overfinal[i]+='-';
        }
       // cout<<overfinal[i]<<endl;
    }
    vector<pii> alldots;   
    fsc=caloverallscore(overfinal);
    bestscore=sumscore(overfinal);
    int ret=0;
    fr(i,fsc.size()){
        fr(j,fsc[0].size()){
            if(overfinal[i][j]=='-') alldots.push_back(make_pair(i,j));
            ret+=fsc[i][j];
        }
    }
    /*
    int inblock;
    if(blocks>1){
       if(maxblocklength[0]>maxblocklength[1]) inblock=maxblocklength[1];
       else inblock=maxblocklength[0]; 
    }
    */
    /*
    int smallDP[150][150];
    memset(smallDP,0,sizeof(DP));
    string s1=splitstrings[0][maxpoints[0]];
    s1.reverse();
    string s2=splitstrings[1][maxpoints[1]];
    s2.reverse();
    int l1=s1.length();
    int l2=s2.length();
    for(int xy=1;xy<=s1.length();xy++) {
        char c1=s1[xy]; 
        smallDP[xy][0]=CC+score[numchar][ifd[c1]]+DP[a][xy-1][0];
    }
    for(int xy=1;xy<=s2.length();xy++) {
        char c1=s2[xy]; 
        smallDP[0][xy]=CC+score[ifd[c1]][numchar]+smallDP[0][xy-1];
    }
    for(int x=1;x<=l1;x++){
        for(int y=1;y<=l2;y++){
            char c1=s1[x-1];
            char c2=s2[y-1];
            if(c1==c2){ 
                smallDP[x][y]=smallDP[x-1][y-1];  
            } 
            else{
                int w1=smallDP[x-1][y-1]+score[ifd[c1]][ifd[c2]];
                int w2=smallDP[x-1][y]+CC+score[numchar][ifd[c2]];
                int w3=smallDP[x][y-1]+CC+score[ifd[c1]][numchar];
                smallDP[x][y]=min(w1,min(w2,w3));
            }
        }
    }
    */
    map<int,bool> checkvisit;
    while(true){
        clock_t end = clock();
        float elapsed_secs  = float(end - begin) / CLOCKS_PER_SEC; 
         int x=rand() % alldots.size();
        if(checkvisit.find(x)!=checkvisit.end()){
           pii a=alldots[x];
           swap(a.first,a.second);
           checkvisit[x]=true;
        }
        if(elapsed_secs>=ti) {
            fr(i,numstrings){
                cout<<overfinal[i]<<endl;
            } 
            
            break;
        }
    }   
}


