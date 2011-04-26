#include "gmock\gmock.h"
#include "gtest\gtest.h"

#include "network/http_header.h"

using ::testing::_;
using ::testing::Return;

//////////////////////////////////////////////////////////////////////////

TEST(http_header, parseLineTest)
{
    typedef unsigned char uchar;

    std::string line = "Header-Name: value";

    http_header::HeaderNameValue para =
        http_header::parseLine(line);

    std::string answer1 = "Header-Name";
    std::string answer2 = "value";

    EXPECT_STREQ(answer1.c_str(), para.first.c_str());
    EXPECT_STREQ(answer2.c_str(), para.second.c_str());
}

//////////////////////////////////////////////////////////////////////////

TEST(http_header, parseLineMultiTest)
{
    typedef unsigned char uchar;

    std::string line = "Header-Name: value\r\n"
        " ending\r\n"
        " finishing";

    http_header::HeaderNameValue para =
        http_header::parseLine(line);

    std::string answer1 = "Header-Name";
    std::string answer2 = "value\r\n ending\r\n finishing";

    EXPECT_STREQ(answer1.c_str(), para.first.c_str());
    EXPECT_STREQ(answer2.c_str(), para.second.c_str());
}

//////////////////////////////////////////////////////////////////////////

TEST(http_header, parseHeaderTest)
{
    typedef unsigned char uchar;

    std::string header = "Header-Name1: value1\r\n"
                         "Header-Name2: value1 value2\r\n"
                         "\r\n";

    http_header::HeaderNameValueList paras =
        http_header::parseHeader(header);

    http_header::HeaderNameValue a1("Header-Name1", "value1");
    http_header::HeaderNameValue a2("Header-Name2", "value1 value2");

    EXPECT_STREQ(a1.first.c_str(), paras[0].first.c_str());
    EXPECT_STREQ(a1.second.c_str(), paras[0].second.c_str());
    EXPECT_STREQ(a2.first.c_str(), paras[1].first.c_str());
    EXPECT_STREQ(a2.second.c_str(), paras[1].second.c_str());

}

//////////////////////////////////////////////////////////////////////////

TEST(http_header, parseHeaderMultilineTest)
{
    typedef unsigned char uchar;

    std::string header = "Header-Name1: value1\r\n"
                         "Header-Name2: value1 value2\r\n"
                                      " value3 value4\r\n"
                                      " value5 value6\r\n"
                         "\r\n";

    http_header::HeaderNameValueList paras =
        http_header::parseHeader(header);

    http_header::HeaderNameValue a1("Header-Name1", "value1");
    http_header::HeaderNameValue a2("Header-Name2", "value1 value2 value3 value4 value5 value6");

    EXPECT_STREQ(a1.first.c_str(), paras[0].first.c_str());
    EXPECT_STREQ(a1.second.c_str(), paras[0].second.c_str());
    EXPECT_STREQ(a2.first.c_str(), paras[1].first.c_str());
    EXPECT_STREQ(a2.second.c_str(), paras[1].second.c_str());

}

//////////////////////////////////////////////////////////////////////////
