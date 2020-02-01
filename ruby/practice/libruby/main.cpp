#include <ruby.h>
#include <time.h>
#include <thread>
#include <string>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

extern "C"
{
    //The function that ruby expects, typedefed because once __cplusplus is defined
    //the function becomes VALUE (*)(...) which if a method's only args are (...) the va_args aren't accessible
    typedef VALUE (*RubyFunc)(...);

    //The module
    VALUE module_CTime = Qnil;

    //Init method for the module, must have same name as module
    void Init_CTime();
    
    //Methods belonging to the module
    VALUE method_CPrintTime(VALUE self);
    VALUE method_CFormatTime(VALUE self, VALUE format);
    VALUE method_CTimerStart(VALUE self);
    VALUE method_CTimerElapsed(VALUE self, VALUE time);

    void Init_CTime()
    {
        //Naming the module
        module_CTime = rb_define_module("CTime");

        //Define the methods belonging to the module
        //param1: the module, param2: the method name, param3: the function, param4: number of args + 1 (self value + actual args)
        rb_define_method(module_CTime, "CPrintTime", (RubyFunc)method_CPrintTime, 0);
        rb_define_method(module_CTime, "CFormatTime", (RubyFunc)method_CFormatTime, 1);
        rb_define_method(module_CTime, "CTimerStart", (RubyFunc)method_CTimerStart, 0);
        rb_define_method(module_CTime, "CTimerElapsed", (RubyFunc)method_CTimerElapsed, 1);
    }

    VALUE method_CPrintTime(VALUE self)
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::time_t epochTime = std::chrono::high_resolution_clock::to_time_t(now);

        std::cout << std::ctime(&epochTime);
        
        return Qnil;
    }

    VALUE method_CFormatTime(VALUE self, VALUE format)
    {
        auto len = RSTRING_LEN(format);
        auto ptr = StringValueCStr(format);

        std::string buffer;

        buffer.resize(len + 1);
        buffer.insert(buffer.begin(), ptr, ptr + len);

        std::cout << buffer << std::endl;        
        return Qnil;
    }

    VALUE method_CTimerStart(VALUE self)
    {
        time_t epochTime = time(NULL);

        VALUE retval = ULONG2NUM(epochTime);
        return retval;
    }

    VALUE method_CTimerElapsed(VALUE self, VALUE input)
    {
        time_t then = NUM2ULONG(input);
        time_t now = time(NULL);

        std::cout << "Then: " << then << std::endl;
        std::cout << "Now: " << now << std::endl;

        auto diff = difftime(now, then);

        std::cout << "Diff: " << diff << std::endl;

        VALUE retval = DBL2NUM(diff);
        return retval;
    }
}