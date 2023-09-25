// LibAK030.h
#ifndef LIB_AK030_H_INCLUDE
#define LIB_AK030_H_INCLUDE

#include <AK-030.h>

class LibAK030 {
public:
  LibAK030();
  static void Show();
  static bool Connect(AK030 *);
  static bool Close(AK030 *);
  static const char *DnsLookup(AK030 *, const char *);
  static bool OpenTCP(AK030 *, const char *, int);

  static bool RequestPost(AK030 *ak030, const char *req);

private:
  bool debug;
};

#endif
