--TEST--
PHP Bencode - Strings with Null
--FILE--
<?php
error_reporting(E_ALL);

$key = "hello\0world";
$val = "foo\0bar";

$bdict = new bdict();
$bdict->set($key, new bstr($val));
echo strlen($bdict->get($key)->get()) . "\n";
echo bin2hex($bdict->get($key)->get());

exit(0);
?>
--EXPECTF--
7
666f6f00626172

