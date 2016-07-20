#ifndef CHIP_TOOLS_COMPOSITE_HPP_
#define CHIP_TOOLS_COMPOSITE_HPP_

#include <chip_tools/basic/ErrorCodes.hpp>
#include <chip_tools/basic/Common.hpp>
#include <chip_tools/basic/Log.hpp>
#include <ostream>
#include <boost/property_tree/ptree.hpp>

namespace chip_tools
{

template<class... Types>
struct Initialize;

template<class Base>
struct Initialize<Base>
{
   static Errcode perform(Base *, boost::property_tree::ptree *)
   { return Errcode::success; }
};

template<class Base, class First, class... Rest>
struct Initialize<Base, First, Rest...>
{
   static Errcode perform(First *first, boost::property_tree::ptree *node)
   {
      RETURN_IF_ERROR(first->initialize_all(node));
      return Initialize<Base, Rest...>::perform(static_cast<Base *>(first), node);
   }
};

template<const std::string &name,
         class This,
         class... Successors>
class Composite : public Successors...
{
public:
   explicit Composite(std::ostream &output = std::cout) :
      Successors()..., log_(output) {}

   std::string get_name() const {return name_;}
   std::string get_info() const {return info_;}

   bool is_using() const {return using_;}

   Errcode initialize_all(boost::property_tree::ptree *node);

   void trace(const std::string &text)
   { log_.trace(get_name() + ": " + text); }

   void debug(const std::string &text)
   { log_.debug(get_name() + ": " + text); }

   void info(const std::string &text)
   { log_.info(get_name() + ": " + text); }

   void warning(const std::string &text)
   { log_.warning(get_name() + ": " + text); }

   void error(const std::string &text)
   { log_.error(get_name() + ": " + text); }

   void fatal(const std::string &text)
   { log_.fatal(get_name() + ": " + text); }

private:
   std::string name_ = name;
   std::string info_ = "";
   bool using_ = false;
   Log<> log_;
}; // class Composite

CALL_IFDEF1(initialize, Errcode, boost::property_tree::ptree *);

template<const std::string &name,
         class This,
         class... Successors>
Errcode Composite<name, This, Successors...>::initialize_all(boost::property_tree::ptree *node)
{
   if (node == nullptr)
      return Errcode::wrong_file_format;
   boost::optional<boost::property_tree::ptree &> current_node_exists =
      node->get_child_optional(get_name());
   if (!current_node_exists)
      return Errcode::wrong_file_format;

   auto current_node = current_node_exists.value();
   using_ = current_node.get<bool>("<xmlattr>.Using", false);
   info_ = current_node.get<std::string>("<xmlattr>.Info", get_name());

   if (using_)
      RETURN_IF_ERROR(try_initialize::call<This>(static_cast<This *>(this), &current_node));

   return Initialize<Composite, Successors...>::perform(this, &current_node);
}

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_COMPOSITE_HPP_
