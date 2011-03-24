#ifndef EXPECT_FILE_EQ_H
#define EXPECT_FILE_EQ_H

#include <string>

void EXPECT_FILE_EQ(const std::string filename1,
                    const std::string filename2);
void EXPECT_PCAPFILE_EQ(const std::string filename1,
                       const std::string filename2);

#endif // EXPECT_FILE_EQ_H