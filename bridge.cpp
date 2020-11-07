# include "bridge.h"
# include <iostream>
# include <vector>
# include <set>
# include <queue>
# include <string>
using namespace std;


// Possible change - find alternative for this

set<message,messageComparer> SendMessage(message m,vector<bridge> b_network, vector<lan> l_network)
{
	set<message,messageComparer> messages;
	int root = m.root;
	bridge source = m.source;
	int d = m.dist;
	int si;

	for(int i=0; i<b_network.size();i++){
		if(source.id == b_network[i].id){
			si = i;
		}
	}

	for(int j=0; j<b_network[si].adjacent_lans.size(); j++){
		for(int k=0; k<l_network.size(); k++){
			if(b_network[si].adjacent_lans[j] == l_network[k].id){
				for(int p=1; p<l_network[k].adjacent_bridges.size()+1;p++){
					if(l_network[k].adjacent_bridges[p-1] != source.id){
						{
							message ms;
							ms.root = root;
							ms.source = source;
							ms.dist = d;
							ms.lan = l_network[k].id;
							ms.destination = l_network[k].adjacent_bridges[p-1];
							messages.insert(ms);
						}
					}
				}
			}
		}
	}
	return messages;
}

message UpdateConfig(message m,vector<bridge>& b_network)
{ //(B1,0,B1) destination B2 lan B source B1
	char lan = m.lan;
	int root = m.root;
	int d = m.dist;
	int destination = m.destination;
	bridge source = m.source;

	message r_msg;

	int di;

	for(int i=0; i<b_network.size();i++)
	{
		if(destination == b_network[i].id)
		{
			di = i;
		}
	}

	bridge b = b_network[di];

	if(root < b.root)
	{
		r_msg.root = root;
		r_msg.source=b_network[di];
		r_msg.dist = d+1;

		b_network[di].root = root;
		b_network[di].RP_lan = lan;
		b_network[di].RP_bridge = source.id;
		b_network[di].root_distance = d+1;
	}
	else if(root == b.root && d+1 < b_network[di].root_distance)
	{
		r_msg.root = root;
		r_msg.source=b_network[di];
		r_msg.dist = d+1;

		b_network[di].root = root;
		b_network[di].RP_lan = lan;
		b_network[di].RP_bridge = source.id;
		// b_network[di].RP = make_pair(lan,source.id);
		b_network[di].root_distance = d+1;
	}
	else if (root == b.root && d+1 == b_network[di].root_distance && source.id<b_network[di].RP_bridge)
	{
		r_msg.root = root;
		r_msg.source=b_network[di];
		r_msg.dist = d+1;

		b_network[di].root = root;
		b_network[di].RP_lan = lan;
		b_network[di].RP_bridge = source.id;
		// b_network[di].RP = make_pair(lan,source.id);
		b_network[di].root_distance = d+1;
	}
	else
	{
		r_msg.root = 0;//change -1
		r_msg.dist = d+1;
		r_msg.source=b_network[di];
	}


	return r_msg;
}

void SpanningTreeProtocol(vector<bridge> &b_network, vector<lan> &l_network,int tr, int &timestamp){

    	// Implementing spanning tree protocol

	queue<message> received,sentmsg;  
	int initial=1;

	for(int i=0; i<b_network.size();i++)
	{
		message m;
		m.root = b_network[i].id;
		m.dist=0;
		m.source=b_network[i];
		sentmsg.push(m);
	}

    //sentmsg !empty , received empty
	while(sentmsg.empty()==false)
	{
		if(initial != 1)
		{
			while(sentmsg.empty()==false)
			{
				sentmsg.pop();
				//cout<<"sentmsg emptied"<<endl;
			}
		}

		while(received.empty()==false)
		{
			message m = received.front();
			//cout<<"Plucked out from received"<<endl;
			message to_be_published = UpdateConfig(m, b_network);
			if(to_be_published.root != 0)
			{
				sentmsg.push(to_be_published);
				//cout<<"pushed in sentmsg"<<endl;
			}
			traces t;
			t.time = timestamp;
            t.status = 'r';
            t.m = m;
			t.bridge = m.destination;
			if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			received.pop();
			//cout<<"receive trace added"<<endl;
		}

		queue<message> temp;
		while(sentmsg.empty()==false)
		{//m= (B1,0,B1)
			message m = sentmsg.front();
			//cout<<"Plucked out from sentmsg"<<endl;
			set<message,messageComparer> received_by_set = SendMessage(m, b_network, l_network);
			sentmsg.pop();
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

			if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			temp.push(m);
		}

		while(temp.empty()==false)
		{
			sentmsg.push(temp.front());
			temp.pop();
		}

		timestamp++;
		initial = 0;

	}

    for(int i=0; i<l_network.size(); i++)

	{	int DP=l_network[i].adjacent_bridges[0];
		int minsofar = b_network[l_network[i].adjacent_bridges[0]-1].root_distance;
		for (int j =0; j< l_network[i].adjacent_bridges.size(); j++){
				int id = l_network[i].adjacent_bridges[j];

				int currentdistance = b_network[id-1].root_distance;
				// cout<< "B"<<id<<":"<<currentdistance;
				if(currentdistance< minsofar){
					minsofar = currentdistance;
					DP = id;
				}

		}

		l_network[i].DP = DP;

		
	}

}