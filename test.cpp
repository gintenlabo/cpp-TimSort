#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TimSortTest
#include <boost/test/unit_test.hpp>

#define HAS_MOVE 1
#include "timsort.hpp"

BOOST_AUTO_TEST_CASE( simple10 ) {
    std::vector<int> a;
    a.push_back(60);
    a.push_back(50);
    a.push_back( 1);
    a.push_back(40);
    a.push_back(80);
    a.push_back(20);
    a.push_back(30);
    a.push_back(70);
    a.push_back(10);
    a.push_back(90);

    timsort(a.begin(), a.end(), std::less<int>());

    BOOST_CHECK_EQUAL( a[0],  1 );
    BOOST_CHECK_EQUAL( a[1], 10 );
    BOOST_CHECK_EQUAL( a[2], 20 );
    BOOST_CHECK_EQUAL( a[3], 30 );
    BOOST_CHECK_EQUAL( a[4], 40 );
    BOOST_CHECK_EQUAL( a[5], 50 );
    BOOST_CHECK_EQUAL( a[6], 60 );
    BOOST_CHECK_EQUAL( a[7], 70 );
    BOOST_CHECK_EQUAL( a[8], 80 );
    BOOST_CHECK_EQUAL( a[9], 90 );

    std::reverse(a.begin(), a.end());

    timsort(a.begin(), a.end(), std::less<int>());

    BOOST_CHECK_EQUAL( a[0],  1 );
    BOOST_CHECK_EQUAL( a[1], 10 );
    BOOST_CHECK_EQUAL( a[2], 20 );
    BOOST_CHECK_EQUAL( a[3], 30 );
    BOOST_CHECK_EQUAL( a[4], 40 );
    BOOST_CHECK_EQUAL( a[5], 50 );
    BOOST_CHECK_EQUAL( a[6], 60 );
    BOOST_CHECK_EQUAL( a[7], 70 );
    BOOST_CHECK_EQUAL( a[8], 80 );
    BOOST_CHECK_EQUAL( a[9], 90 );
}


BOOST_AUTO_TEST_CASE( shuffle30 ) {
    const int size = 30;

    std::vector<int> a;
    for(int i = 0; i < size; ++i) {
        a.push_back((i+1) * 10);
    }
    std::random_shuffle(a.begin(), a.end());

    timsort(a.begin(), a.end(), std::less<int>());

    for(int i = 0; i < size; ++i) {
        BOOST_CHECK_EQUAL( a[i], (i+1) * 10 );
    }
}


BOOST_AUTO_TEST_CASE( shuffle31 ) {
    const int size = 31;

    std::vector<int> a;
    for(int i = 0; i < size; ++i) {
        a.push_back((i+1) * 10);
    }
    std::random_shuffle(a.begin(), a.end());

    timsort(a.begin(), a.end(), std::less<int>());

    for(int i = 0; i < size; ++i) {
        BOOST_CHECK_EQUAL( a[i], (i+1) * 10 );
    }
}

BOOST_AUTO_TEST_CASE( shuffle32 ) {
    const int size = 32;

    std::vector<int> a;
    for(int i = 0; i < size; ++i) {
        a.push_back((i+1) * 10);
    }
    std::random_shuffle(a.begin(), a.end());

    timsort(a.begin(), a.end(), std::less<int>());

    for(int i = 0; i < size; ++i) {
        BOOST_CHECK_EQUAL( a[i], (i+1) * 10 );
    }
}

BOOST_AUTO_TEST_CASE( shuffle128 ) {
    const int size = 128;

    std::vector<int> a;
    for(int i = 0; i < size; ++i) {
        a.push_back((i+1) * 10);
    }
    std::random_shuffle(a.begin(), a.end());

    timsort(a.begin(), a.end(), std::less<int>());

    for(int i = 0; i < size; ++i) {
        BOOST_CHECK_EQUAL( a[i], (i+1) * 10 );
    }
}

BOOST_AUTO_TEST_CASE( shuffle1024 ) {
    const int size = 1024;

    std::vector<int> a;
    for(int i = 0; i < size; ++i) {
        a.push_back((i+1) * 10);
    }

    for(int n = 0; n < 100; ++n) {
        std::random_shuffle(a.begin(), a.end());

        timsort(a.begin(), a.end(), std::less<int>());

        for(int i = 0; i < size; ++i) {
            BOOST_CHECK_EQUAL( a[i], (i+1) * 10 );
        }
    }
}

BOOST_AUTO_TEST_CASE( c_array ) {
    int a[] = { 7, 1, 5, 3, 9 };

    timsort(a, a + sizeof(a) / sizeof(int), std::less<int>());

    BOOST_CHECK_EQUAL(a[0], 1);
    BOOST_CHECK_EQUAL(a[1], 3);
    BOOST_CHECK_EQUAL(a[2], 5);
    BOOST_CHECK_EQUAL(a[3], 7);
    BOOST_CHECK_EQUAL(a[4], 9);
}

