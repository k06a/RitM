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
};
#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////

struct DnsName
{
    typedef std::vector<u8> Blob;
    typedef std::vector<std::string> SymbolicName;
    typedef std::pair<SymbolicName,int> SymbolicNameSizePair;
    typedef std::pair<int,SymbolicName> OffsetNamePair;
    typedef std::vector<OffsetNamePair> OffsetNamePairList;

    static std::string readableName(
        const SymbolicName & names)
    {
        if (names.size() == 0)
            return "";
        std::string str = names[0];
        for (unsigned i = 1; i < names.size(); i++)
            str += "." + names[i];
        return str;
    }

    static SymbolicNameSizePair parse(u8 * data, int size, int pos)
    {
        SymbolicName vec;
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
                int offset = (*(u16be*)ptr) & 0x3FFF;
                SymbolicNameSizePair vp = parse(data, size, offset);
                vec.insert(vec.end(), vp.first.begin(), vp.first.end());
                ptr += 2;
                return make_pair(vec, ptr - (data + pos));
            }
        }
        if (ptr-data < size)
            ptr++;
        return make_pair(vec, ptr - (data + pos));
    }

    static Blob dump(
        OffsetNamePairList & prevNames,
        const SymbolicName & name, int offset)
    {
        Blob data;

        // Поиск наибольшего совпадения
        unsigned longestIndex = (unsigned)-1;
        unsigned longestCount = 0;
        unsigned longestData = 0;
        for (unsigned i = 0; i < prevNames.size(); i++)
        {
            if (prevNames[i].second.size() > name.size())
                continue;

            unsigned currentData = 0;
            unsigned n1 = name.size();
            unsigned n2 = prevNames[i].second.size();
            bool good = true;
            unsigned j;
            for (j = 0; j < n2; j++)
            {
                if (name[n1-1-j] != prevNames[i].second[n2-1-j])
                {
                    good = false;
                    break;
                }
                currentData += name[n1-1-j].size();
            }
            if (good && (currentData > longestData))
            {
                longestIndex = i;
                longestCount = j;
                longestData = currentData;
            }
        }

        // Если нет совпадения для рекурсии
        if (longestIndex == -1)
        {
            prevNames.push_back(make_pair(offset, name));
            return privateDump(name, name.size(), true);
        }
        
        // Если часть имени можно оформить рекурсией
        int partsToCopy = name.size() - longestCount;
        if (partsToCopy)
        {
            Blob vec = privateDump(name, partsToCopy, false);
            data.insert(data.end(), vec.begin(), vec.end());
        }
        u16 value = 0xC000 + prevNames[longestIndex].first;
        data.push_back(value >> 8);
        data.push_back((u8)value);

        if (partsToCopy) // Не ссылаться на 0xC00C
            prevNames.push_back(make_pair(offset, name));
        return data;
    }

private:
    static Blob privateDump(const SymbolicName & name, unsigned sections, bool zero)
    {
        Blob data;
        for (unsigned i = 0; (i < name.size()) && (i < sections); i++)
        {
            data.push_back(name[i].size());
            data.insert(data.end(), name[i].begin(), name[i].end());
        }
        if (zero)
            data.push_back(0);
        return data;
    }
};

//////////////////////////////////////////////////////////////////////////

struct DnsRequest
{
    enum QuestionType
    {
        A	  = 1,      // IPv4-адрес
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
        AAAA  = 28,     // IPv6-адрес
        //ISDN          // Связывает имя ЭВМ с адресом ISDN
        AXFR  = 252,    // Запрос зонного обмена
        ANY	  = 255,    // Запрос всех записей
    };

    typedef std::vector<u8> Blob;
    
    DnsName::SymbolicNameSizePair nameSize;
    u16be questionType;
    u16be questionClass;

    DnsRequest()
    {
    }

    DnsRequest(u8 * data, int size, int pos)
    {
        parse(data, size, pos);
    }

    int parse(u8 * data, int size, int pos)
    {
        nameSize = DnsName::parse(data, size, pos);
        int length = nameSize.second;
        std::copy(data+pos+length, data+pos+length+4, (u8*)&questionType);
        return length + 4;
    }

    Blob dump(DnsName::OffsetNamePairList & prevNames, int offset)
    {
        Blob data = DnsName::dump(prevNames, nameSize.first, offset);
        data.insert(data.end(), (u8*)&questionType, (u8*)&questionType+4);
        return data;
    }
};

//////////////////////////////////////////////////////////////////////////

struct DnsAnswer : public DnsRequest
{
    typedef std::vector<u8> Blob;

    u32be ttl;
    u16be resourceLength;
    Blob resources;

    DnsName::SymbolicNameSizePair resText;
    
    DnsAnswer()
    {
    }

