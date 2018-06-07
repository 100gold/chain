#pragma once

#include <functional>


struct ChainOfRepsonsibility
{
   template<typename... Args>
   struct Chain
   {
      template<typename Callee, typename Next>
      Chain(const Callee c, const Next& n)
      {
         m_impl = c;
         m_next = n;
      }

      template<typename Callee>
      decltype(auto) attach(Callee c)
      {
         return Chain(c, *this);
      }

      void operator()(Args... e)
      {
         m_impl(e..., m_next);
      }

      std::function<void(Args..., std::function<void(Args...)>)> m_impl;
      std::function<void(Args...)> m_next;
   };

   template<typename... Args>
   struct ChainTail
   {
      template<typename Callee>
      ChainTail(Callee c)
      {
         m_impl = c;
      }

      template<typename Callee>
      decltype(auto) attach(Callee c)
      {
         return Chain<Args...>(c, m_impl);
      }

      void operator()(Args... e)
      {
         m_impl(e...);
      }

      std::function<void(Args... e)> m_impl;
   };

   template<typename>
   struct StartChain;

   template<typename C, typename... Args>
   struct StartChain<void (C::*)(Args...) const>
   {
      using Type = ChainTail<Args...>;
   };

   template<typename Callee>
   static decltype(auto) start_new(Callee c)
   {
      return StartChain<decltype(&Callee::operator())>::Type(c);
   }
};