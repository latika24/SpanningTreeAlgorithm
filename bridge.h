# include <iostream>
# include <vector>
# include <set>
# include <queue>
# include <string>
using namespace std;

class lan{

    public:

        char id;
        vector<int> adj_bridges;
        vector<int> hosts;
        int DP;
        lan()
		{
			id = '\0';
			DP = -1;
		}
};


class ftable
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
		int root_distance;
		vector<char> adj_lans;
		vector<ftable> forwarding_table;
		pair<char,int> RP;
		bridge()
		{
			id = -1;
			root = id;
			RP = make_pair('\0',-1);
			root_distance = -1;
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
