#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

class archive {
public:
    virtual ~archive () { }

    /*
     * Fundamental insertion operators.
     */

    virtual archive& operator<< (bool) = 0;

    virtual archive& operator<< (char) = 0;
    virtual archive& operator<< (signed char) = 0;
    virtual archive& operator<< (unsigned char) = 0;

    virtual archive& operator<< (wchar_t) = 0;
    virtual archive& operator<< (char16_t) = 0;
    virtual archive& operator<< (char32_t) = 0;

    virtual archive& operator<< (short int) = 0;
    virtual archive& operator<< (unsigned short int) = 0;

    virtual archive& operator<< (int) = 0;
    virtual archive& operator<< (unsigned int) = 0;

    virtual archive& operator<< (long int) = 0;
    virtual archive& operator<< (unsigned long int) = 0;

    virtual archive& operator<< (long long int) = 0;
    virtual archive& operator<< (unsigned long long int) = 0;

    virtual archive& operator<< (float) = 0;
    virtual archive& operator<< (double) = 0;
    virtual archive& operator<< (long double) = 0;

    /*
     * Fundamental extraction operators.
     */

    virtual archive& operator>> (bool&) = 0;

    virtual archive& operator>> (char&) = 0;
    virtual archive& operator>> (signed char&) = 0;
    virtual archive& operator>> (unsigned char&) = 0;

    virtual archive& operator>> (wchar_t&) = 0;
    virtual archive& operator>> (char16_t&) = 0;
    virtual archive& operator>> (char32_t&) = 0;

    virtual archive& operator>> (short int&) = 0;
    virtual archive& operator>> (unsigned short int&) = 0;

    virtual archive& operator>> (int&) = 0;
    virtual archive& operator>> (unsigned int&) = 0;

    virtual archive& operator>> (long int&) = 0;
    virtual archive& operator>> (unsigned long int&) = 0;

    virtual archive& operator>> (long long int&) = 0;
    virtual archive& operator>> (unsigned long long int&) = 0;

    virtual archive& operator>> (float&) = 0;
    virtual archive& operator>> (double&) = 0;
    virtual archive& operator>> (long double&) = 0;
};

template <typename CastMap>
class cast_adapter : public archive {
public:
    cast_adapter (std::shared_ptr<archive> ar) : m_ar(ar) { }

    /*
     * Fundamental insertion operators.
     */

    // TODO check for overflow
#define INSERTION_OPERATOR(TYPE) \
    archive& operator<< (TYPE x) override { \
        using type = lookup<CastMap, TYPE, TYPE>; \
        *m_ar << static_cast<type>(x); \
        return *this; \
    }

    INSERTION_OPERATOR(bool)
    INSERTION_OPERATOR(char)
    INSERTION_OPERATOR(signed char)
    INSERTION_OPERATOR(unsigned char)

    INSERTION_OPERATOR(wchar_t)
    INSERTION_OPERATOR(char16_t)
    INSERTION_OPERATOR(char32_t)

    INSERTION_OPERATOR(short int)
    INSERTION_OPERATOR(unsigned short int)

    INSERTION_OPERATOR(int)
    INSERTION_OPERATOR(unsigned int)

    INSERTION_OPERATOR(long int)
    INSERTION_OPERATOR(unsigned long int)

    INSERTION_OPERATOR(long long int)
    INSERTION_OPERATOR(unsigned long long int)

    INSERTION_OPERATOR(float)
    INSERTION_OPERATOR(double)
    INSERTION_OPERATOR(long double)

#undef INSERTION_OPERATOR

    /*
     * Fundamental extraction operators.
     */

#define EXTRACTION_OPERATOR(TYPE) \
    archive& operator>> (TYPE&) override { \
        using type = lookup<CastMap, TYPE, TYPE>; \
        type a; \
        *m_ar >> a; \
        x = static_cast<type>(a); \
        return *this; \
    }

    EXTRACTION_OPERATOR(bool)

    EXTRACTION_OPERATOR(char)
    EXTRACTION_OPERATOR(signed char)
    EXTRACTION_OPERATOR(unsigned char)

    EXTRACTION_OPERATOR(wchar_t)
    EXTRACTION_OPERATOR(char16_t)
    EXTRACTION_OPERATOR(char32_t)

    EXTRACTION_OPERATOR(short int)
    EXTRACTION_OPERATOR(unsigned short int)

    EXTRACTION_OPERATOR(int)
    EXTRACTION_OPERATOR(unsigned int)

    EXTRACTION_OPERATOR(long int)
    EXTRACTION_OPERATOR(unsigned long int)

    EXTRACTION_OPERATOR(long long int)
    EXTRACTION_OPERATOR(unsigned long long int)

    EXTRACTION_OPERATOR(float)
    EXTRACTION_OPERATOR(double)
    EXTRACTION_OPERATOR(long double)

#undef EXTRACTION_OPERATOR

private:
    std::shared_ptr<archive> m_ar;
};

class string_archive : public archive {
public:
    string_archive (const std::string& s) : m_ss(s) { }

#define INSERTION_OPERATOR(TYPE) \
    archive& operator<< (TYPE x) override { \
        for (
    }

private:
    std::stringstream m_ss;
};

class any_archive {
public:
    any_archive (std::string&& ss) : m_ar(new string_archive (std::forward<std::string>(ss))) { }

private:
    std::shared_ptr<archive> m_ar;
};

#endif
