#include "my_vector.h"
#include "my_list.h"
#include "my_forward_list.h"

#include <iostream>
#include <string>

template<typename Container>
void print(const Container& c, const std::string& label) {
    std::cout << label;
    bool first = true;
    for (auto it = c.begin(); it != c.end(); ++it) {
        if (!first) std::cout << ", ";
        std::cout << *it;
        first = false;
    }
    std::cout << std::endl;
}

// MyVector
void demo_vector() {
    std::cout << "\n=== MyVector (sequential) ===\n";

    MyVector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);

    print(v, "3.  Contents: ");

    std::cout << "4.  Size: " << v.size() << std::endl;

    // Удаляем 3-й, 5-й, 7-й (индексы 2, 4, 6) — справа налево, чтобы индексы не сдвигались
    v.erase(6);
    v.erase(4);
    v.erase(2);

    print(v, "6.  Contents: ");

    v.insert(0, 10);
    print(v, "8.  Contents: ");

    v.insert(v.size() / 2, 20);
    print(v, "10. Contents: ");

    v.push_back(30);
    print(v, "12. Contents: ");
}

// MyList 
void demo_list() {
    std::cout << "\n=== MyList (doubly linked) ===\n";

    MyList<int> l;
    for (int i = 0; i < 10; ++i) l.push_back(i);

    print(l, "3.  Contents: ");

    std::cout << "4.  Size: " << l.size() << std::endl;

    l.erase(6);
    l.erase(4);
    l.erase(2);

    print(l, "6.  Contents: ");

    l.insert(0, 10);
    print(l, "8.  Contents: ");

    l.insert(l.size() / 2, 20);
    print(l, "10. Contents: ");

    l.push_back(30);
    print(l, "12. Contents: ");
}

//MyForwardList 
void demo_forward_list() {
    std::cout << "\n=== MyForwardList (singly linked) ===\n";

    MyForwardList<int> fl;
    for (int i = 0; i < 10; ++i) fl.push_back(i);

    print(fl, "3.  Contents: ");

    std::cout << "4.  Size: " << fl.size() << std::endl;

    fl.erase(6);
    fl.erase(4);
    fl.erase(2);

    print(fl, "6.  Contents: ");

    fl.insert(0, 10);
    print(fl, "8.  Contents: ");

    fl.insert(fl.size() / 2, 20);
    print(fl, "10. Contents: ");

    fl.push_back(30);
    print(fl, "12. Contents: ");
}

int main(int argc, char** argv) {
    std::string which = (argc > 1) ? argv[1] : "all";

    if (which == "vector" || which == "all") demo_vector();
    if (which == "list"   || which == "all") demo_list();
    if (which == "forward" || which == "all") demo_forward_list();

    return 0;
}