    DnsAnswer(u8 * data, int size, int pos)
        : DnsRequest(data, size, pos)
    {
        parse(data, size, pos + DnsRequest::nameSize.second + 4);
    }

    int parse(u8 * data, int size, int pos)
    {
        int offset = DnsRequest::parse(data, size, pos);
        pos += offset;

        std::copy(data+pos, data+pos+6, (u8*)&ttl);
        resources.assign(data+pos+6, data+pos+6+resourceLength);
        switch (questionType)
        {
            case DnsRequest::NS:
            case DnsRequest::TXT:
                resText = DnsName::parse(data, size, pos+6);
                break;
            case DnsRequest::MX:
                resText = DnsName::parse(data, size, pos+8);
                break;
            default:
                break;
        }
        return offset + 6 + resourceLength;
    }

    Blob dump(DnsName::OffsetNamePairList & prevNames, int offset)
    {

        Blob data = DnsRequest::dump(prevNames, offset);
        data.insert(data.end(), (u8*)&ttl, (u8*)&ttl+6);
        offset += data.size();

        switch (questionType)
        {
            case DnsRequest::NS:
            case DnsRequest::TXT:
                resources = DnsName::dump(prevNames, resText.first, offset);
                break;

            case DnsRequest::MX:
            {
                resources.resize(2); // Keep field preference
                Blob vec = DnsName::dump(prevNames, resText.first, offset+2);
                resources.insert(resources.end(), vec.begin(), vec.end());
                break;
            }
            default:
                break;
        }

        data.insert(data.end(), resources.begin(), resources.end());
        return data;
    }
};

//////////////////////////////////////////////////////////////////////////

struct DnsMessage
{
    typedef std::vector<u8> Blob;

    dns_header header;
    std::vector<DnsRequest> requests;
    std::vector<DnsAnswer> answers;
    std::vector<DnsAnswer> additions;
    
    DnsMessage()
    {
    }

    DnsMessage(u8 * data, int size)
    {
        parse(data, size);
    }

    void parse(u8 * data, int size)
    {
        clear();

        header = *(dns_header*)data;
        int offset = sizeof(dns_header);

        for (unsigned i = 0; i < header.QDCOUNT; i++)
        {
            DnsRequest req;
            offset += req.parse(data, size, offset);
            requests.push_back(req);
        }

        for (unsigned i = 0; i < header.ANCOUNT; i++)
        {
            DnsAnswer ans;
            offset += ans.parse(data, size, offset);
            answers.push_back(ans);
        }

        for (unsigned i = 0; i < header.NSCOUNT; i++)
        {
        }

        for (unsigned i = 0; i < header.ARCOUNT; i++)
        {
            DnsAnswer add;
            offset += add.parse(data, size, offset);
            additions.push_back(add);
        }
    }

    Blob dump()
    {
        header.QDCOUNT = requests.size();
        header.ANCOUNT = answers.size();
        header.NSCOUNT = 0;
        header.ARCOUNT = additions.size();
        
        int offset = 0;
        DnsName::OffsetNamePairList prevNames;

        Blob data((u8*)&header, (u8*)&header+sizeof(dns_header));
        offset = data.size();

        for (unsigned i = 0; i < requests.size(); i++)
        {
            Blob vec = requests[i].dump(prevNames, offset);
            data.insert(data.end(), vec.begin(), vec.end());
            offset = data.size();
        }

        for (unsigned i = 0; i < answers.size(); i++)
        {
            Blob vec = answers[i].dump(prevNames, offset);
            data.insert(data.end(), vec.begin(), vec.end());
            offset = data.size();
        }

        //for (unsigned i = 0; i < answers.size(); i++)
        //{
        //}

        for (unsigned i = 0; i < additions.size(); i++)
        {
            Blob vec = additions[i].dump(prevNames, offset);
            data.insert(data.end(), vec.begin(), vec.end());
            offset = data.size();
        }

        // Отрезаем нуль-символ завершитель строки с конца
        if (data.back() == 0)
        {
            std::vector<DnsAnswer> * vecptr = NULL;
            if ((vecptr == NULL) && (additions.size() != 0))
                vecptr = &additions;
            if ((vecptr == NULL) && (answers.size() != 0))
                vecptr = &answers;
            if (vecptr == NULL)
                return data;

            if ((vecptr->back().questionType == DnsRequest::NS) ||
                (vecptr->back().questionType == DnsRequest::TXT) ||
                (vecptr->back().questionType == DnsRequest::MX))
            {
                data.pop_back();    
            }
        }

        return data;
    }

    void clear()
    {
        header = dns_header();
        requests.clear();
        answers.clear();
        additions.clear();
    }
};

#endif // DNS_HEADER_H