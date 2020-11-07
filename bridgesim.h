# include <iostream>
# include <queue>
# include <vector>
# include <set>
# include <string>
# include "bridge.h"
using namespace std;


class data_packet{
public:
	int bridge;
	char prev;
	int destination;
	int source;

};

// Functions


lan lookIntoTable(vector<frow> fwd_table, int d);

void DataForwarding(vector<bridge> &b_network, vector<lan> &l_network, lan &hostlan, int tr, int &timestamp, int s, int d);
