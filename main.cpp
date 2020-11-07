# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <queue>
# include <utility>
# include <algorithm>
# include "bridgesim.h"
using namespace std;

struct messageComparer
{
    bool operator()(message const &j1, message const &j2)
    {
    	if(j1.destination < j2.destination) return true;
    	else return false;
        //compare j1 and j2 and return true or false
    }
};

// Possible change - find alternative for this

set<message,messageComparer> SendMessage(message m,vector<bridge> bridge_network, vector<lan> lan_network)
{
	set<message,messageComparer> messages;
	int root = m.root;
	bridge source = m.source;
	int d = m.dist;
	int si;

	for(int i=0; i<bridge_network.size();i++){
		if(source.id == bridge_network[i].id){
			si = i;
		}
	}

	for(int j=0; j<bridge_network[si].adj_lans.size(); j++){
		for(int k=0; k<lan_network.size(); k++){
			if(bridge_network[si].adj_lans[j] == lan_network[k].id){
				for(int p=1; p<lan_network[k].adj_bridges.size()+1;p++){
					if(lan_network[k].adj_bridges[p-1] != source.id){
						{
							message ms;
							ms.root = root;
							ms.source = source;
							ms.dist = d;
							ms.lan = lan_network[k].id;
							ms.destination = lan_network[k].adj_bridges[p-1];
							messages.insert(ms);
						}
					}
				}
			}
		}
	}
	return messages;
}

// message UpdateConfig(message m,vector<bridge>& bridge_network)
// { //(B1,0,B1) destination B2 lan B source B1
// 	char lan = m.lan;
// 	int root = m.root;
// 	int d = m.dist;
// 	int destination = m.destination;
// 	bridge source = m.source;

// 	message return_message;

// 	int di;

// 	for(int i=0; i<bridge_network.size();i++)
// 	{
// 		if(destination == bridge_network[i].id)
// 		{
// 			di = i;
// 		}
// 	}

// 	bridge b = bridge_network[di];

// 	if(root < b.root)
// 	{
// 		return_message.root = root;
// 		return_message.source=bridge_network[di];
// 		return_message.dist = d+1;

// 		bridge_network[di].root = root;
// 		bridge_network[di].RP_lan = lan;
// 		bridge_network[di].RP_bridge = source.id;
// 		bridge_network[di].root_distance = d+1;
// 	}
// 	else if(root == b.root && d+1 < bridge_network[di].root_distance)
// 	{
// 		return_message.root = root;
// 		return_message.source=bridge_network[di];
// 		return_message.dist = d+1;

// 		bridge_network[di].root = root;
// 		bridge_network[di].RP_lan = lan;
// 		bridge_network[di].RP_bridge = source.id;
// 		// bridge_network[di].RP = make_pair(lan,source.id);
// 		bridge_network[di].root_distance = d+1;
// 	}
// 	else if (root == b.root && d+1 == bridge_network[di].root_distance && source.id<bridge_network[di].RP_bridge)
// 	{
// 		return_message.root = root;
// 		return_message.source=bridge_network[di];
// 		return_message.dist = d+1;

// 		bridge_network[di].root = root;
// 		bridge_network[di].RP_lan = lan;
// 		bridge_network[di].RP_bridge = source.id;
// 		// bridge_network[di].RP = make_pair(lan,source.id);
// 		bridge_network[di].root_distance = d+1;
// 	}
// 	else
// 	{
// 		return_message.root = 0;//change -1
// 		return_message.dist = d+1;
// 		return_message.source=bridge_network[di];
// 	}


// 	return return_message;
// }


