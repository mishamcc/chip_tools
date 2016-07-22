#include <chip_tools/basic/Device.hpp>

#define BOOST_TEST_MODULE chip_tools_unit_test
#include <boost/test/included/unit_test.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace chip_tools;

BOOST_AUTO_TEST_SUITE(CompositeTest)

template<const std::string &name,
         class... Successors>
class ConcreteDevice : public Device<name, ConcreteDevice<name, Successors...>, Successors...>
{
public:
   ConcreteDevice() :
      Device<name, ConcreteDevice<name, Successors...>, Successors...>() {}

   Errcode connect()
   {
      this->info("Connected");
      connected_ = true;
      return Errcode::success;
   }

   Errcode disconnect()
   {
      this->info("Disconnected");
      connected_ = false;
      return Errcode::success;
   }

   Errcode initialize(boost::property_tree::ptree *)
   {
      this->info("Initialized");
      initialized_ = true;
      return Errcode::success;
   }

   bool is_connected() const {return connected_;}
   bool is_initialized() const {return initialized_;}

private:
   bool connected_ = false;
   bool initialized_ = false;
}; // class ConcreteDevice

std::string name5 = "Device5";
using Device5 = ConcreteDevice<name5>;

std::string name4 = "Device4";
using Device4 = ConcreteDevice<name4, Device5>;

std::string name3 = "Device3";
using Device3 = ConcreteDevice<name3, Device4>;

std::string name2 = "Device2";
using Device2 = ConcreteDevice<name2>;

std::string name1 = "Device1";
using Device1 = ConcreteDevice<name1, Device2, Device3>;

BOOST_AUTO_TEST_CASE(InitializeTest)
{
   Device1 stand;

   BOOST_CHECK_EQUAL(stand.get_name(), name1);
   BOOST_CHECK_EQUAL(stand.Device2::get_name(), name2);
   BOOST_CHECK_EQUAL(stand.Device3::get_name(), name3);
   BOOST_CHECK_EQUAL(stand.Device4::get_name(), name4);
   BOOST_CHECK_EQUAL(stand.Device5::get_name(), name5);

   std::ifstream config_file("config/Config.xml");
   BOOST_CHECK(config_file.is_open());
   boost::property_tree::ptree config_tree;
   BOOST_REQUIRE_NO_THROW(boost::property_tree::xml_parser::read_xml(config_file, config_tree));

   BOOST_CHECK(stand.initialize_all(&config_tree) == Errcode::success);

   BOOST_CHECK(stand.is_initialized());
   BOOST_CHECK(stand.Device2::is_initialized());
   BOOST_CHECK(stand.Device3::is_initialized());
   BOOST_CHECK(stand.Device4::is_initialized());
   BOOST_CHECK(stand.Device5::is_initialized());

   BOOST_CHECK(stand.used());
   BOOST_CHECK(stand.Device2::used());
   BOOST_CHECK(stand.Device3::used());
   BOOST_CHECK(stand.Device4::used());
   BOOST_CHECK(stand.Device5::used());

   BOOST_CHECK_EQUAL(stand.get_info(), name1);
   BOOST_CHECK_EQUAL(stand.Device2::get_info(), "Device2 info");
   BOOST_CHECK_EQUAL(stand.Device3::get_info(), "Device3 info");
   BOOST_CHECK_EQUAL(stand.Device4::get_info(), "Device4 info");
   BOOST_CHECK_EQUAL(stand.Device5::get_info(), "Device5 info");
}

BOOST_AUTO_TEST_CASE(ConnectTest)
{
   Device1 stand;

   std::ifstream config_file("config/Config.xml");
   BOOST_CHECK(config_file.is_open());
   boost::property_tree::ptree config_tree;
   BOOST_REQUIRE_NO_THROW(boost::property_tree::xml_parser::read_xml(config_file, config_tree));

   BOOST_CHECK(stand.initialize_all(&config_tree) == Errcode::success);

   stand.connect_all();
   BOOST_CHECK(stand.is_connected());
   BOOST_CHECK(stand.Device2::is_connected());
   BOOST_CHECK(stand.Device3::is_connected());
   BOOST_CHECK(stand.Device4::is_connected());
   BOOST_CHECK(stand.Device5::is_connected());

   stand.disconnect_all();
   BOOST_CHECK(!stand.is_connected());
   BOOST_CHECK(!stand.Device2::is_connected());
   BOOST_CHECK(!stand.Device3::is_connected());
   BOOST_CHECK(!stand.Device4::is_connected());
   BOOST_CHECK(!stand.Device5::is_connected());
}

BOOST_AUTO_TEST_SUITE_END()
