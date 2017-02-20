/*
 * http://stackoverflow.com/questions/3662654/conditional-debug-output-class-with-templated-operator
 */

#ifndef _DEBUG_W_HPP_
#define _DEBUG_W_HPP_

#include <iostream>

enum DEBUG_LEVEL {DEBUG_LEVEL_NOLOG, DEBUG_LEVEL_ERROR, DEBUG_LEVEL_INFO, DEBUG_LEVEL_DEBUG};

#define DEFAULT_DEBUG_LEVEL             (DEBUG_LEVEL_INFO)

#if defined(_DEBUG)
#define DEBUG_LOG_TEST_CONDITION        output
//#define DEBUG_LOG_DEBUG_TEST_LEVEL(v)   (v) <= DebugCake::DebugLevel
#define DEBUG_LOG_DEBUG_TEST_LEVEL(v)   (v) <= DEFAULT_DEBUG_LEVEL
#else
#define DEBUG_LOG_TEST_CONDITION        false
#define DEBUG_LOG_DEBUG_TEST_LEVEL(v)   false
#endif


template<typename C,typename T = std::char_traits<C> >
struct DInfo
{
    typedef std::basic_ostream<C,T>& (*StrFunc)(std::basic_ostream<C,T>&);
    static std::basic_ostream<C,T>& getDefault();
};

/*
struct DebugCake
{
    static int DebugLevel;
};
*/
template<typename C,typename T = std::char_traits<C> >
struct D
{

    D(int level)
        :stream(DInfo<C,T>::getDefault())
        ,output( DEBUG_LOG_DEBUG_TEST_LEVEL(level) )
    {}
    D(int level, std::basic_ostream<C,T>& s)
        :stream(s)
        ,output( DEBUG_LOG_DEBUG_TEST_LEVEL(level) )
    {}

    template<typename V>
    D& operator<<(V const& value)
    {
        // In release this is optimised away as it is always false
        if (DEBUG_LOG_TEST_CONDITION)
        {
            stream << value;
        }
        return *this;
    }

    D& operator<<(typename DInfo<C,T>::StrFunc func)
    {
        // In release this is optimised away as it is always false
        if (DEBUG_LOG_TEST_CONDITION)
        {
            func(stream);
        }
        return *this;
    }
    private:
       std::basic_ostream<C,T>&  stream;
       bool                      output;

};

/*
template<>
std::ostream&  DInfo<char,std::char_traits<char>       >::getDefault()
{return std::cout; }

template<>
std::wostream& DInfo<wchar_t,std::char_traits<wchar_t> >::getDefault()
{return std::wcout; }
*/


typedef D<char>    Debug;
typedef D<wchar_t> WDebug;
/*
int DebugCake::DebugLevel = 4;

int main()
{
    Debug   debug(1);

    debug << "Plop" << std::endl;

    WDebug  debugWide(2);
    debugWide << L"WIDE" << std::endl;
}
*/

#endif // _DEBUG_W_HPP_

