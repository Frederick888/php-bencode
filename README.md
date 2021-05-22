Code Status
===
GitLab CI [![GitLab CI Status](https://git.tsundere.moe/Frederick888/php-bencode/badges/master/pipeline.svg)](https://git.tsundere.moe/Frederick888/php-bencode/commits/master)

Introduction
===
The `php-bencode` is a PHP extension which can boost the process of encoding and decoding of [Bencode](https://en.wikipedia.org/wiki/Bencode).

`php-bencode` now supports **only PHP 7** with no need of any external libraries. For PHP 5 support, please check the [php-5](https://github.com/Frederick888/php-bencode/tree/php-5) branch, which needs [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP). (PHP 5 may be supported in the future, but it would take me a little long to get familiar with the old Zend APIs. So any PRs are welcomed.)

Installation
===
***Step 1*** Install dependencies
```bash
# Debian, Ubuntu (from launchpad)
apt-get install php-dev
# Redhat, CentOS, Fedora
yum install php-devel
```

***Step 2*** Build and install
```bash
git clone https://git.tsundere.moe/Frederick888/php-bencode.git
cd php-bencode
phpize
./configure --enable-bencode
make
make install
```

***Step 3*** Enable `php-bencode`

Add this to your `php.ini`:
```ini
extension=bencode.so
; optional: register classes in "bencode" namespace
bencode.namespace=1
```

Basic Usage
===
***Example 1*** Parsing a string directly
```
php > $bnode = bitem::parse("d4:key1l5:hello5:worlde4:key2i99ee");
php > print_r($bnode->to_meta_array());
Array
(
    [_type] => bdict
    [_length] => 34
    [_data] => Array
        (
            [key1] => Array
                (
                    [_type] => blist
                    [_length] => 16
                    [_data] => Array
                        (
                            [0] => Array
                                (
                                    [_type] => bstr
                                    [_length] => 7
                                    [_data] => hello
                                )

                            [1] => Array
                                (
                                    [_type] => bstr
                                    [_length] => 7
                                    [_data] => world
                                )

                        )

                )

            [key2] => Array
                (
                    [_type] => bint
                    [_length] => 4
                    [_data] => 99
                )

        )

)
php > $bnode->set('key2', new bint(100));
php > echo $bnode;
d4:key1l5:hello5:worlde4:key2i100ee
```
***Example 2*** Loading from/saving to a file
```
php > $bnode = bitem::load('/path/sample.dat');
php > var_dump($bnode->save('/path/sample_copy.dat'));
bool(true)
php > var_dump(md5_file('/path/sample.dat') === md5_file('/path/sample_copy.dat'));
bool(true)
```
***Example 3*** Set/delete a path

*it may be a better choice to use set_path/get_path/del_path instead, check phpdoc*
```
php > $bnode = new bdict();
php > $bnode->set('key1', new blist());
php > $bnode->get('key1')->add(new bstr('hello'));
php > $bnode->get('key1')->add(new bstr('world'));
php > print_r($bnode->to_array());
Array
(
    [key1] => Array
        (
            [0] => hello
            [1] => world
        )

)
php > var_dump($bnode->del('key2'));        // inexistent key
bool(false)
php > var_dump($bnode->get('key1')->del(1));
bool(true)
php > print_r($bnode->to_array());
Array
(
    [key1] => Array
        (
            [0] => hello
        )

)
```
