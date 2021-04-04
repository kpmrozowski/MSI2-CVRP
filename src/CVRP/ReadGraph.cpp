#include <MSI/CVRP/ReadGraph.h>

namespace msi::cvrp {

Table Reader::read_file(std::string filename,int rows,int cols) {
	/* read_file(const char filename, int rows, int cols); */
	
	std::fstream file;
	file.open(filename.c_str(), std::ios::in);
    
	Table table(cols, std::vector<double>(rows));
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
        	file >> table[j][i];
		}
	}
	file.close();

	return table;
}

void Reader::display_file()
{
    int rows = 51;
	int cols = 3;
	Table En51k5_NODE_COORD, En51k5_DEMANDS;
	
	En51k5_NODE_COORD = read_file("./src/CVRP/E-n51-k5_NODE_COORD.txt",rows,cols);

	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
            std::cout <<En51k5_NODE_COORD[j][i] <<"\t";
        }
		std::cout <<"\n";
	}
	std::cout <<"\n";
	std::cout <<"\n";
    cols = 2;
	En51k5_DEMANDS = read_file("./src/CVRP/E-n51-k5_DEMANDS.txt",rows,cols);
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
            std::cout <<En51k5_DEMANDS[j][i] <<"\t";
        }
		std::cout <<"\n";
	}
}

} // namespace msi::cvrp 