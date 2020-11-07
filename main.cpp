# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <queue>
# include <utility>
# include <algorithm>
# include "bridgesim.h"
using namespace std;



int main(){

    int tr;
    int n;

    cin>>tr;
    cin>>n;

    //bridge and lan defined in bridge.h
    set<char> lan_set; // Possible change - write code for getting unique lans
    vector<bridge> b_network;
    vector<lan> l_network;

    for(int i = 1 ; i<=n+1 ; i++){ //change

        bridge b;
		string line;
		getline(cin,line);

        //First line is empty, so don't do anything in first iteration
        if(i!=1){
			for(int j=1; j<line.size(); j++) //j==0 is just B
			{
				if(j==1)
				{
                    if(line[j+1]==':')
					{
						b.id = (int) line[j] - 48;
						b.root = b.id;  // initially root = id
					}
					else // 2 digit
					{   b.id = 10*((int) line[j] -48) + ((int) line[j+1] -48);
						b.root = b.id;
						j++;
					}
				}
				if(j!=1 & j!=2)
				{
					if(line[j] != ' ' and line[j]!=':')
					{
						b.adjacent_lans.push_back(line[j]);
                        lan_set.insert(line[j]);
					}
				}
			}
			sort(b.adjacent_lans.begin(),b.adjacent_lans.end()); // why do we have to sort?
			b_network.push_back(b);

        }


    }

    while(lan_set.empty()==false)
	{
        lan l;
		char c = *lan_set.begin();
		l.id = c;
		for(int i=0; i<b_network.size();i++)
			for(int j=0; j<b_network[i].adjacent_lans.size(); j++)
				if (b_network[i].adjacent_lans[j] == c) l.adjacent_bridges.push_back(b_network[i].id);
		l.DP= *min_element(l.adjacent_bridges.begin(),l.adjacent_bridges.end());
		l_network.push_back(l);
		lan_set.erase(lan_set.begin());
	}

	int timestamp = 0;

    //passed by reference
    SpanningTreeProtocol(b_network, l_network,tr, timestamp);


	for(int i=0; i<b_network.size(); i++)
	{
		cout<<"B"<<b_network[i].id<<":";
		for(int j=0; j<b_network[i].adjacent_lans.size(); j++)
		{
			int flag=-1;
			char c = b_network[i].adjacent_lans[j];
			cout<<" "<<c<<"-";
			if(c == b_network[i].RP_lan) {cout<<"RP";flag=0;}
			for(int k=0;k<l_network.size();k++)
			{
				if(c==l_network[k].id && b_network[i].id==l_network[k].DP && flag==-1)
					{cout<<"DP";flag=0;break;}
			}
			if(flag==-1)
			{
				cout<<"NP";
				b_network[i].adjacent_lans.erase(remove(b_network[i].adjacent_lans.begin(), b_network[i].adjacent_lans.end(), c), b_network[i].adjacent_lans.end());
				j--;
			}
		}
		cout<<endl;
	}


	// Reflecting DP and NP in lan network

	for(int i=0; i<l_network.size();i++)
	{
		char c = l_network[i].id;
		vector<int> l;
		for(int i=0; i<b_network.size();i++)
			for(int j=0; j<b_network[i].adjacent_lans.size(); j++)
				if (b_network[i].adjacent_lans[j] == c) l.push_back(b_network[i].id);
		l_network[i].adjacent_bridges = l;
	}


	//Data Simulation

	// Input for data simulation

	for(int i=1; i<l_network.size()+1; i++)
	{	//A: H1 H2 H3
		string line;
		getline(cin,line);
		for(int j=0;j<line.size();j++)
		{
			if(line[j]!= ' ' && line[j]!='H' && line[j]!=l_network[i-1].id && line[j]!=':')
			{
				if(line[j+1]!= ' ' && line[j+1]!='\0' && line[j+1]!='H' && line[j+1]!=':'  )
				{
					l_network[i-1].hosts.push_back(10*((int)line[j] -48) + ((int)line[j+1] -48));
					j++;
				}
				else l_network[i-1].hosts.push_back((int)line[j] - 48);
			}
		}
	}

	// vector<traces> trace_data;
	// Input data transfers

	cin>>n;
	for(int k=1; k<=n+1; k++)
	{
		int s,d,flag=-1;
		string line;
		getline(cin,line);
		if(k!=1)
		{
			for (int j = 0; j < line.size(); j++)
			{
				if( line[j]!='H' && line[j]!= ' ')
				{
					if(line[j+1]!= ' ' && line[j+1]!='H' && line[j+1]!='\0')
					{
						if (flag == -1)
						{
							s = 10*((int)line[j] -48) + ((int)line[j+1] -48);
							flag=0;
						}
						else d = 10*((int)line[j] -48) + ((int)line[j+1] -48);
						j++;
					}
					else
					{
						if(flag==-1)
						{
							s = (int)line[j] - 48;
							flag=0;
						}
						else d = (int)line[j] - 48;
					}
				}
			}

		//Processing area;
		lan hostlan;
		for(int i=0; i<l_network.size();i++){
			for(int p=0; p<l_network[i].hosts.size(); p++){
				if(l_network[i].hosts[p]==s) hostlan=l_network[i];
			}
		}

		DataForwarding(b_network, l_network, hostlan, tr, timestamp, s , d);


		for(int i=0; i<b_network.size(); i++)
		{
			cout<<"B"<<b_network[i].id<<":"<<endl;
			cout<<"HOST ID | FORWARDING PORT"<<endl;
			for(int j=b_network[i].fwd_table.size()-1; j>=0; j--)
			{
				frow f = b_network[i].fwd_table[j];
				cout<<"H"<<f.host<<" | "<<f.fport.id<<endl;
			}
		}
		cout<<endl;




		}
	}




}
