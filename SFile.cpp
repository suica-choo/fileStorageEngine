#ifndef SFile_cpp
#define SFile_cpp
#include "SFile.h"
#include "SFHeader.h"

SFile::SFile() {

}

SFile::~SFile() {

}

static void SFile::writeArchive(std::ofstream &archive, vector<int> chunks, std::ifstream &file) {
  int remaining = this->size;
  file.seekg(0);
  for (int i = 0; i < this->numChunks; i++) {
    int cur_chunk = chunks[i];
    int writeSize = (i == this->numChunks-1) ? remaining : chunk_size;
    archive.seekp(header_size+cur_chunk*chunk_size);
    char* buffer = new char[writeSize];
    file.read(buffer, writeSize);
    archive.write(buffer, writeSize);
  }
  file.close();
}
static void SFile::readArchive(std::ifstream &archive, vector<int> chunks, std::ofstream &file, int size) {
  int remaining = this->size;
  file.seekp(0);
  for (int i = 0; i < this->numChunks; i++) {
    int cur_chunk = chunks[i];
    int readSize = (i == this->numChunks-1) ? remaining : chunk_size;
    archive.seekg(header_size+cur_chunk*chunk_size);
    char* buffer = new char[readSize];
    archive.read(buffer, readSize);
    file.write(buffer, readSize);
  }
  file.close();
}
//TODO
static void SFile::writeWholeArchive(std::fstream& oldArchive, std::ofstream& newArchive, std::vector<block_i> oldBlocks, std::vector<block_i> newBlocks, std::unordered_map<int,int> newOldBlockMap) {
    for (int newIdx = 0; newidx < newBlocks.size(); newIdx++) {
        if (newOldBlockMap.find(newIdx) != newOldBlockMap.end()) {

            int oldIdx = newOldBlockMap[newIdx];
            int remaining = oldBlocks[oldIdx].size;
            for (int i = 0; i < oldBlocks[oldIdx].chunks.size(); i++) {
                int readSize = (remaining >= chunk_size) ? chunk_size : remaining;
                char* buffer = new char[readSize];
                int oldChunk = oldBlocks[oldIdx].chunks[i];
                int newChunk = newBlocks[newIdx].chunks[i];
                int oldPos = header_size + oldChunk*chunk_size;
                int newPos = header_size + newChunk*chunk_size;
                oldArchive.seekg(oldPos);
                oldArchive.read(buffer, readSize);
                newArchive.seekp(newPos);
                newArchive.write(buffer, readSize);
                delete[] buffer;
                remaining -= readSize;
            }
        }
    }
}

#endif

