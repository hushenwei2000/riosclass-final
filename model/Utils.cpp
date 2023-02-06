#include "Utils.hpp"

#include <string>
#include <iostream>
#include <cmath>

#include "parameters.hpp"

using namespace std;

string Utils::reverseString(string n, int len) {
  string s;
  for (int i = 0; i < len; i++) {
    if (n[i] == '0') {
      s = s + '1';
    } else {
      s = s + '0';
    }
  }
  return s;
}

INSTRUCTION Utils::toInstruction(long long n) {
  INSTRUCTION ins;
  int i = 0;
  for (int j = 0; j < 32; j++) {
    ins[j] = '0';
  }
  while (n != 0) {
    ins[i] = (n % 2 == 0 ? '0' : '1');
    i++;
    n /= 2;
  }
  return ins;
}

void Utils::dumpPC(int pc_cout) {
  INSTRUCTION cout_pc = toInstruction(pc_cout);
  for (int i = 31; i >= 0; i--) {
    cout << cout_pc[i];
  }
  cout << endl;
}

long long Utils::toIntLittleEndian(string s, int l) {
  // s[0] is the lowest bit
  long long sum = 0;
  for (int i = 0; i < l; i++) {
    sum = sum + (s[i] - 48) * pow(2, i);
  }

  return sum;
}

long long Utils::toIntBigEndian(string s, int l)
{
  // s[0] is the highest bit
    long long sum = 0;
    for(int i = 0; i < l;i++)
    {
        sum = sum + (s[i]-48) * pow(2,l-i-1);
    }

    return sum;
}

uint8_t Utils::makeCacheUint8(string s) {
  uint8_t ans = 0;
  for(int i = 0; i < 8; i++) {
    ans = ans | (s[7 - i] - 48);
    if(i != 7) ans <<= 1;
  }
  return ans;
}

string Utils::makeCacheString(uint8_t num) {
  string ans = "";
  for(int i = 0; i < 8; i++) {
    ans = ans + (char)('0' + (num & 1));
    num >>= 1;
  }
  return ans;
}

string Utils::charArrayToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

// Little endian; Low bit store in low index (both in STORE_DATA_XXX and Memory)

STORE_DATA_DWORD Utils::toBinary_d(long long n) {
  STORE_DATA_DWORD ins;
  int i = 0;
  if (n < 0) {
    long long m = abs(n);
    long long q = m ^ 0xffffffffffffffff;
    q = q + 1;
    n = q;
  }
  for (int j = 0; j < 64; j++) {
    ins[j] = '0';
  }
  while (n != 0 && i < 64) {
    ins[i] = (n % 2 == 0 ? '0' : '1');
    i++;
    n /= 2;
  }
  return ins;
}

STORE_DATA_BYTE Utils::toBinary_b(long long n) {
  STORE_DATA_BYTE ins;
  int i = 0;
  if (n < 0) {
    long long m = abs(n);
    long long q = m ^ 0xff;
    q = q + 1;
    n = q;
  }
  for (int j = 0; j < 8; j++) {
    ins[j] = '0';
  }

  while (n != 0 && i < 8) {
    ins[i] = (n % 2 == 0 ? '0' : '1');
    i++;
    n /= 2;
  }
  return ins;
}

STORE_DATA_HALF Utils::toBinary_h(long long n) {
  STORE_DATA_HALF ins;
  int i = 0;
  if (n < 0) {
    long long m = abs(n);
    long long q = m ^ 0xffff;
    q = q + 1;
    n = q;
  }
  for (int j = 0; j < 16; j++) {
    ins[j] = '0';
  }
  while (n != 0 && i < 16) {
    ins[i] = (n % 2 == 0 ? '0' : '1');
    i++;
    n /= 2;
  }
  return ins;
}

STORE_DATA_WORD Utils::toBinary_w(long long n) {
  STORE_DATA_WORD ins;
  int i = 0;
  if (n < 0) {
    long long m = abs(n);
    long long q = m ^ 0xffffffff;
    q = q + 1;
    n = q;
  }
  for (int j = 0; j < 32; j++) {
    ins[j] = '0';
  }
  while (n != 0 && i < 32) {
    ins[i] = (n % 2 == 0 ? '0' : '1');
    i++;
    n /= 2;
  }
  return ins;
}

void Utils::strmncpy(char* s, int m, int n, char* t) {
  char* p = s;
  char* q = t;
  int a = 0;
  p = p + m;
  while (a < n) {
    *q++ = *p++;
    a++;
  }
  *q = '\0';
}
