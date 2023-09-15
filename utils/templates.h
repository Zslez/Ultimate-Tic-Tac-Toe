/**
 * @file templates.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @copyright Copyright (c) 2023
 */



// SOURCE:
// https://stackoverflow.com/questions/30540101/iterator-for-a-subset-of-a-vector


template <class Iter>
class range {
    Iter b;
    Iter e;

    public:
        range(Iter b1, Iter e1) : b(b1), e(e1) {}

        Iter begin() { return b; }
        Iter end() { return e; }
};



template <class Container>
range<typename Container::iterator> 
makeRange(Container &c, int b, int e) {
    return range<typename Container::iterator> (c.begin() + b, c.begin() + e);
};



template <typename T> int GetIndex(std::vector<std::vector<T>> arr, std::vector<T> elem) {
    int n = arr.size();
    int i = 0;

    while (i < n) {
        if (arr[i] == elem) break;
        i++;
    }

    if (i < n) return i;

    return -1;
};
