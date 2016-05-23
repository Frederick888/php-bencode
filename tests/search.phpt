--TEST--
PHP Bencode - Search Test
--FILE--
<?php
error_reporting(E_ALL);

function println($info) { echo $info . "\n"; }

$broot = bitem::load(__DIR__ . '/test.in');
foreach ($broot->search('world', 1) as $path) {
    println($broot->get_path($path));
}
foreach ($broot->search('key', 0) as $path) {
    println($broot->get_path($path));
}

exit(0);
?>
--EXPECTF--
11:hello world
9:bye world
l11:hello world7:foo bar9:bye worldd6:subkey15:I can eat glassee
15:I can eat glass
i65536e
