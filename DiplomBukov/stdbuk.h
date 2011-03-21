#ifndef STDBUK_H
#define STDBUK_H

template <typename _FwdIt1, typename _FwdIt2>
bool startsWitch(_FwdIt1 _First1, _FwdIt1 _Last1, _FwdIt2 _First2)
{
    _FwdIt1 it1 = _First1;
    _FwdIt1 it2 = _First2;

    while (it1 != _Last1)
    {
        if (*it1 != *it2)
            return false;
        ++it1; ++it2;
    }

    return true;
}

#endif // STDBUK_H