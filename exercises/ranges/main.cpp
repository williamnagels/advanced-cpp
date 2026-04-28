void ranges_ex1();
void ranges_ex2();
void ranges_ex3();
void ranges_ex4();
void ranges_ex5();
void ranges_ex6();
int main()
{
  ranges_ex1();
  ranges_ex2();
  ranges_ex3();
  ranges_ex4();
  ranges_ex5();
  ranges_ex6();
  return 0;
}

//docker run --rm -v $ROOT_CPPTRAINING/exercises:/source -v $ROOT_CPPTRAINING/build:/build advancedcpp /bin/sh -c "cmake --version ;ls -alrt /source; cmake -B /build -S /source; make -C /build"