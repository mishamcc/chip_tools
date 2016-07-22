#ifndef CHIP_TOOLS_COMPOSITE_HPP_
#define CHIP_TOOLS_COMPOSITE_HPP_

#include <chip_tools/basic/ErrorCodes.hpp>
#include <chip_tools/basic/Common.hpp>
#include <chip_tools/basic/Log.hpp>
#include <ostream>
#include <boost/property_tree/ptree.hpp>

/*! \file Composite.hpp
 *  \brief Defines fundamental type Composite
 */

namespace chip_tools
{

/*! \addtogroup Common
 *  @{
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class... Types>
struct Initialize;

template<class Base>
struct Initialize<Base>
{
   static Errcode perform(Base *, boost::property_tree::ptree *)
   { return Errcode::success; }
};

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

/*! \brief Calls the initialize method for all children
 *  Usage:
 *  \code ...
 *  Initialize<This, Children>::perform(static_cast<This *>(this), node);
 *  \endcode
 */
template<class Base, class First, class... Rest>
struct Initialize<Base, First, Rest...>
{
   static Errcode perform(First *first, boost::property_tree::ptree *node)
   {
      RETURN_IF_ERROR(first->initialize_all(node));
      return Initialize<Base, Rest...>::perform(static_cast<Base *>(first), node);
   }
};

/*! \brief The fundamental type of the ChipTools. Composite implements the same-name design pattern at compile-time.
 *  \tparam name  - name of the Composite
 *  \tparam This - type of child (in terms of CRTP)
 *  \tparam Successors... - children
 */
template<const std::string &name,
         class This,
         class... Successors>
class Composite : public Successors...
{
public:
   /*! \param[in] output - output stream of Log
    */
   explicit Composite(std::ostream &output = std::cout) :
      Successors()..., log_(output) {}

   /*! \brief Returns the name of Composite */
   std::string get_name() const {return name_;}

   /*! \brief Returns the info string loaded from an xml-file */
   std::string get_info() const {return info_;}

   /*! \brief Is composite used */
   bool used() const {return used_;}

   /*! \brief Performs loading parameters from xml-file. Then calls initialize_all method of the children.
    *  \param[in] node - pointer to parent node. Current node will be searching by Composite name.
    *  \retval Errcode::success in case of success. Error code in other cases.
    */
   Errcode initialize_all(boost::property_tree::ptree *node);
   /*! \brief Put trace message
    */
   void trace(const std::string &text)
   { log_.trace(get_name() + ": " + text); }
   /*! \brief Put debug message
    */
   void debug(const std::string &text)
   { log_.debug(get_name() + ": " + text); }
   /*! \brief Put info message
    */
   void info(const std::string &text)
   { log_.info(get_name() + ": " + text); }
   /*! \brief Put warning message
    */
   void warning(const std::string &text)
   { log_.warning(get_name() + ": " + text); }
   /*! \brief Put error message
    */
   void error(const std::string &text)
   { log_.error(get_name() + ": " + text); }
   /*! \brief Put fatal message
    */
   void fatal(const std::string &text)
   { log_.fatal(get_name() + ": " + text); }

private:
   std::string name_ = name;
   std::string info_ = "";
   bool used_ = false;
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
   used_ = current_node.get<bool>("<xmlattr>.Used", false);
   info_ = current_node.get<std::string>("<xmlattr>.Info", get_name());

   if (used())
      RETURN_IF_ERROR(try_initialize::call<This>(static_cast<This *>(this), &current_node));

   return Initialize<Composite, Successors...>::perform(this, &current_node);
}

/** @} */

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_COMPOSITE_HPP_
