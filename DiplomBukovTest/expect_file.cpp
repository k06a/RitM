#include <iostream>
#include "expect_file.h"

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
