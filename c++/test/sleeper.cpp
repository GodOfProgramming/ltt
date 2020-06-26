#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <string>
#include <fstream>
#include <cstring>
#include <memory>

using namespace std::chrono_literals;

volatile bool gAlive = true;

class File
{
 public:
  File(std::string filename)
  {
    mFile.open(filename, std::ios::out);
    auto msg = "opened file\n";
    mFile << msg << std::flush;
  }

  ~File()
  {
    auto msg = "closing file\n";
    mFile << msg << std::flush;
    mFile.close();
  }

  void write(std::string msg)
  {
    mFile << msg << std::flush;
  }

 private:
  std::ofstream mFile;
};

std::unique_ptr<File> f;

int main()
{
  f = std::make_unique<File>("/tmp/log.txt");

  auto lam = [](int) {
    f->write("clean shutdown\n");
    gAlive = false;
  };

  signal(SIGINT, lam);
  signal(SIGTERM, lam);

  while (gAlive) {
    f->write("running\n");
    std::this_thread::sleep_for(1s);
  }
}
