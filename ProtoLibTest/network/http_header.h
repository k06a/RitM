#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include "i64u64.h"
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <sstream>

//////////////////////////////////////////////////////////////////////////

typedef std::vector<u8> Blob;

struct http_transfer_coding
{
    static std::pair<Blob,int> parseIdent(Blob blob, unsigned contentLength)
    {
        if (contentLength > blob.size())
            return make_pair(Blob(), -1);

        Blob vec(blob.begin(), blob.begin()+contentLength);
        return make_pair(vec, contentLength);
    }

    // Blob and original size or -1 if not ended
    static std::pair<Blob,int> parseChunked(Blob blob, unsigned contentLength)
    {
        Blob vec;

        Blob::iterator offset = blob.begin();
        while (true)
        {
            const char rn[] = "\r\n";
            Blob::iterator it = std::search(offset, blob.end(), rn, rn+2);
            std::string line(it - offset, 'a');
            std::copy(offset, it, line.begin());
            it = it + 2;

            int chunkSize = 0;
            if (1 != sscanf_s(line.c_str(), "%X", &chunkSize))
                return make_pair(Blob(), -1);
            if (chunkSize == 0)
                return std::make_pair(vec, it - blob.begin());
            if (blob.end() - offset < chunkSize)
                return std::make_pair(Blob(), -1);

            std::copy(it, it+chunkSize, back_inserter(vec));

            offset = it + chunkSize + 2;
        }
    }
};

struct http_header
{
    typedef std::pair<std::string,std::string> HeaderNameValue;
    typedef std::vector<HeaderNameValue> HeaderNameValueList;

    static HeaderNameValue parseLine(std::string line)
    {
        HeaderNameValue para;

        // Copy header name
        unsigned pos = line.find(':');
        para.first.resize(pos);
        std::copy(line.begin(), line.begin()+pos, para.first.begin());
        
        // Find first non-space char after ':'
        do ++pos;
        while((line[pos] == ' ') || (line[pos] == '\t'));
        
        // Copy header value
        para.second.resize(line.size() - pos);
        std::copy(line.begin()+pos, line.end(), para.second.begin());

        return para;
    }

    static HeaderNameValueList parseHeader(std::string header)
    {
        HeaderNameValueList vec;

        std::string::iterator offset = header.begin();
        while (offset != header.end())
        {
            const char rnrn[] = "\r\n";
            std::string::iterator it =
                std::search(offset, header.end(), rnrn, rnrn+2);
            
            std::string line(it - offset, 'a');
            std::copy(offset, it, line.begin());

            if ((*offset == ' ') || (*offset == '\t'))
            {
                if (vec.size())
                    vec.back().second += line;
            }
            else
                vec.push_back(parseLine(line));

            offset = it + 2; // Skip \r\n
        }

        return vec;
    }
};

struct http_respounce
{
    enum Version
    {
        UNRECOGNIZED,
        HTTP_1_0,
        HTTP_1_1
    };

    static std::string versionToString(const Version & version)
    {
        switch (version)
        {
            case HTTP_1_0:  return "HTTP/1.0";
            case HTTP_1_1:  return "HTTP/1.1";
            default:    return "";
        }
    }

    static Version versionToEnum(const std::string & version)
    {
        if (version == "HTTP/1.0")
            return HTTP_1_0;
        if (version == "HTTP/1.1")
            return HTTP_1_1;
        return UNRECOGNIZED;
    }

    enum CodeType
    {
        _1xx = 1,   // Information message
        _2xx = 2,   // Some king success
        _3xx = 3,   // Redirects to another URL
        _4xx = 4,   // Error on the client's part
        _5xx = 5,   // Error on the server's part
    };

    static CodeType codeToType(int code)
    {
        return (CodeType)(code / 100);
    }

    static std::string codeToText(int code)
    {
        switch (code)
        {
            // HTTP 1.0 - http://ftp.ics.uci.edu/pub/ietf/http/rfc1945.html#Status-Codes
            // HTTP 1.1 - http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10
            case 200:  return "OK";
            case 301:  return "Moved Permanently";
            case 302:  return "Moved Temporarily";
            case 303:  return "See Other";  // See in "Location:*"
            case 404:  return "Not Found";
            case 500:  return "Server Error";
            default:   return "";
        }
    }

public:
    Version version;
    int code;
};

//////////////////////////////////////////////////////////////////////////

struct http_request
{
    enum Method
    {
        UNRECOGNIZED,
        GET,
        POST,
        HEAD
    };

    static std::string methodToString(const Method & method)
    {
        switch (method)
        {
            case GET:   return "GET";
            case POST:  return "POST";
            case HEAD:  return "HEAD";
            default:    return "";
        }
    }

    static Method methodToEnum(const std::string & method)
    {
        if (method == "GET")
            return GET;
        if (method == "POST")
            return POST;
        if (method == "HEAD")
            return HEAD;
        return UNRECOGNIZED;
    }

public:
    Method method;
    std::string url;
    http_respounce::Version version;
};

#endif // HTTP_HEADER_H