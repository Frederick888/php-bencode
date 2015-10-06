Introduction
===
The `php-bencode` is a PHP extension which can boost the process of encoding and decoding of [Bencode](https://en.wikipedia.org/wiki/Bencode). Supported by [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP), the project only uses about 1000 lines of codes to implement the basic functions including encoding, decoding, loading, saving and etc.

Installation
===
***Step 1*** Install dependencies
```bash
# Debian, Ubuntu (from launchpad)
apt-get install php5-dev
# Redhat, CentOS, Fedora
yum install php-devel
```
***Step 2*** Install PHP-CPP
```bash
git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP.git
cd PHP-CPP
make -j$(nproc)
make install
```
***Step 3*** Build and install
```bash
git clone https://git.tsundere.moe/Frederick888/php-bencode.git
cd php-bencode
make -j$(nproc)
make install
```

Basic Usage
===
***Example 1*** Parsing a string directly
```
php > $dict = BItem::parse("d4:key1l5:hello5:worlde4:key2i99ee");
php > print_r($dict->toMetaArray());
Array
(
    [_type] => BDict
    [_data] => Array
        (
            [key1] => Array
                (
                    [_type] => BList
                    [_data] => Array
                        (
                            [0] => Array
                                (
                                    [_type] => BStr
                                    [_data] => hello
                                )

                            [1] => Array
                                (
                                    [_type] => BStr
                                    [_data] => world
                                )

                        )

                )

            [key2] => Array
                (
                    [_type] => BInt
                    [_data] => 99
                )

        )

)
php > $dict->set('key2', new BInt(100));
php > echo $dict;
d4:key1l5:hello5:worlde4:key2i100ee
```
***Example 2*** Loading from/saving to a file
```
php > $dict = BItem::load("/path/sample.torrent");
php > $dict->save("/path/sample_copy.torrent");
php > echo md5_file("/path/sample.torrent") === md5_file("/path/sample_copy.torrent");
1
```
***Example 3*** Set/delete a path
```
php > $dict = new BDict();
php > $dict->set('key\/1/0', new BStr('hello'));    // Escape the slash if you need it in a key
php > $dict->set('key\/1/5', new BStr('world'));
php > print_r($dict->toArray());
Array
(
    [key/1] => Array
        (
            [0] => hello
            [1] => world
        )

)
php > echo $dict->del('key2');      // inexistent key
php > echo $dict->del('key\/1/1');
1
php > print_r($dict->toArray());
Array
(
    [key/1] => Array
        (
            [0] => hello
        )

)
```
