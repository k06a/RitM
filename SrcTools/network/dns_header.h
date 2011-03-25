#ifndef DNS_HEADER_H
#define DNS_HEADER_H

#include "i64u64.h"
#include <vector>
#include <string>
#include <utility>

#pragma pack(push,1)
struct dns_header
{
    u16be ID;           // Question-Answer ID

    struct flags_struct
    {
        u16 RD:1;       // 1 - client can work recursive
        u16 TC:1;       // 1 - was truncated (wait for TCP transmitting)
                        // UDP data MUST be less than 512 bytes
        u16 AA:1;       // 1 - Authorized answer
        u16 Opcode:4;   // 0 - Standard request
                        // 1 - Inverse request
                        // 2 - Request server status
                        // 4 - Notify
        u16 QR:1;       // 0 - Request
                        // 1 - Response
        u16 RCODE:4;    // 0 - no errors
                        // 1 - error in request format
                        // 2 - server error
                        // 3 - name does not exist
        u16 Z:3;        // Reserved bits (should be zero)
        u16 RA:1;       // 1 in answer mean, server can work recursive
    } flags;

    u16be QDCOUNT;      // Number of Questions
    u16be ANCOUNT;      // Number of Answers
    u16be NSCOUNT;      // Number of Authority
    u16be ARCOUNT;      // Number of Additional

    enum RequestAnswerType
    {
        A	  = 1,      // IP-адрес
        NS	  = 2,      // Сервер имен
        CNAME = 5,      // Каноническое имя
        SOA	  = 6,	    // Начало списка серверов. Большое число полей, определяющих часть иерархии имен, которую использует сервер
        MB	  = 7,	    // Имя домена почтового ящика
        WKS	  = 11,	    // well-known service — стандартная услуга
        PTR	  = 12,	    // Запись указателя
        HINFO = 13,	    // Информация об ЭВМ
        MINFO = 14,	    // Информация о почтовом ящике или списке поч¬товых адресов
        MX	  = 15,	    // Запись о почтовом сервере. Включает в себя при¬оритет обработчика почты
        TXT	  = 16,	    // Не интерпретируемая строка ASCII-символов
        //ISDN          // Связывает имя ЭВМ с адресом ISDN
        AXFR  = 252,    // Запрос зонного обмена
        ANY	  = 255,    // Запрос всех записей
    };
};
#pragma pack(pop)

struct DnsSymbolicName
{
    typedef std::vector<std::string> NameList;
    typedef std::vector<u8> Blob;

    static NameList recursiveParse(u8 * data, int size, int pos)
    {
        NameList vec;
        u8 * ptr = data + pos;
        while ((*ptr != 0) && (ptr-data < size))
        {
            if ((*ptr & 0xC0) == 0)
            {
                vec.push_back(std::string((char*)ptr+1, *ptr));
                ptr += *ptr + 1;
            }
            else
            {
                int offset = (*(u16be*)ptr) & 0x3F;
                NameList v = recursiveParse(data, size, offset);
                //for (int i = 0; i < v.size(); i++)
                //    vec.push_back(v[i]);
                vec.insert(vec.end(), v.begin(), v.end());
                ptr += 2;
                return vec;
            }
        }
        return vec;
    }

    static int countNameLength(u8 * data, int size, int pos)
    {
        u8 * ptr = data + pos;
        while (*ptr != 0)
        {
            if (*ptr & 0xC0)
            {
                ptr += 2;
                return ptr - (data + pos);
            }
            else
                ptr += *ptr + 1;
        }
        ptr++;
        return ptr - (data + pos);
    }

    static Blob dumpToData(NameList names)
    {
        Blob data;
        for (unsigned i = 0; i < names.size(); i++)
        {
            data.push_back(names[i].size());
            for (unsigned j = 0; j < names[i].size(); j++)
                data.push_back(names[i][j]);
        }
        data.push_back(0);
        return data;
    }

    static Blob recursiveDumpToData(
        std::vector<std::pair<int,NameList> > prevNames, NameList names)
    {
        Blob data;

        unsigned longestIndex = (unsigned)-1;
        unsigned longestData = (unsigned)-1;
        for (unsigned i = 0; i < prevNames.size(); i++)
        {
            unsigned currentData = 0;
            unsigned n = prevNames[i].second.size();
            for (unsigned j = 0; j < n; j++)
            {
                if (names[names.size()-j] != prevNames[i].second[n-1-j])
                    break;
                currentData += names[j].size();
            }
            if (currentData > longestData)
                longestIndex = i;
        }

        if (longestIndex != -1)
        {
            int elementsToCopy = names.size() - prevNames[longestIndex].second.size();
            NameList vec(names.begin(), names.begin()+elementsToCopy);
            data = dumpToData(vec);
            data.pop_back();
            // FIX 0xC0 to 14 bit offset
            data.push_back(0xC0);
            data.push_back(prevNames[longestIndex].first);
        }
        else
        {
            return dumpToData(names);
        }

        return data;
    }

    static std::string readableName(NameList names)
    {
        if (names.size() == 0)
            return "";
        std::string str = names[0];
        for (unsigned i = 1; i < names.size(); i++)
            str += "." + names[i];
        return str;
    }
};

