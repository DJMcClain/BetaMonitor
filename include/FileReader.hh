#include <list>
#include <fstream>
class FileReader
{
 public:
   FileReader();
  ~FileReader();
   static FileReader* Instance();
   void GetAnEvent();
   static std::vector<double> cEn;
   static std::vector<double> cIn;
 private:
   static FileReader* Instance_1;
};