void SpanningTreeProtocol(vector<bridge> &bridge_network, vector<lan> &lan_network,int traceflag, int timestamp){

    	// Implementing spanning tree protocol

	queue<message> spawned,received;
	int initial=1;

	for(int i=0; i<bridge_network.size();i++)
	{
		message m;
		m.root = bridge_network[i].id;
		m.dist=0;
		m.source=bridge_network[i];
		spawned.push(m);
	}

    //spawned !empty , received empty
	while(spawned.empty()==false)
	{
		if(initial != 1)
		{
			while(spawned.empty()==false)
			{
				spawned.pop();
				//cout<<"spawned emptied"<<endl;
			}
		}

		while(received.empty()==false)
		{
			message m = received.front();
			//cout<<"Plucked out from received"<<endl;
			message to_be_published = UpdateConfig(m, bridge_network);
			if(to_be_published.root != 0)
			{
				spawned.push(to_be_published);
				//cout<<"pushed in spawned"<<endl;
			}
			traces t;
			t.time = timestamp;
            t.status = 'r';
            t.m = m;
			t.bridge = m.destination;
			if(traceflag==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			received.pop();
			//cout<<"receive trace added"<<endl;
		}

		queue<message> temp;
		while(spawned.empty()==false)
		{//m= (B1,0,B1)
			message m = spawned.front();
			//cout<<"Plucked out from spawned"<<endl;
			set<message,messageComparer> received_by_set = SendMessage(m, bridge_network, lan_network);
			spawned.pop();
			while(received_by_set.empty()==false)
			{
				received.push(*received_by_set.begin());
				received_by_set.erase(received_by_set.begin());
			}
			traces t;
			t.time = timestamp;
            t.status = 's';
            t.m = m;
			t.bridge = m.source.id;

			if(traceflag==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			temp.push(m);
		}

		while(temp.empty()==false)
		{
			spawned.push(temp.front());
			temp.pop();
		}

		timestamp++;
		initial = 0;

	}

    for(int i=0; i<lan_network.size(); i++)

	{	int DP=lan_network[i].adj_bridges[0];
		int minsofar = bridge_network[lan_network[i].adj_bridges[0]-1].root_distance;
		for (int j =0; j< lan_network[i].adj_bridges.size(); j++){
				int id = lan_network[i].adj_bridges[j];

				int currentdistance = bridge_network[id-1].root_distance;
				// cout<< "B"<<id<<":"<<currentdistance;
				if(currentdistance< minsofar){
					minsofar = currentdistance;
					DP = id;
				}

		}

		lan_network[i].DP = DP;

		// lan_network[i].DP = *min_element(begin(lan_network[i].adj_bridges),end(lan_network[i].adj_bridges));
	}

}

lan lookIntoTable(vector<ftable> forwarding_table, int d)
{
	lan l;
	l.id='\0';
	for(int i=0; i<forwarding_table.size();i++)
	{
		ftable f = forwarding_table[i];
		if(f.host == d) l = f.fport;
	}
	return l;
}
//-----


int main(){

    int tr;
    int n;

    cin>>tr;
    cin>>n;

    //bridge and lan defined in bridge.h
    set<char> lan_set; // Possible change - write code for getting unique lans
    vector<bridge> bridge_network;
    vector<lan> lan_network;

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
						b.adj_lans.push_back(line[j]);
                        lan_set.insert(line[j]);
					}
				}
			}
			sort(b.adj_lans.begin(),b.adj_lans.end()); // why do we have to sort?
			bridge_network.push_back(b);

        }


    }

    while(lan_set.empty()==false)
	{
        lan l;
		char c = *lan_set.begin();
		l.id = c;
		for(int i=0; i<bridge_network.size();i++)
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
				if (bridge_network[i].adj_lans[j] == c) l.adj_bridges.push_back(bridge_network[i].id);
		lan_network.push_back(l);
		lan_set.erase(lan_set.begin());
	}

	int timestamp = 0;
    //passed by reference
    SpanningTreeProtocol(bridge_network, lan_network,tr, timestamp);


	for(int i=0; i<bridge_network.size(); i++)
	{
		cout<<"B"<<bridge_network[i].id<<":";
		for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
		{
			int flag=-1;
			char c = bridge_network[i].adj_lans[j];
			cout<<" "<<c<<"-";
			if(c == bridge_network[i].RP_lan) {cout<<"RP";flag=0;}
			for(int k=0;k<lan_network.size();k++)
			{
				if(c==lan_network[k].id && bridge_network[i].id==lan_network[k].DP && flag==-1)
					{cout<<"DP";flag=0;break;}
			}
			if(flag==-1)
			{
				cout<<"NP";
				bridge_network[i].adj_lans.erase(remove(bridge_network[i].adj_lans.begin(), bridge_network[i].adj_lans.end(), c), bridge_network[i].adj_lans.end());
				j--;
			}
		}
		cout<<endl;
	}


	// Data Simulation
	//Updating Lan network

	for(int i=0; i<lan_network.size();i++)
	{
		char c = lan_network[i].id;
		vector<int> l;
		for(int i=0; i<bridge_network.size();i++)
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
				if (bridge_network[i].adj_lans[j] == c) l.push_back(bridge_network[i].id);
		lan_network[i].adj_bridges = l;
	}

	// Input for data simulation

	for(int i=1; i<lan_network.size()+1; i++)
	{	//A: H1 H2 H3
		string line;
		getline(cin,line);
		for(int j=0;j<line.size();j++)
		{
			if(line[j]!= ' ' && line[j]!='H' && line[j]!=lan_network[i-1].id && line[j]!=':')
			{
				if(line[j+1]!= ' ' && line[j+1]!='\0' && line[j+1]!='H' && line[j+1]!=':'  )
				{
					lan_network[i-1].hosts.push_back(10*((int)line[j] -48) + ((int)line[j+1] -48));
					j++;
				}
				else lan_network[i-1].hosts.push_back((int)line[j] - 48);
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
			for(int i=0; i<lan_network.size();i++){
				for(int p=0; p<lan_network[i].hosts.size(); p++){
					if(lan_network[i].hosts[p]==s) hostlan=lan_network[i];
				}
			}

			queue<data_packet> sent,received;

			for (int i=0; i<hostlan.adj_bridges.size(); i++)
			{
				data_packet data;
				data.source = s;
				data.destination = d;
				data.prev = hostlan.id;
				data.bridge = hostlan.adj_bridges[i];
				received.push(data);
			}
			int fl=1;
			while(!sent.empty() or fl==1)
			{
				while(!sent.empty())
				{
					data_packet data = sent.front();
					sent.pop();
					if(data.bridge > 0)
					{
						received.push(data);
						//cout<<data.bridge<<endl;
					}
					else
					{
						int ds = -1 * data.bridge;
						//cout<<"Bridge lt 0 and "<<data.bridge<<endl;
						lan l;
						char la=data.prev;
						for(int j=0; j<lan_network.size();j++)
							if(lan_network[j].id == la ) l=lan_network[j];

						for(int j=0; j<l.adj_bridges.size(); j++)
						{
							if(ds != l.adj_bridges[j])
							{
								data.bridge = l.adj_bridges[j];
								received.push(data);
								//cout<<"Pushed "<<data.bridge<<endl;
							}
						}
					}
				}

                //change possible
				while(!received.empty())
				{
					data_packet data = received.front();
					received.pop();
					int datai=0;
					for(int i=0; i<bridge_network.size();i++)
					{
						if(data.bridge == bridge_network[i].id)
						{
							datai = i;
						}
					}
					lan lookin = lookIntoTable(bridge_network[datai].forwarding_table, d);
					if(lookin.id != '\0')
					{
						int f=0;
						for(int j=0; j<lookin.hosts.size();j++)
						{
							if(lookin.hosts[j]==data.destination)
							{
								f=1;
								for(int p=0; p<lookin.adj_bridges.size(); p++)
								{
									for(int q=0; q<bridge_network.size(); q++)
									{
										if(bridge_network[datai].id!= bridge_network[q].id and bridge_network[q].id == lookin.adj_bridges[p] and lookIntoTable(bridge_network[q].forwarding_table, s).id == '\0')
										{
											ftable e; e.host = s;
											//for(int j=0; j<lan_network.size();j++)
											//	if(lan_network[j].id == data.prev ) e.fport=lan_network[j];
											e.fport = lookin;
											bridge_network[q].forwarding_table.push_back(e);
											if(tr==1) cout<<timestamp<<" B"<<bridge_network[q].id<<" r "<<s<<"->"<<d<<" "<<data.prev<<endl;
										}
									}
								}
							}
						}
						if(f==0 and data.prev!=lookin.id)
						{
							traces t;
							t.time = timestamp;
							t.bridge = bridge_network[datai].id;
							t.status = 'r';
							message m;
							t.m.root = s;
							t.m.dist = d;
							//trace_data.push_back(t);
							if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" "<<data.prev<<endl;
							t.status = 's';
							//trace_data.push_back(t);
							if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" "<<lookin.id<<endl;

							for(int j=0; j<lookin.adj_bridges.size();j++)
							{
								data_packet datasend;
								datasend.source = data.source;
								datasend.prev = lookin.id;
								datasend.destination = data.destination;								
								datasend.bridge = lookin.adj_bridges[j];
								//cout<<lookin.adj_bridges[j]<<" "<<bridge_network[i].id<<endl;
								if(lookin.adj_bridges[j] != bridge_network[datai].id) sent.push(datasend);
							}
						}
						if(lookIntoTable(bridge_network[datai].forwarding_table, s).id == '\0')
						{
							ftable e; e.host = s;
							for(int j=0; j<lan_network.size();j++)
								if(lan_network[j].id == data.prev ) e.fport=lan_network[j];
							bridge_network[datai].forwarding_table.push_back(e);
						}
					}
					else
					{
						ftable e; e.host = s;
						for(int j=0; j<lan_network.size();j++)
							if(lan_network[j].id == data.prev ) e.fport=lan_network[j];

						bridge_network[datai].forwarding_table.push_back(e);

						for(int j=0; j<bridge_network[datai].adj_lans.size(); j++)
						{
							data_packet datasend;
							datasend.source = data.source;
							datasend.destination = data.destination;
							datasend.prev = bridge_network[datai].adj_lans[j];
							datasend.bridge = -1 * bridge_network[datai].id;
							if(data.prev != bridge_network[datai].adj_lans[j]) sent.push(datasend);
						}

						traces t;
						t.time = timestamp;
						t.bridge = bridge_network[datai].id;
						t.status = 'r';
						message m;
						t.m.root = s;
						t.m.dist = d;
						if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" "<<data.prev<<endl;
						//trace_data.push_back(t);
						t.status = 's';
						if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" - "<<endl;
						//trace_data.push_back(t);
					}

				}
				timestamp++;
				fl=0;
			}

			for(int i=0; i<bridge_network.size(); i++)
			{
				cout<<"B"<<bridge_network[i].id<<":"<<endl;
				cout<<"HOST ID | FORWARDING PORT"<<endl;
				for(int j=bridge_network[i].forwarding_table.size()-1; j>=0; j--)
				{
					ftable f = bridge_network[i].forwarding_table[j];
					cout<<"H"<<f.host<<" | "<<f.fport.id<<endl;
				}
			}
			cout<<endl;




		}
	}




}
