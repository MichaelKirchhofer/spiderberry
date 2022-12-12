// adctest.h
// simple test routines which use BCM2835 functions directly

  class ADCTest
  {
    public:
      ADCTest(int address);
      ~ADCTest();

      int Write(int value);
      int Read(int channel, int &value);

    private:
      int m_address = 0;
  };
