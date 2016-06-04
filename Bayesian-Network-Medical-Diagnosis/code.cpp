#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

vector<float>roundoff(vector<float> cpt)
{


for(int l=0;l<cpt.size();l++)
{
  float p=cpt[l];
  p = (((float)((int)(p * 10000)))/10000);
  cpt[l]=p;
  
}
return cpt;

}
vector<float> normalize(vector<float> tcp,int n)
{
    vector<float> rcpt;
    int num = tcp.size()/n;

    for(int k=0;k<tcp.size();k++)
    {
        rcpt.push_back(0.0);
    }

    for(int i=0;i<n;i++)
    {
        int sum = 0;
        for(int j=0;j<num;j++)
        {
            sum = sum+tcp[i+j*n];
        }
        for(int j=0;j<num;j++)
        {
            rcpt[i+j*n] = tcp[i+j*n]/sum;
        }
    }
    return rcpt;
}




vector<float> fun(network Alarm,string A[][37],string z,vector<string> parent,int lines)
{
    vector<float> r_cpt;

    int length = parent.size();
    
    for(int i=0;i<lines;i++)
    {
        int pos = 0;

        vector<int> prod;
        prod.push_back(1);

        int len = length;
        int j;
        while(len>0)
        {
            vector<string> temp_all_values = Alarm.search_node(parent[len-1])->get_values();

            int jj = Alarm.get_index(parent[len-1]);
            if(A[i][jj].substr(1,1) == "?")
                goto BRE;
                //break;
            else
            {
                for(int k=0;k<temp_all_values.size();k++)
                {
                    if(temp_all_values[k] == A[i][jj])
                    {
                        pos = pos+k*prod[0];
                        break;
                    }
                }
            }
            prod.insert(prod.begin(),temp_all_values.size()*prod[0]); //pushing front
            len--;
            //cout<<len<<endl;
        }

        j = Alarm.get_index(z);
        if(A[i][j].substr(1,1) != "?")
        {

            vector<string> temp_all_values = Alarm.search_node(z)->get_values();

            for(int k=0;k<temp_all_values.size();k++)
            {
                if(temp_all_values[k] == A[i][j])
                {
                    pos = pos+k*prod[0];
                    break;
                }
            }
            //cout<<"here"<<endl;
            if(r_cpt.size() == 0)
            {
                int zz=0;
                int pro = prod[0]*temp_all_values.size();
                while(zz<pro)
                {
                    r_cpt.push_back(0);
                    zz++;
                }
            }
            r_cpt[pos] = r_cpt[pos]+1;//update r_cpt
        }
        BRE:
        int iii;
    }
    //cout<<i<<endl;
    return r_cpt;
}


