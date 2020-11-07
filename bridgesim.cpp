# include <iostream>
# include <vector>
# include <set>
# include <queue>
# include <string>
# include "bridgesim.h" 
using namespace std;

message UpdateConfig(message m,vector<bridge>& bridge_network)
{ //(B1,0,B1) destination B2 lan B source B1
	char lan = m.lan;
	int root = m.root;
	int d = m.dist;
	int destination = m.destination;
	bridge source = m.source;

	message return_message;

	int di;

	for(int i=0; i<bridge_network.size();i++)
	{
		if(destination == bridge_network[i].id)
		{
			di = i;
		}
	}

	bridge b = bridge_network[di];

	if(root < b.root)
	{
		return_message.root = root;
		return_message.source=bridge_network[di];
		return_message.dist = d+1;

		bridge_network[di].root = root;
		bridge_network[di].RP_lan = lan;
		bridge_network[di].RP_bridge = source.id;
		bridge_network[di].root_distance = d+1;
	}
	else if(root == b.root && d+1 < bridge_network[di].root_distance)
	{
		return_message.root = root;
		return_message.source=bridge_network[di];
		return_message.dist = d+1;

		bridge_network[di].root = root;
		bridge_network[di].RP_lan = lan;
		bridge_network[di].RP_bridge = source.id;
		// bridge_network[di].RP = make_pair(lan,source.id);
		bridge_network[di].root_distance = d+1;
	}
	else if (root == b.root && d+1 == bridge_network[di].root_distance && source.id<bridge_network[di].RP_bridge)
	{
		return_message.root = root;
		return_message.source=bridge_network[di];
		return_message.dist = d+1;

		bridge_network[di].root = root;
		bridge_network[di].RP_lan = lan;
		bridge_network[di].RP_bridge = source.id;
		// bridge_network[di].RP = make_pair(lan,source.id);
		bridge_network[di].root_distance = d+1;
	}
	else
	{
		return_message.root = 0;//change -1
		return_message.dist = d+1;
		return_message.source=bridge_network[di];
	}


	return return_message;
}