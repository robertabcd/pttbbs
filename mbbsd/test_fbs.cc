extern "C" {
#include <stdio.h>
#include "bbs.h"
}

#include "flatbuffers/flatbuffers.h"
#include "test.fbs.h"

namespace {

void do_test_read(const uint8_t *buf, size_t len) {
  auto verifier = flatbuffers::Verifier(buf, len);
  if (!mbbsd_fbs::VerifyTestUserLoginBuffer(verifier)) {
    fprintf(stderr, "fbs verify failed\n");
    return;
  }
  auto user_login = mbbsd_fbs::GetTestUserLogin(buf);
  if (strcmp(user_login->userid()->c_str(), currutmp->userid) != 0) {
    fprintf(stderr, "fbs userid does not match [%s] vs [%s]\n",
            user_login->userid()->c_str(), currutmp->userid);
    return;
  }
#ifdef DEBUG
  fprintf(stderr, "fbs test passed for [%s]\n", currutmp->userid);
#endif
}

}  // namespace

void do_test_fbs() {
  flatbuffers::FlatBufferBuilder fbb(1024);

  auto userid = fbb.CreateString(currutmp->userid);

  static_assert(sizeof(currutmp->from_ip) == 4, "expect 4 bytes for from_ip");
  auto ip = fbb.CreateVector<uint8_t>(
      reinterpret_cast<const uint8_t *>(&currutmp->from_ip),
      sizeof(currutmp->from_ip));

  mbbsd_fbs::TestUserLoginBuilder ulb(fbb);
  ulb.add_pid(currutmp->pid);
  ulb.add_uid(currutmp->uid);
  ulb.add_userid(userid);
  ulb.add_ip(ip);
  ulb.add_timestamp(now);
  auto user_login = ulb.Finish();

  fbb.Finish(user_login);
  do_test_read(fbb.GetBufferPointer(), fbb.GetSize());
}
