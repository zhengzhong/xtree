//
// Created by ZHENG Zhong on 2011-06-06.
//

#include "xtree_test_utils.hpp"

#if defined(__GNUC__) && __GNUC__ >= 4
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define XTREE_TEST_WIN32
#endif


#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127 4512 4702)
#endif

#include <boost/algorithm/string/trim.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#ifdef XTREE_TEST_WIN32
#  include <windows.h>
#endif

#include <cstdlib>
#include <functional>
#include <string>


namespace test_utils {


    namespace {


        struct is_slash_: public std::unary_function<std::string::value_type, bool>
        {
            bool operator()(const std::string::value_type& c) const
            {
                return (c == '/' || c == '\\');
            }
        };


#ifdef XTREE_TEST_WIN32  // windows platform .......................................................


        std::string file_sep_()
        {
            return "\\";
        }


        std::string get_sys_var_(const std::string& name)
        {
            // Try to retrieve the variable value and put it into the buffer.
            unsigned int buffer_size = 1024U;
            boost::scoped_array<char> buffer(new char[buffer_size]);
            buffer[0] = 0;
            DWORD count = GetEnvironmentVariable(name.c_str(), buffer.get(), buffer_size);
            // Check if the buffer is big enough to hold the variable value.
            if (count >= buffer_size)
            {
                buffer_size = count + 1; // buffer_size = count should be enough.
                buffer.reset(new char[buffer_size]);
                buffer[0] = 0;
                count = GetEnvironmentVariable(name.c_str(), buffer.get(), buffer_size);
            }
            // Check if the variable value is retrieved successfully.
            if (count > 0 && count < buffer_size)
            {
                return std::string(buffer.get());
            }
            else
            {
                return std::string();
            }
        }


#else  // non-windows platform .....................................................................


        std::string file_sep_()
        {
            return "/";
        }


        std::string get_sys_var_(const std::string& name)
        {
            const char* value = std::getenv(name.c_str());
            return (value != 0 ? std::string(value) : std::string());
        }


#endif  // .........................................................................................


    }  // anonymous namespace


    ////////////////////////////////////////////////////////////////////////////////////////////////


    std::string get_sys_var(const std::string& name)
    {
        return get_sys_var_(name);
    }


    std::string join_path(const std::string& dir, const std::string& name)
    {
        return ( boost::trim_right_copy_if(dir, is_slash_())
               + file_sep_()
               + boost::trim_left_copy_if(name, is_slash_()) );
    }


    std::string get_fixture_path(const std::string& name)
    {
        const std::string fixture_dir = get_sys_var("XTREE_FIXTURE_DIR");
        return join_path(fixture_dir, name);
    }


}  // namespace test_utils