struct NonDefaultConstructible
{
    int i;
    
    NonDefaultConstructible( int i_ )
        : i( i_ ) {}
    
    friend bool operator<( NonDefaultConstructible const& x, NonDefaultConstructible const& y ) {
        return x.i < y.i;
    }
    
};

BOOST_AUTO_TEST_CASE( non_default_constructible ) {
    NonDefaultConstructible a[] = { 7, 1, 5, 3, 9 };

    timsort(a, a + sizeof(a) / sizeof(a[0]), std::less<NonDefaultConstructible>());

    BOOST_CHECK_EQUAL(a[0].i, 1);
    BOOST_CHECK_EQUAL(a[1].i, 3);
    BOOST_CHECK_EQUAL(a[2].i, 5);
    BOOST_CHECK_EQUAL(a[3].i, 7);
    BOOST_CHECK_EQUAL(a[4].i, 9);
}

enum id { foo, bar, baz };
typedef std::pair<int, id> pair_t;
bool less_in_first(pair_t x, pair_t y) {
    return x.first < y.first;
}

BOOST_AUTO_TEST_CASE( stability ) {
    std::vector< pair_t > a;

    for(int i = 100; i >= 0; --i) {
        a.push_back( std::make_pair(i, foo) );
        a.push_back( std::make_pair(i, bar) );
        a.push_back( std::make_pair(i, baz) );
    }

    timsort(a.begin(), a.end(), &less_in_first);

    BOOST_CHECK_EQUAL(a[0].first,  0);
    BOOST_CHECK_EQUAL(a[0].second, foo);
    BOOST_CHECK_EQUAL(a[1].first,  0);
    BOOST_CHECK_EQUAL(a[1].second, bar);
    BOOST_CHECK_EQUAL(a[2].first,  0);
    BOOST_CHECK_EQUAL(a[2].second, baz);

    BOOST_CHECK_EQUAL(a[3].first,  1);
    BOOST_CHECK_EQUAL(a[3].second, foo);
    BOOST_CHECK_EQUAL(a[4].first,  1);
    BOOST_CHECK_EQUAL(a[4].second, bar);
    BOOST_CHECK_EQUAL(a[5].first,  1);
    BOOST_CHECK_EQUAL(a[5].second, baz);

    BOOST_CHECK_EQUAL(a[6].first,  2);
    BOOST_CHECK_EQUAL(a[6].second, foo);
    BOOST_CHECK_EQUAL(a[7].first,  2);
    BOOST_CHECK_EQUAL(a[7].second, bar);
    BOOST_CHECK_EQUAL(a[8].first,  2);
    BOOST_CHECK_EQUAL(a[8].second, baz);

    BOOST_CHECK_EQUAL(a[9].first,   3);
    BOOST_CHECK_EQUAL(a[9].second, foo);
    BOOST_CHECK_EQUAL(a[10].first,  3);
    BOOST_CHECK_EQUAL(a[10].second, bar);
    BOOST_CHECK_EQUAL(a[11].first,  3);
    BOOST_CHECK_EQUAL(a[11].second, baz);
}

#if HAS_MOVE || __cplusplus > 199711L // C++11

#include <memory> // for std::unique_ptr ( move-only class )
#include <cstdlib> // for std::rand

struct MoveOnly
{
    std::unique_ptr<int> p;
    
    MoveOnly( int i )
        : p( new int(i) ) {}
    
    
    int operator*() const {
        BOOST_REQUIRE( p != 0 );
        return *p;
    }
    
    friend bool operator<( MoveOnly const& x, MoveOnly const& y ) {
        BOOST_REQUIRE( x.p != 0 );
        BOOST_REQUIRE( y.p != 0 );
        return *x < *y;
    }
    
};

BOOST_AUTO_TEST_CASE( move_only ) {
    const int size = 1024;

    for(int n = 0; n < 100; ++n) {
        std::vector<MoveOnly> a, b;
        a.reserve( size ); b.reserve( size );
        for(int i = 0; i < size; ++i) {
            int const x = std::rand() % size;
            a.push_back( x );
            b.push_back( x );
        }
        
        timsort(a.begin(), a.end(), std::less<MoveOnly>());
        std::stable_sort(b.begin(), b.end(), std::less<MoveOnly>());

        for(int i = 0; i < size; ++i) {
            BOOST_CHECK_EQUAL( *a[i], *b[i] );
        }
    }
}

#endif// #if HAS_MOVE || __cplusplus > 199711L // C++11
