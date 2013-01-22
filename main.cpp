int foo (int i, double d);

int main () {
    any_function foowrap { foo };

    auto ret = static_cast<int>(foowrap(iarchive("23 3.14")));
    auto rt2 = foowrap(iarchive("")).get<int>();
}

boost::any any_function::operator() (iarchive&& ia) {
    return (*m_func)(std::forward<iarchive>(ia));
}

template <>
boost::any uncurried<int(int,double)>::operator() (iarchive&& ia) {
    std::tuple<int,double> t;
    ia >> t;
    return m_func(t);
}
