--TEST--
PHP Bencode - Path Test
--FILE--
<?php
error_reporting(E_ALL);

function println($info) { echo $info . "\n"; }

$broot = bitem::load(__DIR__ . '/test.in');
println($broot->get('key1'));
println($broot->get_path('key1/2'));
println($broot->get_path('key2\/slash'));
$broot->set_path('key3/0/new_bdict/new_str', new bstr('it does not hurt me'));
println($broot);

exit(0);
?>
--EXPECTF--
l11:hello world7:foo bar9:bye worldd6:subkey15:I can eat glassee
9:bye world
i65536e
d4:key1l11:hello world7:foo bar9:bye worldd6:subkey15:I can eat glassee10:key2/slashi65536e4:key3ld9:new_bdictd7:new_str19:it does not hurt meeeee

