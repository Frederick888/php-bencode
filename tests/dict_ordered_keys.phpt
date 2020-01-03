--TEST--
PHP Bencode - Dictionary Ordered Keys Test
--FILE--
<?php
error_reporting(E_ALL);

$unordered_encoded = 'd7:ttg_tag32:8032a74ec22927a5bd6367537eafd87e7:privatei1e6:source20:[ttys3.bencode.test]e';

$bdict = bitem::parse($unordered_encoded);
echo $bdict;

exit(0);
?>
--EXPECTF--
d7:privatei1e6:source20:[ttys3.bencode.test]7:ttg_tag32:8032a74ec22927a5bd6367537eafd87ee
