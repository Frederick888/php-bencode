--TEST--
PHP Bencode - Basic Test With Namespace
--INI--
bencode.namespace=1
--FILE--
<?php
error_reporting(E_ALL);

function println($info) { echo $info . "\n"; }

$broot = bencode\bitem::load(__DIR__ . '/test.in');
println(sha1($broot));
println(serialize($broot->to_array()));
println(serialize($broot->to_meta_array()));
$broot->save(__DIR__ . '/test.out');
println(sha1_file(__DIR__ . '/test.in') === sha1_file(__DIR__ . '/test.out'));
unlink(__DIR__ . '/test.out');

exit(0);
?>
--EXPECTF--
bef3e94aa2c1bf31a4991c86bb3774c527a553a2
a:2:{s:4:"key1";a:4:{i:0;s:11:"hello world";i:1;s:7:"foo bar";i:2;s:9:"bye world";i:3;a:1:{s:6:"subkey";s:15:"I can eat glass";}}s:10:"key2/slash";i:65536;}
a:3:{s:5:"_type";s:5:"bdict";s:7:"_length";i:92;s:5:"_data";a:2:{s:4:"key1";a:3:{s:5:"_type";s:5:"blist";s:7:"_length";i:64;s:5:"_data";a:4:{i:0;a:3:{s:5:"_type";s:4:"bstr";s:7:"_length";i:14;s:5:"_data";s:11:"hello world";}i:1;a:3:{s:5:"_type";s:4:"bstr";s:7:"_length";i:9;s:5:"_data";s:7:"foo bar";}i:2;a:3:{s:5:"_type";s:4:"bstr";s:7:"_length";i:11;s:5:"_data";s:9:"bye world";}i:3;a:3:{s:5:"_type";s:5:"bdict";s:7:"_length";i:28;s:5:"_data";a:1:{s:6:"subkey";a:3:{s:5:"_type";s:4:"bstr";s:7:"_length";i:18;s:5:"_data";s:15:"I can eat glass";}}}}}s:10:"key2/slash";a:3:{s:5:"_type";s:4:"bint";s:7:"_length";i:7;s:5:"_data";i:65536;}}}
1

