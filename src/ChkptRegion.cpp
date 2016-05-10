#include "ChkptRegion.h"

#include <fstream>
#include <sstream>

ChkptRegion::ChkptRegion(std::string filename): filename(filename)
{
    // Open file for reading
    std::ifstream ifs(filename, std::ifstream::binary);
    if(!ifs.is_open())
    {
        std::cerr << "[ERROR] Could not open CHECKPOINT_REGION file for reading" << std::endl;
        exit(1);
    }

    char* buffer = new char[32];
    ifs.read(buffer, 32);

    int blockNum = 0;
    for (int i = 0; i < 40; i++)
    {
        ifs.read((char*)&blockNum, sizeof(blockNum));
        imap[i] = blockNum;
        std::cerr << "Read in block number: " << blockNum << std::endl;
    }

    // Close file for reading
    ifs.close();

    // Read in segment information
    nextFreeSeg = -1;
    for(int i = 0; i < 32; ++i)
    {
        segInfo[i] = (buffer[i] == '1');
        if(nextFreeSeg == -1 && !segInfo[i])
        {
            nextFreeSeg = i;
        }
    }

    // Free buffer
    delete[] buffer;
}

ChkptRegion::~ChkptRegion()
{
    // Open file for writing
    std::ofstream ofs(filename, std::ofstream::binary | std::ofstream::trunc);
    if(!ofs.is_open())
    {
        std::cerr << "[ERROR] Could not open CHECKPOINT_REGION file for writing" << std::endl;
        exit(1);
    }

    // Update file contents
    for (int i = 0; i < 32; i++)
    {
        char live[1];
        live[0] = (segInfo[i]) ? '1' : '0';
        ofs.write(live, sizeof(char));
    }

    for (int i = 0; i < 40; i++)
    {
        ofs.write(reinterpret_cast<const char *>(&imap[i]), sizeof(int));
    }

    ofs.close();
}

void ChkptRegion::addimap(int blkNumber) { imap[nextimapPiece++] = blkNumber; }

int ChkptRegion::getimap(int imapPiece) { return imap[imapPiece]; }

int ChkptRegion::getNextFreeSeg() { return nextFreeSeg; }

void ChkptRegion::markSegment(int segment, bool state)
{
    if(segment < 40 && segment >= 0)
    {
        segInfo[segment] = state;
        // Calculate next free segment
        for (int i = 0; i < 40; ++i)
        {
            if (!segInfo[i])
            {
                nextFreeSeg = i;
                break;
            }
        }
    }
}