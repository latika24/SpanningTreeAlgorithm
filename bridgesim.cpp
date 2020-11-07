# include <iostream>
# include <vector>
# include <queue>
# include <set>
# include <string>
# include "bridgesim.h" 
using namespace std;

lan lookIntoTable(vector<frow> fwd_table, int d)
{
	lan l;
	l.id='\0';
	for(int i=0; i<fwd_table.size();i++)
	{
		frow f = fwd_table[i];
		if(f.host == d) l = f.fport;
	}
	return l;
}

void DataForwarding(vector<bridge> &b_network, vector<lan> &l_network,lan &hostlan, int tr, int &timestamp, int s , int d){

	queue<data_packet> sent,received;

		for (int i=0; i<hostlan.adjacent_bridges.size(); i++)
		{
			data_packet data;
			
			data.bridge = hostlan.adjacent_bridges[i];
			data.source = s;			
			data.prev = hostlan.id;
			data.destination = d;
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
				}
				else
				{
					int ds = -1 * data.bridge;

					lan l;
					char la=data.prev;
					for(int j=0; j<l_network.size();j++)
						if(l_network[j].id == la ) l=l_network[j];

					for(int j=0; j<l.adjacent_bridges.size(); j++)
					{
						if(ds != l.adjacent_bridges[j])
						{
							data.bridge = l.adjacent_bridges[j];
							received.push(data);
						}
					}
				}
			}

			while(received.empty()==false)
			{
				data_packet data = received.front();
				received.pop();
				int datai=0;
				for(int i=0; i<b_network.size();i++)
				{
					if(data.bridge == b_network[i].id)
					{
						datai = i;
					}
				}

				lan entry = lookIntoTable(b_network[datai].fwd_table, d);
				if(entry.id != '\0')
				{
					int f=0;
					for(int j=0; j<entry.hosts.size();j++)
					{
						if(entry.hosts[j]==data.destination)
						{
							f=1;
							for(int p=0; p<entry.adjacent_bridges.size(); p++)
							{
								for(int q=0; q<b_network.size(); q++)
								{
									if(b_network[datai].id!= b_network[q].id and b_network[q].id == entry.adjacent_bridges[p] and lookIntoTable(b_network[q].fwd_table, s).id == '\0')
									{
										frow e; e.host = s;
										//for(int j=0; j<l_network.size();j++)
										//	if(l_network[j].id == data.prev ) e.fport=l_network[j];
										e.fport = entry;
										b_network[q].fwd_table.push_back(e);
										if(tr==1) cout<<timestamp<<" B"<<b_network[q].id<<" r "<<s<<"->"<<d<<" "<<data.prev<<endl;
									}
								}
							}
						}
					}
					if(f==0 and data.prev!=entry.id)
					{
						traces t;
						t.time = timestamp;
						t.bridge = b_network[datai].id;
						t.status = 'r';
						message m;
						t.m.root = s;
						t.m.dist = d;
						//trace_data.push_back(t);
						if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" "<<data.prev<<endl;
						t.status = 's';
						//trace_data.push_back(t);
						if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" "<<t.m.root<<"->"<<t.m.dist<<" "<<entry.id<<endl;

						for(int j=0; j<entry.adjacent_bridges.size();j++)
						{
							data_packet datas;
							datas.source = data.source;
							datas.prev = entry.id;
							datas.destination = data.destination;								
							datas.bridge = entry.adjacent_bridges[j];
							//cout<<entry.adjacent_bridges[j]<<" "<<b_network[i].id<<endl;
							if(entry.adjacent_bridges[j] != b_network[datai].id) sent.push(datas);
						}
					}
					if(lookIntoTable(b_network[datai].fwd_table, s).id == '\0')
					{
						frow e; e.host = s;
						for(int j=0; j<l_network.size();j++)
							if(l_network[j].id == data.prev ) e.fport=l_network[j];
						b_network[datai].fwd_table.push_back(e);
					}
				}
				else
				{
					frow e; e.host = s;
					for(int j=0; j<l_network.size();j++)
						if(l_network[j].id == data.prev ) e.fport=l_network[j];

					b_network[datai].fwd_table.push_back(e);

					for(int j=0; j<b_network[datai].adjacent_lans.size(); j++)
					{
						data_packet datas;
						datas.source = data.source;
						datas.destination = data.destination;
						datas.prev = b_network[datai].adjacent_lans[j];
						datas.bridge = -1 * b_network[datai].id;
						if(data.prev != b_network[datai].adjacent_lans[j]) sent.push(datas);
					}

					traces t;
					t.time = timestamp;
					t.bridge = b_network[datai].id;
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

}