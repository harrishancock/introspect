#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

struct archive {
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

/**
 * An archive which serializes to a format suitable for data interchange over
 * a network. In particular, the fundamental types are serialized in network-
 * byte order, with insertion operations using the lower /n/ bits of the type,
 * where /n/ is the minimum width of bits guaranteed by the C++ standard. It
 * is therefore a run-time error to pass a value that is unrepresentable by /n/
 * bits to the insertion operators, even if the local implementation 
 */
struct binary_network_archive : archive {
    binary_network_archive (std::stringstream& ss) : m_ss(ss) { }

    /*
     * Fundamental insertion operators.
     */

    archive& operator<< (bool) override;

    archive& operator<< (char) override;
    archive& operator<< (signed char) override;
    archive& operator<< (unsigned char) override;

    archive& operator<< (wchar_t) override;
    archive& operator<< (char16_t) override;
    archive& operator<< (char32_t) override;

    archive& operator<< (short int) override;
    archive& operator<< (unsigned short int) override;

    archive& operator<< (int) override;
    archive& operator<< (unsigned int) override;

    archive& operator<< (long int) override;
    archive& operator<< (unsigned long int) override;

    archive& operator<< (long long int) override;
    archive& operator<< (unsigned long long int) override;

    archive& operator<< (float) override;
    archive& operator<< (double) override;
    archive& operator<< (long double) override;

    /*
     * Fundamental extraction operators.
     */

    virtual archive& operator>> (bool&) override;

    virtual archive& operator>> (char&) override;
    virtual archive& operator>> (signed char&) override;
    virtual archive& operator>> (unsigned char&) override;

    virtual archive& operator>> (wchar_t&) override;
    virtual archive& operator>> (char16_t&) override;
    virtual archive& operator>> (char32_t&) override;

    virtual archive& operator>> (short int&) override;
    virtual archive& operator>> (unsigned short int&) override;

    virtual archive& operator>> (int&) override;
    virtual archive& operator>> (unsigned int&) override;

    virtual archive& operator>> (long int&) override;
    virtual archive& operator>> (unsigned long int&) override;

    virtual archive& operator>> (long long int&) override;
    virtual archive& operator>> (unsigned long long int&) override;

    virtual archive& operator>> (float&) override;
    virtual archive& operator>> (double&) override;
    virtual archive& operator>> (long double&) override;
};



#endif
