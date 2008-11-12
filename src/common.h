#include <vector>

namespace trissa{
	class Player;
	
	typedef Player * (*function_creator_ptr)();
	struct Move {
		int x;
		int y;
		int z;
	};
		
	typedef std::vector<int, std::vector<int, std::vector<int, int> > > Cube;
	
}
