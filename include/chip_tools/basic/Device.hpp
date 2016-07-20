#ifndef CHIP_TOOLS_DEVICE_HPP_
#define CHIP_TOOLS_DEVICE_HPP_

#include <chip_tools/basic/Composite.hpp>

namespace chip_tools
{

template<class... Types>
struct Connect;

template<class Base>
struct Connect<Base>
{
   static Errcode perform(Base *)
   { return Errcode::success; }
};

template<class Base, class First, class... Rest>
struct Connect<Base, First, Rest...>
{
   static Errcode perform(First *first)
   {
      RETURN_IF_ERROR(first->connect_all());
      return Connect<Base, Rest...>::perform(static_cast<Base *>(first));
   }
};

template<class... Types>
struct Disconnect;

template<class Base>
struct Disconnect<Base>
{
   static Errcode perform(Base *)
   { return Errcode::success; }
};

template<class Base, class First, class... Rest>
struct Disconnect<Base, First, Rest...>
{
   static Errcode perform(First *first)
   {
      RETURN_IF_ERROR(first->disconnect_all());
      return Disconnect<Base, Rest...>::perform(static_cast<Base *>(first));
   }
};

CALL_IFDEF(connect, Errcode);
CALL_IFDEF(disconnect, Errcode);

template<const std::string &name,
         class This,
         class... Successors>
class Device : public Composite<name, This, Successors...>
{
public:
   explicit Device(std::ostream &output = std::cout) :
      Composite<name, This, Successors...>(output) {}

   Errcode connect_all()
   {
      if (this->is_using())
         RETURN_IF_ERROR(try_connect::call<This>(static_cast<This *>(this)));
      return Connect<Device, Successors...>::perform(this);
   }

   Errcode disconnect_all()
   {
      if (this->is_using())
         RETURN_IF_ERROR(try_disconnect::call<This>(static_cast<This *>(this)));
      return Disconnect<Device, Successors...>::perform(this);
   }
}; // class Device

} // namespace chip_tools

#endif // #ifndef CHIP_TOOLS_DEVICE_HPP_