int main(int argc, char *argv[])
{
	network Alarm;
	Alarm=read_network();
    
// Example: to do something
	//cout<<"Perfect! Hurrah! \n";
    
  	string temp;

  	int num_variables=Alarm.netSize(); // = 37
  	struct category{
     string name;
     int count;
  	};


    ifstream myfile(argv[2]);
    string line;
    int lines=0;
    while (getline(myfile,line))
    {
     lines++;
    }
    //cout<<lines<<endl;
    myfile.close();
     
    string records[lines][37];

    myfile.open (argv[2]);
     
  	if (myfile.is_open())
    {
    	for(int i=0;i<lines;i++)
    	{
    		stringstream ss;
      		getline (myfile,line);
       		ss.str(line);

     		for(int j=0;j<37;j++)
     		{
     		   ss>>temp;
     		  // if(temp.compare("?")!=0)
     		   records[i][j]=temp;	


     		}

     	}
    myfile.close(); 	
    } 


  	/*for(int zz=0;zz<lines;zz++)    //for printing the values of the records.
    {
        for(int xx = 0;xx<37;xx++)
        {
            cout<<records[zz][xx]<<"\t";
        }
        cout<<endl;
    }*/

    for(int i=0;i<num_variables;i++)
        {
             
            //cout<<i<<endl;
            list<Graph_Node>::iterator akhil = Alarm.get_nth_node(i);
            //cout<<akhil->get_name()<<endl;
        	if((akhil)->get_Parents().empty())
	        {
                //cout<<i<<endl;
                 string s=(akhil)->get_name();
                 int c = akhil->get_nvalues();
                 //cout<<c<<endl;
                 vector<int> tcp;
                 vector<float> cp;
                 float sum = 0.0;
                 for(int j=0;j<c;j++)
                 {
                 	//category n;
                 	//n.count=0;
                 	string stri = akhil->get_values()[j];
                 	
                 	int tc=0;

                 	for(int k=0;k<lines;k++)
                 	{
                 		if(records[k][i] == stri)
                 		{
                 			tc++;
                 		}
                 	}
                 	tcp.push_back(tc);
                 	sum = sum+tc;
                 	//cout<<n.name<<"\t"<<n.count<<endl;
                 }
                 
                 for(int l=0;l<c;l++)
                 {
                    float p=tcp[l]/sum;
                 	p = (((float)((int)(p * 10000)))/10000);
                 	cp.push_back(p);
                 	//cout<<cp[l]<<"\t";
                 }
                 //cout<<here<<endl;
                 Alarm.get_nth_node(i)->set_CPT(cp);
                 //cout<<endl;
            }
            else
            {
                //cout<<i<<endl;
            	vector<string> sai = (akhil)->get_Parents();
                float p = 1.0;
                //cout<<"here"<<"\t"<< (akhil)->get_name()<<endl;
                vector<float> tcp = fun(Alarm,records,akhil->get_name(),sai,lines);
                //smoothing
                for(int llll = 0;llll<tcp.size();llll++)
                {
                    if(tcp[llll] == 0)
                        tcp[llll] = tcp[llll]+1;
                }

                vector<float> fcp = normalize(tcp,akhil->get_nvalues());
                Alarm.get_nth_node(i)->set_CPT(roundoff(fcp));
                for(int lll = 0;lll<tcp.size();lll++)
                {
                    //cout<<fcp[lll]<<"\t";
                }
                //cout<<endl;
                //cout<<endl;
                //cout<<endl;
                //cout<<endl;
                //cout<<tcp[0]<<endl;
                //cout<<"here"<<endl;
                //cout<<i<<endl;
            }
        }

                 //cout<<Alarm.get_nth_node(0)->get_CPT()[0]<<endl;

    

















    string line1;
	int find=0;
	myfile.open (argv[1]); 
  	string temp1;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof())
    	{
    		stringstream ss;
      		getline (myfile,line1);
      		
      		
      		ss.str(line1);
     		ss>>temp1;
     		
     		
     		if(temp1.compare("probability")!=0)
     	    {             
     		cout<<line1;
     		if(!myfile.eof())
     		cout<<endl;    				
     		}
     		else if(temp1.compare("probability")==0)
     		{
                    cout<<line1<<endl;
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                
     				listIt=Alarm.search_node(temp);
                    //int index=Alarm.get_index(temp);

                    //list<Graph_Node>::iterator dude = Alarm.get_nth_node(index);

                    ss>>temp;
     				while(temp.compare(")")!=0)
     				{
       				ss>>temp;
    				}
    				getline (myfile,line1);
     				stringstream ss2;
                    
     				ss2.str(line1);
     				ss2>> temp;
     				cout<<"\t"<<temp<<" ";
                    
     				ss2>> temp;
                    
     				vector<float> CPT=listIt->get_CPT();
                    //string::size_type sz= CPT.size();
                    int cou=0;
     				while(temp.compare(";")!=0)
     				{
                        
     					cout<<CPT[cou]<<" ";
     					cou++;
     					
     					ss2>>temp;
                       
                        

    				}
    				cout<<";"<<endl;
                    
                    


     		}
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
     		
	
}





