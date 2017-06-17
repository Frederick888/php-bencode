--TEST--
PHP Bencode - Strings with Null
--FILE--
<?php
error_reporting(E_ALL);

$bdict = new bdict();
$bdict->set('hello', new bstr("foo\0bar"));
echo strlen($bdict->get('hello')->get()) . "\n";
echo bin2hex($bdict->get('hello')->get());

exit(0);
?>
--EXPECTF--
7
666f6f00626172

