#include <vector>

namespace trissa{
	using namespace std;
	
	struct Move {
		int x;
		int y;
		int z;
	};
		
	typedef vector<int, vector<int, vector<int, int> > > Cube;
	
}
