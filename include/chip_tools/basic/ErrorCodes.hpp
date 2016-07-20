#ifndef CHIP_TOLS_ERROR_CODES_HPP_
#define CHIP_TOLS_ERROR_CODES_HPP_

#include <chip_tools/basic/Errno.hpp>
#include <string>

#define RETURN_IF_NEQ(result, expected) \
   if (true)                            \
   {                                    \
      auto true_result = (result);      \
      auto true_expected = (expected);  \
      if (true_result != true_expected) \
         return true_result;            \
   }                                    \
   else                                 \
      void(0)

#define RETURN_IF_ERROR(result) \
   RETURN_IF_NEQ(result, Errcode::success)

namespace chip_tools
{

enum class Errcode
{
   success = 0,
   //----------------common------------------
   wrong_checksum = WRONG_CHECKSUM,
   invalid_handler = INVALID_HANDLER,
   //--------------filesystem----------------
   failed_create_directory = FAILED_CREATE_DIRECTORY,
   failed_create_file = FAILED_CREATE_FILE,
   failed_open_file = FAILED_OPEN_FILE,
   write_file_failed = WRITE_FILE_FAILED,
   read_file_failed = READ_FILE_FAILED,
   //-------------config files---------------
   wrong_file_format = WRONG_FILE_FORMAT,
   incorrect_data_value = INCORRECT_DATA_VALUE,
   //------------Log messagers---------------
   messager_not_available = MESSAGER_NOT_AVAILABLE,
   debug_level_not_enough = DEBUG_LEVEL_NOT_ENOUGH,
   //------------devices---------------------
   unused_device = UNUSED_DEVICE,
   couldnt_connect = COULDNT_CONNECT,
   device_io_failed = DEVICE_IO_FAILED
}; // enum class Errcode

namespace // detail
{
   template< Errcode Errc >
   struct errorname
   {
      static const std::string value()
      {
         return "unknown error";
      }
   }; // struct errorname
} // namespace

   template< Errcode Errc >
   const std::string print_error()
   {
      return errorname< Errc >::value();
   }

   inline std::string err_to_str(Errcode errc)
   {
      switch (errc)
      {
      case Errcode::success: return print_error<Errcode::success>();
      case Errcode::wrong_checksum: return print_error<Errcode::wrong_checksum>();
      case Errcode::invalid_handler: return print_error< Errcode::invalid_handler>();
      case Errcode::failed_create_directory: return print_error<Errcode::failed_create_directory>();
      case Errcode::failed_create_file: return print_error<Errcode::failed_create_file>();
      case Errcode::failed_open_file: return print_error<Errcode::failed_open_file>();
      case Errcode::write_file_failed: return print_error<Errcode::write_file_failed>();
      case Errcode::read_file_failed: return print_error<Errcode::read_file_failed>();
      case Errcode::wrong_file_format: return print_error<Errcode::wrong_file_format>();
      case Errcode::incorrect_data_value: return print_error<Errcode::incorrect_data_value>();
      case Errcode::messager_not_available: return print_error<Errcode::messager_not_available>();
      case Errcode::debug_level_not_enough: return print_error<Errcode::debug_level_not_enough>();
      case Errcode::unused_device: return print_error<Errcode::unused_device>();
      case Errcode::couldnt_connect: return print_error<Errcode::couldnt_connect>();
      case Errcode::device_io_failed: return print_error<Errcode::device_io_failed>();
      default: return print_error< Errcode::success>();
      }
   }

namespace // detail
{
   template<> struct errorname< Errcode::success >
   { static const std::string value() { return "success"; } };

   template<> struct errorname< Errcode::wrong_checksum >
   { static const std::string value() { return "wrong checksum"; } };

   template<> struct errorname< Errcode::invalid_handler >
   { static const std::string value() { return "invalid handler"; } };

   template<> struct errorname< Errcode::failed_create_directory >
   { static const std::string value() { return "failed create directory"; } };

   template<> struct errorname< Errcode::failed_create_file >
   { static const std::string value() { return "failed create file"; } };

   template<> struct errorname< Errcode::failed_open_file >
   { static const std::string value() { return "failed open file"; } };

   template<> struct errorname< Errcode::write_file_failed >
   { static const std::string value() { return "write file failed"; } };

   template<> struct errorname< Errcode::read_file_failed >
   { static const std::string value() { return "read_file_failed"; } };

   template<> struct errorname< Errcode::wrong_file_format >
   { static const std::string value() { return "wrong file format"; } };

   template<> struct errorname< Errcode::incorrect_data_value >
   { static const std::string value() { return "incorrect data value"; } };

   template<> struct errorname< Errcode::messager_not_available >
   { static const std::string value() { return "messager not available"; } };

   template<> struct errorname< Errcode::debug_level_not_enough >
   { static const std::string value() { return "debug level not enough"; } };

   template<> struct errorname< Errcode::unused_device >
   { static const std::string value() { return "unused device"; } };

   template<> struct errorname< Errcode::couldnt_connect >
   { static const std::string value() { return "couldnt connect"; } };

   template<> struct errorname< Errcode::device_io_failed >
   { static const std::string value() { return "device_io_failed"; } };

} // namespace

inline bool chip_tools_ok(Errcode result)
{
   return result == Errcode::success;
}

} // namespace chip_tools

#endif // #ifndef CHIP_TOLS_ERROR_CODES_HPP_
