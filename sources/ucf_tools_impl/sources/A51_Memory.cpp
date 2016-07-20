#include <chip_tools/ucf_tools/A51_Memory.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cassert>

using namespace chip_tools;
using namespace chip_tools::ucf_tools;

A51HexReader::A51HexReader():
   hex_file_()
{
}

A51HexReader::~A51HexReader()
{
   hex_file_.clear();
}

int A51HexReader::hex2dec(const std::string &hex_string)
{
   std::stringstream converter;
   converter << std::hex << hex_string.c_str();

   int dec_value = 0;
   converter >> dec_value;
   return dec_value;
}

int A51HexReader::add_record(const std::string &hex_string)
{
   if (hex_string[0] == ':')
   {
      int type = hex2dec(hex_string.substr(1 + HEX_LENGTH_SIZE + HEX_ADDRESS_SIZE, HEX_TYPE_SIZE));
      if (type == END_OF_FILE)
      {
         return END_OF_FILE;
      }

      int address = hex2dec(hex_string.substr(1 + HEX_LENGTH_SIZE, HEX_ADDRESS_SIZE));
      int length = hex2dec(hex_string.substr(1, HEX_LENGTH_SIZE));
      if (length > 0)
      {
         unsigned char checksum = static_cast< unsigned char >(-length - type - address - (address >> 8));
         std::vector< unsigned char > hex_data;
         for (int i = 0; i < length; ++i)
         {
            char byte = hex2dec(hex_string.substr(HEX_HEADER_SIZE + i*2, 2)) & 0xff;
            hex_data.push_back(byte);
            checksum -= byte;
         }
         if (checksum == (hex2dec(hex_string.substr(HEX_HEADER_SIZE + 2*length, 2)) & 0xff))
         {
            hex_file_[address] = hex_data;
            return 0;
         }
         return WRONG_HEX_CHECKSUM;
      }
   }
   return WRONG_HEX_FORMAT;
}

Errcode A51HexReader::open(const std::string &hex_file_name)
{
   std::ifstream a51_file(hex_file_name);
   if (!a51_file.fail())
   {
      std::istream_iterator< std::string > itr(a51_file);
      for (; itr != std::istream_iterator< std::string >(); ++itr)
      {
         int result = add_record(*itr);
         if (result != 0)
         {
            switch (result)
            {
            case END_OF_FILE:
               return Errcode::success;
            case WRONG_HEX_CHECKSUM:
               return Errcode::wrong_checksum;
            case WRONG_HEX_FORMAT:
               return Errcode::wrong_file_format;
            default:
               return Errcode::wrong_file_format;
            }
         }
      }
      return Errcode::wrong_file_format;
   }
   return Errcode::failed_open_file;
}
