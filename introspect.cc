#include "introspect.hh"
#include <string>
#include <iostream>

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
        printf("%c ", index ? ',' : '{');
        std::cout << name << " : ";
        introspect::for_each(t, call_print(indent_+shiftwidth_));
        std::cout << " }\n";
    }

    template <typename T>
    typename std::enable_if<!boost::fusion::traits::is_sequence<T>::value, void>::type
    operator() (size_t index, const char* name, T& t) const {
        indent();
        printf("%c ", index ? ',' : '{');
        std::cout << name << " : " << t << '\n';
    }

private:
    int indent_;
    int shiftwidth_;
};

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
    printf("}\n");

    return 0;
}
