#ifndef DNS_HEADER_H
#define DNS_HEADER_H

#include "i64u64.h"
#include <vector>
#include <string>

#pragma pack(push,1)
struct dns_header
{
    u16be ID;           // Question-Answer ID

    struct flags_struct
    {
        u16 RCODE:4;    // 0 - no errors
                        // 1 - error in request format
                        // 2 - server error
                        // 3 - name does not exist
        u16 Z:3;        // Reserved bits (should be zero)
        u16 RA:1;       // 1 in answer mean, server can work recursive
        u16 RD:1;       // 1 - client can work recursive
        u16 TC:1;       // 1 - was truncated (wait for TCP transmitting)
                        // UDP data MUST be less than 512 bytes
        u16 AA:1;       // 1 - Authorized answer
        u16 Opcode:4;   // 0 - Standard request
                        // 1 - Inverse request
                        // 2 - Request server status
                        // 4 - Notify
        u16 QR:1;       // 0 - Request
                        // 1 - Respounce
    } flags;

    u16be QDCOUNT;      // Number of Questions
    u16be ANCOUNT;      // Number of Answers
    u16be NSCOUNT;      // Number of Authority
    u16be ARCOUNT;      // Number of Additional
};
#pragma pack(pop)

struct DnsSymbolicName
{
    static std::vector<std::string> recursiveParse(u8 * data, int size, int pos)
    {
        std::vector<std::string> vec;
        u8 * ptr = data + pos;
        while (*ptr != 0)
        {
            if ((*ptr & 0xC0) == 0)
            {
                vec.push_back(std::string((char*)ptr+1, *ptr));
                ptr += *ptr + 1;
            }
            else
            {
                int offset = (*(u16be*)ptr) & 0x3F;
                std::vector<std::string> v =
                    recursiveParse(data, size, offset);
                //for (int i = 0; i < v.size(); i++)
                //    vec.push_back(v[i]);
                vec.insert(vec.end(), v.begin(), v.end());
                ptr += 2;
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
                ptr += 2;
            else
                ptr += *ptr + 1;
        }
        ptr++;
        return ptr - (data + pos);
    }

    static std::vector<u8> dumpToData(std::vector<std::string> names)
    {
        std::vector<u8> data;
        for (unsigned i = 0; i < names.size(); i++)
        {
            data.push_back(names[i].size());
            for (unsigned j = 0; j < names[i].size(); j++)
                data.push_back(names[i][j]);
        }
        return data;
    }

    std::string readableName(std::vector<std::string> names)
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
        data.push_back(questionType >> 8);
        data.push_back(questionType & 0xff);
        data.push_back(questionClass >> 8);
        data.push_back(questionClass & 0xff);
        return data;
    }

    void parseFromData(u8 * data, int size, int pos)
    {
        symbolicName = DnsSymbolicName::recursiveParse(data, size, pos);
        int length = DnsSymbolicName::countNameLength(data, size, pos);
        questionType  = (*(u16be*)(data+pos+length+0));
        questionClass = (*(u16be*)(data+pos+length+2));
        fieldsSumSize = length + 4;
    }
};

struct DnsAnswer
{
    enum AnswerType
    {
        A	  = 1,   // IP-адрес
        NS	  = 2,   // Сервер имен
        CNAME = 5,   // Каноническое имя
        SOA	  = 6,	 // Начало списка серверов. Большое число полей, определяющих часть иерархии имен, которую использует сервер
        MB	  = 7,	 // Имя домена почтового ящика
        WKS	  = 11,	 // well-known service — стандартная услуга
        PTR	  = 12,	 // Запись указателя
        HINFO = 13,	 // Информация об ЭВМ
        MINFO = 14,	 // Информация о почтовом ящике или списке поч¬товых адресов
        MX	  = 15,	 // Запись о почтовом сервере. Включает в себя при¬оритет обработчика почты
        TXT	  = 16,	 // Не интерпретируемая строка ASCII-символов
        //ISDN       // Связывает имя ЭВМ с адресом ISDN
        AXFR  = 252, // Запрос зонного обмена
        ANY	  = 255, // Запрос всех записей
    };

    std::vector<std::string> symbolicName;
    u16be questionType;
    u16be questionClass;
    u32be ttl;
    u16be resourceLength;
    std::vector<u8> resources;
    int fieldsSumSize;

    DnsAnswer(u8 * data, int size, int pos)
        : fieldsSumSize(0)
    {
        parseFromData(data, size, pos);
    }

    std::vector<u8> dumpToData()
    {
        std::vector<u8> data = DnsSymbolicName::dumpToData(symbolicName);
        data.push_back(questionType >> 8);
        data.push_back(questionType & 0xff);
        data.push_back(questionClass >> 8);
        data.push_back(questionClass & 0xff);
        return data;
    }

    void parseFromData(u8 * data, int size, int pos)
    {
        symbolicName = DnsSymbolicName::recursiveParse(data, size, pos);
        int length = DnsSymbolicName::countNameLength(data, size, pos);
        questionType  = (*(u16be*)(data+pos+length+0));
        questionClass = (*(u16be*)(data+pos+length+2));
        fieldsSumSize = length + 4;
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
            requests.push_back(req);
            offset += req.fieldsSumSize;
            requests.push_back(req);
        }

        for (int i = 0; i < header.ANCOUNT; i++)
        {
        }

        for (int i = 0; i < header.NSCOUNT; i++)
        {
        }

        for (int i = 0; i < header.ARCOUNT; i++)
        {
        }
    }


};

#endif // DNS_HEADER_H