#include "SFHeader.h"
#include <unordered_set>
#include <unordered_map>
std::vector<int> SFHeader::assignChunks(int chunkNum) {
    std::vector<int> res;
    std::unordered_set<int> used;
    for (int i = 0; i < this->blocks.size(); i++) {
        if (this->blocks[i].exist) {
            for (int j = 0; j < this->blocks[i].chunks.size(); j++) {
                used.insert(this->blocks[i].chunks[j]);
            }
        }
    }
    int cur_chunk = 0;
    int assigned = 0;
    while (assigned != chunkNum) {
        if (used.find(cur_chunk) != used.end()) {
            res.push_back(cur_chunk);
            assigned++;
        }
        cur_chunk++;
    }
    return res;
}

std::vector<int> SFHeader::addFileHeader(block_i &block, std::ofstream archive) {
    int size = block.size;
    int chunkNum = size / chunk_size;
    if (size % chunk_size != 0) {
        chunkNum++;
    }
    std::vector<int> assignedChunks = this->assignChunks(chunkNum);
    for (int i = 0; i < chunkNum; i++) {
        block.chunks.push_back(assignedChunks[i]);
    }
    int blockIdx = 0;
    for (int i = 0; i < this->blocks.size(); i++) {
        if (!this->blocks[i].exist) {
            blockIdx = i;
            break;
        }
    }
    block.exist = true;
    int pos = blockIdx*block_size;
    int writeSize = block_size;
    archive.seekp(pos);
    archive.write((char*)&block, writeSize);
    return assignedChunks;
}
void SFHeader::delFileHeader(int atype, std::string aname, std::ofstream archive) {
    int blockIdx = 0;
    for (int i = 0; i < this->blocks.size(); i++) {
        int type = this->blocks[i].type;
        std::string name(this->blocks[i].name);
        if (type == atype && name == aname) {
            this->blocks[i].exist = false;
            blockIdx = i;
            break;
        }
    }
    int pos = blockIdx*block_size;
    int writeSize = block_size;
    archive.seekp(pos);
    archive.write((char*)&(this->blocks[i]), writeSize);
}

std::vector<int> SFHeader::getFile(int atype, std::string aname) {
    for (int i = 0; i < this->blocks.size(); i++) {
        int type = this->blocks[i].type;
        std::string name(this->blocks[i].name);
        if (type == atype && name == aname && this->blocks[i].exist) {
            return this->blocks[i].chunks;
        }
    }
}

int SFHeader::getFileSize(int atype, std::string aname) {
    for (int i = 0; i < this->blocks.size(); i++) {
        int type = this->blocks[i].type;
        std::string name(this->blocks[i].name);
        if (type == atype && name == aname && this->blocks[i].exist) {
            return this->blocks[i].size;
        }
    }
}


void SFHeader::readHeader(std::ifstream& archive) {
    archive.seekg(0);
    archive.read((char*)&(this->blocks), header_size);
}
void SFHeader::writeHeader(std::ofsream& archive) {
    archive.seekp(0);
    archive.write((char*)&(this->blocks), header_size);
}
void SFHeader::updateWholeHeader(std::fstream& archive) {
    int usedChunks = 0;
    int neededChunks = 0;
    archive.seekg(0, std::ios::end);
    int size = archive.tellg() - header_size;
    usedChunks = size / chunk_size;
    for (int i = 0; i < this->blocks.size(); i++) {
        if (this->blocks[i].exist) {
            neededChunks += (this->blocks[i].chunks.size());
        }
    }
    if (usedChunks > neededChunks*2) {
        std::vector<block_i> newBlocks;
        for (int i = 0; i < maxFileNumber; i++) {
            newBlocks[i].type = AVA;
            for (int j = 0; j < name_size; j++) {
                newBlocks[i].name[j] = 'x';
            }
            newBlocks[i].exist = false;
            newBlocks[i].size = 0;
            newBlocks[i].date = __DATE__;
        }
        int newPtr = 0;
        int chunkPtr = 0;
        std::unordered_map<int, int> newOldBlockMap;
        for (int i = 0; i < this->blocks.size(); i++) {
            if (this->blocks[i].exist) {
                for (int j = 0; j < this->blocks[i].chunks.size(); j++) {
                    newBlocks[newPtr].chunks.push_back(chunkPtr);
                    chunkPtr++;
                }
                newBlocks[newPtr].exist = true;
                for (int j = 0; j < name_size; j++) {
                    newBlocks[newPtr].name[j] = this->blocks[i].name[j];
                }
                for (int j = 0; j < 10; j++) {
                    newBlocks[newPtr].date[j] = this->blocks[i].date[j];
                }
                newOldBlockMap[newPtr] = i;
                newBlocks[newPtr].size = this->blocks[i].size;
                newBlocks[newPtr].type = this->blocks[i].type;
                newPtr++;
            }
        }
      //TODO
      /* 0.create new archive, write header to new archive.
       *
       * 1.write content to new archive by calling SFile.writeWholeArchive()
       * 2.delete old archive, rename new archive to archive.bin.
       */

    }
}


bool contains(char* name, std::string s) { //TODO: check if the name contains string s.

}
void SFHeader::listFiles(std::string s) {
    // go through this->blocks, check file name, list.
    for (int i = 0; i < this->blocks.size(); i++) {
        if (this->blocks[i].exist) {
            if (contains(this->blocks[i].name, s)) {
                std::cout << this->blocks[i].name << " " << this->blocks[i].size <<"byte " << this->blocks[i].date << endl;
            }
        }
    }
}

void SFHeader::listFiles() {
    for (int i = 0; i < this->blocks.size(); i++) {
        if (this->blocks[i].exist) {
            std::cout << this->blocks[i].name << " " << this->blocks[i].size <<"byte " << this->blocks[i].date << endl;
        }
    }
}

