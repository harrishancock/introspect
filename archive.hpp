struct archive;

struct extract {
    extract (archive& ar) : m_ar(ar) { }

    template <typename T>
    int operator() (int index, T& t) {
        m_ar >> t;
        return z + 1;
    }

private:
    archive& m_ar;
};

template <typename... Types>
archive& operator>> (archive& ar, std::tuple<Types...>& t) {
    foldl(extract(ar), 0, t);
}


