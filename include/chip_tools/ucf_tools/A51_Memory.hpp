#ifndef CHIP_TOOLS_A51_MEMORY_HPP_
#define CHIP_TOOLS_A51_MEMORY_HPP_

#include <chip_tools/basic/ErrorCodes.hpp>
#include <map>
#include <vector>

namespace chip_tools
{

namespace ucf_tools
{

// http://www.keil.com/support/docs/1584.htm

// Record Format
// An Intel HEX file is composed of any number of HEX records. Each record is made up
// of five fields that are arranged in the following format:

// :llaaaatt[dd...]cc

// Each group of letters corresponds to a different field, and each letter represents
// a single hexadecimal digit. Each field is composed of at least two hexadecimal
// digits-which make up a byte-as described below:

// : is the colon that starts every Intel HEX record.
// ll is the record-length field that represents the number of data bytes (dd) in the record.
// aaaa is the address field that represents the starting address for subsequent data in the
//    record.
// tt is the field that represents the HEX record type, which may be one of the following:
//    00 - data record
//    01 - end-of-file record
//    02 - extended segment address record
//    04 - extended linear address record
//    05 - start linear address record (MDK-ARM only)
// dd is a data field that represents one byte of data. A record may have multiple data bytes.
//    The number of data bytes in the record must match the number specified by the ll field.
// cc is the checksum field that represents the checksum of the record. The checksum is
//    calculated by summing the values of all hexadecimal digit pairs in the record modulo 256 and
//    taking the two's complement.

class A51HexReader
{
public:
   typedef std::map< int, std::vector< unsigned char > > HexFile;

   A51HexReader();
   ~A51HexReader();

   Errcode open(const std::string &hex_file_name);

   inline size_t size() const
   {
      return hex_file_.size();
   }

   inline HexFile::iterator begin()
   {
      return hex_file_.begin();
   }

   inline HexFile::iterator end()
   {
      return hex_file_.end();
   }

   inline bool empty() const
   {
      return hex_file_.empty();
   }

private:
   static const size_t DEFAULT_HEXRECORD_SIZE = 256;
   static const size_t HEX_LENGTH_SIZE = 2;
   static const size_t HEX_ADDRESS_SIZE = 4;
   static const size_t HEX_TYPE_SIZE = 2;
   static const size_t HEX_HEADER_SIZE = 9;

   static const int WRONG_HEX_FORMAT = -1;
   static const int WRONG_HEX_CHECKSUM = -2;

   static const int DATA_RECORD = 0;
   static const int END_OF_FILE = 1;
   static const int EXTENDED_SEGMENT_ADDRESS = 2;
   static const int EXTENDED_LINEAR_ADDRESS = 4;
   static const int START_LINEAR_ADDRESS = 5;

   int hex2dec(const std::string &hex_string);
   int add_record(const std::string &hex_string);

   HexFile hex_file_;
}; // class A51HexReader

} // namespace ucf_tools

} // namespace chip_tools

#endif // CHIP_TOOLS_A51_MEMORY_HPP_

