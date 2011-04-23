#include <iostream>
#include "expect_file.h"

struct pcap_file_header 
{
    unsigned   int magic_number;   /* magic number */
    unsigned short version_major;  /* major version number */
    unsigned short version_minor;  /* minor version number */
    signed   int thiszone;         /* GMT to local correction */
    unsigned   int sigfigs;        /* accuracy of timestamps */
    unsigned   int snaplen;        /* max length of captured packets, in octets */
    unsigned   int network;        /* data link type */
};

struct pcap_packet_header
{
    unsigned int ts_sec;	/* timestamp seconds */
    unsigned int ts_usec;   /* timestamp microseconds */
    unsigned int incl_len;  /* number of octets of packet saved in file */
    unsigned int orig_len;  /* actual length of packet */
};

void EXPECT_FILE_EQ(const std::string filename1,
                    const std::string filename2)
{
    int ret = 0;
    
    FILE * file1 = NULL;
    ret = fopen_s(&file1, filename1.c_str(), "rb");
    if (ret != 0)
    {
        std::cerr << "File \"" << filename1 << "\" does not exist! (code #"
                  << (int)errno << ")\n";
        return;
    }

    FILE * file2 = NULL;
    ret = fopen_s(&file2, filename2.c_str(), "rb");
    if (ret != 0)
    {
        std::cerr << "File \"" << filename2 << "\" does not exist! (code #"
                  << (int)errno << ")\n";
        return;
    }

    char buf1[4096];
    char buf2[4096];

    // While two files not ended
    while (!feof(file1) || !feof(file2))
    {
        int size1 = fread(buf1, 1, sizeof(buf1), file1);
        int size2 = fread(buf2, 1, sizeof(buf2), file2);

        if (size1 != size2)
        {
            std::cerr << "Files \"" << filename1 << "\" and \"" 
                      << filename2 << "\" have different sizes! (code #"
                      << (int)errno << ")\n";
            return;
        }

        int cmp = memcmp(buf1, buf2, size1);
        if (cmp != 0)
        {
            std::cerr << "Files \"" << filename1 << "\" and \"" 
                      << filename2 << "\" have different content!\n";
            return;
        }
    }
    
    fclose(file1);
    fclose(file2);
}

void EXPECT_PCAPFILE_EQ(const std::string filename1,
                        const std::string filename2)
{
    int ret = 0;

    FILE * file1 = NULL;
    ret = fopen_s(&file1, filename1.c_str(), "rb");
    if (ret != 0)
    {
        std::cerr << "File \"" << filename1 << "\" does not exist! (code #"
            << (int)errno << ")\n";
        return;
    }

    FILE * file2 = NULL;
    ret = fopen_s(&file2, filename2.c_str(), "rb");
    if (ret != 0)
    {
        std::cerr << "File \"" << filename2 << "\" does not exist! (code #"
            << (int)errno << ")\n";
        return;
    }

    pcap_file_header pfh1;
    pcap_file_header pfh2;
    int size1 = fread(&pfh1, 1, sizeof(pfh1), file1);
    int size2 = fread(&pfh2, 1, sizeof(pfh2), file2);
    if (size1 != size2)
    {
        std::cerr << "Files \"" << filename1 << "\" and \"" 
                  << filename2 << "\" have different sizes! (code #"
                  << (int)errno << ")\n";
        return;
    }

    int cmp = memcmp(&pfh1, &pfh2, sizeof(pfh1));
    if (cmp != 0)
    {
        std::cerr << "Files \"" << filename1 << "\" and \"" 
                  << filename2 << "\" have different PCAP file headef!\n";
        return;
    }

    char buf1[65536];
    char buf2[65536];

    // While two files not ended
    while (!feof(file1) || !feof(file2))
    {
        pcap_packet_header pph1;
        pcap_packet_header pph2;
        int size1 = fread(&pph1, 1, sizeof(pph1), file1);
        int size2 = fread(&pph2, 1, sizeof(pph2), file2);
        if (size1 != size2)
        {
            std::cerr << "Files \"" << filename1 << "\" and \"" 
                      << filename2 << "\" have different sizes! (code #"
                      << (int)errno << ")\n";
            return;
        }

        size1 = fread(buf1, 1, pph1.incl_len, file1);
        size2 = fread(buf2, 1, pph1.incl_len, file2);
        if (size1 != size2)
        {
            std::cerr << "Files \"" << filename1 << "\" and \"" 
                      << filename2 << "\" have different sizes! (code #"
                      << (int)errno << ")\n";
            return;
        }

        cmp = memcmp(buf1, buf2, size1);
        if (cmp != 0)
        {
            std::cerr << "Files \"" << filename1 << "\" and \"" 
                      << filename2 << "\" have different content!\n";
            return;
        }
    }

    fclose(file1);
    fclose(file2);
}