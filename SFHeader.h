#ifndef SFHeader_h
#define SFHeader_h
#include <string>
#include <vector>
#include "SFile.h"
#include <fstream>
class SFHeader { // TODO: SFHeader's structure is not clear. Some functions are not needed. SFHeader's function is also not clear enough.
private:
	std::vector<SFile> files;
	std::vector<int> emptyChunks;
public:
	SFHeader();
	SFHeader(char* headerChar);
	~SFHeader();
	vector<int> assignChunks(int chunkNum);
	void updateHeader();
	void writeHeader(std::ofsream file);
	void readHeader(std::ifstream file);
	std::string encode();
	void decode();
};

#endif
