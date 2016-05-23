--TEST--
PHP Bencode - Path Test
--FILE--
<?php
error_reporting(E_ALL);

function println($info) { echo $info . "\n"; }

$broot = bitem::load(__DIR__ . '/test.in');
println(sha1($broot));
print_r($broot->to_array());
print_r($broot->to_meta_array());
$broot->save(__DIR__ . '/test.out');
println(sha1_file(__DIR__ . '/test.in') === sha1_file(__DIR__ . '/test.out'));
unlink(__DIR__ . '/test.out');

exit(0);
?>
--EXPECTF--
bef3e94aa2c1bf31a4991c86bb3774c527a553a2
Array
(
    [key1] => Array
        (
            [0] => hello world
            [1] => foo bar
            [2] => bye world
            [3] => Array
                (
                    [subkey] => I can eat glass
                )

        )

    [key2/slash] => 65536
)
Array
(
    [_type] => bdict
    [_length] => 92
    [_data] => Array
        (
            [key1] => Array
                (
                    [_type] => blist
                    [_length] => 64
                    [_data] => Array
                        (
                            [0] => Array
                                (
                                    [_type] => bstr
                                    [_length] => 14
                                    [_data] => hello world
                                )

                            [1] => Array
                                (
                                    [_type] => bstr
                                    [_length] => 9
                                    [_data] => foo bar
                                )

                            [2] => Array
                                (
                                    [_type] => bstr
                                    [_length] => 11
                                    [_data] => bye world
                                )

                            [3] => Array
                                (
                                    [_type] => bdict
                                    [_length] => 28
                                    [_data] => Array
                                        (
                                            [subkey] => Array
                                                (
                                                    [_type] => bstr
                                                    [_length] => 18
                                                    [_data] => I can eat glass
                                                )

                                        )

                                )

                        )

                )

            [key2/slash] => Array
                (
                    [_type] => bint
                    [_length] => 7
                    [_data] => 65536
                )

        )

)
1

