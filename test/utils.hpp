//
// Created by ZHENG Zhong on 2011-03-16.
//

#ifndef XTREE_TEST_UTILS_20110316__
#define XTREE_TEST_UTILS_20110316__

#include <xtree/config.hpp>

#ifdef XTREE_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4512 4702)
#endif

#include <boost/algorithm/string/trim.hpp>

#ifdef XTREE_MSVC
#  pragma warning(pop)
#endif

#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#ifdef XTREE_WIN32
#  include <windows.h>
#endif

#include <functional>
#include <iostream>
#include <string>


namespace utils {
namespace detail {


    struct is_slash: public std::unary_function<std::string::value_type, bool>
    {
        bool operator()(const std::string::value_type& c) const
        {
            return (c == '/' || c == '\\');
        }
    };


        inline std::string file_sep()
        {
            #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
                return "\\";
            #else
                return "/";
            #endif
        }


}  // namespace utils::detail
}  // namespace utils


///////////////////////////////////////////////////////////////////////////////////////////////////
// win32 platform
//


#ifdef XTREE_WIN32


namespace utils {


    //! Returns the system variable value by name.
    inline std::string get_sys_var(const std::string& name)
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


    inline std::string join_path(const std::string& dir, const std::string& name)
    {
        return ( boost::trim_right_copy_if(dir, detail::is_slash())
               + "\\"
               + boost::trim_left_copy_if(name, detail::is_slash()) );
    }


    inline std::string get_fixture_path(const std::string& name)
    {
        const std::string fixture_dir = get_sys_var("XTREE_FIXTURE_DIR");
        return join_path(fixture_dir, name);
    }


}  // namespace utils


#endif  // XTREE_WIN32


#endif  // XTREE_TEST_UTILS_20110316__

