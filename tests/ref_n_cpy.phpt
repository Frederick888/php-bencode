--TEST--
PHP Bencode - Reference and Copy Test
--FILE--
<?php
error_reporting(E_ALL);

function println($info) { echo $info . "\n"; }

$broot = bitem::load(__DIR__ . '/test.in');
$ref1 = $broot->get_path('key1/2');
$cpy1 = $broot->get_path_copy('key1/2');
$ref1->set('ref world');
$cpy1->set('cpy world');
unset($ref1);
unset($cpy1);
println($broot);
$ref2 = $broot->get('key2/slash');
$cpy2 = $broot->get_copy('key2/slash');
$ref2->set(-1024);
$cpy2->set(2048);
unset($ref2);
unset($cpy2);
println($broot);

exit(0);
?>
--EXPECTF--
d4:key1l11:hello world7:foo bar9:ref worldd6:subkey15:I can eat glassee10:key2/slashi65536ee
d4:key1l11:hello world7:foo bar9:ref worldd6:subkey15:I can eat glassee10:key2/slashi-1024ee

