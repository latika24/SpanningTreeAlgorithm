# include <iostream>
# include <vector>
# include <set>
# include <queue>
# include <string>
# include "bridge.h"
using namespace std;

class traces{
	public:
		int time;
		int bridge;
		char status;
		message m;
};

class data_packet{
public:
	int source;
	int destination;
	char prev;
	int bridge;
};