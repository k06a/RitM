#ifndef TRIPLET_H
#define TRIPLET_H

template <typename T1, typename T2, typename T3>
struct triplet
{
    typedef std::pair<T1,std::pair<T2,T3> > Type;
};

template <typename T1, typename T2, typename T3>
typename triplet<T1,T2,T3>::Type make_triplet(const T1 & t1, const T2 & t2, const T3 & t3)
{
    return std::make_pair(t1, std::make_pair(t2,t3));
}

#endif // TRIPLET_H