PHP_ARG_ENABLE(bencode, 
    [Whether to enable the "Bencode" extension],
    [ --enable-bencode          Enable "Bencode" extension support])

if test "$PHP_BENCODE" != "no"; then
    AC_DEFINE(HAVE_BENCODE, 1, [Whether you have Bencode extension])
    PHP_REQUIRE_CXX()
    PHP_SUBST(BENCODE_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, BENCODE_SHARED_LIBADD)
    PHP_NEW_EXTENSION(bencode, bencode.cc, $ext_shared)
fi
