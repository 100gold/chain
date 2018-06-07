#include <string>
#include <iostream>
#include<conio.h>

#include "chain.h"


class Elephant
{
public:
   std::string touch_leg()
   {
      return "it's like a pillar";
   }

   std::string touch_trunk()
   {
      return "it's like a snake";
   }

   std::string touch_tail()
   {
      return "it's like a rope";
   }

   void run_away()
   {
      m_is_gone = true;
      std::cout << "*** Sound of running out elephant ***\n";
   }

   bool is_elephant_here()
   {
      return !m_is_gone;
   }

private:
   bool m_is_gone = false;
};


int main(int argc, char** argv)
{
   auto blind_sage3 = ChainOfRepsonsibility::start_new([](Elephant& e) {
      std::cout << "Third blind sage: " << e.touch_tail() << "\n";
   });
   auto blind_sage2 = blind_sage3.attach([](Elephant& e, auto& next) {
      std::cout << "Second blind sage: " << e.touch_trunk() << "\n";
      next(e);
   });
   auto blind_sage1 = blind_sage2.attach([](Elephant& e, auto& next) {
      if (!e.is_elephant_here())
      {
         std::cout << "First blind sage: So empty... so true\n";
      }
      else
      {
         std::cout << "First blind sage: " << e.touch_leg() << "\n";
         next(e);
      }
   });

   Elephant e;
   blind_sage1(e);

   std::cout << std::endl;
   e.run_away();
   blind_sage1(e);
   
   _getch();
   return 0;
}