<?php

/**
 * Created by PhpStorm.
 * User: frederick
 * Date: 9/22/15
 * Time: 1:09 AM
 */

/**
 * Class bitem
 *
 * The base class of all other types. You're not allowed to instantiate it.
 * Actually, only parse(), load() and save() are useful at PHP level
 */
abstract class bitem
{

    /**
     * @param string $ben The string to parse
     * @return bdict|blist|bstr|bint
     *
     * Generally, calling this function will handle the type adaption work for you
     * But if you're really sure about the original data and want only a specific type,
     *  you can also call other functions named parse the same in other classes
     */
    static function parse($ben)
    {
    }

    /**
     * @return string
     */
    abstract function encode();

    /**
     * @param string $file The path of the file to be decoded
     * @return bdict|blist|bstr|bint
     */
    static function load($file)
    {
    }

    /**
     * @param string $file The path of the file to save the encoded content into
     */
    function save($file)
    {
    }
}

/**
 * Class bdict
 *
 * Dictionary of Bencode.
 * The keys of a bdict is always bstr while values could be bdict, blist, bstr or bint.
 * This is handled by std::map in C++
 */
class bdict extends bitem
{
    /**
     * @return string "bdict" is always returned
     */
    function get_type()
    {
    }

    /**
     * @param string $key The key of the object to set
     * @param bdict|blist|bstr|bint $value The object
     * @return bool
     */
    function set($key, $value)
    {
    }

    /**
     * @param string $key The key of the object to get
     * @return bdict|blist|bstr|bint|null $value The object
     *
     * This function will return a reference of the node
     * If you need a copy instead, please use get_copy()
     */
    function get($key)
    {
    }

    /**
     * @param string $key The key of the object to get
     * @return bdict|blist|bstr|bint|null $value The object
     */
    function get_copy($key)
    {
    }

    /**
     * @param string $key The key of the object to check
     * @return bool
     */
    function has($key)
    {
    }

    /**
     * @param string $key The key of the object to delete
     * @return bool Deleted, true; not found, false
     */
    function del($key)
    {
    }


    /**
     * @return int Return the length of encoded content
     */
    function length()
    {
    }

    /**
     * @return int Return the number of elements
     */
    function count()
    {
    }

    /**
     * @param string $ben
     * @return bdict
     * @see bitem::parse()
     */
    static function parse($ben)
    {
    }

    /**
     * @return string
     */
    function encode()
    {
    }

    /**
     * @return array Return the array without meta
     *
     * convert the Bencode to a simple PHP array
     * notice that it may cause data loss,
     * so DO NOT use this function if you're not pretty sure about the content
     *
     * @see bdict::to_meta_array()
     */
    function to_array()
    {
    }

    /**
     * @return array Return the meta array
     *
     * in a meta array, each single element will be described as a array
     * for example,
     * bint(10) will be
     *      [ '_type' => 'bint', '_length' => 4, '_data' => 10 ]
     * bdict will be
     *      [ '_type' => 'bdict', '_length' => ..., '_size' => ..., '_data' => [ 'key1' => ..., 'key2' => ..., ... ] ]
     * this is very useful when the Bencode contains empty bdict or blist,
     * with out the _type the only way to distinguish them is to call get_type()
     */
    function to_meta_array()
    {
    }


    /**
     * @return string The encoded content
     *
     * this magic method will be called implicitly when being converted to string
     */
    function __toString()
    {
    }

    /**
     * PHP constructor
     */
    function __construct()
    {
    }

    /**
     * PHP destructor
     */
    function __destruct()
    {
    }
}

/**
 * Class blist
 *
 * List of Bencode
 * This is handled by std::vector in C++
 */
class blist extends bitem
{
    /**
     * @return string "blist" is always returned
     */
    function get_type()
    {
    }

    /**
     * @param string $key The key of the object to set
     * @param bdict|blist|bstr|bint $value The object
     *
     * If the key of a blist is larger than the size of it,
     * the value will be pushed to the end of the blist
     */
    function set($key, $value)
    {
    }

    /**
     * @param string $key The key of the object to get
     * @return bdict|blist|bstr|bint|null $value The object
     */
    function get($key)
    {
    }

    /**
     * @param string $key The key of the object to get
     * @return bdict|blist|bstr|bint|null $value The object
     */
    function get_copy($key)
    {
    }

    /**
     * @param string $key The key of the object to check
     * @return bool
     */
    function has($key)
    {
    }

    /**
     * @param string $key The key of the object to delete
     * @return bool Deleted, true; not found, false
     */
    function del($key)
    {
    }

    /**
     * @return int Return the length of encoded content
     */
    function length()
    {
    }

    /**
     * @return int Return the number of elements
     */
    function count()
    {
    }

    /**
     * @param string $ben
     * @return blist
     * @see bitem::parse()
     */
    static function parse($ben)
    {
    }

    /**
     * @return string
     */
    function encode()
    {
    }

    /**
     * @return array Return the array without meta
     * @see bdict::to_array()
     */
    function to_array()
    {
    }

    /**
     * @return array Return the meta array
     * @see bdict::to_meta_array()
     */
    function to_meta_array()
    {
    }


    /**
     * @return string The encoded content
     *
     * this magic method will be called implicitly when being converted to string
     */
    function __toString()
    {
    }

    /**
     * PHP constructor
     */
    function __construct()
    {
    }

    /**
     * PHP destructor
     */
    function __destruct()
    {
    }
}

/**
 * Class bstr
 *
 * String of Bencode.
 */
class bstr extends bitem
{
    /**
     * @return string "bstr" is always returned
     */
    function get_type()
    {
    }

    /**
     * @return string
     */
    function get()
    {
    }

    /**
     * @param string $value
     */
    function set($value)
    {
    }

    /**
     * @return int
     */
    function length()
    {
    }

    /**
     * @param string $ben
     * @return bstr
     * @see bitem::parse()
     */
    static function parse($ben)
    {
    }

    /**
     * @return string
     */
    function encode()
    {
    }

    /**
     * @return string This will simply return the contained value
     *
     * this is somehow useless to PHP developers
     * it will be called mostly when recursively converting a bdict or blist to array
     */
    function to_array()
    {
    }

    /**
     * @return array
     * @see bdict::to_meta_array()
     */
    function to_meta_array()
    {
    }

    /**
     * @return string
     */
    function __toString()
    {
    }

    /**
     * @param string|null $value
     */
    function __construct($value)
    {
    }
}

/**
 * Class bint
 *
 * Integer of Bencode.
 */
class bint extends bitem
{
    /**
     * @return string "bint" is always returned
     */
    function get_type()
    {
    }

    /**
     * @return int
     */
    function get()
    {
    }

    /**
     * @param int $value
     */
    function set($value)
    {
    }

    /**
     * @return int
     */
    function length()
    {
    }

    /**
     * @param string $ben
     * @return bint
     * @see bitem::parse()
     */
    static function parse($ben)
    {
    }

    /**
     * @return string
     */
    function encode()
    {
    }

    /**
     * @return int This will simply return the contained value
     *
     * this is somehow useless to PHP developers
     * it will be called mostly when recursively converting a bdict or blist to array
     */
    function to_array()
    {
    }

    /**
     * @return array
     * @see bdict::to_meta_array()
     */
    function to_meta_array()
    {
    }

    /**
     * @return string
     */
    function __toString()
    {
    }

    /**
     * @param int|null $value
     */
    function __construct($value)
    {
    }
}