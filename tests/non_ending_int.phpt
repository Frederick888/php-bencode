--TEST--
PHP Bencode - Non-Ending Integer Test
--FILE--
<?php
error_reporting(E_ALL);

$bstr = bitem::parse('i10');
echo $bstr;

exit(0);
?>
--EXPECTF--
i10e