struct DnsRequest
{
    std::vector<std::string> symbolicName;
    u16be questionType;
    u16be questionClass;
    int fieldsSumSize;

    DnsRequest(u8 * data, int size, int pos)
        : fieldsSumSize(0)
    {
        parseFromData(data, size, pos);
    }

    std::vector<u8> dumpToData()
    {
        std::vector<u8> data = DnsSymbolicName::dumpToData(symbolicName);
        //data.push_back(questionType >> 8);
        //data.push_back(questionType & 0xff);
        //data.push_back(questionClass >> 8);
        //data.push_back(questionClass & 0xff);
        data.insert(data.end(), (u8*)&questionType, (u8*)&questionType+4);
        return data;
    }

    void parseFromData(u8 * data, int size, int pos)
    {
        symbolicName = DnsSymbolicName::recursiveParse(data, size, pos);
        int length = DnsSymbolicName::countNameLength(data, size, pos);
        //questionType  = (*(u16be*)(data+pos+length+0));
        //questionClass = (*(u16be*)(data+pos+length+2));
        std::copy(data+pos+length, data+pos+length+4, (u8*)&questionType);
        fieldsSumSize = length + 4;
    }
};

struct DnsAnswer
{
    std::vector<std::string> symbolicName;
    u16be questionType;
    u16be questionClass;
    u32be ttl;
    u16be resourceLength;
    std::vector<u8> resources;
    std::vector<std::string> resText;
    int fieldsSumSize;
    

    DnsAnswer(u8 * data, int size, int pos)
        : fieldsSumSize(0)
    {
        parseFromData(data, size, pos);
    }

    std::vector<u8> dumpToData()
    {
        std::vector<u8> data = DnsSymbolicName::dumpToData(symbolicName);
        //data.push_back(questionType >> 8);
        //data.push_back((u8)questionType);
        //data.push_back(questionClass >> 8);
        //data.push_back((u8)questionClass);
        //data.push_back(ttl >> 24);
        //data.push_back(ttl >> 16);
        //data.push_back(ttl >> 8);
        //data.push_back((u8)ttl);
        //data.push_back(resourceLength >> 8);
        //data.push_back((u8)resourceLength);
        data.insert(data.end(), (u8*)&questionType, (u8*)&questionType+10);
        switch (questionType)
        {
            case dns_header::NS:
            case dns_header::TXT:
                resources = DnsSymbolicName::dumpToData(resText);
                break;
            default:
                break;
        }
        data.insert(data.end(), resources.begin(), resources.end());
        return data;
    }

    void parseFromData(u8 * data, int size, int pos)
    {
        symbolicName = DnsSymbolicName::recursiveParse(data, size, pos);
        int length = DnsSymbolicName::countNameLength(data, size, pos);
        //questionType   = (*(u16be*)(data+pos+length+0));
        //questionClass  = (*(u16be*)(data+pos+length+2));
        //ttl            = (*(u32be*)(data+pos+length+4));
        //resourceLength = (*(u16be*)(data+pos+length+8));
        std::copy(data+pos+length, data+pos+length+10, (u8*)&questionType);
        resources.assign(data+pos+length+10, data+pos+length+10+resourceLength);
        switch (questionType)
        {
            case dns_header::NS:
            case dns_header::TXT:
                resText = DnsSymbolicName::recursiveParse(data, size, pos+length+10);
                break;
            default:
                break;
        }
        fieldsSumSize = length + 10 + resourceLength;
    }
};

struct DnsMessage
{
    dns_header header;
    std::vector<DnsRequest> requests;
    std::vector<DnsAnswer> answers;
    
    DnsMessage()
    {
    }

    DnsMessage(u8 * data, int size)
    {
        Init(data, size);
    }

    void Init(u8 * data, int size)
    {
        header = *(dns_header*)data;
        int offset = sizeof(dns_header);

        for (int i = 0; i < header.QDCOUNT; i++)
        {
            DnsRequest req(data, size, offset);
            offset += req.fieldsSumSize;
            requests.push_back(req);
        }

        for (int i = 0; i < header.ANCOUNT; i++)
        {
            DnsAnswer ans(data, size, offset);
            offset += ans.fieldsSumSize;
            answers.push_back(ans);
        }

        for (int i = 0; i < header.NSCOUNT; i++)
        {
        }

        for (int i = 0; i < header.ARCOUNT; i++)
        {
        }
    }

    std::vector<u8> dumpToData()
    {
        std::vector<u8> data((u8*)&header, (u8*)&header+sizeof(dns_header));
        for (int i = 0; i < header.QDCOUNT; i++)
        {
            std::vector<u8> vec = requests[i].dumpToData();
            data.insert(data.end(), vec.begin(), vec.end());
        }
        for (int i = 0; i < header.ANCOUNT; i++)
        {
            std::vector<u8> vec = answers[i].dumpToData();
            data.insert(data.end(), vec.begin(), vec.end());
        }
        return data;
    }

    void clear()
    {
        header = dns_header();
        requests.clear();
        answers.clear();
    }
};

#endif // DNS_HEADER_H