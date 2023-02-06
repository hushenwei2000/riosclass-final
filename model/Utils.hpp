#pragma once
#include <string>

#include "parameters.hpp"

using namespace std;

class Utils {
 public:
  static string reverseString(string n, int len);
  static INSTRUCTION toInstruction(long long n);
  static void dumpPC(int pc_cout);
  static long long toIntLittleEndian(string s, int l);
  static long long toIntBigEndian(string s, int l);
  static uint8_t makeCacheUint8(string s);
  static string makeCacheString(uint8_t num);
  static string charArrayToString(char* a, int size);
  static STORE_DATA_DWORD toBinary_d(long long n);
  static STORE_DATA_BYTE toBinary_b(long long n);
  static STORE_DATA_HALF toBinary_h(long long n);
  static STORE_DATA_WORD toBinary_w(long long n);
  static void strmncpy(char* s, int m, int n, char* t);
};