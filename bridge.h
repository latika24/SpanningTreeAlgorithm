# include <iostream>
# include <queue>
# include <vector>
# include <set>
# include <string>
using namespace std;

class lan{

    public:

        char id;
        vector<int> adjacent_bridges;
        vector<int> hosts;
        int DP;
        lan()
		{
			id = '\0';
			DP = -1;
		}
};


class frow
{
	public:
		int host;
		lan fport;
};
class bridge{

// defn of class bridge
   public:
		int id;
		int root;
		vector<frow> fwd_table;
        int root_distance;
		vector<char> adjacent_lans;
		// pair<char,int> RP;
        char RP_lan;
        int RP_bridge;
		bridge()
		{
			id = -1;
			root = id;
			// RP = make_pair('\0',-1);
            RP_lan = '\0';
            RP_bridge = -1;
			root_distance = -1;
		}
};
struct less_than_host
{
    inline bool operator() (const frow& f1, const frow& f2)
    {
        return (f1.host < f2.host);
    }
};

class message{

    public:
		int root;
		int dist;
		bridge source;
		int destination;
		char lan;
		message()
		{
			root = -1;
			dist = -1;
			destination = -1;
			lan = '\0';
		}
};

class traces{
	public:
		int bridge;
		char status;
		int time;
		message m;
};
struct messageComparer
{
    bool operator()(message const &j1, message const &j2)
    {
    	if(j1.destination < j2.destination) return true;
    	else return false;
        //compare j1 and j2 and return true or false
    }
};


// Functions useful for Implementing spanning tree protocol

message UpdateConfig(message m,vector<bridge>& b_network);

set<message,messageComparer> SendMessage(message m,vector<bridge> b_network, vector<lan> l_network);

void SpanningTreeProtocol(vector<bridge> &b_network, vector<lan> &l_network,int tr, int &timestamp);
