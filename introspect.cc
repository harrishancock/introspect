#include "introspect.hh"
#include <string>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////

struct print_category_floating_point { };
struct print_category_non_boolean_integral { };
struct print_category_boolean { };
struct print_category_string { };
struct print_category_fusion_sequence { };

//////////////////////////////////////////////////////////////////////////////

template <typename T, typename Enable = void>
struct print_traits { };

template <typename T>
struct print_traits<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
    typedef print_category_floating_point print_tag;
};

template <typename T>
struct print_traits<T, typename std::enable_if<std::is_integral<T>::value
                                            && !std::is_same<T, bool>::value>::type> {
    typedef print_category_non_boolean_integral print_tag;
};

template <>
struct print_traits<bool> {
    typedef print_category_boolean print_tag;
};

template <>
struct print_traits<std::string> {
    typedef print_category_string print_tag;
};

template <typename T>
struct print_traits<T, typename std::enable_if<boost::fusion::traits::is_sequence<T>::value>::type> {
    typedef print_category_fusion_sequence print_tag;
};

//////////////////////////////////////////////////////////////////////////////

struct call_print {
    call_print (int indent=0, int shiftwidth=4)
        : indent_(indent), shiftwidth_(shiftwidth) { }

    void indent () const {
        std::cout << std::string(indent_, ' ').c_str();
    }

    template <typename T>
    typename std::enable_if<boost::fusion::traits::is_sequence<T>::value, void>::type
    operator() (size_t index, const char* name, T& t) const {
        indent();
        std::cout << '(' << index << ", " << name << ") : {\n";
        introspect::for_each(t, call_print(indent_+shiftwidth_));
        std::cout << "}\n";
    }

    template <typename T>
    typename std::enable_if<!boost::fusion::traits::is_sequence<T>::value, void>::type
    operator() (size_t index, const char* name, T& t) const {
        indent();
        std::cout << '(' << index << ", " << name << ") : " << t << '\n';
    }

private:
    int indent_;
    int shiftwidth_;
};

//////////////////////////////////////////////////////////////////////////////

template <typename T>
void print (T& t, print_category_floating_point) {
    printf("%f\n", t);
}

template <typename T>
void print (T& t, print_category_non_boolean_integral) {
    printf("%hd\n", t);
}

template <typename T>
void print (T& t, print_category_boolean) {
    printf(t ? "true\n" : "false\n");
}

template <typename T>
void print (T& t, print_category_string) {
    printf("%s\n", t.c_str());
}

template <typename T>
void print (T& t, print_category_fusion_sequence) {
    introspect::for_each(t, call_print());
}

//////////////////////////////////////////////////////////////////////////////

template <typename T>
void print (T& t) {
    typedef typename print_traits<T>::print_tag print_tag;
    print(t, print_tag());
}

//////////////////////////////////////////////////////////////////////////////

struct balls {
    int count;
};

struct penis {
    double girth;   // trust me, I need a double
    short length;
    std::string name;
    balls b;
};

INTROSPECT(balls, (count));
INTROSPECT(penis, (girth)(length)(name)(b));

int main () {
    penis p = {
        800.21785623435,
        3,
        "Raskolnikov",
        { 1 }
    };

    introspect::for_each(p, call_print());

    return 0;
}
