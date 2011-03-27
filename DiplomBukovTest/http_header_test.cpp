#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "network\http_header.h"

using ::testing::_;
using ::testing::Return;

//////////////////////////////////////////////////////////////////////////

TEST(http_transfer_coding, parseIdentGood)
{
    typedef unsigned char uchar;
    typedef std::vector<uchar> Blob;

    unsigned char * data = (uchar *)"abcdefgh";
    int contentLength = 8;
    Blob blob(data, data + contentLength);
    
    std::pair<Blob,int> para =
        http_transfer_coding::parseIdent(blob, contentLength);

    EXPECT_EQ(contentLength, para.second);
    EXPECT_EQ(blob, para.first);
}

//////////////////////////////////////////////////////////////////////////

TEST(http_transfer_coding, parseIdentTooShort)
{
    typedef unsigned char uchar;
    typedef std::vector<uchar> Blob;

    unsigned char * data = (uchar *)"abcdefgh";
    int realSize = 8;
    int contentLength = 20;
    Blob blob(data, data + realSize);

    std::pair<Blob,int> para =
        http_transfer_coding::parseIdent(blob, contentLength);

    EXPECT_EQ(-1, para.second);
}

//////////////////////////////////////////////////////////////////////////

TEST(http_transfer_coding, parseIdentTooLong)
{
    typedef unsigned char uchar;
    typedef std::vector<uchar> Blob;

    unsigned char * data = (uchar *)"abcdefgh";
    int realSize = 8;
    int contentLength = 4;
    Blob blob(data, data + realSize);

    std::pair<Blob,int> para =
        http_transfer_coding::parseIdent(blob, contentLength);

    Blob ans;
    ans.push_back((unsigned char)'a');
    ans.push_back((unsigned char)'b');
    ans.push_back((unsigned char)'c');
    ans.push_back((unsigned char)'d');

    EXPECT_EQ(contentLength, para.second);
    EXPECT_EQ(ans, para.first);
}

//////////////////////////////////////////////////////////////////////////

TEST(http_transfer_coding, parseIdentEmpty)
{
    typedef unsigned char uchar;
    typedef std::vector<uchar> Blob;

    unsigned char * data = (uchar *)"abcdefgh";
    int realSize = 8;
    int contentLength = 0;
    Blob blob(data, data + realSize);

    std::pair<Blob,int> para =
        http_transfer_coding::parseIdent(blob, contentLength);

    EXPECT_EQ(contentLength, para.second);
    EXPECT_EQ(Blob(), para.first);
}

//////////////////////////////////////////////////////////////////////////

TEST(http_transfer_coding, parseChunkSingle)
{
    typedef unsigned char uchar;
    typedef std::vector<uchar> Blob;

    char data[] = "11\r\nabcdefghabcdefghx\r\n0\r\n";
    int realSize = sizeof(data)-1;
    int contentLength = realSize;
    Blob blob((uchar*)data, (uchar*)data + realSize);

    std::pair<Blob,int> para =
        http_transfer_coding::parseChunked(blob, contentLength);

    char answer[] = "abcdefghabcdefghx";
    int realAnswerSize = sizeof(answer)-1;
    Blob ans((uchar*)answer, (uchar*)answer + realAnswerSize);

    EXPECT_EQ(contentLength, para.second);
    EXPECT_EQ(ans, para.first);
}

//////////////////////////////////////////////////////////////////////////
