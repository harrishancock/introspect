#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

template <typename T, unsigned int W>
struct width_policy {
    using type = T;
    constexpr static unsigned int value = W;
};

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
 */
template <typename... Policies>
struct typesetter : archive, Policies... {
    typesetter (archive&& ar) : m_ar(ar) { }

    template <typename From, typename To>
    struct policy {
    protected:
        static To cast (From f) {
            printf("static_cast<%s>(%s)\n", typeid(To).name(), typeid(From).name());
            return static_cast<To>(f);
        }
    };

    /*
     * Fundamental insertion operators.
     */

    archive& operator<< (bool x) override { m_ar << cast(x); }
    archive& operator<< (char x) override { m_ar << cast(x); }
    archive& operator<< (signed char) override { m_ar << cast(x); }
    archive& operator<< (unsigned char) override { m_ar << cast(x); }

    archive& operator<< (wchar_t) override { m_ar << cast(x); }
    archive& operator<< (char16_t) override { m_ar << cast(x); }
    archive& operator<< (char32_t) override { m_ar << cast(x); }

    archive& operator<< (short int) override { m_ar << cast(x); }
    archive& operator<< (unsigned short int) override { m_ar << cast(x); }

    archive& operator<< (int) override { m_ar << cast(x); }
    archive& operator<< (unsigned int) override { m_ar << cast(x); }

    archive& operator<< (long int) override { m_ar << cast(x); }
    archive& operator<< (unsigned long int) override { m_ar << cast(x); }

    archive& operator<< (long long int) override { m_ar << cast(x); }
    archive& operator<< (unsigned long long int) override { m_ar << cast(x); }

    archive& operator<< (float) override { m_ar << cast(x); }
    archive& operator<< (double) override { m_ar << cast(x); }
    archive& operator<< (long double) override { m_ar << cast(x); }

    /*
     * Fundamental extraction operators.
     */

    archive& operator>> (bool&) override;

    archive& operator>> (char&) override;
    archive& operator>> (signed char&) override;
    archive& operator>> (unsigned char&) override;

    archive& operator>> (wchar_t&) override;
    archive& operator>> (char16_t&) override;
    archive& operator>> (char32_t&) override;

    archive& operator>> (short int&) override;
    archive& operator>> (unsigned short int&) override;

    archive& operator>> (int&) override;
    archive& operator>> (unsigned int&) override;

    archive& operator>> (long int&) override;
    archive& operator>> (unsigned long int&) override;

    archive& operator>> (long long int&) override;
    archive& operator>> (unsigned long long int&) override;

    archive& operator>> (float&) override;
    archive& operator>> (double&) override;
    archive& operator>> (long double&) override;

private:
    template <typename T>
    static T cast (T t) {
        printf("%s\n", typeid(T).name());
        return t;
    }

};



#endif
